#include <pfs/test.hpp>
#include <pfs/string.hpp>
#include <pfs/string.hpp>
#include "test_regex_match.hpp"

typedef pfs::string string_type;

int main ()
{
    BEGIN_TESTS(0);

    test_regex_match<string_type>();

    return END_TESTS;
}

