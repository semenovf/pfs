#pragma once

void test_compare ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Compare                                   //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(8);

    TEST_OK(time_type(9, 48, 0) == time_type(9, 48, 0));
    TEST_OK(time_type(9, 48, 0) != time_type(9, 48, 1));
    TEST_OK(time_type(9, 48, 0) <  time_type(9, 48, 1));
    TEST_OK(time_type(9, 48, 0) <= time_type(9, 48, 0));
    TEST_OK(time_type(9, 48, 0) <= time_type(9, 48, 1));
    TEST_OK(time_type(9, 48, 0) >  time_type(8, 48, 0));
    TEST_OK(time_type(9, 48, 0) >= time_type(9, 48, 0));
    TEST_OK(time_type(9, 48, 0) >= time_type(8, 48, 0));
}
