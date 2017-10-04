
/* 
 * File:   iterator.hpp
 * Author: wladt
 *
 * Created on October 3, 2017, 12:06 PM
 */

#ifndef __PFS_IO_ITERATOR_HPP__
#define __PFS_IO_ITERATOR_HPP__

#include <pfs/iterator.hpp>
#include <pfs/io/device.hpp>

namespace pfs {
namespace io {

// TODO Implement io::input_iterator
#if __TODO__

template <typename CharType>
class input_iterator : public pfs::iterator_facade<
          pfs::input_iterator_tag
        , forward_iterator
        , CharType, CharType *, CharType &>
{
public:
    typedef CharType char_type;

private:
    device * _pd;
    char_type _value;

public:
    input_iterator ()
        : _pd(0)
    {}
    
    input_iterator (device & d)
        : _pd(& d)
    {
        ++*this;
    }

    input_iterator (input_iterator const & rhs)
        : _pd(rhs._pd)
        , _value(rhs._value)
    {}

    static reference ref (input_iterator & it)
    {
        return it._value;
    }
    
    static pointer ptr (input_iterator & it)
    {
        return & it._value;
    }
    
    static void increment (input_iterator & it, difference_type)
    {
        pfs::error_code ec;
                
        it._d.read(& it._c, sizeof(char_type), ec);
        
        if (!ec)
            throw io_exception(ec);
    }
    
    static bool equals (input_iterator const & it1, input_iterator const & it2)
    {
        int flag = 0;
        
        if (it1._d.is_null() || it1._d.at_end())
            flag |= 0x01;

        if (it2._d.is_null() || it2._d.at_end())
            flag |= 0x02;

        return flag == 0x03;
    }
    
private:
    void read ()
    {
        if (_pd) {
            pfs::error_code ec;
            ssize_t n = _pd->read(& _value, sizeof(char_type), ec);

            if (n <= 0)
               _pd = 0;
        }
    }
};

#endif

}} // pfs::io

#endif /* __PFS_IO_ITERATOR_HPP__ */

