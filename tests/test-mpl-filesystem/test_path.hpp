/* 
 * File:   test_path.hpp
 * Author: wladt
 *
 * Created on June 20, 2017
 */

#ifndef TEST_PATH_HPP
#define TEST_PATH_HPP

#include <pfs/filesystem.hpp>
#include <pfs/operationsystem.hpp>
#include <iostream>

void test_path ()
{
    typedef pfs::filesystem::path path;
    
    // Constructors
    {
        ADD_TESTS(3);
        
        path p1 = "/usr/lib/sendmail.cf"; // portable format
        path p2 = "C:\\users\\abcdef\\AppData\\Local\\Temp\\"; // native format
        path p3 = L"D:/猫.txt";           // wide string

//        std::cout << "p1 = " << p1 << '\n'
//                  << "p2 = " << p2 << '\n'
//                  << "p3 = " << p3 << '\n';

        TEST_OK(p1 == path::string_type("/usr/lib/sendmail.cf"));
        TEST_OK(p2 == path::string_type("C:\\users\\abcdef\\AppData\\Local\\Temp\\"));
        TEST_OK(p3 == path::string_type("D:/猫.txt"));
    }
    
    // Assign operators
    {
        ADD_TESTS(2);

        path p = "C:/users/abcdef/AppData/Local";
        p = p / "Temp"; // move assignment
        
        TEST_OK(p == path::string_type("C:/users/abcdef/AppData/Local/Temp"));
        
        wchar_t const * wstr = L"D:/猫.txt";
        p = wstr; // assignment from a source
        
        TEST_OK(p == path::string_type("D:/猫.txt"));
    }
    
    //
    // Concatenation #1
    //
    {
        ADD_TESTS(4);
        
        // where "//host" is a root-name
        TEST_OK((path("//host") /= "foo")  == path::string_type("//host/foo")); // appends with separator
        TEST_OK((path("//host/") /= "foo") == path::string_type("//host/foo")); // appends without separator

        // Non-member function
        TEST_OK(path("//host") / "foo"  == path::string_type("//host/foo")); // appends with separator
        TEST_OK(path("//host/") / "foo" == path::string_type("//host/foo")); // appends without separator

#if PFS_OS_POSIX
        ADD_TESTS(4);
        
        TEST_OK((path("foo") /= "") == path::string_type("foo")); // the result is "foo/" (appends)
        TEST_OK((path("foo") /= "/bar") == path::string_type("foo/bar")); // the result is "/bar" (replaces)
        
        // Non-member function
        TEST_OK(path("foo") / "" == path::string_type("foo")); // the result is "foo/" (appends)
        TEST_OK(path("foo") / "/bar" == path::string_type("foo/bar")); // the result is "/bar" (replaces)
#endif

#if PFS_OS_DOS
        ADD_TESTS(10);
        TEST_OK((path("foo") /= "C:/bar") == path::string_type("C:/bar"));     // the result is "C:/bar" (replaces)
        TEST_OK((path("foo") /= "C:") == path::string_type("C:"));             // the result is "C:"     (replaces)
        TEST_OK((path("C:") /= "") == path::string_type("C:"));                // the result is "C:"     (appends, without separator)
        TEST_OK((path("C:foo") /= "/bar") == path::string_type("C:/bar"));     // yields "C:/bar"        (removes relative path, then appends)
        TEST_OK((path("C:foo") /= "C:bar") == path::string_type("C:foo/bar")); // yields "C:foo/bar"     (appends, omitting p's root-name)

        // Non-member function
        TEST_OK(path("foo") / "C:/bar" == path::string_type("C:/bar"));     // the result is "C:/bar" (replaces)
        TEST_OK(path("foo") / "C:" == path::string_type("C:"));             // the result is "C:"     (replaces)
        TEST_OK(path("C:") / "" == path::string_type("C:"));                // the result is "C:"     (appends, without separator)
        TEST_OK(path("C:foo") / "/bar" == path::string_type("C:/bar"));     // yields "C:/bar"        (removes relative path, then appends)
        TEST_OK(path("C:foo") / "C:bar" == path::string_type("C:foo/bar")); // yields "C:foo/bar"     (appends, omitting p's root-name)
#endif
    }
    
    //
    // Concatenation #2
    //
    {
        ADD_TESTS(2);
        
        path p1; // empty path
        p1 += "var"; // does not insert a separator
        
        TEST_OK(p1 == path::string_type("var"));
        
        p1 += "lib"; // does not insert a separator
        
        TEST_OK(p1 == path::string_type("varlib"));
    }
    
    //
    // Modifiers #1
    //
    {
        ADD_TESTS(2);
        
        path p("/path/to/file");
        
        TEST_OK(!p.empty());
        
        p.clear();
        
        TEST_OK(p.empty());
    }
    
    //
    // Modifiers #2
    //
    {
        // TODO Test path::make_preferred()
    }
    
    //
    // Modifiers #3
    //
    {
        ADD_TESTS(4);
        
        TEST_OK(path("foo/bar").remove_filename() == path::string_type("foo/")); // FAIL on C++11
        TEST_OK(path("foo/").remove_filename() == path::string_type("foo/")); // FAIL on C++11
        TEST_OK(path("/foo").remove_filename() == path::string_type("/"));
        TEST_OK(path("/").remove_filename() == path::string_type("/")); // FAIL on C++11
    }
    
    //
    // Modifiers #4
    //
    {
        ADD_TESTS(2);
 
        TEST_OK(path("/foo").replace_filename("bar") == path::string_type("/bar"));
        TEST_OK(path("/").replace_filename("bar") == path::string_type("bar"));
    }

    //
    // Modifiers #5
    //
    {
        ADD_TESTS(2);
        
        path p = "/foo/bar.jpeg";
        
        TEST_OK(p == path::string_type("/foo/bar.jpeg"));
        
        p.replace_extension(".jpg");
        
        TEST_OK(p == path::string_type("/foo/bar.jpg"));
    }

    //
    // Modifiers #6
    //
    {
        ADD_TESTS(4);
        
        path p1 = "/foo/bar";
        path p2 = "/foo/baz";
        
        TEST_OK(p1 == path::string_type("/foo/bar"));
        TEST_OK(p2 == path::string_type("/foo/baz"));
        
        p1.swap(p2);
        
        TEST_OK(p1 == path::string_type("/foo/baz"));
        TEST_OK(p2 == path::string_type("/foo/bar"));
    }
    
    //
    // Compare
    //
    {
        ADD_TESTS(3);
        
        path p1 = "/a/b/"; // as if "a/b/." for lexicographical iteration
        path p2 = "/a/b/#";
        path p3 = "/a/b/_";
        
        TEST_OK(p1.compare(p1) == 0);
        TEST_OK(p1.compare(p2) > 0);
        TEST_OK(p1.compare(p3) < 0);
    }
    
    //
    // Generation
    //
    {
        // TODO Implement path::lexically_xxx() methods
//        ADD_TESTS(8);
//        
//        TEST_OK(path("foo/./bar/..").lexically_normal() == "foo/");
//        TEST_OK(path("foo/.///bar/../").lexically_normal() == "foo/");
//        TEST_OK(path("/a/d").lexically_relative("/a/b/c") == "../../d");
//        TEST_OK(path("/a/b/c").lexically_relative("/a/d") == "../b/c");
//        TEST_OK(path("a/b/c").lexically_relative("a") == "b/c");
//        TEST_OK(path("a/b/c").lexically_relative("a/b/c/x/y") == "../..");
//        TEST_OK(path("a/b/c").lexically_relative("a/b/c") == ".");
//        TEST_OK(path("a/b").lexically_relative("c/d") == "../../a/b");
    }
}

#endif /* TEST_PATH_HPP */

