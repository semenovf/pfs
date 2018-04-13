#ifndef __TEST_ROUTE_HPP__
#define __TEST_ROUTE_HPP__

#include <pfs/test.hpp>
#include <pfs/string.hpp>
#include <pfs/cli/route.hpp>
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/traits/stdcxx/list.hpp>
#include <pfs/traits/stdcxx/multimap.hpp>

namespace test {
namespace cli {

//struct stdcxx_cli_traits
//{
//    typedef pfs::mpl::string<char const *>  string_type;
//    typedef pfs::mpl::multimap<char, >typename traits_type::shortoptmap_traits shortoptmap_traits;
//    typedef typename traits_type::longoptmap_traits  longoptmap_traits;
//    typedef typename traits_type::optlist_traits     optlist_traits;
//
//    typedef typename traits_type::option_type        value_type;
//    typedef typename traits_type::string_type        string_type;
//    typedef typename traits_type::char_type          char_type;
//    typedef typename traits_type::shortoptmap_type   shortoptmap_type;
//    typedef typename traits_type::longoptmap_type    longoptmap_type;
//    typedef typename traits_type::optlist_type       optlist_type;
//};

void test_route ()
{
    ADD_TESTS(1);
    
    char const * __argv[] = { "/path/to/program.bin"
            , "domain"
            , "command"
            , "-b"
            , "-bcd"
            , "--option1"
            , "--option2=option2_arg"
            , "--option 3=option3 arg"
            , "--option-multi"
            , "--option-multi=123"
            , "--option-multi=3.14"
            , "--option-multi=string"
            , "--"
            , "arg1"
            , "--arg2"
    };
    
    int __argc = sizeof(__argv)/sizeof(__argv[0]);
   
    typedef pfs::cli::traits<std::string
        , pfs::container<std::list>
        , pfs::container<std::multimap> >  traits_type;
    typedef pfs::cli::cmdline<std::string> cmdline_type;
    typedef pfs::cli::route<traits_type>   route_type;
    
    route_type r1;
    route_type r2;
    route_type r3;
    route_type r4;

    r1.set_program("program.bin");
    r1.add_domain("domain");
    r1.add_domain("command");

    r2.set_program("program.bin");
    r2.add_domain("domain");
    r2.add_domain("command1");

    r3.set_program("program.bin");
    r3.add_domain("domain1");
    r3.add_domain("command1");
    
    bool        yes          = true;
//    short       short_value  = 0;
//    int         int_value    = 0;
//    long        long_value   = 0;
//    intmax_t    intmax_value = 0;
//    float       float_value  = 0.0f;
//    double      double_value = 0.0f;
//    real_t      real_value   = 0.0f;   
//    pfs::string string_value;
//    
//    r.add(& yes          ,      _u8("bool")  , required);
//    r.add(& short_value  ,      _u8("short") , required);
//    r.add(& int_value    , 'i', _u8("int")   , required);
//    r.add(& long_value   ,      _u8("long")  , required);
//    r.add(& intmax_value ,      _u8("intmax"), required);
//    r.add(& float_value  , 'f'               , required);
//    r.add(& double_value , 'd', _u8("double"), required);
//    r.add(& real_value   ,      _u8("real")  , required);
//    r.add(& string_value , 's', _u8("string"), required);
    
    TEST_OK(r1.parse(__argc, __argv, pfs::mpl::cli::relaxed_flags));
    TEST_OK(! r2.parse(__argc, __argv, pfs::mpl::cli::relaxed_flags));
    TEST_OK(! r3.parse(__argc, __argv, pfs::mpl::cli::relaxed_flags));
    TEST_OK(! r4.parse(__argc, __argv, pfs::mpl::cli::relaxed_flags));
}

}}

#endif /* __TEST_ROUTE_HPP__ */