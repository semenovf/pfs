/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 10:49 AM
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#include <ostream>
#include <pfs/assert.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/ctype.hpp>
#include <pfs/limits.hpp>
#include <pfs/exception.hpp>
#include <pfs/iterator.hpp>
#include <pfs/unicode/char.hpp>
#include <pfs/unicode/unicode_iterator.hpp>

//
// Unicode string (utf8/16/32-encoded string)
//

namespace pfs {

template <typename StringImplType>
class string
{
    typedef StringImplType internal_type;
    
public:
    typedef string<typename internal_type::string_value_type>     string_value_type;
    typedef string<typename internal_type::string_reference_type> string_reference_type;

    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;

    typedef typename internal_type::value_type             code_unit_type;
    typedef typename unicode::char_t                       value_type;
//    typedef typename internal_type::reference              reference;
//    typedef typename internal_type::const_reference        const_reference;
//    typedef typename internal_type::pointer                pointer;
    typedef typename internal_type::const_pointer             const_pointer;
    
    typedef typename unicode::unicode_iterator_traits<
            typename internal_type::iterator>::iterator       iterator;
    typedef typename unicode::unicode_iterator_traits<
            typename internal_type::const_iterator>::iterator const_iterator;
    typedef pfs::reverse_iterator<iterator>                   reverse_iterator;
    typedef pfs::reverse_iterator<const_iterator>             const_reverse_iterator;
    
    typedef typename internal_type::difference_type difference_type;
    typedef typename internal_type::size_type       size_type;
    
    static size_type const npos = size_type(-1);
    
protected:
    internal_type _p;

protected:
    string (typename internal_type::const_pointer first
            , typename internal_type::const_pointer last)
        : _p(first, last)
    {}

    string (typename internal_type::pointer first
            , typename internal_type::pointer last)
        : _p(first, last)
    {}

    string (typename internal_type::iterator first
            , typename internal_type::iterator last)
        : _p(first, last)
    {}

    string (typename internal_type::const_iterator first
            , typename internal_type::const_iterator last)
        : _p(first, last)
    {}

public:
    string ()
    {}

    /**
     * Initializes with reference to native container
     */
    string (native_reference rhs)
        : _p(rhs)
    {}

    string (const_native_reference rhs)
        : _p(rhs)
    {}

    string (const_pointer s)
        : _p(s)
    {}
    
    string (iterator first, iterator last)
        : _p(first.base(), last.base())
    {}

    string (const_iterator first, const_iterator last)
        : _p(first.base(), last.base())
    {}
    
    string (size_type count, value_type ch)
        : _p(count, ch)
    {}
    
    /**
     * Casts to const native container reference
     */
    operator const_native_reference () const
    {
        return _p;
    }

    /**
     * Casts to native container reference
     */
    operator native_reference ()
    {
        return _p;
    }
    
    const_native_reference native () const
    {
        return _p;
    }

    native_reference native ()
    {
        return _p;
    }
    
    // 
    /**
     * @return Size in code units
     */
    size_type size () const
    {
        return _p.size();
    }

    /**
     * @return Length in code points
     */    
    size_type length () const
    {
        return pfs::distance(cbegin(), cend());
    }

    /**
     * @details Returns the maximum number of elements the string is able to 
     *          hold due to system or library implementation limitations, 
     *          i.e. ​std::distance(begin(), end())​ for the largest string. 
     * @return 
     */
    size_type max_size () const
    {
        // Formulas for calculate max_size 
        //
        // * std::basic_string (GNU)
        //   max_size = (((npos - sizeof(_Rep_base))/sizeof(_CharT)) - 1) / 4
        //
        // * std::basic_string (STLport)
        //   max_size = size_type(-1)/sizeof(CharT) - 1
        //
        // * QString (has no max_size) so formula will look like:
        //   INT_MAX/sizeof(QChar) - 1;

        return _p.max_size();
    }
    
    const_pointer data () const
    {
        return _p.data();
    }

    const_pointer c_str () const
    {
        return _p.c_str();
    }
    
  	/**
	 * @brief Checks if string is empty.
	 * @return @c true if string is empty (size() == 0) or @c false otherwise.
	 */
    bool empty () const
    {
    	return this->size() == 0;
    }
    
    const_iterator begin () const
    {
        return const_iterator(_p.begin());
    }

    const_iterator end () const
    {
        return const_iterator(_p.end());
    }

    const_iterator cbegin () const
    {
        return this->begin();
    }

    const_iterator cend () const
    {
        return this->end();
    }

    const_reverse_iterator rbegin () const
    {
        return const_reverse_iterator(_p.rbegin());
    }

    const_reverse_iterator rend () const
    {
        return const_reverse_iterator(_p.rend());
    }

    const_reverse_iterator crbegin () const
    {
        return this->rbegin();
    }

    const_reverse_iterator crend () const
    {
        return this->rend();
    }

    // TODO reference front ()
    // TODO const_reference front () const;
    // TODO reference back ();
    // TODO const_reference back () const;

#if __DEPRECATED__
    /**
     * @brief
     * @param pos Position of the character to return.
     * @trows out_of_range if pos >= size()
     */
    value_type at (size_type pos) const
    {
        if (pos >= this->size())
            throw out_of_range("string::at");
        return _p.at(pos);
    }
    
    value_type operator [] (size_type pos) const
    {
    	return this->at(pos);
    }
#endif
    
    string & append (size_type count, value_type ch)
    {
        while (count--)
            push_back(ch);
        return *this;
    }
    
    string & append (string const & s)
    {
        _p.append(s._p);
        return *this;
    }
    
//    string & append (string const & s
//                , const_iterator pos
//                , size_type count)
//    {
//        if (pos >= s.size())
//            throw out_of_range("string::append()");
//        
//        if (count == size_type(-1) || pos + count > s.size())
//            count = s.size() - pos;
//        
//        return append(s.data() + pos, count);
//    }
    
    string & append (const_pointer str, size_type count)
    {
        _p.append(str, count);
        return *this;
    }
    
    string & append (const_pointer str)    
    {
        return append(str, internal_type::length(str));
    }
    
    string & append (const_native_reference s)
    {
        _p.append(s);
        return *this;
    }
    
    string & append (iterator first, iterator last)
    {
        _p.append(first, last);
        return *this;
    }

    string & append (const_iterator first, const_iterator last)
    {
        _p.append(first, last);
        return *this;
    }
    
    void clear ()
    {
        _p.clear();
    }

    /**
     * @brief Find position of a string.
     * 
     * @details Starting from @a pos, searches forward for value 
     *          of @a str within this string. 
     *          If found, returns the index where it begins.
     *          If not found, returns size_type(-1).
     * 
     * @param rhs  String to locate.
     * @param pos  Index of character to search from (default 0).
     * @return Index of start of first occurrence.
     *
     */
    const_iterator find (string const & rhs, const_iterator pos) const
    {
        size_type index = _p.find(rhs._p, pfs::distance(pos.base(), this->cend().base()));
        return index == npos ? this->cend() : const_iterator(this->cbegin().base() + index);
    }

    const_iterator find (string const & rhs) const
    {
        return this->find(rhs, cbegin());
    }
    
//    size_type find (const_native_reference rhs, size_type pos = 0) const
//    {
//        return _p.find(rhs._p, pos);
//    }
//    
//    size_type find (value_type c, size_type pos = 0) const
//    {
//        return _p.find(c, pos);
//    }
//    
//    size_type rfind (string const & rhs, size_type pos = size_type(-1)) const
//    {
//        return _p.rfind(rhs._p, pos);
//    }
//    
//    size_type rfind (const_native_reference rhs, size_type pos = size_type(-1)) const
//    {
//        return _p.find(rhs._p, pos);
//    }
//    
//    size_type rfind (value_type c, size_type pos = size_type(-1)) const
//    {
//        return _p.rfind(c, pos);
//    }
//        
    
    void push_back (value_type ch)
    {
        if (is_ascii(ch))
            _p.push_back(to_ascii(ch));
        else {
            iterator::encode(ch, pfs::back_inserter(_p));
        }
    }
    
    /**
     * 
     * @param rhs
     * @return 
     */
  	bool ends_with (string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;
        
        return _p.compare(_p.size() - rhs._p.size()
                , rhs._p.size(), rhs._p, 0, rhs._p.size()) == 0;
    }
    
    bool starts_with (string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;

        return _p.compare(0, rhs._p.size(), rhs._p, 0, rhs._p.size()) == 0;
    }
    
    /**
     * @brief  Get a substring.
     * @details Construct and return a new string using the @a n
     *          characters starting at @a pos.  If the string is too
     *          short, use the remainder of the characters.  If @a pos is
     *          beyond the end of the string, out_of_range is thrown. 
     * @param pos  Index of first character (default 0).
     * @param n  Number of characters in substring (default remainder).
     * @return The new string.
     * @throw out_of_range  If pos > size().
     */
    string_value_type substr (const_iterator pos, size_type count = npos) const;
    
	string_value_type left (size_t count) const
	{
		return substr(0, count);
	}

	string_value_type right (size_t count) const
	{
		return substr(this->size() - count, count);
	}
    
#if __TODO__   
    int compare (size_type pos1, size_type count1
        , string const & rhs, size_type pos2, size_type count2) const
    {
        return _p.compare(pos1, count1, rhs._p, pos2, count2) ;
    }
    
    int compare (size_type pos1, size_type count1, string const & rhs) const
    {
        return this->compare(pos1, count1, rhs, 0, rhs.size());
    }
    
    int compare (string const & rhs) const
    {
        return this->compare(0, this->size(), rhs._p, 0, rhs.size());
    }
    
//    bool contains (string const & rhs, size_type pos = 0) const
//	{
//		return this->find(rhs._p, pos) != size_type(-1);
//	}
//
//    bool contains (const_native_reference rhs, size_type pos = 0)
//    {
//        return this->find(rhs._p, pos) != size_type(-1);
//    }
//    
//	bool contains (value_type c, size_type pos = 0) const
//	{
//        return this->find(c, pos) != size_type(-1);
//	}
   
   
    string & erase (size_type index = 0, size_type count = size_type(-1))
    {
        if (index > size())
            throw out_of_range("string::erase()");
        _p.erase(index, count);
        return *this;
    }
    
    iterator erase (const_iterator position)
    {
        return this->erase(position, position + 1);
    }
    
    iterator erase (const_iterator first, const_iterator last)
    {
        return _p.erase(first, last);
    }
  
    string & insert (size_type index, size_type count, value_type ch);
    
    string & insert (size_type index, const_pointer s);
    
    string & insert (size_type index, const_pointer s, size_type count);
    
    string & insert (size_type index, string const & str)
    {
        return insert(index, str.data(), str.size());
    }
    
    string & insert (size_type index, string const & str, size_type index_str, size_type count)
    {
        if (index_str > str.size())
            throw pfs::out_of_range("string::insert()");
        
        return insert(index, str.data() + index_str, count);
    }

    // TODO
    // iterator insert (const_iterator pos, CharT ch);
    
    // TODO
    // iterator insert (iterator pos, size_type count, CharT ch);
    
    // TODO
    // template <typename InputIt>
    // iterator insert (const_iterator i, InputIt first, InputIt last);
    
    // TODO
//    void swap (string & rhs)
//    {
//        _p.swap(rhs._p);
//    }
#endif // __TODO__
    
    friend inline bool operator == (string const & lhs, string const & rhs)
    {
        return lhs._p == rhs._p;
    }

    friend inline bool operator != (string const & lhs, string const & rhs)
    {
        return lhs._p != rhs._p;
    }

    friend inline bool operator < (string const & lhs, string const & rhs)
    {
        return lhs._p < rhs._p;
    }

    friend inline bool operator <= (string const & lhs, string const & rhs)
    {
        return lhs._p <= rhs._p;
    }

    friend inline bool operator > (string const & lhs, string const & rhs)
    {
        return lhs._p > rhs._p;
    }

    friend inline bool operator >= (string const & lhs, string const & rhs)
    {
        return lhs._p >= rhs._p;
    }
    
    friend inline string_value_type operator + (string const & lhs, string const & rhs)
    {
        string_value_type result;
        result._p = lhs._p + rhs._p;
        return result;
    }

    friend inline string_value_type operator + (const_pointer lhs, string const & rhs)
    {
        string_value_type result;
        result._p = lhs + rhs._p;
        return result;
    }

    friend inline string_value_type operator + (value_type lhs, string const & rhs)
    {
        string_value_type result;
        result._p = lhs + rhs._p;
        return result;
    }

    friend inline string_value_type operator + (string const & lhs, const_pointer rhs)
    {
        string_value_type result;
        result._p = lhs._p + rhs;
        return result;
    }

    friend inline string_value_type operator + (string const & lhs, value_type rhs)
    {
        string_value_type result;
        result._p = lhs._p + rhs;
        return result;
    }
    
    friend inline std::ostream & operator << (std::ostream & os, string const & s)
    {
        os << s.native();
        return os;
    }
};

template <typename StringImplType>
typename string<StringImplType>::string_value_type
string<StringImplType>::substr (const_iterator pos, size_type count) const
{
    if (pos >= cend() || pos < cbegin())
        throw out_of_range("string::substr()");
    
    // This rough condition guarantees need to return the tail
    if (count > size())
        return string_value_type(pos, cend());

    const_iterator epos(pos);
    difference_type n = integral_cast_check<difference_type, size_type>(count);
    epos.advance_safe(cend(), n);
    
    return string_value_type(pos, epos);
}

namespace details {
namespace integral {

//#define BITS_SIZE(T) (sizeof(T) * 8)
template <typename CharT>
CharT * uintmax_to_cstr (uintmax_t num
        , int radix
        , bool uppercase
        , CharT * buf
        , size_t n)
{
    static const CharT digits_lower[] = { 
          CharT('0'), CharT('1'), CharT('2'), CharT('3'), CharT('4')
        , CharT('5'), CharT('6'), CharT('7'), CharT('8'), CharT('9')
        , CharT('a'), CharT('b'), CharT('c'), CharT('d'), CharT('e')
        , CharT('f'), CharT('g'), CharT('h'), CharT('i'), CharT('j')
        , CharT('k'), CharT('l'), CharT('m'), CharT('n'), CharT('o')
        , CharT('p'), CharT('q'), CharT('r'), CharT('s'), CharT('t')
        , CharT('u'), CharT('v'), CharT('w'), CharT('x'), CharT('y')
        , CharT('z')
    };

    static const CharT digits_upper[] = { 
          CharT('0'), CharT('1'), CharT('2'), CharT('3'), CharT('4')
        , CharT('5'), CharT('6'), CharT('7'), CharT('8'), CharT('9')
        , CharT('A'), CharT('B'), CharT('C'), CharT('D'), CharT('E')
        , CharT('F'), CharT('G'), CharT('H'), CharT('I'), CharT('J')
        , CharT('K'), CharT('L'), CharT('M'), CharT('N'), CharT('O')
        , CharT('P'), CharT('Q'), CharT('R'), CharT('S'), CharT('T')
        , CharT('U'), CharT('V'), CharT('W'), CharT('X'), CharT('Y')
        , CharT('Z')
    };

	CharT * p = & buf[n - 1];

	if (!(radix >= 2 && radix <= 36))
        throw pfs::invalid_argument("uintmax_to_cstr(): bad radix");

	buf[n - 1] = '\0';
    const CharT * digits = uppercase ? digits_upper : digits_lower;
    
	if (num) {
		while (num > 0) {
            *--p = digits[num % radix];
			num /= radix;
		}
	} else { /* num == 0 */
		*--p = CharT('0');
	}

	return p;    
}

template <typename CharT>
CharT * intmax_to_cstr (intmax_t num
        , int radix
        , bool uppercase
        , CharT * buf
        , size_t n)
{
	CharT * p = 0;

	if (num < 0) {
		p = uintmax_to_cstr<CharT>(static_cast<uintmax_t>(num * -1)
                , radix
                , uppercase
                , buf
                , n);
		*--p = '-';
	} else {
		p = uintmax_to_cstr<CharT>(static_cast<uintmax_t>(num)
                , radix
                , uppercase
                , buf
                , n);
	}

	return p;    
}

/**
 * 
 * @param value
 * @param field_width Specifies the minimum amount of space that a is padded 
 *        to and filled with the character fillChar. A positive value produces 
 *        right-aligned text; a negative value produces left-aligned text.
 * @param radix
 * @param uppercase
 * @param fill_char
 * @return 
 */
template <typename UintType, typename StringType>
StringType to_string (typename pfs::enable_if<pfs::is_unsigned<UintType>::value, UintType>::type value
		, int radix
		, bool uppercase)
{
    typedef typename StringType::code_unit_type char_type;
    
	char_type buf[sizeof(UintType) * 8 + 1];
	char_type * str = uintmax_to_cstr<char_type>(static_cast<uintmax_t>(value)
			, radix
			, uppercase
			, buf
			, sizeof(buf));
    
    return StringType(str);
}

template <typename IntType, typename StringType>
StringType to_string (typename enable_if<is_signed<IntType>::value, IntType>::type value
		, int radix
		, bool uppercase)
{
    typedef typename StringType::code_unit_type char_type;
    
	char_type buf[sizeof(IntType) * 8 + 1];
	char_type * str = intmax_to_cstr<char_type>(static_cast<intmax_t>(value)
			, radix
			, uppercase
            , buf
			, sizeof(buf));
    
    return StringType(str);
}

}} // details::integral

template <typename StringType>
inline StringType to_string (bool value)
{
	return value ? StringType("true") : StringType("false");
}

template <typename StringType>
inline StringType to_string (char a)
{
    return StringType(1, typename StringType::value_type(a));
}

template <typename StringType>
inline StringType to_string (signed char a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<signed char, StringType>(a
            , radix
            , uppercase);
}

template <typename StringType>
inline StringType to_string (unsigned char a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned char, StringType>(a
            , radix
            , uppercase);
}

template <typename StringType>
inline StringType to_string (short a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<short, StringType>(a
            , radix
            , uppercase);
}

template <typename StringType>
inline StringType to_string (unsigned short a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned short, StringType>(a
            , radix
            , uppercase);
}

template <typename StringType>
inline StringType to_string (int a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<int, StringType>(a
            , radix
            , uppercase);
}

template <typename StringType>
inline StringType to_string (unsigned int a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned int, StringType>(a
            , radix
            , uppercase);
}

template <typename StringType>
inline StringType to_string (long a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<long, StringType>(a
            , radix
            , uppercase);
}

template <typename StringType>
inline StringType to_string (unsigned long a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned long, StringType>(a
            , radix
            , uppercase);
}

#if PFS_HAVE_LONGLONG

template <typename StringType>
inline StringType to_string (long long a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<long long, StringType>(a
            , radix
            , uppercase);
}

template <typename StringType>
inline StringType to_string (unsigned long long a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned long long, StringType>(a
            , radix
            , uppercase);
}
#endif

namespace details {
namespace fp {

char * double_to_cstr (double num
        , char f
        , int prec
        , char * buf
        , size_t * n);

// 1.18973e+4932 with 'f' flag has length 4940
//
template <typename Float, typename StringType>
StringType to_string (typename pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type value
		, char f
		, int prec)
{
	static const size_t FP_BUFSZ = 128;

	StringType s;

	size_t sz = FP_BUFSZ;
	char buf[FP_BUFSZ];
	char * pbuf = buf;

	char * str = double_to_cstr(value, f, prec, pbuf, & sz);

	if (!str) {
		++sz;
		pbuf = new char [sz];
		str = double_to_cstr(value, f, prec, pbuf, & sz);
		PFS_ASSERT(str);
		s = StringType(str);
		delete [] pbuf;
	} else {
		s = StringType(str);
	}

	return s;
}

}} // details::fp

template <typename StringType>
inline StringType to_string (float a
        , char format = 'f'
        , int precision = -1)
{
    return details::fp::to_string<float, StringType>(a
            , format
            , precision);
}

template <typename StringType>
inline StringType to_string (double a
        , char format = 'g'
        , int precision = -1)
{
    return details::fp::to_string<double, StringType>(a
            , format
            , precision);
}

//template <typename StringType>
//inline StringType left_justified (StringType const & s
//        , int width
//        , typename StringType::value_type fill_char = typename StringType::value_type(' ')
//        , bool trancate = false)
//{
//    if (width < 0)
//        return right_justified(s, -1 * width, fill_char, trancate);
//
//    if (s.size())
//    // TODO Implement left-aligned text (field_width is negative)
//    if (width > 0 && s.size() < static_cast<size_t>(width)) {
//        if (value < 0 && !pfs::is_space(fill_char)) { // -000000123
//            return StringType(1, '-') 
//                    + StringType(static_cast<size_t>(field_width) - s.size() - 1, fill_char)
//                    + StringType(s.cbegin() + 1, s.cend()); // ignore sign
//        } else { // -123, 123, _______-123, ________123
//            return StringType(static_cast<size_t>(field_width) - s.size(), fill_char) + s;
//        }
//    }
//}
//
//template <typename StringType>
//inline StringType right_justified (StringType const & s
//        , int width
//        , typename StringType::value_type fill_char = typename StringType::value_type(' ')
//        , bool trancate = false)
//{
//    if (width < 0)
//        return left_justified(s, -1 * width, fill_char, trancate);
//
//    // TODO Implement left-aligned text (field_width is negative)
//    if (width > 0 && s.size() < static_cast<size_t>(width)) {
//        if (value < 0 && !pfs::is_space(fill_char)) { // -000000123
//            return StringType(1, '-') 
//                    + StringType(static_cast<size_t>(field_width) - s.size() - 1, fill_char)
//                    + StringType(s.cbegin() + 1, s.cend()); // ignore sign
//        } else { // -123, 123, _______-123, ________123
//            return StringType(static_cast<size_t>(field_width) - s.size(), fill_char) + s;
//        }
//    }
//    
//    return s;
//}
//
//template <typename StringType>
//inline StringType justified (StringType const & s
//        , int width
//        , typename StringType::value_type fill_char = typename StringType::value_type(' ')
//        , bool trancate = false)
//{
//    if (width < 0)
//        return left_justified(s, -1 * width, fill_char, trancate);
//    
//    return right_justified(s, width, fill_char, trancate);
//}

} // pfs

#endif /* __PFS_STRING_HPP__ */

