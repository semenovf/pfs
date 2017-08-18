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
//#include "test_compare.hpp"
//#include "test_find.hpp"
//#include "test_substr.hpp"
//#include "test_cast.hpp"
//#include "test_erase.hpp"

namespace stdcxx {

typedef pfs::string<pfs::traits::stdcxx::string> string_type;

typedef pfs::json::json<
          bool
        , intmax_t
        , double
        , string_type
        , pfs::traits::stdcxx::vector
        , pfs::traits::stdcxx::map> json;

} //stdcxx::json

#ifdef HAVE_QT

namespace qt {

typedef pfs::string<pfs::traits::qt::string> string_type;

typedef pfs::json::json<
          bool
        , intmax_t
        , double
        , string_type
        , pfs::traits::qt::vector
        , pfs::traits::qt::map>
    json;

} // qt

#endif

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_basic::test<stdcxx::json>();
    test_iterator::test<stdcxx::json>();
    test_parse::test<stdcxx::json>();

#ifdef HAVE_QT
    test_basic::test<qt::json>();
    test_iterator::test<qt::json>();
    test_parse::test<qt::json>();
#endif

	return END_TESTS;
}
