#pragma once

void test_compare ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "// Compare operators                                                     //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(1);

    typedef pfs::list<int> int_list;

    int_list l1;
    int_list l2;

    TEST_OK2(l1 == l2, "Lists are equal")
}
