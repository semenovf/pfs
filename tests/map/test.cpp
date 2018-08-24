#include <cstring>
#include <iostream>
#include <pfs/test.hpp>
#include <pfs/map.hpp>

#include "test_constructors.hpp"
#include "test_assignments.hpp"
// #include "test_access.hpp"
// #include "test_iterators.hpp"
// #include "test_capacity.hpp"
// #include "test_operations.hpp"
// #include "test_search.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_constructors();
    test_assignments();
//     test_access();
//     test_iterators();
//     test_capacity();
//     test_operations();
//     test_search();

    return END_TESTS;
}
