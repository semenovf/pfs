#pragma once
#include <cstring>

void test_assignments ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Assignments                               //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    typedef pfs::vector<int> int_vector;

    ADD_TESTS(4);

    //
    // Default copy assignment operator
    //
    int_vector v;
    int_vector v0;

    v = v0;

    TEST_OK(v.size() == v0.size());
    TEST_OK(v.size() == 0);

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    //
    // Copy assignment operator
    //
    int_vector v1;

    v1 = v;

    TEST_OK(v.size() == 3);
    TEST_OK(v1.size() == 3);

#if __cplusplus >= 201103L

    ADD_TESTS(2);

    //
    // Move assignment operator
    //
    int_vector v2;
    v2 = std::move(v);

    TEST_OK(v.size() == 0);
    TEST_OK(v2.size() == 3);

#endif
}
