#pragma once
#include "pfs/numeric.hpp"

void test_iota ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             iota                                      //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(1);

    {
        int arr[6];
        int n = sizeof(arr)/sizeof(arr[0]);
        pfs::iota(arr, arr + n, -2);
        TEST_OK2(arr[0] == -2
                && arr[1] == -1
                && arr[2] == 0
                && arr[3] == 1
                && arr[4] == 2
                && arr[5] == 3
                , "Fills a range with integers");
    }
}


