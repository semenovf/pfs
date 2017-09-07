
/* 
 * File:   u8string.hpp
 * Author: wladt
 *
 * Created on September 7, 2017, 11:43 AM
 */

#ifndef __PFS_TEST_U8STRING_HPP__
#define __PFS_TEST_U8STRING_HPP__

#include "pfs/byte_string.hpp"

#if HAVE_QT
#   include <QByteArray>
#endif

template <typename StringImplType>
void test_u8string ()
{
    typedef pfs::string<StringImplType>          string_type;
    typedef typename string_type::const_pointer  const_pointer;

    test_description<const_pointer>(__PRETTY_FUNCTION__);
    
    ADD_TESTS(1);

    string_type s("ABCabc");
    
    pfs::byte_string b1 = pfs::u8string<pfs::byte_string>(s);
    
    TEST_OK(b1 == "ABCabc");
    
#if HAVE_QT
    ADD_TESTS(1);
    
    QByteArray b2 = pfs::u8string<QByteArray>(s);
    TEST_OK(b2 == "ABCabc");
#endif    

}

#endif /* __PFS_TEST_U8STRING_HPP__ */

