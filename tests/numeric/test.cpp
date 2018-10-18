#include <cstring>
#include <iostream>
#include <pfs/test.hpp>

#include "test_iota.hpp"
#include "test_summation.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_summation();
    test_iota();

    return END_TESTS;
}
