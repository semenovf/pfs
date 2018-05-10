#pragma once

void test_bind ()
{
    ADD_TESTS(16);

    ///////////////////////////////////////////////////////////////////////////
    // Bind to function                                                      //
    ///////////////////////////////////////////////////////////////////////////

    PFS_AUTO(e1, pfs::bind(e));
    TEST_OK(e1() == INT_RESULT);

    PFS_AUTO(f1, pfs::bind(f, _1, _2, _3, _4, _5));
    PFS_AUTO(f2, pfs::bind(f, 1, _1, _2, _3, _4));
    PFS_AUTO(f3, pfs::bind(f, 1, 2, _1, _2, _3));
    PFS_AUTO(f4, pfs::bind(f, 1, 2, 3, _1, _2));
    PFS_AUTO(f5, pfs::bind(f, 1, 2, 3, 4, _1));
    PFS_AUTO(f6, pfs::bind(f, 1, 2, 3, 4, 5));
    PFS_AUTO(f7, pfs::bind(f, _5, _4, _3, _2, _1));

    TEST_OK(f1(1, 2, 3, 4, 5) == SUM_RESULT);
    TEST_OK(f2(2, 3, 4, 5) == SUM_RESULT);
    TEST_OK(f3(3, 4, 5) == SUM_RESULT);
    TEST_OK(f4(4, 5) == SUM_RESULT);
    TEST_OK(f5(5) == SUM_RESULT);
    TEST_OK(f6() == SUM_RESULT);
    TEST_OK(f7(1, 2, 3, 4, 5) == SUM_RESULT);

    ///////////////////////////////////////////////////////////////////////////
    // Bind to a pointer to member function                                  //
    ///////////////////////////////////////////////////////////////////////////

    Bar bar;

    // by pointer to class instance
    PFS_AUTO(b1, pfs::bind(& Bar::sum, & bar, _1, _2));
    PFS_AUTO(b2, pfs::bind(& Bar::sum, & bar, _1, 2));
    PFS_AUTO(b3, pfs::bind(& Bar::sum, & bar, 1, 2));

    // by value (or rvalue) of class instance
    PFS_AUTO(b4, pfs::bind(& Bar::sum, bar, 1, 2));

    // by smart pointer of class instance
    PFS_AUTO(b5, pfs::bind(& Bar::sum, pfs::make_shared<Bar>(), 1, 2));

    TEST_OK(b1(1, 2) == 3);
    TEST_OK(b2(1) == 3);
    TEST_OK(b3() == 3);
    TEST_OK(b4() == 3);
    TEST_OK(b5() == 3);

    ///////////////////////////////////////////////////////////////////////////
    // Bind to a pointer to data member                                      //
    ///////////////////////////////////////////////////////////////////////////

    PFS_AUTO(d1, pfs::bind(& Bar::data, _1));

    TEST_OK(d1(bar) == INT_RESULT);

    ///////////////////////////////////////////////////////////////////////////
    // Bind to a functional object                                           //
    ///////////////////////////////////////////////////////////////////////////

    Func o;
    PFS_AUTO(o1, pfs::bind(o));

    TEST_OK(o1() == INT_RESULT);

    ///////////////////////////////////////////////////////////////////////////
    // Assign to a function wrapper                                          //
    ///////////////////////////////////////////////////////////////////////////

    pfs::function<int ()> w1 = pfs::bind(f, 1, 2, 3, 4, 5);

    TEST_OK(w1() == SUM_RESULT);
}
