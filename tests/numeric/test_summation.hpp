#pragma once
#include "pfs/numeric.hpp"

void test_summation ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Summation                                 //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(7);

    {
        int arr[] = { -5, 2, -10, 10, 3, 7, 0 };
        int n = sizeof(arr)/sizeof(arr[0]);
        int sum = pfs::summate<pfs::naive_summation>(arr, arr + n, 0);
        TEST_OK2(sum == 7, "Naive summation of integers");
    }

    {
        int arr[] = { -5, 2, -10, 10, 3, 7, 0 };
        int n = sizeof(arr)/sizeof(arr[0]);
        int sum = pfs::summate<pfs::compensated_summation>(arr, arr + n, 0);
        TEST_OK2(sum == 7, "Compensated summation of integers (same as Naive)");
    }

    {
        float arr[] = { -5.1f, 2.4f, -10.9f, 10.4f, 3.0f, 7.3f, 0.2f };
        int n = sizeof(arr)/sizeof(arr[0]);
        float sum = pfs::summate<pfs::naive_summation>(arr, arr + n, float(0));
        TEST_OK2(sum >= float(7.3) && sum <= float(7.31), "Naive summation of floats");
    }

    {
        float arr[] = { -5.1f, 2.4f, -10.9f, 10.4f, 3.0f, 7.3f, 0.2f };
        int n = sizeof(arr)/sizeof(arr[0]);
        float sum = pfs::summate<pfs::compensated_summation>(arr, arr + n, float(0));
        TEST_OK2(sum >= float(7.3) && sum <= float(7.31), "Compensated summation of floats");
    }

    {
        double arr[] = { double(-5.1), double(2.4), double(-10.9), double(10.4)
                , double(3.0), double(7.3), double(0.2) };
        int n = sizeof(arr)/sizeof(arr[0]);
        double sum = pfs::summate<pfs::naive_summation>(arr, arr + n, double(0));
        TEST_OK2(sum >= double(7.3) && sum <= double(7.31), "Naive summation of doubles");
    }

    {
        double arr[] = { double(-5.1), double(2.4), double(-10.9), double(10.4)
                , double(3.0), double(7.3), double(0.2) };
        int n = sizeof(arr)/sizeof(arr[0]);
        double sum = pfs::summate<pfs::compensated_summation>(arr, arr + n, double(0));
        TEST_OK2(sum >= double(7.3) && sum <= double(7.31), "Compensated summation of doubles");
    }

    {
        static double const PI = 3.1415926535897932384626;
        static int const COUNT = 10000;

        double naive_pi_sum = 0;

        for (int i = 0; i < COUNT; i++)
            naive_pi_sum += PI;

        double arr[COUNT];

        for (int i = 0; i < COUNT; i++)
            arr[i] = PI;

        double compensated_pi_sum = pfs::summate<pfs::compensated_summation>(arr, arr + COUNT, double(0));

        std::cout << "Naive sum of 10000 PIs = " << std::setprecision(28) << naive_pi_sum << std::endl;
        std::cout << "Kahan sum of 10000 PIs = " << std::setprecision(28) << compensated_pi_sum << std::endl;

        std::cout << "It is seems that this two values are equal ... ";

        if (naive_pi_sum == compensated_pi_sum) {
            std::cout << "and this is true" << std::endl;
        } else {
            std::cout << "but this is false" << std::endl;
        }

        TEST_OK(naive_pi_sum != compensated_pi_sum);
    }
}

