#ifndef TEST_DIRECTORY_ENTRY_HPP
#define TEST_DIRECTORY_ENTRY_HPP

#include <pfs/filesystem.hpp>
#include <pfs/operationsystem.hpp>
#include <iostream>

void test_directory_entry ()
{
    //    typedef pfs::filesystem::path path;
    //    typedef pfs::filesystem::directory_entry directory_entry;
#if __COMMENT__
    // Constructors
    {
        ADD_TESTS(3);

        directory_entry p1 = "/usr/lib"; // portable format
        directory_entry p2 = "C:\\users\\abcdef\\AppData\\Local\\Temp\\"; // native format
        directory_entry p3 = L"D:/猫.txt"; // wide string

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
        TEST_OK((path("//host") /= "foo") == path::string_type("//host/foo")); // appends with separator
        TEST_OK((path("//host/") /= "foo") == path::string_type("//host/foo")); // appends without separator

        // Non-member function
        TEST_OK(path("//host") / "foo" == path::string_type("//host/foo")); // appends with separator
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
        TEST_OK((path("foo") /= "C:/bar") == path::string_type("C:/bar")); // the result is "C:/bar" (replaces)
        TEST_OK((path("foo") /= "C:") == path::string_type("C:")); // the result is "C:"     (replaces)
        TEST_OK((path("C:") /= "") == path::string_type("C:")); // the result is "C:"     (appends, without separator)
        TEST_OK((path("C:foo") /= "/bar") == path::string_type("C:/bar")); // yields "C:/bar"        (removes relative path, then appends)
        TEST_OK((path("C:foo") /= "C:bar") == path::string_type("C:foo/bar")); // yields "C:foo/bar"     (appends, omitting p's root-name)

        // Non-member function
        TEST_OK(path("foo") / "C:/bar" == path::string_type("C:/bar")); // the result is "C:/bar" (replaces)
        TEST_OK(path("foo") / "C:" == path::string_type("C:")); // the result is "C:"     (replaces)
        TEST_OK(path("C:") / "" == path::string_type("C:")); // the result is "C:"     (appends, without separator)
        TEST_OK(path("C:foo") / "/bar" == path::string_type("C:/bar")); // yields "C:/bar"        (removes relative path, then appends)
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

    //
    // Decomposition
    //
    {
        ADD_TESTS(27);

        TEST_OK(path("//server/path/to/file").root_name() == "//server");
        TEST_OK(path("//server/path/to/file").root_directory() == "/");
        TEST_OK(path("//server/path/to/file").root_path() == "//server/");
        TEST_OK(path("//server/path/to/file").relative_path() == "path/to/file");

        TEST_OK(path("//server/path/to/file").parent_path() == "//server/path/to");
        TEST_OK(path("/path/to/.").parent_path() == "/path/to");
        TEST_OK(path("/").parent_path() == "");

        TEST_OK(path("/foo/bar.txt").filename() == "bar.txt");
        TEST_OK(path("/foo/.bar").filename() == ".bar");
        TEST_OK(path("/foo/bar/").filename() == ".");
        TEST_OK(path("/foo/.").filename() == ".");
        TEST_OK(path("/foo/..").filename() == "..");
        TEST_OK(path(".").filename() == ".");
        TEST_OK(path("..").filename() == "..");
        TEST_OK(path("/").filename() == "/");

        TEST_OK(path("/foo/bar.txt").stem() == "bar");
        TEST_OK(path("/foo/.bar").stem() == ".bar"); // FIXME (for std::experimental::filesystem::path::stem() returns "")

        TEST_OK(path("/foo/bar.txt").extension() == ".txt");
        TEST_OK(path("/foo/bar.").extension() == ".");
        TEST_OK(path("/foo/bar").extension() == "");
        TEST_OK(path("/foo/bar.txt/bar.cc").extension() == ".cc");
        TEST_OK(path("/foo/bar.txt/bar.").extension() == ".");
        TEST_OK(path("/foo/bar.txt/bar").extension() == "");
        TEST_OK(path("/foo/.").extension() == "");
        TEST_OK(path("/foo/..").extension() == "");
        TEST_OK(path("/foo/.hidden").extension() == ""); // FIXME (for std::experimental::filesystem::path::stem() returns ".hidden")
        TEST_OK(path("/foo/..bar").extension() == ".bar");

#if PFS_OS_DOS
        ADD_TESTS(7);
        TEST_OK(path("c:\\path\\to\\file").root_name() == "c:");
        TEST_OK(path("c:\\path\\to\\file").root_directory() == "\\");
        TEST_OK(path("c:\\path\\to\\file").root_path() == "c:\\");
        TEST_OK(path("c:\\path\\to\\file").relative_path() == "path\\to\\file");

        TEST_OK(path("c:\\path\\to\\file").parent_path() == "c:\\path\\to");
        TEST_OK(path("c:\\path\\to\\.").parent_path() == "c:\\path\\to");
        TEST_OK(path("\\").parent_path() == "");
#endif
    }

    //
    // Queries
    //
    {
        ADD_TESTS(31);

        TEST_OK(!path("//server/path/to/file").empty());
        TEST_OK(path("").empty());
        TEST_OK(path().empty());

        TEST_OK(path("//server/path/to/file").has_root_path());
        TEST_OK(path("//server/path/to/file").has_root_name());
        TEST_OK(path("//server/path/to/file").has_root_directory());
        TEST_OK(path("//server/path/to/file").has_relative_path());

        TEST_OK(path("//server/path/to/file").has_parent_path());
        TEST_OK(path("/path/to/.").has_parent_path());
        TEST_OK(!path("/").has_parent_path());

        TEST_OK(path("/foo/bar.txt").has_filename());
        TEST_OK(path("/foo/.bar").has_filename());
        TEST_OK(path("/foo/bar/").has_filename());
        TEST_OK(path("/foo/.").has_filename());
        TEST_OK(path("/foo/..").has_filename());
        TEST_OK(path(".").has_filename());
        TEST_OK(path("..").has_filename());
        TEST_OK(path("/").has_filename());

        TEST_OK(path("/foo/bar.txt").has_stem());
        TEST_OK(path("/foo/.bar").has_stem()); // FIXME (for std::experimental::filesystem::path::stem() returns "")

        TEST_OK(path("/foo/bar.txt").has_extension());
        TEST_OK(path("/foo/bar.").has_extension());
        TEST_OK(!path("/foo/bar").has_extension());
        TEST_OK(path("/foo/bar.txt/bar.cc").has_extension());
        TEST_OK(path("/foo/bar.txt/bar.").has_extension());
        TEST_OK(!path("/foo/bar.txt/bar").has_extension());
        TEST_OK(!path("/foo/.").has_extension());
        TEST_OK(!path("/foo/..").has_extension());
        TEST_OK(!path("/foo/.hidden").has_extension()); // FIXME (for std::experimental::filesystem::path::stem() returns ".hidden")
        TEST_OK(path("/foo/..bar").has_extension());

        TEST_OK(path("path/to").is_relative());

#if PFS_OS_POSIX
        ADD_TESTS(1);

        // The path "/" is absolute on a POSIX OS, but is relative on Windows.
        TEST_OK(path("/").is_absolute());
#endif

#if PFS_OS_DOS
        ADD_TESTS(1);

        // The path "/" is absolute on a POSIX OS, but is relative on Windows.
        TEST_OK(path("/").is_relative());
#endif
    }

    //
    // Iterators
    //

    {

#if PFS_OS_POSIX
        ADD_TESTS(8);

        path p = "/home/user/abcdef/app_data/Local/Temp/";
        path::iterator it = p.begin();

        TEST_OK(*it++ == "/");
        TEST_OK(*it++ == "home");
        TEST_OK(*it++ == "user");
        TEST_OK(*it++ == "abcdef");
        TEST_OK(*it++ == "app_data");
        TEST_OK(*it++ == "Local");
        TEST_OK(*it++ == "Temp");
        TEST_OK(*it++ == ".");
        TEST_OK(it == p.end());

        //std::cout << *it << '\n';
#endif

#if PFS_OS_DOS
        ADD_TESTS(8);
        path p = "C:\\users\\abcdef\\AppData\\Local\\Temp\\";
        path::iterator it = p.begin();

        TEST_OK(*it++ == "C:");
        TEST_OK(*it++ == "/");
        TEST_OK(*it++ == "users");
        TEST_OK(*it++ == "abcdef");
        TEST_OK(*it++ == "AppData");
        TEST_OK(*it++ == "Local");
        TEST_OK(*it++ == "Temp");
        TEST_OK(*it++ == ".");
        TEST_OK(it == p.end());
#endif
    }

    //
    // Non-member functions
    //
    {
        ADD_TESTS(4);

        path p1("/path/to1");
        path p2("/path/to2");

        TEST_OK(p1 == "/path/to1");
        TEST_OK(p2 == "/path/to2");

        pfs::filesystem::swap(p1, p2);

        TEST_OK(p1 == "/path/to2");
        TEST_OK(p2 == "/path/to1");
    }

    {
        ADD_TESTS(7);

        path p1("/path/to");
        path p2("/path/to");
        path p3("/path/to/a");

        TEST_OK(p1 == p2);
        TEST_OK(p1 <= p2);
        TEST_OK(p1 >= p2);

        TEST_OK(p1 < p3);
        TEST_OK(p1 <= p3);

        TEST_OK(p3 > p1);
        TEST_OK(p3 >= p1);
    }
#endif
}

#endif /* TEST_DIRECTORY_ENTRY_HPP */
