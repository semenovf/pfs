/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing pfs::mpl::string.hpp.
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

#include <cstring>
#include <cwchar>
#include <iostream>
#include <sstream>
#include <pfs/foundation/cxx/stdcxx/string.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/foundation/cxx/qt/string.hpp>
#endif

template <typename CharT>
char const * stringify_string_impl ();

template <>
inline char const * stringify_string_impl<char> ()
{ return "char"; }

template <>
inline char const * stringify_string_impl<wchar_t> ()
{ return "wchar_t"; }


#ifdef QT_CORE_LIB

template <>
inline char const * stringify_string_impl<QChar> ()
{ return "QChar"; }

#endif

template <typename CharT>
void test_description (char const * title)
{
    
    std::cout << "=== "
            << title << ": string implemented using <" 
            << stringify_string_impl<CharT>() << "> ==="
            << std::endl;
}

#define STR_ORIG    0
#define STR_SAME    1
#define STR_LESS    2
#define STR_GREATER 3
#define STR_HEAD    2
#define STR_TAIL    3
#define STR_MID     4
#define STR_DIFF    4
#define STR_EMPTY   5

template <typename CharT>
CharT const * string_samples (int i);

template <>
char const * string_samples<char> (int i)
{
    static char const * s[] = {
          "ABCDEF"
        , "ABCDEF"
        , "ABCDE"
        , "BCDEF"
        , "BCDE"
        , ""
    };
    
    return s[i];
}

template <>
wchar_t const * string_samples<wchar_t> (int i)
{
    static wchar_t const * s[] = {
          L"ABCDEF"
        , L"ABCDEF"
        , L"ABCDE"
        , L"BCDEF"
        , L"BCDE"
        , L""
    };
    
    return s[i];
}

#ifdef QT_CORE_LIB
template <>
QChar const * string_samples<QChar> (int i)
{
    static QString s[] = {
          QString("ABCDEF")
        , QString("ABCDEF")
        , QString("ABCDE")
        , QString("BCDEF")
        , QString("BCDE")
        , QString()
    };
    
    return s[i].constData();
}
#endif

#include "test_basic.hpp"
#include "test_compare.hpp"
#include "test_find.hpp"
#include "test_substr.hpp"
#include "test_cast.hpp"
#include "test_erase.hpp"

int main (int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

    test_basic<foundation::stdcxx, char>();
    test_basic<foundation::stdcxx, wchar_t>();
    test_find<foundation::stdcxx, char>();
    test_find<foundation::stdcxx, wchar_t>();
    test_substr<foundation::stdcxx, char>();
    test_substr<foundation::stdcxx, wchar_t>();
    test_c_str_cast<foundation::stdcxx, char>();
//    test_c_str_cast<std::wstring>();    // TODO
    test_compare<foundation::stdcxx, char>();
    test_compare<foundation::stdcxx, wchar_t>();
    test_compare_cstr<foundation::stdcxx, char>();
    //test_compare_cstr<wchar_t>();
    
    test_erase<foundation::stdcxx, char>();
    test_erase<foundation::stdcxx, wchar_t>();

#ifdef QT_CORE_LIB
    test_basic<foundation::qt, QChar>();
    test_find<foundation::qt, QChar>();
    test_substr<foundation::qt, QChar>();
    test_c_str_cast<foundation::qt, QChar>();
    
    test_compare<foundation::qt, QChar>();
    test_compare_cstr<foundation::qt, QChar>();
    
    test_erase<foundation::qt, QChar>();
#endif

    return END_TESTS;
}
