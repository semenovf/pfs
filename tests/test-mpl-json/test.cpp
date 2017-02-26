/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing pfs::mpl::json.
 */

#include <pfs/test/test.hpp>
#include <pfs/json/value.hpp>
//
//#include <cstring>
//#include <cwchar>
//#include <iostream>
//#include <sstream>
#include <pfs/foundation/cxx/stdcxx/string.hpp>
#include <pfs/foundation/cxx/stdcxx/map.hpp>
#include <pfs/foundation/cxx/stdcxx/vector.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/foundation/cxx/qt/string.hpp>
#   include <pfs/foundation/cxx/qt/map.hpp>
#   include <pfs/foundation/cxx/qt/vector.hpp>
#endif

#include "test_basic.hpp"
#include "test_iterator.hpp"
//#include "test_compare.hpp"
//#include "test_find.hpp"
//#include "test_substr.hpp"
//#include "test_cast.hpp"
//#include "test_erase.hpp"

namespace stdcxx {
namespace json {

typedef pfs::json::value<
          char
        , bool
        , intmax_t
        , uintmax_t
        , double
        , stdcxx::vector
        , stdcxx::map>
    value;

}} //stdcxx::json

#ifdef QT_CORE_LIB

namespace qt {
namespace json {

typedef pfs::json::value<
          QChar
        , bool
        , intmax_t
        , uintmax_t, double
        , qt::vector
        , qt::map> 
    value;

#endif

}} // qt::json

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_basic::test<stdcxx::json::value>();
    test_iterator::test<stdcxx::json::value>();

#ifdef QT_CORE_LIB
    test_basic::test<qt::json::value>();
    test_iterator::test<qt::json::value>();
#endif

	return END_TESTS;
}
