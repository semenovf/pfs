#include <cstring>
#include <cwchar>
#include <iostream>
#include <sstream>
#include <pfs/config.h>
#include <pfs/test.hpp>
#include <pfs/limits.hpp>
#include <pfs/v2/string.hpp>
#include <pfs/v2/stdcxx/basic_string.hpp>

// Enabled by `qt_enable`
#ifdef HAVE_QT_CORE
//#   include <pfs/traits/qt/string.hpp>
#endif


template <typename ConstPointer>
char const * stringify_string_impl ();

template <>
inline char const * stringify_string_impl<char const *> ()
{ return "char"; }

template <>
inline char const * stringify_string_impl<wchar_t const *> ()
{ return "wchar_t"; }


#ifdef HAVE_QT_CORE

// template <>
// inline char const * stringify_string_impl<QChar const *> ()
// { return "QChar"; }

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

#if HAVE_QT_CORE
// template <>
// QChar const * string_samples<QChar const *> (int i)
// {
//     static QString s[] = {
//           QString("ABCDEF")
//         , QString("ABCDEF")
//         , QString("ABCDE")
//         , QString("BCDEF")
//         , QString("BCDE")
//         , QString()
//         , QString("ABCDEFABCDEF")
//     };
//
//     return s[i].constData();
// }
#endif

#include "test_constructors.hpp"

int main ()
{
    BEGIN_TESTS(0);

    test_constructors<pfs::string>();

#ifdef HAVE_QT_CORE
    //test_constructors<pfs::qt::string>();
#endif

    return END_TESTS;
}
