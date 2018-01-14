#ifndef __PFS_CXX17_STRING_VIEW_HPP__
#define __PFS_CXX17_STRING_VIEW_HPP__

#include <string_view>

namespace pfs {

template <typename CharT, typename Traits = std::char_traits<CharT>>
using basic_string_view = std::basic_string_view<CharT, Traits>;

typedef string_view    std::string_view;
typedef wstring_view   std::wstring_view;
typedef u16string_view std::u16string_view;
typedef u32string_view std::u32string_view;

} // pfs

#endif /* __PFS_CXX17_STRING_VIEW_HPP__ */

