/* 
 * File:   utf8_iterator.hpp
 * Author: wladt
 *
 * Created on March 30, 2017, 3:18 PM
 */

#ifndef __PFS_UNICODE_UTF8_ITERATOR_HPP__
#define __PFS_UNICODE_UTF8_ITERATOR_HPP__

#include <pfs/iterator.hpp>
#include <pfs/exception.hpp>
#include <pfs/unicode/char.hpp>

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
class utf8_iterator : public iterator_facade<bidirectional_iterator_tag
        , utf8_iterator<CodePointIter>
        , char_t
        , char_t *  // unused pointer
        , char_t &> // unused reference
{
    CodePointIter _p;

public:
    typedef iterator_facade<bidirectional_iterator_tag
        , utf8_iterator<CodePointIter>
        , char_t
        , char_t *
        , char_t &> base_class;
    
    typedef typename base_class::difference_type difference_type;
    
private:
    char_t * operator -> () const; // avoid '->' operator
        
public:
    utf8_iterator (CodePointIter p)
        : _p(p)
    {}

    char_t operator * () const
    {
        return decode(_p, 0);
    }
    
    operator CodePointIter ()
    {
        return _p;
    }

protected:
    static void advance_forward (CodePointIter & p, difference_type n);
    static void advance_backward (CodePointIter & p, difference_type n);
    static char_t decode (CodePointIter const & p, CodePointIter * pnewpos);

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
    
public: // static
    void advance (CodePointIter & p, difference_type n)
    {
        advance_forward(p, n);
    }
    
    /**
     * @brief Decodes UTF-8 octet sequence into Unicode code point.
     *
     * @return Unicode code point.
     */
    static char_t decode (CodePointIter & p)
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

template <typename OctetIt>
void utf8_iterator<OctetIt>::advance_forward (OctetIt & p, difference_type n)
{
    if (n < 0) {
        advance_backward(p, -n);
        return;
    }
        
    while (n--) {
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
}

template <typename OctetIt>
void utf8_iterator<OctetIt>::advance_backward (OctetIt & p, difference_type n)
{
    if (n < 0) {
        advance_forward(p, -n);
        return;
    }

    while (n--) {
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
}

template <typename OctetIt>
char_t utf8_iterator<OctetIt>::decode (OctetIt const & p, OctetIt * pnewpos)
{
    OctetIt newpos = p;
	uint8_t b = static_cast<uint8_t>(*newpos);
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
    	throw out_of_range("utf8_iterator::decode()");
    }

    ++newpos;

    while (--nunits) {
    	b = static_cast<uint8_t>(*newpos);

        if ((b & 0xC0) == 0x80) {
        	result = (result << 6) | (b & 0x3F);
        } else {
        	// Invalid
        	throw out_of_range("utf8_iterator::decode()");
        }
    	++newpos;
    }

    if (pnewpos)
        *pnewpos = newpos;
    
    return result;
}

template <typename OctetIt>
template <typename BackInsertIt>
BackInsertIt utf8_iterator<OctetIt>::encode (char_t uc, BackInsertIt it)
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

#endif /* __PFS_UNICODE_UTF8_ITERATOR_HPP__ */

