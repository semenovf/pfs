//#include <istream>
//#include <iterator>
//#include <streambuf>
//#include <fstream>
//#include "pfs/type_traits.hpp"
//#include "pfs/string.hpp"
//#include "pfs/io/device.hpp"
//#include "pfs/io/iterator.hpp"
//#include "pfs/unicode/utf8_iterator.hpp"
//#include "pfs/traits/stdcxx/string.hpp"
//
//#if HAVE_QT
//#   include "pfs/traits/qt/string.hpp"
//#endif

namespace pfs {

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
//#if HAVE_QT
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

