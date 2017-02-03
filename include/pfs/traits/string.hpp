/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 10:49 AM
 * 
 * @brief Immutable string interface.
 */

#ifndef __PFS_TRAITS_STRING_HPP__
#define __PFS_TRAITS_STRING_HPP__

#include <ostream>
#include <pfs/exception.hpp>
#include <pfs/iterator.hpp>
#include <pfs/algo/find.hpp>
#include <pfs/type_traits.hpp>

namespace pfs {
namespace traits {

template <typename CharT>
struct string_traits
{
    typedef CharT const * native_type;
    typedef CharT const * const_native_reference;
    typedef size_t        size_type;
    typedef CharT         value_type;
    typedef CharT const * const_pointer;
    typedef CharT const * const_iterator;
    typedef CharT const * const_reverse_iterator;
    typedef native_type   data_type;

    static void xassign (data_type & d, const_native_reference lhs);
    static void xassign (data_type & d, const_pointer lhs, size_type n);
    static size_type xsize (data_type const & d);
    static const_iterator xbegin (data_type const & d);
    static const_iterator xend (data_type const & d);
    static const_reverse_iterator xrbegin (data_type const & d);
    static const_reverse_iterator xrend (data_type const & d);
    static value_type xat (data_type const & d, size_type pos);

    static int xcompare (data_type const & d
            , size_type pos1, size_type count1
            , data_type const & rhs
            , size_type pos2, size_type count2);
    
    static size_type xfind (data_type const & d
            , data_type const & rhs
            , size_type pos);
    
    static size_type xfind (data_type const & d
            , value_type c
            , size_type pos);

    static size_type xrfind (data_type const & d
            , data_type const & rhs
            , size_type pos);
    
    static size_type xrfind (data_type const & d
            , value_type c
            , size_type pos);
    
    static const_pointer xdata (data_type const & d);
    
    static const_native_reference xcast (data_type const & d);
};

template <typename CharT>
class string
{
public:
    typedef string_traits<CharT>                         traits_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::const_native_reference const_native_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::value_type             value_type;
    typedef typename traits_type::const_pointer          const_pointer;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::data_type              data_type;
    
private:
    data_type _d;
    
public:
    string ()
        : _d()
    {}
    
    explicit string (const_native_reference s)
        : _d(s)
    {}

    /**
     * @details @a str Interpreted as UTF-8 encoded string. 
     * @param str
     * @note 
     */
    explicit string (CharT const * str, size_type n)
        : _d(str, n)
    {}

    explicit string (const_pointer str)
        : _d(str)
    {}
    
    string (const_iterator begin, const_iterator end)
        : _d(begin, end)
    {}

    string (string const & rhs)
        : _d(rhs._d)
    {}
    
    string & operator = (string const & rhs)
    {
        traits_type::xassign(_d, traits_type::xcast(rhs._d));
        return *this;
    }

    string & operator = (const_native_reference rhs)
    {
        traits_type::xassign(_d, rhs);
        return *this;
    }

    string & operator = (const_pointer rhs)
    {
        traits_type::xassign(_d, rhs, size_type(-1));
        return *this;
    }
    
    const_pointer data () const
    {
        return traits_type::xdata(_d);
    }
    
    const_native_reference cast () const
    {
        return traits_type::xcast(_d);
    }

    size_type size () const
    {
        return traits_type::xsize(_d);
    }
    
    size_type length () const
    {
        return this->size();
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
        return traits_type::xbegin(_d);
    }

    const_iterator end () const
    {
        return traits_type::xend(_d);
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
        return traits_type::xrbegin(_d);
    }

    const_reverse_iterator rend () const
    {
        return traits_type::xrend(_d);
    }

    const_reverse_iterator crbegin () const
    {
        return this->rbegin();
    }

    const_reverse_iterator crend () const
    {
        return this->rend();
    }

    /**
     * @brief
     * @param pos Position of the character to return.
     * @trows out_of_range if pos >= size()
     */
    value_type at (size_type pos) const
    {
        if (pos >= this->size())
            throw out_of_range("string::at");
        return traits_type::xat(_d, pos);
    }
    
    value_type operator [] (size_type pos) const
    {
    	return this->at(pos);
    }
    
    int compare (size_type pos1, size_type count1
        , string const & rhs, size_type pos2, size_type count2) const
    {
        return traits_type::xcompare(_d, pos1, count1, rhs._d, pos2, count2) ;
    }
    
    int compare (size_type pos1, size_type count1, string const & rhs) const
    {
        return traits_type::xcompare(_d, pos1, count1, rhs._d, 0, rhs.size());
    }
    
    int compare (string const & rhs) const
    {
        return traits_type::xcompare(_d, 0, this->size(), rhs._d, 0, rhs.size());
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
    
    bool contains (string const & rhs, size_type pos = 0) const
	{
		return traits_type::xfind(_d, rhs._d, pos) != size_type(-1);
	}

    bool contains (const_native_reference rhs, size_type pos = 0)
    {
        return traits_type::xfind(_d, rhs._d, pos) != size_type(-1);
    }
    
	bool contains (value_type c, size_type pos = 0) const
	{
        return traits_type::xfind(_d, c, pos) != size_type(-1);
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
    size_type find (string const & rhs, size_type pos = 0) const
    {
        return traits_type::xfind(_d, rhs._d, pos);
    }
    
    size_type find (const_native_reference rhs, size_type pos = 0) const
    {
        return traits_type::xfind(_d, rhs._d, pos);
    }
    
    size_type find (value_type c, size_type pos = 0) const
    {
        return traits_type::xfind(_d, c, pos);
    }
    
    size_type rfind (string const & rhs, size_type pos = size_type(-1)) const
    {
        return traits_type::xrfind(_d, rhs._d, pos);
    }
    
    size_type rfind (const_native_reference rhs, size_type pos = size_type(-1)) const
    {
        return traits_type::xfind(_d, rhs._d, pos);
    }
    
    size_type rfind (value_type c, size_type pos = size_type(-1)) const
    {
        return traits_type::xrfind(_d, c, pos);
    }
    
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
    string substr (size_type pos, size_type count = size_type(-1)) const;
    
	string left (size_t count) const
	{
		return substr(0, count);
	}

	string right (size_t count) const
	{
		return substr(this->size() - count, count);
	}
  
    template <typename CharU>
    friend int compare (string<CharU> const & lhs, char const * rhs);

    template <typename CharU>
    friend int compare (string<CharU> const & lhs, wchar_t const * rhs);
};

template <typename CharT>
string<CharT> string<CharT>::substr (size_type pos, size_type count) const
{
    if (pos >= this->size())
        throw out_of_range("string::substr");
    size_type n = this->size() - pos;

    if (n > count)
        n = count;

    const_iterator b(this->begin());
    const_iterator e(this->begin());
    pfs::advance(b, pos);
    pfs::advance(e, pos + n);

    return string(b, e);
}

template <typename CharT>
inline bool operator == (string<CharT> const & lhs, string<CharT> const & rhs)
{
    return lhs.compare(rhs) == 0;
}

template <typename CharT>
inline bool operator != (string<CharT> const & lhs, string<CharT> const & rhs)
{
    return lhs.compare(rhs) != 0;
}

template <typename CharT>
inline bool operator < (string<CharT> const & lhs, string<CharT> const & rhs)
{
    return lhs.compare(rhs) < 0;
}

template <typename CharT>
inline bool operator <= (string<CharT> const & lhs, string<CharT> const & rhs)
{
    return lhs.compare(rhs) <= 0;
}

template <typename CharT>
inline bool operator > (string<CharT> const & lhs, string<CharT> const & rhs)
{
    return lhs.compare(rhs) > 0;
}

template <typename CharT>
inline bool operator >= (string<CharT> const & lhs, string<CharT> const & rhs)
{
    return lhs.compare(rhs) >= 0;
}

template <typename CharT>
inline bool operator == (string<CharT> const & lhs, char const * rhs)
{
    return compare(lhs, rhs) == 0;
}

template <typename CharT>
inline bool operator == (char const * lhs, string<CharT> const & rhs)
{
    return compare(rhs, lhs) == 0;
}

template <typename CharT>
inline bool operator != (string<CharT> const & lhs, char const * rhs)
{
    return compare(lhs, rhs) != 0;
}

template <typename CharT>
inline bool operator != (char const * lhs, string<CharT> const & rhs)
{
    return compare(rhs, lhs) != 0;
}

template <typename CharT>
inline bool operator < (string<CharT> const & lhs, char const * rhs)
{
    return compare(lhs, rhs) < 0;
}

template <typename CharT>
inline bool operator < (char const * lhs, string<CharT> const & rhs)
{
    return compare(rhs, lhs) > 0;
}

template <typename CharT>
inline bool operator <= (string<CharT> const & lhs, char const * rhs)
{
    return compare(lhs, rhs) <= 0;
}

template <typename CharT>
inline bool operator <= (char const * lhs, string<CharT> const & rhs)
{
    return compare(rhs, lhs) >= 0;
}

template <typename CharT>
inline bool operator > (string<CharT> const & lhs, char const * rhs)
{
    return compare(lhs, rhs) > 0;
}

template <typename CharT>
inline bool operator > (char const * lhs, string<CharT> const & rhs)
{
    return compare(rhs, lhs) < 0;
}

template <typename CharT>
inline bool operator >= (string<CharT> const & lhs, char const * rhs)
{
    return compare(lhs, rhs) >= 0;
}

template <typename CharT>
inline bool operator >= (char const * lhs, string<CharT> const & rhs)
{
    return compare(rhs, lhs) <= 0;
}

template <typename CharT>
class c_str;

template <typename CharT>
class c_wstr;

}} // pfs::traits

namespace std {

template <typename CharT>
ostream & operator << (ostream & out, pfs::traits::string<CharT> const & s)
{
    return out << pfs::traits::c_str<CharT>(s)();
}

template <typename CharT>
wostream & operator << (wostream & out, pfs::traits::string<CharT> const & s)
{
    return out << pfs::traits::c_wstr<CharT>(s)();
}

} // std

#endif /* __PFS_TRAITS_STRING_HPP__ */

