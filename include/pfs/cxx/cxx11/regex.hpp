/* 
 * File:   regex.hpp
 * Author: wladt
 *
 * Created on February 2, 2017, 10:10 AM
 */

#ifndef __PFS_CXX11_REGEX_HPP__
#define __PFS_CXX11_REGEX_HPP__

#include <regex>
//#include <pfs/string.hpp>

namespace pfs {

namespace regex_constants {

typedef std::regex_constants::match_flag_type match_flag_type;

constexpr match_flag_type match_default     = std::regex_constants::match_default;
constexpr match_flag_type match_not_bol     = std::regex_constants::match_not_bol;
constexpr match_flag_type match_not_eol     = std::regex_constants::match_not_eol;
constexpr match_flag_type match_not_bow     = std::regex_constants::match_not_bow;
constexpr match_flag_type match_not_eow     = std::regex_constants::match_not_eow;
constexpr match_flag_type match_any         = std::regex_constants::match_any;
constexpr match_flag_type match_not_null    = std::regex_constants::match_not_null;
constexpr match_flag_type match_continuous  = std::regex_constants::match_continuous;
constexpr match_flag_type match_prev_avail  = std::regex_constants::match_prev_avail;
constexpr match_flag_type format_default    = std::regex_constants::format_default;
constexpr match_flag_type format_sed        = std::regex_constants::format_sed;
constexpr match_flag_type format_no_copy    = std::regex_constants::format_no_copy;
constexpr match_flag_type format_first_only = std::regex_constants::format_first_only;

} // regex_constants

template <typename CharT = char, typename Traits = std::regex_traits<CharT>> 
using basic_regex = std::basic_regex<CharT, Traits>;

typedef basic_regex<char>    regex;
typedef basic_regex<wchar_t> wregex;

template <typename BidirIt, typename Alloc = std::allocator<std::sub_match<BidirIt>>>
using match_results = std::match_results<BidirIt, Alloc>;

using cmatch  = std::match_results<const char *>;
using wcmatch = std::match_results<const wchar_t *>;
using smatch  = std::match_results<std::string::const_iterator>;
using wsmatch = std::match_results<std::wstring::const_iterator>;

template <typename BidirIt, typename Alloc, typename CharT, typename Traits>
inline bool regex_match (BidirIt first, BidirIt last
        , match_results<BidirIt, Alloc> & m
        , basic_regex<CharT,Traits> & e
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return std::regex_match(first, last, m, e, flags);
}
    
template <typename BidirIt, typename CharT, typename Traits>
inline bool regex_match (BidirIt first, BidirIt last
        , basic_regex<CharT,Traits> const & e
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return std::regex_match(first, last, e, flags);
}

template< typename CharT, typename Alloc, typename Traits>
inline bool regex_match (CharT const * str
        , match_results<CharT const *, Alloc> & m
        , basic_regex<CharT,Traits> const & e
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return std::regex_match(str, m, e, flags);
}

template <typename StringT, typename Alloc, typename CharT, typename Traits>
inline bool regex_match (StringT const & s
        , match_results<typename StringT::const_iterator, Alloc> & m
        , basic_regex<CharT,Traits> const & e
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return std::regex_match(s, m, e, flags);
}

template <typename CharT, typename Traits>
inline bool regex_match (CharT const * str
        , basic_regex<CharT,Traits> const & e
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return std::regex_match(str, e, flags);
}
    
template <typename StringT, typename CharT, typename Traits>
inline bool regex_match (StringT  const & s
        , basic_regex<CharT,Traits> const & e
        , regex_constants::match_flag_type flags = regex_constants::match_default)
{
    return std::regex_match(s, e, flags);
}

// TODO Implement regex_match versions for pfs::string.

} // namespace pfs

#endif /* __PFS_CXX11_REGEX_HPP__ */

