#pragma once
#include <cstring>

void test_constructors ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Constructors                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(2);

    typedef pfs::list<int> int_list;

    //
    // Default constructor
    //
    int_list v0;

    TEST_OK(v0.size() == 0);

    //
    // Copy constructor
    //
    int_list v1(v0);

    TEST_OK(v1.size() == 0);
}
