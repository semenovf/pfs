#include <cstring>
#include <iostream>
#include <pfs/test.hpp>
#include <pfs/vector.hpp>

#include "test_minmaxelement.hpp"
#include "test_sort.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_minmaxelement();
    test_partial_sort();

    return END_TESTS;
}
