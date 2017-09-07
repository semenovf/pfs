/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing pfs::json.
 */

#include <pfs/test/test.hpp>
#include <pfs/json/json.hpp>
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/traits/stdcxx/map.hpp>

// Enabled by `qt_enable`
#ifdef HAVE_QT
#   include <pfs/traits/qt/string.hpp>
#   include <pfs/traits/qt/vector.hpp>
#   include <pfs/traits/qt/map.hpp>
#endif

#include "test_basic.hpp"
#include "test_iterator.hpp"
#include "test_parse.hpp"
#include "test_serialize.hpp"
#include "test_stringify.hpp"
#include "test_pretty_printer.hpp"
//#include "test_compare.hpp"
//#include "test_find.hpp"
//#include "test_substr.hpp"
//#include "test_cast.hpp"
//#include "test_erase.hpp"

namespace stdcxx {

typedef pfs::string<pfs::stdcxx::string> string_type;
typedef pfs::json::traits<
          bool
        , intmax_t
        , double
        , string_type
        , pfs::stdcxx::vector
        , pfs::stdcxx::map> json_traits;

typedef pfs::json::json<json_traits> json;

} //stdcxx::json

#ifdef HAVE_QT

namespace qt {

typedef pfs::string<pfs::qt::string> string_type;
typedef pfs::json::traits<
          bool
        , intmax_t
        , double
        , string_type
        , pfs::qt::vector
        , pfs::qt::map> json_traits;

typedef pfs::json::json<json_traits> json;

} // qt

#endif

int main ()
{
    BEGIN_TESTS(0);

    test_basic::test<stdcxx::json>();
    test_iterator::test<stdcxx::json>();
    test_parse::test<stdcxx::json>();
    test_serialize::test<stdcxx::json>();
    test_stringify::test<stdcxx::json>();
    test_pretty_printer::test<stdcxx::json>();

#ifdef HAVE_QT
    test_basic::test<qt::json>();
    test_iterator::test<qt::json>();
    test_parse::test<qt::json>();
    test_serialize::test<qt::json>();
    test_stringify::test<qt::json>();
    test_pretty_printer::test<qt::json>();
#endif

    return END_TESTS;
}
