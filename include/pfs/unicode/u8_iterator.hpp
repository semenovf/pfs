/* 
 * File:   u8_iterator.hpp
 * Author: wladt
 *
 * Created on March 30, 2017, 3:18 PM
 */

#ifndef __PFS_UNICODE_U8_ITERATOR_HPP__
#define __PFS_UNICODE_U8_ITERATOR_HPP__

#include <pfs/iterator.hpp>
#include <pfs/unicode/char.hpp>
#include <pfs/unicode/traits.hpp>

/* UTF-8
 *
 * Bits Last code point/    Byte 1      Byte 2      Byte 3      Byte 4      Byte 5      Byte 6
 *  7   U+007F              0xxxxxxx
 *  11  U+07FF              110xxxxx    10xxxxxx
 *  16  U+FFFF              1110xxxx    10xxxxxx    10xxxxxx
 *  21  U+1FFFFF            11110xxx    10xxxxxx    10xxxxxx    10xxxxxx
 *  26  U+3FFFFFF           111110xx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
 *  31  U+7FFFFFFF          1111110x    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
 */

namespace pfs {
namespace unicode {

template <typename CodePointIter>
struct unicode_iterator_traits;

template <typename OctetInputIt, typename BrokenSeqAction = ignore_broken_sequence>
class u8_input_iterator : public iterator_facade<input_iterator_tag
        , u8_input_iterator<OctetInputIt>
        , char_t
        , char_t *  // unused
        , char_t &> // unused
{
    typedef iterator_facade<input_iterator_tag
        , u8_input_iterator<OctetInputIt>
        , char_t
        , char_t *
        , char_t &> base_class;
    
public:
    typedef typename base_class::pointer         pointer;
    typedef typename base_class::reference       reference;
    typedef typename base_class::difference_type difference_type;
    typedef BrokenSeqAction broken_sequence_action;

private:
    static int8_t const ATEND_FLAG  = 0x01;
    static int8_t const BROKEN_FLAG = 0x02;
    
    OctetInputIt * _p;
    OctetInputIt _last;
    char_t       _value;
    int8_t       _flag;

public:
    u8_input_iterator ()
        : _flag(0)
    {}

    u8_input_iterator (OctetInputIt & first, OctetInputIt last)
        : _p(& first)
        , _last(last)
        , _flag(0)
    {
        if (*_p == _last) {
            _flag |= ATEND_FLAG;
        } else {
            increment(*this, 1);
            _flag |= (_flag & BROKEN_FLAG) ? ATEND_FLAG : 0;
        }
    }
    
    u8_input_iterator (OctetInputIt last)
        : _p(0)
        , _last(last)
        , _flag(ATEND_FLAG)
    {}

//    operator OctetInputIt ()
//    {
//        return *_p;
//    }
//    
//    OctetInputIt base () const
//    {
//        return _p;
//    }

public:
    static reference ref (u8_input_iterator & it)
    {
        return it._value;
    }
    
    static pointer ptr (u8_input_iterator & it)
    {
        return & it._value;
    }

    static void increment (u8_input_iterator & it, difference_type);

    static bool equals (u8_input_iterator const & it1, u8_input_iterator const & it2)
    {
        return ((it1._flag & ATEND_FLAG) && (it2._flag & ATEND_FLAG))
                && (it1._p == it2._p);
    }

private:
    void broken_sequence ()
    {
        // Broken utf-8 sequence
        _value = char_t::replacement_char;
        _p = 0;
        _flag |= (ATEND_FLAG | BROKEN_FLAG);
        broken_sequence_action()();
    }
};

template <typename OctetInputIt, typename BrokenSeqAction>
void u8_input_iterator<OctetInputIt, BrokenSeqAction>::increment (u8_input_iterator & it, difference_type)
{
    if (! it._p)
        return;

    if (*it._p == it._last) {
        it._p = 0;
        it._flag |= ATEND_FLAG;
        return;
    }

    uint8_t b = code_point_cast<uint8_t>(**it._p);
    ++(*it._p);
    char_t::value_type result;
    int nunits = 0;

    if (b < 128) {
        result = b;
        nunits = 1;
    } else if ((b & 0xE0) == 0xC0) {
        result = b & 0x1F;
        nunits = 2;
    } else if ((b & 0xF0) == 0xE0) {
        result = b & 0x0F;
        nunits = 3;
    } else if ((b & 0xF8) == 0xF0) {
        result = b & 0x07;
        nunits = 4;
    } else if ((b & 0xFC) == 0xF8) {
        result = b & 0x03;
        nunits = 5;
    } else if ((b & 0xFE) == 0xFC) {
        result = b & 0x01;
        nunits = 6;
    } else {
        it.broken_sequence();
        return;
    }

    while (--nunits) {
        if (*it._p == it._last) {
            it.broken_sequence();
            return;
        }

        b = code_point_cast<uint8_t>(**it._p);
        ++(*it._p);

        if ((b & 0xC0) == 0x80) {
            result = (result << 6) | (b & 0x3F);
        } else {
            it.broken_sequence();
            return;
        }
    }

    it._value = static_cast<intmax_t>(result);
}


// TODO OBSOLETE Must be replaced by u8_input_iterator (see above) and u8_output_iterator
template <typename OctetInputIt>
class utf8_iterator : public iterator_facade<bidirectional_iterator_tag
        , utf8_iterator<OctetInputIt>
        , char_t
        , char_t *  // unused
        , char_t>   // used as reference in 'std::reverse_iterator's operator: reference operator * () const'
{
    OctetInputIt _p;

public:
    typedef iterator_facade<bidirectional_iterator_tag
        , utf8_iterator<OctetInputIt>
        , char_t
        , char_t *
        , char_t> base_class;
    
    typedef typename base_class::difference_type difference_type;
    
private:
    char_t * operator -> () const; // avoid '->' operator
        
public:
    // It is no matter if _p (when it is a regular pointer) 
    // will be initialized with 0 (zero) or uninitialized
    utf8_iterator ()
    {}
    
    utf8_iterator (OctetInputIt p)
        : _p(p)
    {}

    char_t operator * () const
    {
        return decode(_p, 0);
    }
    
    operator OctetInputIt ()
    {
        return _p;
    }
    
    OctetInputIt base () const
    {
        return _p;
    }

protected:
    static void advance_forward_safe (OctetInputIt & p, OctetInputIt end, difference_type & n)
    {
        if (n < 0) {
            difference_type n1 = -n;
            advance_backward_safe(p, end, n1);
            n += n1;
            return;
        }

        OctetInputIt prev = p;
        difference_type nprev = n;
        
        while (n-- && p < end) {
            prev = p;
            nprev = n + 1;
            
            if ((*p & 0x80) == 0x00) {
                ++p;
            } else if ((*p & 0xE0) == 0xC0) {
                p += 2;
            } else if ((*p & 0xF0) == 0xE0) {
                p += 3;
            } else if ((*p & 0xF8) == 0xF0) {
                p += 4;
            } else if ((*p & 0xFC) == 0xF8) {
                p += 5;
            } else if ((*p & 0xFE) == 0xFC) {
                p += 6;
            } else { // Invalid char
                ++p;
            }
        }
        
        if (p > end) {
            p = prev;
            n = nprev;
        }
    }

    static void advance_backward_safe (OctetInputIt & p, OctetInputIt begin, difference_type & n)
    {
        if (n < 0) {
            difference_type n1 = -n;
            advance_forward_safe(p, begin, n1);
            n += n1;
            return;
        }

        OctetInputIt prev = p;
        difference_type nprev = n;
        
        while (n-- && p > begin) {
            prev = p;
            nprev = n + 1;
            
            if ((*(p - 1) & 0x80) == 0x00) {
                --p;
            } else if ((*(p - 2) & 0xE0) == 0xC0) {
                p -= 2;
            } else if ((*(p - 3) & 0xF0) == 0xE0) {
                p -= 3;
            } else if ((*(p - 4) & 0xF8) == 0xF0) {
                p -= 4;
            } else if ((*(p - 5) & 0xFC) == 0xF8) {
                p -= 5;
            } else if ((*(p - 6) & 0xFE) == 0xFC) {
                p -= 6;
            } else {
                --p;
            }
        }
        
        if (p < begin) {
            p = prev;
            n = nprev;
        }
    }    

    static void advance_forward (OctetInputIt & p, difference_type n)
    {
        advance_forward_safe(p, unicode_iterator_traits<OctetInputIt>::max(), n);
    }
    
    static void advance_backward (OctetInputIt & p, difference_type n)
    {
        advance_backward_safe(p, unicode_iterator_traits<OctetInputIt>::min(), n);
    }
    
    static char_t decode (OctetInputIt const & p, OctetInputIt * pnewpos)
    {
        OctetInputIt newpos = p;
        uint8_t b = code_point_cast<uint8_t> (*newpos);
        char_t::value_type result;
        int nunits = 0;

        if (b < 128) {
            result = b;
            nunits = 1;
        } else if ((b & 0xE0) == 0xC0) {
            result = b & 0x1F;
            nunits = 2;
        } else if ((b & 0xF0) == 0xE0) {
            result = b & 0x0F;
            nunits = 3;
        } else if ((b & 0xF8) == 0xF0) {
            result = b & 0x07;
            nunits = 4;
        } else if ((b & 0xFC) == 0xF8) {
            result = b & 0x03;
            nunits = 5;
        } else if ((b & 0xFE) == 0xFC) {
            result = b & 0x01;
            nunits = 6;
        } else {
            // Invalid
            throw range_error("utf8_iterator::decode()");
        }

        ++newpos;

        while (--nunits) {
            b = code_point_cast<uint8_t>(*newpos);

            if ((b & 0xC0) == 0x80) {
                result = (result << 6) | (b & 0x3F);
            } else {
                // Invalid
                throw range_error("utf8_iterator::decode()");
            }
            ++newpos;
        }

        if (pnewpos)
            *pnewpos = newpos;

        return result;
    }    

public:
    static void increment (utf8_iterator & it, difference_type)
    {
        advance_forward(it._p, 1);
    }

    static bool equals (utf8_iterator const & it1, utf8_iterator const & it2)
    {
        return it1._p == it2._p;
    }
    
    static void decrement (utf8_iterator & it, difference_type)
    {
        advance_backward(it._p, 1);
    }
    
public:
    void advance_safe (OctetInputIt end, difference_type & n)
    {
        advance_forward_safe(_p, end, n);
    }
        
    static void advance (OctetInputIt & p, difference_type n)
    {
        advance_forward(p, n);
    }

    /**
     * @brief Decodes UTF-8 octet sequence into Unicode code point.
     *
     * @return Unicode code point.
     */
    static char_t decode (OctetInputIt & p)
    {
        return decode(p, & p);
    }
    
    template <typename BackInsertIt>
    static BackInsertIt encode (char_t uc, BackInsertIt it);

    friend bool operator < (utf8_iterator const & lhs, utf8_iterator const & rhs)
    {
        return lhs._p < rhs._p;
    }

    friend bool operator <= (utf8_iterator const & lhs, utf8_iterator const & rhs)
    {
        return lhs._p <= rhs._p;
    }

    friend bool operator > (utf8_iterator const & lhs, utf8_iterator const & rhs)
    {
        return lhs._p > rhs._p;
    }

    friend bool operator >= (utf8_iterator const & lhs, utf8_iterator const & rhs)
    {
        return lhs._p >= rhs._p;
    }
};

template <typename OctetInputIt>
template <typename BackInsertIt>
BackInsertIt utf8_iterator<OctetInputIt>::encode (char_t uc, BackInsertIt it)
{
    if (uc.value < 0x80) {
        *it++ = uint8_t(uc.value);
    } else if (uc.value < 0x0800) {
    	*it++ = 0xC0 | uint8_t(uc.value >> 6);
    	*it++ = 0x80 | uint8_t(uc.value & 0x3f);
    } else if (uc.value < 0x10000) {
    	*it++ = 0xE0 | uint8_t(uc.value >> 12);
    	*it++ = 0x80 | (uint8_t(uc.value >> 6)  & 0x3F);
    	*it++ = 0x80 | uint8_t(uc.value & 0x3F);
    } else if (uc.value < 0x200000) {
    	*it++ = 0xF0 | uint8_t(uc.value >> 18);
    	*it++ = 0x80 | (uint8_t(uc.value >> 12) & 0x3F);
    	*it++ = 0x80 | (uint8_t(uc.value >> 6)  & 0x3F);
    	*it++ = 0x80 | uint8_t(uc.value & 0x3F);
    } else if (uc.value < 0x4000000) {
    	*it++ = 0xF8 | uint8_t(uc.value >> 24);
    	*it++ = 0x80 | (uint8_t(uc.value >> 18) & 0x3F);
    	*it++ = 0x80 | (uint8_t(uc.value >> 12) & 0x3F);
    	*it++ = 0x80 | (uint8_t(uc.value >> 6)  & 0x3F);
    	*it++ = 0x80 | uint8_t(uc.value & 0x3F);
    } else if (uc.value < 0x80000000) {
    	*it++ = 0xFC | uint8_t(uc.value >> 30);
    	*it++ = 0x80 | (uint8_t(uc.value >> 24) & 0x3F);
    	*it++ = 0x80 | (uint8_t(uc.value >> 18) & 0x3F);
    	*it++ = 0x80 | (uint8_t(uc.value >> 12) & 0x3F);
    	*it++ = 0x80 | (uint8_t(uc.value >> 6)  & 0x3F);
    	*it++ = 0x80 | uint8_t(uc.value & 0x3F);
    }

    return it;
}

}} // pfs::unicode

namespace pfs {

template <typename StringType, typename OctetInputIt>
StringType read_line_u8 (OctetInputIt & first, OctetInputIt last)
{
    typedef unicode::u8_input_iterator<OctetInputIt> utf8_input_iterator;
    
    StringType result;
    utf8_input_iterator it(first, last);
    utf8_input_iterator end(last);
    
    while (it != end && *it != '\n') {
        if (*it != '\r')
            result.push_back(*it);
        ++it;
    }
    
    return result;
}

template <typename StringType, typename OctetInputIt>
StringType read_all_u8 (OctetInputIt & first, OctetInputIt last)
{
    typedef unicode::u8_input_iterator<OctetInputIt> utf8_input_iterator;
    
    StringType result;
    
    utf8_input_iterator it(first, last);
    utf8_input_iterator end(last);

    while (it != end) {
        if (*it != '\r')
            result.push_back(*it);
        ++it;
    }
    
    return result;
}

} // pfs

#endif /* __PFS_UNICODE_U8_ITERATOR_HPP__ */

