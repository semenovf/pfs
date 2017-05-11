/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 10:49 AM
 */

#ifndef __PFS_TRAITS_STRING_HPP__
#define __PFS_TRAITS_STRING_HPP__

#include <pfs/exception.hpp>
#include <pfs/iterator.hpp>

namespace pfs {
namespace traits {

template <typename StringSpecificTraits>
class string
{
    typedef StringSpecificTraits                           internal_type;
public:
    typedef string<typename internal_type::string_value_type>      string_value_type;
    typedef string<typename internal_type::string_reference_type>  string_reference_type;

    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;

    typedef typename internal_type::value_type             value_type;
    typedef typename internal_type::reference              reference;
    typedef typename internal_type::const_reference        const_reference;
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
        return this->compare(pos1, count1, rhs._p, 0, rhs.size());
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
    
    string & append (size_type count, const_reference ch)
    {
        _p.append(count, ch);
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
        _p.append(s, count);
        return *this;
    }
    
    string & append (const_pointer s)
    {
        return append(s, internal_type::length(s));
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
};

template <typename StringSpecificTraits>
typename string<StringSpecificTraits>::string_value_type
string<StringSpecificTraits>::substr (size_type pos, size_type count) const
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

}} // pfs::traits

#if __OBSOLETE__

#include <ostream>
#include <pfs/exception.hpp>
#include <pfs/iterator.hpp>
#include <pfs/algo/find.hpp>
#include <pfs/type_traits.hpp>

namespace pfs {
namespace traits {

template <typename StringT>
string<StringT> & 
string<StringT>::insert (size_type index
        , size_type count
        , value_type ch)
{
    if (index > size())
        throw pfs::out_of_range("string::insert()");

    if (size() + count > max_size())
        throw pfs::length_error("string::insert()");

    _d.insert(index, count, ch);
    return *this;
}
    
template <typename StringT>
string<StringT> &
string<StringT>::insert (size_type index, const_pointer s)
{
    if (index > size())
        throw pfs::out_of_range("string::insert()");

    if (size() + rep_type::length(s) > max_size())
        throw pfs::length_error("string::insert()");

    _d.insert(index, s);
    return *this;
}
    
template <typename StringT>
string<StringT> &
string<StringT>::insert (size_type index
        , const_pointer s
        , size_type count)
{
    if (index > size())
        throw pfs::out_of_range("string::insert()");

    if (size() + count > max_size())
        throw pfs::length_error("string::insert()");

    _d.insert(index, s, count);
    return *this;
}

template <typename StringT>
class c_str;

template <typename StringT>
class c_wstr;

}} // pfs::traits

namespace std {

template <typename StringT>
ostream & operator << (ostream & out, pfs::traits::string<StringT> const & s)
{
    return out << pfs::traits::c_str<StringT>(s)();
}

template <typename StringT>
wostream & operator << (wostream & out, pfs::traits::string<StringT> const & s)
{
    return out << pfs::traits::c_wstr<StringT>(s)();
}

} // std

#endif

#endif /* __PFS_TRAITS_STRING_HPP__ */

