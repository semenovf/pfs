#include <cstring>
#include <iostream>
#include <pfs/test.hpp>

#include "test_summation.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_summation();

    return END_TESTS;
}
