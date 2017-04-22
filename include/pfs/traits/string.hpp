/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 10:49 AM
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

template <typename Foundation, typename CharT>
struct string_traits
{
    typedef struct __Use_Specialized_Traits__ {} native_type;
    typedef native_type const & const_native_reference;
    typedef size_t        size_type;
    typedef CharT         value_type;
    typedef CharT &       reference;
    typedef CharT const & const_reference;
    typedef CharT const * const_pointer;
    typedef CharT *       iterator;
    typedef CharT const * const_iterator;
    typedef CharT const * const_reverse_iterator;
    typedef native_type   data_type;

    static size_type xlength (const_pointer p);
    static void xassign (data_type & d, const_native_reference lhs);
    static void xassign (data_type & d, const_pointer lhs, size_type n);
    static size_type xsize (data_type const & d);
    static size_type xmax_size (data_type const & d);
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

    static void xerase (data_type & d, size_type index, size_type count);
    static iterator xerase (data_type & d, const_iterator first, const_iterator last);

    static void xclear (data_type & d);
   
    static void xappend (data_type & d, size_type count, const_reference ch);
    static void xappend (data_type & d, const_pointer s, size_type count);
    
    static void xinsert (data_type & d, size_type index, size_type count, value_type ch);
    static void xinsert (data_type & d, size_type index, const_pointer s);
    static void xinsert (data_type & d, size_type index, const_pointer s, size_type count);

    static void xpush_back (data_type & d, value_type ch);

    static const_pointer xdata (data_type const & d);
    
    static const_native_reference xcast (data_type const & d);
    
    static intmax_t xint_cast (const_iterator first
            , const_iterator last
            , int radix
            , intmax_t min_value
            , uintmax_t max_value
            , const_iterator * endptr);

    static intmax_t xuint_cast (const_iterator first
            , const_iterator last
            , int radix
            , uintmax_t max_value
            , const_iterator * endptr);
};

template <typename Foundation, typename CharT>
class string
{
public:
    typedef Foundation                                   foundation;
    typedef string_traits<Foundation, CharT>             traits_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::const_native_reference const_native_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::value_type             value_type;
    typedef typename traits_type::reference              reference;
    typedef typename traits_type::const_reference        const_reference;
    typedef typename traits_type::const_pointer          const_pointer;
    typedef typename traits_type::const_iterator         iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::data_type              data_type;

    static size_type const npos = size_type(-1);

private:
    data_type _d;
    
public:
    string ()
        : _d()
    {}
    
    explicit string (const_native_reference s)
        : _d(s)
    {}

    explicit string (const_pointer str, size_type n)
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
    
    operator const_native_reference () const
    {
        return cast();
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

        return traits_type::xmax_size(_d);
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
    
    string & erase (size_type index = 0, size_type count = size_type(-1))
    {
        if (index > size())
            throw out_of_range("string::erase()");
        traits_type::xerase(_d, index, count);
        return *this;
    }
    
    iterator erase (const_iterator position)
    {
        return traits_type::xerase(_d, position, position + 1);
    }
    
    iterator erase (const_iterator first, const_iterator last)
    {
        return traits_type::xerase(_d, first, last);
    }
  
    void clear ()
    {
        traits_type::xclear(_d);
    }
    
    string & append (size_type count, const_reference ch)
    {
        traits_type::xappend(_d, count, ch);
        return *this;
    }
    
    string & append (string const & str)
    {
        return append(str, 0, str.size());
    }
    
    string & append (string const & str
                , size_type pos
                , size_type count)
    {
        if (pos >= str.size())
            throw out_of_range("string::append()");
        
        if (count == size_type(-1) || pos + count > str.size())
            count = str.size() - pos;
        
        return append(str.data() + pos, count);
    }
    
    string & append (const_pointer s, size_type count)
    {
        traits_type::xappend(_d, s, count);
        return *this;
    }
    
    string & append (const_pointer s)
    {
        return append(s, traits_type::xlength(s));
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
        traits_type::xpush_back(_d, ch);
    }

    template <typename FoundationU, typename CharU>
    friend int compare (string<FoundationU, CharU> const & lhs, char const * rhs);

    template <typename FoundationU, typename CharU>
    friend int compare (string<FoundationU, CharU> const & lhs, wchar_t const * rhs);
};

template <typename Foundation, typename CharT>
string<Foundation, CharT> 
string<Foundation, CharT>::substr (size_type pos, size_type count) const
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

    return string(b, e);
}

template <typename Foundation, typename CharT>
string<Foundation, CharT> & 
string<Foundation, CharT>::insert (size_type index
        , size_type count
        , value_type ch)
{
    if (index > size())
        throw pfs::out_of_range("string::insert()");

    if (size() + count > max_size())
        throw pfs::length_error("string::insert()");

    traits_type::xinsert(_d, index, count, ch);
    return *this;
}
    
template <typename Foundation, typename CharT>
string<Foundation, CharT> &
string<Foundation, CharT>::insert (size_type index, const_pointer s)
{
    if (index > size())
        throw pfs::out_of_range("string::insert()");

    if (size() + traits_type::xlength(s) > max_size())
        throw pfs::length_error("string::insert()");

    traits_type::xinsert(_d, index, s);
    return *this;
}
    
template <typename Foundation, typename CharT>
string<Foundation, CharT> &
string<Foundation, CharT>::insert (size_type index
        , const_pointer s
        , size_type count)
{
    if (index > size())
        throw pfs::out_of_range("string::insert()");

    if (size() + count > max_size())
        throw pfs::length_error("string::insert()");

    traits_type::xinsert(_d, index, s, count);
    return *this;
}

template <typename Foundation, typename CharT>
inline bool operator == (string<Foundation, CharT> const & lhs
        , string<Foundation, CharT> const & rhs)
{
    return lhs.compare(rhs) == 0;
}

template <typename Foundation, typename CharT>
inline bool operator != (string<Foundation, CharT> const & lhs
        , string<Foundation, CharT> const & rhs)
{
    return lhs.compare(rhs) != 0;
}

template <typename Foundation, typename CharT>
inline bool operator < (string<Foundation, CharT> const & lhs
        , string<Foundation, CharT> const & rhs)
{
    return lhs.compare(rhs) < 0;
}

template <typename Foundation, typename CharT>
inline bool operator <= (string<Foundation, CharT> const & lhs
        , string<Foundation, CharT> const & rhs)
{
    return lhs.compare(rhs) <= 0;
}

template <typename Foundation, typename CharT>
inline bool operator > (string<Foundation, CharT> const & lhs
        , string<Foundation, CharT> const & rhs)
{
    return lhs.compare(rhs) > 0;
}

template <typename Foundation, typename CharT>
inline bool operator >= (string<Foundation, CharT> const & lhs
        , string<Foundation, CharT> const & rhs)
{
    return lhs.compare(rhs) >= 0;
}

template <typename Foundation, typename CharT>
inline bool operator == (string<Foundation, CharT> const & lhs
        , char const * rhs)
{
    return compare(lhs, rhs) == 0;
}

template <typename Foundation, typename CharT>
inline bool operator == (char const * lhs
        , string<Foundation, CharT> const & rhs)
{
    return compare(rhs, lhs) == 0;
}

template <typename Foundation, typename CharT>
inline bool operator != (string<Foundation, CharT> const & lhs
        , char const * rhs)
{
    return compare(lhs, rhs) != 0;
}

template <typename Foundation, typename CharT>
inline bool operator != (char const * lhs
        , string<Foundation, CharT> const & rhs)
{
    return compare(rhs, lhs) != 0;
}

template <typename Foundation, typename CharT>
inline bool operator < (string<Foundation, CharT> const & lhs
        , char const * rhs)
{
    return compare(lhs, rhs) < 0;
}

template <typename Foundation, typename CharT>
inline bool operator < (char const * lhs
        , string<Foundation, CharT> const & rhs)
{
    return compare(rhs, lhs) > 0;
}

template <typename Foundation, typename CharT>
inline bool operator <= (string<Foundation, CharT> const & lhs
        , char const * rhs)
{
    return compare(lhs, rhs) <= 0;
}

template <typename Foundation, typename CharT>
inline bool operator <= (char const * lhs
        , string<Foundation, CharT> const & rhs)
{
    return compare(rhs, lhs) >= 0;
}

template <typename Foundation, typename CharT>
inline bool operator > (string<Foundation, CharT> const & lhs
        , char const * rhs)
{
    return compare(lhs, rhs) > 0;
}

template <typename Foundation, typename CharT>
inline bool operator > (char const * lhs
        , string<Foundation, CharT> const & rhs)
{
    return compare(rhs, lhs) < 0;
}

template <typename Foundation, typename CharT>
inline bool operator >= (string<Foundation, CharT> const & lhs
        , char const * rhs)
{
    return compare(lhs, rhs) >= 0;
}

template <typename Foundation, typename CharT>
inline bool operator >= (char const * lhs
        , string<Foundation, CharT> const & rhs)
{
    return compare(rhs, lhs) <= 0;
}

template <typename Foundation, typename CharT>
class c_str;

template <typename Foundation, typename CharT>
class c_wstr;

}} // pfs::traits

namespace std {

template <typename Foundation, typename CharT>
ostream & operator << (ostream & out
        , pfs::traits::string<Foundation, CharT> const & s)
{
    return out << pfs::traits::c_str<Foundation, CharT>(s)();
}

template <typename Foundation, typename CharT>
wostream & operator << (wostream & out
        , pfs::traits::string<Foundation, CharT> const & s)
{
    return out << pfs::traits::c_wstr<Foundation, CharT>(s)();
}

} // std

#endif /* __PFS_TRAITS_STRING_HPP__ */

