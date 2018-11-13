#include <cstring>
#include <iostream>
#include <pfs/test.hpp>
#include <pfs/math.hpp>

#include "test_ceil.hpp"
#include "test_floor.hpp"
#include "test_sign_of.hpp"
#include "test_gcd_lcm.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_ceil();
    test_floor();
    test_sign_of();
    test_gcd_lcm();

    return END_TESTS;
}
