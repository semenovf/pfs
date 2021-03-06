#include <pfs/test.hpp>
#include <pfs/json/json.hpp>
#include <pfs/string.hpp>
#include <pfs/vector.hpp>
#include <pfs/map.hpp>

// #ifdef HAVE_QT_CORE
// #   include <pfs/traits/qt/string.hpp>
// #   include <pfs/traits/qt/vector.hpp>
// #   include <pfs/traits/qt/map.hpp>
// #endif

#include "test_basic.hpp"
#include "test_iterator.hpp"
#include "test_parse.hpp"
#include "test_stringify.hpp"
#include "test_pretty_printer.hpp"
#include "test_reference_wrapper.hpp"
#include "test_serialize.hpp"
//#include "test_compare.hpp"
//#include "test_find.hpp"
//#include "test_substr.hpp"
//#include "test_cast.hpp"
//#include "test_erase.hpp"
#include "test_rpc.hpp"

namespace stdcxx {

typedef pfs::string string_type;
typedef pfs::json::json<
          bool
        , intmax_t
        , double
        , string_type
        , pfs::vector
        , pfs::map> json_t;

} //stdcxx::json

// #ifdef HAVE_QT_CORE
//
// namespace qt {
//
// typedef pfs::string<pfs::qt::string> string_type;
// typedef pfs::json::traits<
//           bool
//         , intmax_t
//         , double
//         , string_type
//         , pfs::qt::vector
//         , pfs::qt::map> json_traits;
//
// typedef pfs::json::json<json_traits> json;
//
// } // qt
//
// #endif

int main ()
{
    BEGIN_TESTS(0);

    test_basic::test<stdcxx::json_t>();
    test_iterator::test<stdcxx::json_t>();
    test_reference_wrapper::test<stdcxx::json_t>();
    test_parse::test<stdcxx::json_t>();
    test_stringify::test<stdcxx::json_t>();
    test_pretty_printer::test<stdcxx::json_t>();
    test_serialize::test<stdcxx::json_t>();
    test_rpc::test<stdcxx::json_t>();

// #ifdef HAVE_QT_CORE
//     std::cout << "===== HAVE_QT_CORE =====" << std::endl;
//     test_basic::test<qt::json>();
//     test_iterator::test<qt::json>();
//     test_reference_wrapper::test<qt::json>();
//     test_parse::test<qt::json>();
//     test_stringify::test<qt::json>();
//     test_pretty_printer::test<qt::json>();
//     test_serialize::test<qt::json>();
// #endif

    return END_TESTS;
}
