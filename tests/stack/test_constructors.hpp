#pragma once
#include <cstring>

void test_constructors ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Constructors                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    typedef pfs::stack<int> int_stack;

    ADD_TESTS(2);

    //
    // Default constructor
    //
    int_stack v0;

    TEST_OK(v0.size() == 0);

    //
    // Copy constructor
    //
    int_stack v1(v0);

    TEST_OK(v1.size() == 0);
}
