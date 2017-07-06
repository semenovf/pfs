/* 
 * File:   ctype.hpp
 * Author: wladt
 *
 * Created on March 16, 2017, 7:34 PM
 */

#ifndef __PFS_COMPILER_GNUC_CTYPE_HPP__
#define __PFS_COMPILER_GNUC_CTYPE_HPP__

#include <cctype>
#include <cwctype>
#include <locale>

namespace pfs {

//
// ISALPHA(3) man page
// ...
// POSIX.1-2008 marks isascii() as obsolete, noting that it cannot 
// be used portably in a localized application.
// ...
//

template <>
inline bool is_alnum<char> (char c)
{
    return std::isalnum(c);
}

template <>
inline bool is_alpha<char> (char c)
{
    return std::isalpha(c);
}

template <>
inline bool is_cntrl<char> (char c)
{
    return std::iscntrl(c);
}

template <>
inline bool is_digit<char>(char c)
{
    return std::isdigit(c);
}

template <>
inline bool is_graph<char> (char c)
{
    return std::isgraph(c);
}

template <>
inline bool is_lower<char> (char c)
{
    return std::islower(c);
}

template <>
inline bool is_print<char> (char c)
{
    return std::isprint(c);
}

template <>
inline bool is_punct<char> (char c)
{
    return std::ispunct(c);
}

template <>
inline bool is_space<char> (char c)
{
    return std::isspace(c);
}

template <>
inline bool is_upper<char> (char c)
{
    return std::isupper(c);
}

template <>
inline bool is_xdigit<char> (char c)
{
    return std::isxdigit(c);
}

//---

template <>
inline bool is_alnum<wchar_t> (wchar_t c)
{
    return std::iswalnum(c);
}

template <>
inline bool is_alpha<wchar_t> (wchar_t c)
{
    return std::iswalpha(c);
}

template <>
inline bool is_cntrl<wchar_t> (wchar_t c)
{
    return std::iswcntrl(c);
}

template <>
inline bool is_digit<wchar_t>(wchar_t c)
{
    return std::iswdigit(c);
}

template <>
inline bool is_graph<wchar_t> (wchar_t c)
{
    return std::iswgraph(c);
}

template <>
inline bool is_lower<wchar_t> (wchar_t c)
{
    return std::iswlower(c);
}

template <>
inline bool is_print<wchar_t> (wchar_t c)
{
    return std::iswprint(c);
}

template <>
inline bool is_punct<wchar_t> (wchar_t c)
{
    return std::iswpunct(c);
}

template <>
inline bool is_space<wchar_t> (wchar_t c)
{
    return std::iswspace(c);
}

template <>
inline bool is_upper<wchar_t> (wchar_t c)
{
    return std::iswupper(c);
}

template <>
inline bool is_xdigit<wchar_t> (wchar_t c)
{
    return std::iswxdigit(c);
}

} // pfs

#endif /* __PFS_COMPILER_GNUC_CTYPE_HPP__ */

