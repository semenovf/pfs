/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing pfs::mpl::json.
 */

#include <pfs/test/test.hpp>
#include <pfs/json/json.hpp>
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/traits/stdcxx/map.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
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
namespace json {

typedef pfs::string<pfs::traits::stdcxx::string> string_type;

typedef pfs::json::json<
          bool
        , intmax_t
        , double
        , string_type
        , pfs::traits::stdcxx::vector
        , pfs::traits::stdcxx::map> value;

}} //stdcxx::json

#ifdef QT_CORE_LIB

namespace qt {
namespace json {

typedef pfs::json::json<
          pfs::traits::qt::string
        , bool
        , intmax_t
        , uintmax_t
        , double
        , pfs::traits::qt::vector
        , pfs::traits::qt::map>
    value;

}} // qt::json

#endif

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_basic::test<stdcxx::json::value>();
    test_iterator::test<stdcxx::json::value>();
    test_parse::test<stdcxx::json::value>();

#ifdef QT_CORE_LIB
    test_basic::test<qt::json::value>();
    test_iterator::test<qt::json::value>();
#endif

	return END_TESTS;
}


//‘pfs::json::number<pfs::json::value<bool, long int, long unsigned int, double, pfs::traits::stdcxx::basic_string<pfs::string_value<char, std::__cxx11::basic_string<char> > >, pfs::traits::stdcxx::vector, pfs::traits::stdcxx::map>, pfs::traits::stdcxx::stack>’ (type ‘pfs::json::grammar<pfs::json::value<bool, long int, long unsigned int, double, pfs::traits::stdcxx::basic_string<pfs::string_value<char, std::__cxx11::basic_string<char> > >, pfs::traits::stdcxx::vector, pfs::traits::stdcxx::map>, pfs::traits::stdcxx::stack>::string_type(pfs::json::grammar<pfs::json::value<bool, long int, long unsigned int, double, pfs::traits::stdcxx::basic_string<pfs::string_value<char, std::__cxx11::basic_string<char> > >, pfs::traits::stdcxx::vector, pfs::traits::stdcxx::map>, pfs::traits::stdcxx::stack>::native_iterator, pfs::json::grammar<pfs::json::value<bool, long int, long unsigned int, double, pfs::traits::stdcxx::basic_string<pfs::string_value<char, std::__cxx11::basic_string<char> > >, pfs::traits::stdcxx::vector, pfs::traits::stdcxx::map>, pfs::traits::stdcxx::stack>::native_iterator) {aka pfs::string<pfs::traits::stdcxx::basic_string<pfs::string_value<char, std::__cxx11::basic_string<char> > > >(__gnu_cxx::__normal_iterator<const char*, std::__cxx11::basic_string<char> >, __gnu_cxx::__normal_iterator<const char*, std::__cxx11::basic_string<char> >)}’) to type ‘const pfs::string<pfs::traits::stdcxx::basic_string<pfs::string_value<char, std::__cxx11::basic_string<char> > > >&’