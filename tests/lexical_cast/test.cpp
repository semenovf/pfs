#include <pfs/test/test.hpp>
#include <pfs/lexical_cast.hpp>
#include <pfs/string.hpp>
#include <pfs/traits/stdcxx/string.hpp>

#ifdef QT_CORE_LIB
#   include <pfs/traits/qt/string.hpp>
#endif

#include "test_string_to_uint.hpp"
#include "test_string_to_int.hpp"
#include "test_string_to_real.hpp"
#include "test_lexical_cast_uint.hpp"
#include "test_lexical_cast_int.hpp"
#include "test_lexical_cast_real.hpp"

int main ()
{
    BEGIN_TESTS(0);

    {
        typedef pfs::string<pfs::stdcxx::string> string_type;

        test_string_to_uint<string_type>();
        test_string_to_int<string_type>();
        test_string_to_real<float, string_type>();
        test_string_to_real<double, string_type>();
        test_lexical_cast_uint<string_type>();
        test_lexical_cast_int<string_type>();
        test_lexical_cast_float<string_type>();
    }

#ifdef QT_CORE_LIB
    {
        //typedef pfs::string<pfs::qt::string> string_type;

//        test_string_to_uint<string_type>();
//        test_string_to_int<string_type>();
//        test_string_to_float<string_type>();
//        test_lexical_cast_uint<string_type>();
//        test_lexical_cast_int<string_type>();
//        test_lexical_cast_float<string_type>();
    }
#endif

    return END_TESTS;
}
