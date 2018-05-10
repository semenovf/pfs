#include <iostream>
#include <pfs/test.hpp>
#include <pfs/memory.hpp>
#include <pfs/functional.hpp>

static int const INT_RESULT = 123;
static int const SUM_RESULT = 15;

int e ()
{
    return INT_RESULT;
}

int f (int n1, int n2 , int n3, int const & n4, int n5)
{
    std::cout << n1
            << ' ' << n2
            << ' ' << n3
            << ' ' << n4
            << ' ' << n5
            << std::endl;

    return n1 + n2 + n3 + n4 + n5;
}

struct Bar
{
    int sum (int n1, int n2)
    {
        std::cout << n1
                << ' ' << n2
                << std::endl;
        return n1 + n2;
    }

    Bar () : data(INT_RESULT) {}

    int data;
};

struct Func
{
    typedef int result_type;

    int operator () () const
    {
        return INT_RESULT;
    }
};

#include "test_bind.hpp"
#include "test_function.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_bind();
    test_function::run();

    return END_TESTS;
}
