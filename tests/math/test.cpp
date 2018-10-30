#include <cstring>
#include <iostream>
#include <pfs/test.hpp>
#include <pfs/math.hpp>

#include "test_sign_of.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_sign_of();

    return END_TESTS;
}
