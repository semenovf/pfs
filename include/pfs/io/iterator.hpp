#pragma once
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
    static int const DEFAULT_TIMEOUT = 100;

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

    details::device * _pd;
    char_type         _value;
    int8_t            _flag;

public:
    input_iterator ()
        : _pd(0)
        , _value(0)
        , _flag(ATEND_FLAG)
    {}

    input_iterator (device_ptr & d)
        : _pd(d.get())
        , _value(0)
        , _flag(0)
    {
        ++*this;
    }

    input_iterator (input_iterator const & rhs)
        : _pd(rhs._pd)
        , _value(rhs._value)
        , _flag(rhs._flag)
    {}

    input_iterator & operator = (input_iterator const & rhs)
    {
        _pd    = rhs._pd;
        _value = rhs._value;
        _flag  = rhs._flag;
        return *this;
    }

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
        it.read();
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
            error_code ec;
            ssize_t n = _pd->read(& _value, sizeof(char_type), ec);

            if (n < 0)
                PFS_THROW(io_exception(ec));

            if (n == 0) {
                _value = 0;
               _pd = 0;
               _flag |= ATEND_FLAG;
            }
        }
    }
};

}} // pfs::io

namespace pfs {

template <typename StringType>
inline StringType read_all_u8 (io::device_ptr & dev)
{
    io::input_iterator<char> first(dev);
    io::input_iterator<char> last;

    return read_all_u8<StringType>(first, last);
}

} // pfs
