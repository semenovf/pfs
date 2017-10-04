#include <istream>
#include <iterator>
#include <streambuf>
#include "pfs/type_traits.hpp"
#include "pfs/string.hpp"
#include "pfs/unicode/utf8_iterator.hpp"
#include "pfs/traits/stdcxx/string.hpp"

#if HAVE_QT
#   include "pfs/traits/qt/string.hpp"
#endif

namespace pfs {

typedef string<stdcxx::string> stdcxx_string_type;

template <>
stdcxx_string_type read_all<stdcxx_string_type> (std::basic_istream<char> & is)
{
    typedef std::istreambuf_iterator<char> octet_input_iterator;
    typedef unicode::utf8_input_iterator<octet_input_iterator> utf8_input_iterator;
    
    stdcxx_string_type result;
    
    octet_input_iterator i1(is);
    octet_input_iterator i2;
            
    utf8_input_iterator it(i1, i2);
    utf8_input_iterator last(i2);

    while (it != last)
        result.push_back(*it++);
    
    return result;
}

template <>
stdcxx_string_type read_all<stdcxx_string_type> (std::basic_streambuf<char> & is)
{
    typedef std::istreambuf_iterator<char> octet_input_iterator;
    typedef unicode::utf8_input_iterator<octet_input_iterator> utf8_input_iterator;
    
    stdcxx_string_type result;
    
    octet_input_iterator i1(& is);
    octet_input_iterator i2;
            
    utf8_input_iterator it(i1, i2);
    utf8_input_iterator last(i2);

    while (it != last)
        result.push_back(*it++);
    
    return result;
}

#if HAVE_QT

typedef string<qt::string> qt_string_type;

//template <>
//qt_string_type read_all<qt_string_type> (std::basic_istream<char> & is)
//{
//    return qt_string_type(istreambuf_iterator<char>(is)
//            , istreambuf_iterator<char>());
//}

#endif

} // pfs

