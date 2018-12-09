#include <pfs/test.hpp>
#include <pfs/string.hpp>
#include <pfs/lexical_cast.hpp>
//#include <pfs/rational.hpp>
// #include "test_string_to_uint.hpp"
// #include "test_string_to_int.hpp"
// #include "test_string_to_real.hpp"
// #include "test_string_to_rational.hpp"
// #include "test_lexical_cast_uint.hpp"
// #include "test_lexical_cast_int.hpp"
// #include "test_lexical_cast_real.hpp"

extern void test_parse_digits ();

int main ()
{
    BEGIN_TESTS(0);

    test_parse_digits();
//     test_string_to_uint();
//     test_string_to_int();
//     test_string_to_real<float>();
//     test_string_to_real<double>();
//     test_string_to_rational();
//     test_lexical_cast_uint();
//     test_lexical_cast_int();
//     test_lexical_cast_float();

    return END_TESTS;
}
