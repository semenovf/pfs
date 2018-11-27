#pragma once

void test_constructor ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                              Constructors                             //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(6);

    pfs::datetime zero;
    pfs::datetime zero_copy1(zero);

    // pfs::datetime zero_copy2(pfs::date(), pfs::time()); // <== Do not compile properly:
    // error: request for member ‘to_intmax’ in ‘zero_copy2’, which is of non-class type ‘pfs::datetime(pfs::date (*)(), pfs::time (*)())’
    pfs::date d;
    pfs::datetime zero_copy2(d, pfs::time());

    TEST_OK(!zero.valid());
    TEST_OK(!zero_copy1.valid());
    TEST_OK(!zero_copy2.valid());

    pfs::datetime zero_from_epoch = pfs::datetime::from_seconds_since_epoch(0);
    TEST_OK(zero_from_epoch == pfs::datetime(pfs::date(1970, 1, 1), pfs::time(0, 0, 0)));

    TEST_OK(pfs::datetime::from_seconds_since_epoch(1) == pfs::datetime(pfs::date(1970, 1, 1), pfs::time(0, 0, 1)));
    TEST_OK(pfs::datetime::from_seconds_since_epoch(1543305320UL) == pfs::datetime(pfs::date(2018,11,27), pfs::time(7,55,20)));
}
