
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

template <typename CharType>
class input_iterator : public pfs::iterator_facade<
          pfs::input_iterator_tag
        , input_iterator<CharType>
        , CharType, CharType *, CharType &>
{
    typedef pfs::iterator_facade<
          pfs::input_iterator_tag
        , input_iterator
        , CharType, CharType *, CharType &> base_class;
public:
    typedef typename base_class::pointer         pointer;
    typedef typename base_class::reference       reference;
    typedef typename base_class::difference_type difference_type;

    typedef CharType char_type;

private:
    static int8_t const ATEND_FLAG = 0x01;
    
    device *  _pd;
    char_type _value;
    int8_t    _flag;

public:
    input_iterator ()
        : _pd(0)
        , _flag(ATEND_FLAG)
    {}
    
    input_iterator (device & d)
        : _pd(& d)
        , _flag(0)
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
        if (it._pd && !(it._flag & ATEND_FLAG)) {
            it._pd->read(& it._value, sizeof(char_type));

            if (it._pd->is_error())
                throw io_exception(it._pd->errorcode());
        }
    }
    
    static bool equals (input_iterator const & it1, input_iterator const & it2)
    {
        if (it1._pd == it2._pd) {
            if (it1._pd == 0)
                return true;
            
            if ((it1._flag & ATEND_FLAG) && (it2._flag & ATEND_FLAG))
                return true;
        }
        return false;
    }
    
private:
    void read ()
    {
        if (_pd) {
            pfs::error_code ec;
            ssize_t n = _pd->read(& _value, sizeof(char_type), ec);

            if (ec)
                throw io_exception(ec);

            PFS_ASSERT(n >= 0);
            
            if (n == 0) {
                _value = 0;
               _pd = 0;
               _flag |= ATEND_FLAG;
            }
        }
    }
};

}} // pfs::io

#endif /* __PFS_IO_ITERATOR_HPP__ */

