#pragma once
#include <cstring>

void test_constructors ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Constructors                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    typedef pfs::map<int, int> int_map;

    ADD_TESTS(2);

    //
    // Default constructor
    //
    int_map v0;

    TEST_OK(v0.size() == 0);

    //
    // Copy constructor
    //
    int_map v1(v0);

    TEST_OK(v1.size() == 0);
}
