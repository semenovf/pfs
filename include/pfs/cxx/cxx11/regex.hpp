/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   regex.hpp
 * Author: wladt
 *
 * Created on February 2, 2017, 10:10 AM
 */

#ifndef __PFS_CXX11_REGEX_HPP__
#define __PFS_CXX11_REGEX_HPP__

#include <regex>

namespace pfs {

template <typename CharT = char, typename Traits = std::regex_traits<CharT>> 
using basic_regex = std::basic_regex<CharT, Traits>;

typedef basic_regex<char>    regex;
typedef basic_regex<wchar_t> wregex;

} // namespace pfs

#endif /* __PFS_CXX11_REGEX_HPP__ */

