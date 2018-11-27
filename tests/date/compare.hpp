#pragma once

void test_compare ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Compare                                   //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(8);

    TEST_OK(pfs::date(2019, 11, 27) == pfs::date(2019, 11, 27));
    TEST_OK(pfs::date(2019, 11, 27) != pfs::date(2019, 11, 26));
    TEST_OK(pfs::date(2019, 11, 27) <  pfs::date(2019, 11, 28));
    TEST_OK(pfs::date(2019, 11, 27) <= pfs::date(2019, 11, 27));
    TEST_OK(pfs::date(2019, 11, 27) <= pfs::date(2019, 11, 28));
    TEST_OK(pfs::date(2019, 11, 27) >  pfs::date(2019, 11, 26));
    TEST_OK(pfs::date(2019, 11, 27) >= pfs::date(2019, 11, 27));
    TEST_OK(pfs::date(2019, 11, 27) >= pfs::date(2019, 11, 26));
}
