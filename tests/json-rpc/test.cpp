#include <pfs/test/test.hpp>
#include <pfs/json/json.hpp>
#include <pfs/json/rpc.hpp>
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/traits/stdcxx/map.hpp>

// Enabled by `qt_enable`
#ifdef HAVE_QT_CORE
#   include <pfs/traits/qt/string.hpp>
#   include <pfs/traits/qt/vector.hpp>
#   include <pfs/traits/qt/map.hpp>
#endif

namespace stdcxx {

typedef pfs::string<pfs::stdcxx::string> string_t;
typedef pfs::json::traits<
          bool
        , intmax_t
        , double
        , string_t
        , pfs::stdcxx::vector
        , pfs::stdcxx::map> json_traits;

typedef pfs::json::json<json_traits> json_t;

typedef pfs::json::rpc::traits<json_t
        , string_t
        , int
        , pfs::qt::map> traits_t;

} //stdcxx::json

#ifdef HAVE_QT_CORE

namespace qt {

typedef pfs::string<pfs::qt::string> string_t;
typedef pfs::json::traits<
          bool
        , intmax_t
        , double
        , string_t
        , pfs::qt::vector
        , pfs::qt::map> json_traits;

typedef pfs::json::json<json_traits> json_t;

typedef pfs::json::rpc::traits<json_t
        , string_t
        , int
        , pfs::qt::map> traits_t;
} // qt

#endif


template <typename Traits>
void test ()
{
    ADD_TESTS(0);
    
    typedef pfs::json::rpc::server<Traits> server_t;
    typedef pfs::json::rpc::client<Traits> client_t;
}

int main ()
{
    BEGIN_TESTS(0);

    test<stdcxx::traits_t>();

#ifdef HAVE_QT_CORE
    test<qt::traits_t>();
#endif

    return END_TESTS;
}
