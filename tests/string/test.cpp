/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing pfs::mpl::string.hpp.
 */

#include <cstring>
#include <cwchar>
#include <iostream>
#include <sstream>
#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/limits.hpp>
#include <pfs/traits/stdcxx/string.hpp>

// TODO Delete line below
#undef QT_CORE_LIB

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/traits/qt/string.hpp>
#endif

template <typename ConstPointer>
char const * stringify_string_impl ();

template <>
inline char const * stringify_string_impl<char const *> ()
{ return "char"; }

template <>
inline char const * stringify_string_impl<wchar_t const *> ()
{ return "wchar_t"; }


#ifdef QT_CORE_LIB

template <>
inline char const * stringify_string_impl<QChar const *> ()
{ return "QChar"; }

#endif

template <typename ConstPointer>
void test_description (char const * title)
{
    std::cout << "================================================================================\n"
            << title << ": string implemented using <" 
            << stringify_string_impl<ConstPointer>() << "> ==="
            << std::endl
            << "================================================================================\n";
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

template <typename ConstPointer>
ConstPointer string_samples (int i);

template <typename ConstPointer>
ConstPointer string_samples_ru (int i);

template <>
char const * string_samples<char const *> (int i)
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
char const * string_samples_ru<char const *> (int i)
{
    static char const * s[] = {
          "АБВГДE"
        , "АБВГДE"
        , "АБВГД"
        , "БВГДE"
        , "БВГД"
        , ""
        , "АБВГДEАБВГДE"
    };
    
    return s[i];
}

template <>
wchar_t const * string_samples<wchar_t const *> (int i)
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
QChar const * string_samples<QChar const *> (int i)
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
#include "test_find.hpp"
#include "test_substr.hpp"
//#include "test_cast.hpp"
//#include "test_erase.hpp"
//#include "test_append.hpp"
//#include "test_compare.hpp"
#include "test_to_string.hpp"

int main (int argc, char *argv[])
{
    PFS_UNUSED(argc);
    PFS_UNUSED(argv);

    BEGIN_TESTS(0);

//    test_basic<pfs::traits::stdcxx::string>();
//    test_basic<pfs::traits::stdcxx::wstring>();
//    test_find<pfs::traits::stdcxx::string>();
//    test_find<pfs::traits::stdcxx::wstring>();
    test_substr<pfs::traits::stdcxx::string>();
//    test_substr<pfs::traits::stdcxx::wstring>();
//    test_c_str_cast<std::string>();
////    test_c_str_cast<std::wstring>();    // TODO
//    test_compare<std::string>();
//    test_compare<std::wstring>();
//    test_compare_cstr<std::string>();
//    //test_compare_cstr<wchar_t>();
//    
//    test_erase<std::string>();
//    test_erase<std::wstring>();
//
//    test_append<std::string>();
//    test_append<std::wstring>();
//    test_to_string<pfs::traits::stdcxx::string>();

#ifdef QT_CORE_LIB
//    test_basic<pfs::traits::qt::string>();
//    test_find<pfs::traits::qt::string>();
//    test_substr<pfs::traits::qt::string>();
//    test_c_str_cast<QString>();
//    
//    test_compare<QString>();
//    test_compare_cstr<QString>();
//    
//    test_erase<QString>();
//    test_append<QString>();
//    test_to_string<pfs::traits::qt::string>();
#endif

    return END_TESTS;
}
