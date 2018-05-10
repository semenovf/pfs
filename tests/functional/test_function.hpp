#pragma once

namespace test_function {

void run ()
{
    ADD_TESTS(5);

    ///////////////////////////////////////////////////////////////////////////
    // Assign to a function wrapper                                          //
    ///////////////////////////////////////////////////////////////////////////

    Bar bar;

    pfs::function<int ()> w1 = e;
    pfs::function<int ()> w2 = pfs::bind(f, 1, 2, 3, 4, 5);
    pfs::function<int (Bar &, int, int)> w3 = & Bar::sum;
    pfs::function<int (Bar &)> w4 = & Bar::data;
    pfs::function<int ()> w5 = Func();

    TEST_OK(w1() == INT_RESULT);
    TEST_OK(w2() == SUM_RESULT);
    TEST_OK(w3(bar, 1, 2) == 3);
    TEST_OK(w4(bar) == INT_RESULT);
    TEST_OK(w5() == INT_RESULT);
}

} // namespace test_function
