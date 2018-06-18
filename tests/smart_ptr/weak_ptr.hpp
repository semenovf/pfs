#pragma once
#include <pfs/memory.hpp>

void test_weak_ptr ()
{
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                               weak_ptr                                //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    ADD_TESTS(6);

    pfs::weak_ptr<int> wp0;
    TEST_OK2(wp0.use_count() == 0, "Create weak_ptr with default constructor");
    TEST_OK2(wp0.use_count() == 0, "weak_ptr().use_count() == 0");
    TEST_OK2(wp0.expired() == true, "weak_ptr().expired() == true");
    TEST_OK2(wp0.lock() == 0, "weak_ptr().lock() is nullptr");


    pfs::weak_ptr<int> wp1;

    {
        pfs::shared_ptr<int> sp1 = pfs::make_shared<int>(42);
        wp1 = sp1;

        TEST_OK2(wp1.use_count() == 1, "Assign weak_ptr from shared_ptr");
    }

    TEST_OK2(wp1.expired(), "weak_ptr automatically expired out of the scope");

    // TODO Continue tests ...
}

