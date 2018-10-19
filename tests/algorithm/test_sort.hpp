#pragma once
#include "pfs/string.hpp"
#include "pfs/algorithm.hpp"
#include "pfs/functional.hpp"
#include "pfs/numeric.hpp"

static pfs::string concat (pfs::string a, int b)
{
    return a + ',' + pfs::to_string(b);
}

static pfs::string to_string (int * first, int * last)
{
    return pfs::accumulate(pfs::next(first), last, pfs::to_string(*first), concat);
}

void test_partial_sort ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                         Partial sorting (nth_element)                 //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(2);

    {
        int arr[] = { 5, 6, 4, 3, 2, 6, 7, 9, 3 };
        int n = sizeof(arr)/sizeof(arr[0]);

        std::cout << "Initial array:" << std::endl
                << "\t[" << to_string(arr, arr + n) << "]" << std::endl;

        pfs::nth_element(arr, arr + n / 2, arr + n);
        std::cout << "Array after nth_element(begin, middle, last):" << std::endl
                << "\t[" << to_string(arr, arr + n) << "]" << std::endl;

        TEST_OK(arr[n / 2] == 5);

        pfs::nth_element(arr, arr + 1, arr + n, pfs::greater<int>());
        std::cout << "Array after nth_element(begin, second, last, greater-op):" << std::endl
                << "\t[" << to_string(arr, arr + n) << "]" << std::endl;

        TEST_OK(arr[1] == 7);
    }
}


