#pragma once
#include "pfs/algorithm.hpp"
#include "pfs/math.hpp"

static bool abs_compare (int a, int b)
{
    return pfs::abs(a) < pfs::abs(b);
}

void test_minmaxelement ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Min/Max element                           //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(16);

    int arr[] = { -5, 2, -10, 10, 3, 7, 0 };
    int n = sizeof(arr)/sizeof(arr[0]);

    {
        int * mx = pfs::max_element(arr, arr + n);
        TEST_OK(mx == arr + 3);
        TEST_OK(*mx == arr[3]);
    }

    {
        int * mx = pfs::max_element(arr, arr + n, abs_compare);
        TEST_OK(mx == arr + 2 || mx == arr + 3);
        TEST_OK(*mx == arr[2] || *mx == arr[3]);
    }

    {
        int * mn = pfs::min_element(arr, arr + n);
        TEST_OK(mn == arr + 2);
        TEST_OK(*mn == arr[2]);
    }

    {
        int * mn = pfs::min_element(arr, arr + n, abs_compare);
        TEST_OK(mn == arr + 6);
        TEST_OK(*mn == arr[6]);
    }

    {
        pfs::pair<int *,int *> m = pfs::minmax_element(arr, arr + n);
        TEST_OK(m.first == arr + 2);
        TEST_OK(*m.first == arr[2]);
        TEST_OK(m.second == arr + 3);
        TEST_OK(*m.second == arr[3]);
    }

    {
        pfs::pair<int *,int *> m = pfs::minmax_element(arr, arr + n, abs_compare);
        TEST_OK(m.first == arr + 6);
        TEST_OK(*m.first == arr[6]);
        TEST_OK(m.second == arr + 2 || m.second == arr + 3);
        TEST_OK(*m.second == arr[2] || *m.second == arr[3]);
    }
}

