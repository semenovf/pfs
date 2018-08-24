#pragma once
#include <cstring>

void test_constructors ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Constructors                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    typedef pfs::set<int> int_set;

    ADD_TESTS(2);

    //
    // Default constructor
    //
    int_set v0;

    TEST_OK(v0.size() == 0);

    //
    // Copy constructor
    //
    int_set v1(v0);

    TEST_OK(v1.size() == 0);
}
