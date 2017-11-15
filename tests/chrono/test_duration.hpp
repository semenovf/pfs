#ifndef TEST_DURATION_HPP
#define TEST_DURATION_HPP

#include <pfs/chrono.hpp>

void test_duration ()
{
    ADD_TESTS(3);

    // FIXME
#if __FIXME__    
    pfs::chrono::milliseconds ms(3); // 3 milliseconds
    pfs::chrono::microseconds us = 2 * ms; // 6000 microseconds constructed from 3 milliseconds
    pfs::chrono::duration<double, pfs::ratio<1, 30> > hz30(3.5); // 30Hz clock using fractional ticks
 
    TEST_OK2(ms.count() == 3, "3 ms duration has 3 ticks");
    TEST_OK2(us.count() == 6000, "6000 us duration has 6000 ticks");
    TEST_OK2(hz30.count() == 3.5f, "3.5 30Hz duration has 3.5 ticks");
#endif
}

#endif /* TEST_DURATION_HPP */

