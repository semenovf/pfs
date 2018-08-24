#pragma once
#include <cstring>

void test_assignments ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Assignments                               //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    typedef pfs::map<int, int> int_map;

    ADD_TESTS(4);

    //
    // Default copy assignment operator
    //
    int_map v;
    int_map v0;

    v = v0;

    TEST_OK(v.size() == v0.size());
    TEST_OK(v.size() == 0);

    v.insert(1, 1);
    v.insert(2, 2);
    v.insert(3, 3);

    //
    // Copy assignment operator
    //
    int_map v1;

    v1 = v;

    TEST_OK(v.size() == 3);
    TEST_OK(v1.size() == 3);

#if __cplusplus >= 201103L

    ADD_TESTS(2);

    //
    // Move assignment operator
    //
    int_map v2;
    v2 = std::move(v);

    TEST_OK(v.size() == 0);
    TEST_OK(v2.size() == 3);

#endif
}
