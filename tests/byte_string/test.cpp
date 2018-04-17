#include <cstring>
#include <iostream>
#include <pfs/test.hpp>
#include <pfs/byte_string.hpp>

static char const * raw_chars = "byte_string";
static pfs::byte_string::const_pointer raw_bytes = reinterpret_cast<pfs::byte_string::const_pointer>(raw_chars);
static pfs::byte_string bs_sample(raw_bytes);

typedef pfs::byte_string            byte_string;
typedef byte_string::size_type      size_type;
typedef byte_string::value_type     value_type;
typedef byte_string::pointer        pointer;
typedef byte_string::const_pointer  const_pointer;
typedef byte_string::iterator       iterator;
typedef byte_string::const_iterator const_iterator;
typedef byte_string::reverse_iterator reverse_iterator;
typedef byte_string::const_reverse_iterator const_reverse_iterator;

#include "test_constructors.hpp"
#include "test_assignments.hpp"
#include "test_access.hpp"
#include "test_iterators.hpp"
#include "test_capacity.hpp"
#include "test_operations.hpp"
#include "test_search.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_constructors();
    test_assignments();
    test_access();
    test_iterators();
    test_capacity();
    test_operations();
    test_search();

    return END_TESTS;
}
