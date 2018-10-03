#include <cstring>
#include <iostream>
#include <pfs/test.hpp>

// TODO Need to implement for C++ prior to C++11
#if __cplusplus >= 201103L

#include <pfs/variant.hpp>

#include "test_constructors.hpp"
#include "test_assignments.hpp"
#include "test_destructors.hpp"
#include "test_get.hpp"
#include "test_modifiers.hpp"
#include "test_comparisons.hpp"
#include "test_swap.hpp"
#include "test_visit.hpp"

#endif

int main ()
{
    BEGIN_TESTS(0);

#if __cplusplus >= 201103L
    test_constructors();
    test_assignments();
    test_destructors();
    test_get();
    test_modifiers();
    test_comparisons();
    test_swap();
    test_visit();
#endif

    return END_TESTS;
}

