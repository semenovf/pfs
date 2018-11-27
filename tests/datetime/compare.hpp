#pragma once

void test_compare ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                              Compare                                  //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(8);

    TEST_OK(pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)) == pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)));
    TEST_OK(pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)) != pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 1)));
    TEST_OK(pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)) <  pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 1)));
    TEST_OK(pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)) <= pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)));
    TEST_OK(pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)) <= pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 1)));
    TEST_OK(pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)) >  pfs::datetime(pfs::date(2019, 11, 27), pfs::time(8, 48, 0)));
    TEST_OK(pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)) >= pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)));
    TEST_OK(pfs::datetime(pfs::date(2019, 11, 27), pfs::time(9, 48, 0)) >= pfs::datetime(pfs::date(2019, 11, 27), pfs::time(8, 48, 0)));
}
