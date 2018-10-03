#pragma once
#include <string>

void test_modifiers ()
{
    ADD_TESTS(6);

#if __cplusplus >= 201103L
    ADD_TESTS(2);
#endif

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                              Modifiers                                //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    // IndexDirect
    {
        pfs::variant<int, std::string> v;
        v.emplace<1>("42");
        TEST_OK("42" == pfs::get<1>(v));
    }

    // IndexDirectDuplicate
    {
        pfs::variant<int, int> v;
        v.emplace<1>(42);
        TEST_OK(42 == pfs::get<1>(v));
    }

    // IndexConversion
    {
        pfs::variant<int, std::string> v;
        v.emplace<1>("42");
        TEST_OK("42" == pfs::get<1>(v));
    }

    // IndexConversionDuplicate
    {
        pfs::variant<int, int> v;
        v.emplace<1>(1.1);
        TEST_OK(1 == pfs::get<1>(v));
    }

    // IndexInitializerList
    {
#if __cplusplus >= 201103L
        pfs::variant<int, std::string> v;
        v.emplace<1>({'4', '2'});
        TEST_OK("42" == pfs::get<1>(v));
#endif
    }

    // TypeDirect
    {
        pfs::variant<int, std::string> v;
        v.emplace<std::string>("42");
        TEST_OK("42" == pfs::get<std::string>(v));
    }

    // TypeConversion
    {
        pfs::variant<int, std::string> v;
        v.emplace<int>(1.1);
        TEST_OK(1 == pfs::get<int>(v));
    }

    // TypeInitializerList
    {
#if __cplusplus >= 201103L
        pfs::variant<int, std::string> v;
        v.emplace<std::string>({'4', '2'});
        TEST_OK("42" == pfs::get<std::string>(v));
#endif
    }
}
