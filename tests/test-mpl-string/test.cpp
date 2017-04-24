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
#define STR_CONCAT  6

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
        , "ABCDEFABCDEF"
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
        , L"ABCDEFABCDEF"
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
        , QString("ABCDEFABCDEF")
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
#include "test_append.hpp"

int main (int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

    test_basic<std::string>();
    test_basic<std::wstring>();
    test_find<std::string>();
    test_find<std::wstring>();
    test_substr<std::string>();
    test_substr<std::wstring>();
    test_c_str_cast<std::string>();
//    test_c_str_cast<std::wstring>();    // TODO
    test_compare<std::string>();
    test_compare<std::wstring>();
    test_compare_cstr<std::string>();
    //test_compare_cstr<wchar_t>();
    
    test_erase<std::string>();
    test_erase<std::wstring>();

    test_append<std::string>();
    test_append<std::wstring>();

#ifdef QT_CORE_LIB
    test_basic<QString>();
    test_find<QString>();
    test_substr<QString>();
    test_c_str_cast<QString>();
    
    test_compare<QString>();
    test_compare_cstr<QString>();
    
    test_erase<QString>();
    test_append<QString>();
#endif

    return END_TESTS;
}
