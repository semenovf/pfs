#include <pfs/test.hpp>
#include <pfs/string.hpp>
#include <pfs/string.hpp>
#include "test_regex_match.hpp"

typedef pfs::string string_t;

int main ()
{
    BEGIN_TESTS(0);

    test_regex_match<string_t>();

    return END_TESTS;
}

