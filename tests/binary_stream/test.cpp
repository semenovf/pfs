#include <pfs/test.hpp>
#include "test_binary_ostream.hpp"
#include "test_binary_istream.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_binary_ostream();
    test_binary_istream();

    return END_TESTS;
}
