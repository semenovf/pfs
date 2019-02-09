#include <cctype>
#include "pfs/string.hpp"
#include "pfs/unicode/char.hpp"
#include "pfs/unicode/unicode_iterator.hpp"

namespace pfs {

string to_lower (string::const_iterator first, string::const_iterator last)
{
    typedef pfs::unicode::unicode_iterator_traits<
            string::const_iterator>::iterator unicode_iterator;

    typedef pfs::unicode::unicode_iterator_traits<
            string::iterator>::output_iterator output_iterator;

    string result;

    output_iterator out(pfs::back_inserter(result));

    unicode_iterator f(first);
    unicode_iterator l(last);

    while (f != l)
        *out++ = unicode::to_lower(*f++);

    return result;
}

string to_upper (string::const_iterator first, string::const_iterator last)
{
    typedef pfs::unicode::unicode_iterator_traits<
            string::const_iterator>::iterator unicode_iterator;

    typedef pfs::unicode::unicode_iterator_traits<
            string::iterator>::output_iterator output_iterator;

    string result;

    output_iterator out(pfs::back_inserter(result));

    unicode_iterator f(first);
    unicode_iterator l(last);

    while (f != l)
        *out++ = unicode::to_upper(*f++);

    return result;
}

//template <typename StringType, typename OctetInputIt>
//StringType __read_all (OctetInputIt first, OctetInputIt last)
//{
//    typedef unicode::u8_input_iterator<OctetInputIt> utf8_input_iterator;
//
//    StringType result;
//
//    utf8_input_iterator it(first, last);
//    utf8_input_iterator end(last);
//
//    while (it != end)
//        result.push_back(*it++);
//
//    return result;
//}

//template <typename StringType, typename OctetInputIt>
//StringType __read_line (OctetInputIt first, OctetInputIt last)
//{
//    typedef unicode::utf8_input_iterator<OctetInputIt> utf8_input_iterator;
//
//    StringType result;
//
//    utf8_input_iterator it(first, last);
//    utf8_input_iterator end(last);
//
//    while (it != end && *it != '\n')
//        result.push_back(*it++);
//
//    return result;
//}

//typedef string<stdcxx::string> stdcxx_string_type;
//
//template <>
//stdcxx_string_type read_all<stdcxx_string_type> (std::basic_istream<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_all<stdcxx_string_type>(octet_input_iterator(is), octet_input_iterator());
//}
//
//template <>
//stdcxx_string_type read_line<stdcxx_string_type> (std::basic_istream<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_line<stdcxx_string_type>(octet_input_iterator(is), octet_input_iterator());
//}
//
//template <>
//stdcxx_string_type read_all<stdcxx_string_type> (std::basic_ifstream<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_all<stdcxx_string_type>(octet_input_iterator(is), octet_input_iterator());
//}
//
//template <>
//stdcxx_string_type read_line<stdcxx_string_type> (std::basic_ifstream<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_line<stdcxx_string_type>(octet_input_iterator(is), octet_input_iterator());
//}
//
//template <>
//stdcxx_string_type read_all<stdcxx_string_type> (std::basic_streambuf<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_all<stdcxx_string_type>(octet_input_iterator(& is), octet_input_iterator());
//}
//
//template <>
//stdcxx_string_type read_line<stdcxx_string_type> (std::basic_streambuf<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_line<stdcxx_string_type>(octet_input_iterator(& is), octet_input_iterator());
//}
//
//template <>
//stdcxx_string_type read_all<stdcxx_string_type> (io::device & dev)
//{
//    typedef io::input_iterator<char> octet_input_iterator;
//    return __read_all<stdcxx_string_type>(octet_input_iterator(dev), octet_input_iterator());
//}
//
//template <>
//stdcxx_string_type read_line<stdcxx_string_type> (io::device & dev)
//{
//    typedef io::input_iterator<char> octet_input_iterator;
//    return __read_line<stdcxx_string_type>(octet_input_iterator(dev), octet_input_iterator());
//}
//
//#if HAVE_QT_CORE
//
//typedef string<qt::string> qt_string_type;
//
//template <>
//qt_string_type read_all<qt_string_type> (std::basic_istream<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_all<qt_string_type>(octet_input_iterator(is), octet_input_iterator());
//}
//
//template <>
//qt_string_type read_line<qt_string_type> (std::basic_istream<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_line<qt_string_type>(octet_input_iterator(is), octet_input_iterator());
//}
//
//template <>
//qt_string_type read_all<qt_string_type> (std::basic_ifstream<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_all<qt_string_type>(octet_input_iterator(is), octet_input_iterator());
//}
//
//template <>
//qt_string_type read_line<qt_string_type> (std::basic_ifstream<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_line<qt_string_type>(octet_input_iterator(is), octet_input_iterator());
//}
//
//template <>
//qt_string_type read_all<qt_string_type> (std::basic_streambuf<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_all<qt_string_type>(octet_input_iterator(& is), octet_input_iterator());
//}
//
//template <>
//qt_string_type read_line<qt_string_type> (std::basic_streambuf<char> & is)
//{
//    typedef std::istreambuf_iterator<char> octet_input_iterator;
//    return __read_line<qt_string_type>(octet_input_iterator(& is), octet_input_iterator());
//}
//
//template <>
//qt_string_type read_all<qt_string_type> (io::device & dev)
//{
//    typedef io::input_iterator<char> octet_input_iterator;
//    return __read_all<qt_string_type>(octet_input_iterator(dev), octet_input_iterator());
//}
//
//template <>
//qt_string_type read_line<qt_string_type> (io::device & dev)
//{
//    typedef io::input_iterator<char> octet_input_iterator;
//    return __read_line<qt_string_type>(octet_input_iterator(dev), octet_input_iterator());
//}
//
//#endif

} // pfs
