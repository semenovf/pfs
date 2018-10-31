#include <cstring>
#include <iostream>
#include <pfs/test.hpp>
#include <pfs/math.hpp>

#include "test_sign_of.hpp"
#include "test_gcd_lcm.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_sign_of();
    test_gcd_lcm();

    return END_TESTS;
}
