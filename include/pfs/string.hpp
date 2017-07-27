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
#include <pfs/exception.hpp>
#include <pfs/iterator.hpp>

namespace pfs {

template <typename StringImplType>
class string
{
    typedef StringImplType internal_type;
    
public:
    typedef string<typename internal_type::string_value_type>      string_value_type;
    typedef string<typename internal_type::string_reference_type>  string_reference_type;

    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;

    typedef typename internal_type::value_type             value_type;
    typedef typename internal_type::reference              reference;
    typedef typename internal_type::const_reference        const_reference;
    typedef typename internal_type::pointer                pointer;
    typedef typename internal_type::const_pointer          const_pointer;
    typedef typename internal_type::iterator               iterator;
    typedef typename internal_type::const_iterator         const_iterator;
    typedef typename internal_type::reverse_iterator       reverse_iterator;
    typedef typename internal_type::const_reverse_iterator const_reverse_iterator;
//    typedef typename internal_type::difference_type        difference_type;
    typedef typename internal_type::size_type              size_type;
    
protected:
    internal_type _p;

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
    
    template <typename InputIt>
    string (InputIt first, InputIt last)
        : _p(first, last)
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
    
    size_type size () const
    {
        return _p.size();
    }
    
    size_type length () const
    {
        return _p.size();
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
        return _p.begin();
    }

    const_iterator end () const
    {
        return _p.end();
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
        return _p.rbegin();
    }

    const_reverse_iterator rend () const
    {
        return _p.rend();
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
    
    bool starts_with (string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;
        
        return this->compare(0, rhs.size(), rhs) == 0;
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
        
        return this->compare(this->size() - rhs.size(), rhs.size(), rhs) == 0;
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
//    size_type find (string const & rhs, size_type pos = 0) const
//    {
//        return _p.find(rhs._p, pos);
//    }
//    
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
    /**
     * @function string substr (size_type pos, size_type count) const;
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
    string_value_type substr (size_type pos, size_type count = size_type(-1)) const;
    
	string_value_type left (size_t count) const
	{
		return substr(0, count);
	}

	string_value_type right (size_t count) const
	{
		return substr(this->size() - count, count);
	}
    
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
  
    void clear ()
    {
        _p.clear();
    }
    
    string & append (size_type count, value_type ch)
    {
        _p.append(count, ch);
        return *this;
    }
    
    string & append (string const & s)
    {
        return append(s, 0, s.size());
    }
    
    string & append (string const & s
                , size_type pos
                , size_type count)
    {
        if (pos >= s.size())
            throw out_of_range("string::append()");
        
        if (count == size_type(-1) || pos + count > s.size())
            count = s.size() - pos;
        
        return append(s.data() + pos, count);
    }
    
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
    
    template <typename InputIt>
    string & append (InputIt first, InputIt last)
    {
        _p.append<InputIt>(first, last);
        return *this;
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
    
    void push_back (value_type ch)
    {
        _p.push_back(ch);
    }

    // TODO
//    void swap (string & rhs)
//    {
//        _p.swap(rhs._p);
//    }
    
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
string<StringImplType>::substr (size_type pos, size_type count) const
{
    if (pos >= this->size())
        throw out_of_range("string::substr()");
    size_type n = this->size() - pos;

    if (n > count)
        n = count;

    const_iterator b(this->begin());
    const_iterator e(this->begin());
    pfs::advance(b, pos);
    pfs::advance(e, pos + n);

    return string_value_type(b, e);
}

namespace details {
namespace integral {

//#define BITS_SIZE(T) (sizeof(T) * 8)
char * uintmax_to_cstr (uintmax_t num
        , int radix
        , bool uppercase
        , char * buf
        , size_t n);

char * intmax_to_cstr (intmax_t num
        , int radix
        , bool uppercase
        , char * buf
        , size_t n);

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
        , int field_width
		, int radix
		, bool uppercase
        , typename StringType::value_type fill_char)
{
	char buf[sizeof(UintType) * 8 + 1]; // char buf[BITS_SIZE(UintType) + 1];
	char * str = uintmax_to_cstr(static_cast<uintmax_t>(value)
			, radix
			, uppercase
			, buf
			, sizeof(buf));
    
    StringType s(str);
    
    // TODO Implement left-aligned text (field_width is negative)
    if (field_width > 0 && s.size() < static_cast<size_t>(field_width)) {
        return StringType(static_cast<size_t>(field_width) - s.size(), fill_char) + s;
    }
    
	return s;
}

template <typename IntType, typename StringType>
StringType to_string (typename enable_if<is_signed<IntType>::value, IntType>::type value
        , int field_width
		, int radix
		, bool uppercase
        , typename StringType::value_type fill_char)
{
	char buf[sizeof(IntType) * 8 + 1];
	char * str = intmax_to_cstr(static_cast<intmax_t>(value)
			, radix
			, uppercase
            , buf
			, sizeof(buf));
    
    StringType s(str);

    // TODO Implement left-aligned text (field_width is negative)
    if (field_width > 0 && s.size() < static_cast<size_t>(field_width)) {
        if (value < 0 && !pfs::is_space(fill_char)) { // -000000123
            return StringType(1, '-') 
                    + StringType(static_cast<size_t>(field_width) - s.size() - 1, fill_char)
                    + StringType(s.cbegin() + 1, s.cend()); // ignore sign
        } else { // -123, 123, _______-123, ________123
            return StringType(static_cast<size_t>(field_width) - s.size(), fill_char) + s;
        }
    }
    
	return s;
}

}} // details::integral

template <typename StringType>
inline StringType to_string (bool value)
{
	return value ? StringType("true") : StringType("false");
}

template <typename StringType>
inline StringType to_string (char a
        , int field_width = 0
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    StringType s(1, typename StringType::value_type(a));
    
    if (field_width > 0 && s.size() < static_cast<size_t>(field_width)) {
        return StringType(static_cast<size_t>(field_width) - s.size(), fill_char) + s;
    }
    
    return s;
}

template <typename StringType>
inline StringType to_string (signed char a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<signed char, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

template <typename StringType>
inline StringType to_string (unsigned char a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<unsigned char, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

template <typename StringType>
inline StringType to_string (short a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<short, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

template <typename StringType>
inline StringType to_string (unsigned short a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<unsigned short, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

template <typename StringType>
inline StringType to_string (int a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<int, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

template <typename StringType>
inline StringType to_string (unsigned int a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<unsigned int, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

template <typename StringType>
inline StringType to_string (long a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<long, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

template <typename StringType>
inline StringType to_string (unsigned long a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<unsigned long, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

#if PFS_HAVE_LONGLONG

template <typename StringType>
inline StringType to_string (long long a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<long long, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
}

template <typename StringType>
inline StringType to_string (unsigned long long a
        , int field_width = 0
        , int radix = 10
        , bool uppercase = false
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::integral::to_string<unsigned long long, StringType>(a
            , field_width
            , radix
            , uppercase
            , fill_char);
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
		, int prec
        , int field_width
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
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
    
    
    // TODO Implement left-aligned text (field_width is negative)
    if (field_width > 0 && s.size() < static_cast<size_t>(field_width)) {
        if (value < 0 && !pfs::is_space(fill_char)) { // -000000123
            return StringType(1, '-') 
                    + StringType(static_cast<size_t>(field_width) - s.size() - 1, fill_char)
                    + StringType(s.cbegin() + 1, s.cend()); // ignore sign
        } else { // -123, 123, _______-123, ________123
            return StringType(static_cast<size_t>(field_width) - s.size(), fill_char) + s;
        }
    }

	return s;
}

}} // details::fp

template <typename StringType>
inline StringType to_string (float a
        , char format = 'f'
        , int precision = -1
        , int field_width = 0
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::fp::to_string<float, StringType>(a
            , format
            , precision
            , field_width
            , fill_char);
}

template <typename StringType>
inline StringType to_string (double a
        , char format = 'g'
        , int precision = -1
        , int field_width = 0
        , typename StringType::value_type fill_char = typename StringType::value_type(' '))
{
    return details::fp::to_string<double, StringType>(a
            , format
            , precision
            , field_width
            , fill_char);
}

template <typename StringType>
inline StringType left_justified (StringType const & s
        , int width
        , typename StringType::value_type fill_char = typename StringType::value_type(' ')
        , bool trancate = false)
{
    if (width < 0)
        return right_justified(s, -1 * width, fill_char, trancate);
        
    // TODO Implement left-aligned text (field_width is negative)
    if (width > 0 && s.size() < static_cast<size_t>(width)) {
        if (value < 0 && !pfs::is_space(fill_char)) { // -000000123
            return StringType(1, '-') 
                    + StringType(static_cast<size_t>(field_width) - s.size() - 1, fill_char)
                    + StringType(s.cbegin() + 1, s.cend()); // ignore sign
        } else { // -123, 123, _______-123, ________123
            return StringType(static_cast<size_t>(field_width) - s.size(), fill_char) + s;
        }
    }
}

template <typename StringType>
inline StringType right_justified (StringType const & s
        , int width
        , typename StringType::value_type fill_char = typename StringType::value_type(' ')
        , bool trancate = false)
{
    if (width < 0)
        return left_justified(s, -1 * width, fill_char, trancate);

    // TODO Implement left-aligned text (field_width is negative)
    if (width > 0 && s.size() < static_cast<size_t>(width)) {
        if (value < 0 && !pfs::is_space(fill_char)) { // -000000123
            return StringType(1, '-') 
                    + StringType(static_cast<size_t>(field_width) - s.size() - 1, fill_char)
                    + StringType(s.cbegin() + 1, s.cend()); // ignore sign
        } else { // -123, 123, _______-123, ________123
            return StringType(static_cast<size_t>(field_width) - s.size(), fill_char) + s;
        }
    }
    
    return s;
}

template <typename StringType>
inline StringType justified (StringType const & s
        , int width
        , typename StringType::value_type fill_char = typename StringType::value_type(' ')
        , bool trancate = false)
{
    if (width < 0)
        return left_justified(s, -1 * width, fill_char, trancate);
    
    return right_justified(s, width, fill_char, trancate);
}

} // pfs

#endif /* __PFS_STRING_HPP__ */

