#pragma once
#include <string>
#include <iterator>
#include <pfs/types.hpp>
//#include <pfs/limits.hpp>
//#include <pfs/type_traits.hpp>
#include <pfs/memory.hpp>
//#include <pfs/algorithm.hpp>
//#include <pfs/iterator.hpp>
//#include <pfs/utility.hpp>
//#include <pfs/io/exception.hpp>

namespace pfs {

class byte_string : public std::basic_string<uint8_t>
{
    typedef std::basic_string<uint8_t> base_class;

public:
    //using base_class::append;
    using base_class::assign;
    using base_class::data;
    using base_class::erase;
    using base_class::insert;

    typedef base_class::value_type       value_type;
    typedef base_class::traits_type      traits_type;
    typedef base_class::size_type        size_type;
    typedef base_class::difference_type  difference_type;
    typedef base_class::reference        reference;
    typedef base_class::const_reference  const_reference;
    typedef base_class::pointer          pointer;
    typedef base_class::const_pointer    const_pointer;
    typedef base_class::iterator         iterator;
    typedef base_class::const_iterator   const_iterator;
    typedef base_class::reverse_iterator reverse_iterator;
    typedef base_class::const_reverse_iterator const_reverse_iterator;

    typedef pfs::shared_ptr<byte_string> shared_type;

    static const size_type npos = base_class::npos;

// private:
//     explicit byte_string (base_class const & s)
//         : base_class(s)
//     {}
//
//     byte_string & assign (base_class const & d)
//     {
//         _d.assign(d);
//         return *this;
//     }

public:
    byte_string () : base_class () {}

    byte_string (byte_string const & other)
        : base_class(other)
    {}

    byte_string (byte_string const & other,
            size_type pos,
            size_type count = npos)
        : base_class(other, pos, count)
    {}

    explicit byte_string (const_pointer s)
        : base_class(s)
    {}

    byte_string(const_pointer s, size_type n)
        : base_class(s, n)
    {}

    explicit byte_string (const char * s)
        : base_class(reinterpret_cast<const uint8_t *> (s))
    {}

    byte_string (const char * s, size_type n)
        : base_class(reinterpret_cast<const uint8_t *> (s), n)
    {}

    byte_string (size_type n, uint8_t c)
        : base_class(n, c)
    {}

    byte_string (size_type n, char c)
        : base_class(n, static_cast<uint8_t> (c))
    {}

    template <typename InputIterator>
    byte_string (InputIterator first, InputIterator last)
        : base_class(first, last)
    {}

    ~byte_string ()
    {}

    /**
     * @brief Default copy assignment
     * @details Replaces the contents with the copy of @a s
     */
    byte_string & operator = (byte_string const & s)
    {
        base_class::operator = (s);
        return *this;
    }

    byte_string & operator = (const_pointer s)
    {
        base_class::operator = (s);
        return *this;
    }

    byte_string & operator = (char const * s)
    {
        base_class::operator = (reinterpret_cast<const_pointer>(s));
        return *this;
    }

    byte_string & operator = (value_type c)
    {
        base_class::operator = (c);
        return *this;
    }

    byte_string & operator = (char c)
    {
        base_class::operator = (value_type(c));
        return *this;
    }

    /**
     * @fn byte_string & assign (size_type count, value_type c)
     * @brief Replaces the contents with count copies of value @a c.
     */

    /**
     * @fn byte_string & assign (size_type count, char c)
     * @brief Replaces the contents with count copies of character @a c.
     */

    /**
     * @fn byte_string & assign (byte_string const & bs)
     * @brief Replaces the contents with a copy of @a bs.
     */

    /**
     * @fn byte_string & assign (byte_string const & bs, size_type pos, size_type count)
     * @details Replaces the contents with a substring [pos, pos + count) of @a bs.
     */

    /**
     * @fn byte_string & assign (const_pointer s, size_type count)
     * @details Replaces the contents with the first count of values from @a s.
     */

    /**
     * @details Replaces the contents with the first count of chars from @a s.
     */
    byte_string & assign (char const * s, size_type count)
    {
        base_class::assign(reinterpret_cast<const_pointer>(s), count);
        return *this;
    }

    /**
     * @fn byte_string & assign (const_pointer s)
     */

    /**
     */
    byte_string & assign (char const * s)
    {
        base_class::assign(reinterpret_cast<const_pointer>(s));
        return *this;
    }

    /**
     * @fn template <typename InputIterator>
     *     byte_string & assign (InputIterator first, InputIterator last)
     */

    //
    // Element access
    //

    /**
     * @fn reference at (size_type pos)
     */

    /**
     * @fn const_reference at (size_type pos) const
     */

    /**
     * @fn reference operator[] (size_type pos)
     */

    /**
     * @fn const_reference operator [] (size_type pos) const
     */

#if __cplusplus < 201103L
    reference front ()
    {
        PFS_ASSERT(!empty());
        return at(0);
    }
#endif

#if __cplusplus < 201103L
    const_reference front () const
    {
        PFS_ASSERT(!empty());
        return at(0);
    }
#endif

#if __cplusplus < 201103L
    reference back ()
    {
        PFS_ASSERT(!empty());
        return at(size() - 1);
    }
#endif

#if __cplusplus < 201103L
    const_reference back () const
    {
        PFS_ASSERT(!empty());
        return at(size() - 1);
    }
#endif

    /**
     * @fn const_pointer data () const
     */

#if __cplusplus < 201103L
    /**
    */
    pointer data ()
    {
        return & operator [] (0);
    }
#endif

    //
    // @note `const_pointer c_str () const` is inaccessable
    //
    const char * c_str () const
    {
        return reinterpret_cast<const char *> (base_class::c_str());
    }

    ///////////////////////////////////////////////////////////////////////////
    // Iterators                                                             //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn iterator begin ()
     */

    /**
     * @fn const_iterator begin () const
     */

#if __cplusplus < 201103L
    const_iterator cbegin () const
    {
        return begin();
    }
#endif

    /**
     * @fn iterator end ()
     */

    /**
     * @fn const_iterator end () const
     */

#if __cplusplus < 201103L
    const_iterator cend () const
    {
        return end();
    }
#endif

    /**
     * @fn reverse_iterator rbegin ()
     */

    /**
     * @fn const_reverse_iterator rbegin () const
     */

#if __cplusplus < 201103L
    const_reverse_iterator crbegin () const
    {
        return base_class::rbegin();
    }
#endif

    /**
     * @fn reverse_iterator rend ()
     */

    /**
     * @fn const_reverse_iterator rend () const
     */

#if __cplusplus < 201103L
    const_reverse_iterator crend () const
    {
        return base_class::rend();
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Capacity                                                              //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn bool empty () const
     * @brief Checks if byte string is empty.
     * @return @c true if byte string is empty (size() == 0) or @c false otherwise.
     */

    /**
     * @fn size_type size () const
     * @brief Return size of string in code units.
     * @return String in code units.
     */

    /**
     * @fn size_type length () const
     * @brief Return size of byte string.
     * @return Size of byte string.
     * @see size()
     */

    /**
     * @fn size_type max_size () const
     */

    /**
     */
    void reserve (size_type new_cap)
    {
        base_class::reserve(new_cap);
    }

    /**
     * @fn size_type capacity () const
     */

#if __cplusplus < 201103L
    // @note Result may not be the same as for C++11 version
    void shrink_to_fit ()
    {
        if (capacity() > size()) {
            reserve(0);
        }
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Operations                                                            //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn void clear ()
     */

    /**
     * @fn byte_string & insert (size_type index, size_type count, value_type ch)
     *
     * @brief Inserts @c count copies of character @c ch at the position @c index.
     * @param index Position at which the content will be inserted.
     * @param count Number of characters to insert.
     * @param ch Character to insert.
     * @return @code *this @endcode
     */

    /**
     * @fn byte_string & insert (size_type index, size_type count, char ch)
     *
     * @brief Inserts @c count copies of character @c ch at the position @c index.
     * @param index Position at which the content will be inserted.
     * @param count Number of characters to insert.
     * @param ch Character to insert.
     * @return @code *this @endcode
     */

    /**
     * @fn byte_string & insert (size_type index, const_pointer s)
     *
     * @brief Inserts null-terminated character string pointed to by @c s at the position @c index.
     *        The length of the string is determined by the first null character.
     * @param index Position at which the content will be inserted.
     * @param s Pointer to the character string to insert.
     * @return @code *this @endcode
     */

    /**
     * @brief Inserts null-terminated character string pointed to by @c s at the position @c index.
     *        The length of the string is determined by the first null character.
     * @param index Position at which the content will be inserted.
     * @param s Pointer to the character string to insert.
     * @return @code *this @endcode
     */
    byte_string & insert (size_type index, char const * s)
    {
        base_class::insert(index, reinterpret_cast<const_pointer>(s));
        return *this;
    }

    /**
     * @fn byte_string & insert (size_type index, const_pointer s, size_type count)
     */

    /**
     * @fn byte_string & insert (size_type index, char const * s, size_type count)
     */
    byte_string & insert (size_type index, char const * s, size_type count)
    {
        base_class::insert(index, reinterpret_cast<const_pointer>(s), count);
        return *this;
    }

    /**
     * @fn byte_string & insert (size_type index, byte_string const & s)
     */

    /**
     */
    byte_string & insert (size_type index, std::string const & s)
    {
        base_class::insert(index, reinterpret_cast<const_pointer>(s.data()), s.size());
        return *this;
    }

    /**
     * @fn basic_string& insert (size_type index, basic_string const & str
     *              , size_type index_str, size_type count)
     */
#if __cplusplus < 201402L
    // Reimplement to meet C++14 
    basic_string & insert (size_type index, basic_string const & str
                   , size_type index_str, size_type count)
    {
        base_class::insert(index, str, index_str, count);
        return *this;
    }
#endif

    basic_string & insert (size_type index, std::string const & str
                   , size_type index_str, size_type count)
    {
        base_class::insert(index
                , reinterpret_cast<const_pointer>(str.data() + index_str)
                , count);
        return *this;
    }

    /**
     * @fn iterator insert (const_iterator pos, value_type ch)
     */
    //
    // http://en.cppreference.com/w/cpp/string/basic_string/insert
    // says 
    //      iterator insert (iterator pos, CharT ch ); (until C++11)
    // but gcc implements it as meet C++11 standard
    //      iterator insert (iterator pos, CharT ch ); (since C++11)
    // so need checks with various versions of standard C++ library
    //
#if __cplusplus < 201103L
    iterator insert (const_iterator pos, value_type ch)
    {
        size_type index = pos - begin();
        byte_string::insert(index, 1, ch);
        return iterator(begin() + index);
    }
#endif

    /**
     */
    iterator insert (const_iterator pos, char ch)
    {
        // Do not use base_class::insert here.
        // Will be used above method for C++ prior to C++11
        return byte_string::insert(pos, value_type(ch));
    }

    /**
     * @fn iterator insert (const_iterator pos, size_type count, value_type ch);
     */
#if __cplusplus < 201103L
    iterator insert (const_iterator pos, size_type count, value_type ch)
    {
        size_type index = std::distance(cbegin(), pos);
        base_class::insert(begin() + index, count, ch);
        return begin() + index;
    }
#endif

    iterator insert (const_iterator pos, size_type count, char ch)
    {
        return byte_string::insert(pos, count, value_type(ch));
    }

    /**
     * @fn template <typename InputIt>
     *     iterator insert (const_iterator pos, InputIt first, InputIt last);
     */
#if __cplusplus < 201103L
    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        size_type index = std::distance(cbegin(), pos);
        base_class::insert(begin() + index, first, last);
        return begin() + index;
    }
#endif

#if __cplusplus >= 201103L
    /**
    * @fn iterator insert (const_iterator pos, std::initializer_list<value_type> ilist)
    */
    
    /**
     */
    iterator insert (const_iterator pos, std::initializer_list<char> ilist)
    {
        return base_class::insert(pos, ilist.begin(), ilist.end());
    }
#endif

#if __cplusplus > 201402L
    /**
     * @fn template <typename T>
     *     basic_string & insert (size_type pos, T const & t)
     */
    
    /**
     * /@fn template <typename T>
     *      basic_string & insert (size_type index, T const & t
     *              , size_type index_str, size_type count = npos)
     */
#endif




//     byte_string & erase (size_type index = 0, size_type count = npos)
//     {
//         PFS_ASSERT(index <= size());
//         _d.erase(index, count);
//         return *this;
//     }

#if __cplusplus < 201103L
    iterator erase (const_iterator position)
    {
// || PFS_CC_GCC_VERSION <= 40800 // TODO Check for valid version
        size_type index = position - begin();
        if (index < size()) {
            erase(index, 1);
            return begin() + index;
        }
        return end();
    }
#endif

#if __cplusplus < 201103L
    iterator erase (const_iterator first, const_iterator last)
    {
// || PFS_CC_GCC_VERSION <= 40800 // TODO Check for valid version
        size_type pos1 = first - begin();
        size_type pos2 = last - begin();
        if (pos1 < pos2) {
            erase(pos1, pos2 - pos1);
            return begin() + pos1;
        }
        return end();
    }
#endif

//     void push_back (value_type ch)
//     {
//         _d.push_back(ch);
//     }

#if __cplusplus < 201103L
    void pop_back ()
    {
        if (!empty())
            erase(size() - 1, 1);
    }
#endif

//     byte_string & append (size_type count, value_type ch)
//     {
//         _d.append(count, ch);
//         return *this;
//     }

//     byte_string & append (size_type count, char ch)
//     {
//         return append(count, static_cast<value_type> (ch));
//     }

//     byte_string & append (byte_string const & s)
//     {
//         _d.append(s._d);
//         return *this;
//     }

//     byte_string & append (byte_string const & s
//             , size_type pos
//             , size_type count = npos)
//     {
//         _d.append(s._d, pos, count);
//         return *this;
//     }

//     byte_string & append (const_pointer s, size_type count)
//     {
//         _d.append(s, count);
//         return *this;
//     }

//     byte_string & append (const char * s, size_type count)
//     {
//         return append(reinterpret_cast<const_pointer> (s), count);
//     }

//     byte_string & append (const_pointer s)
//     {
//         _d.append(s);
//         return *this;
//     }

//     byte_string & append (const char * s)
//     {
//         return append(reinterpret_cast<const_pointer> (s));
//     }

//     template <typename InputIterator>
//     byte_string & append (InputIterator first, InputIterator last)
//     {
//         _d.append<InputIterator>(first, last);
//         return *this;
//     }

//     byte_string & operator += (byte_string const & s)
//     {
//         _d.operator += (s._d);
//         return *this;
//     }
//
//     byte_string & operator += (value_type ch)
//     {
//         _d.operator += (ch);
//         return *this;
//     }

//     byte_string & operator += (char ch)
//     {
//         return operator += (static_cast<value_type> (ch));
//     }

//     byte_string & operator += (const_pointer s)
//     {
//         _d.operator+= (s);
//         return *this;
//     }

//     byte_string & operator += (const char * s)
//     {
//         return operator += (reinterpret_cast<const_pointer> (s));
//     }

//     int compare (byte_string const & s) const
//     {
//         return _d.compare(s._d);
//     }

//     int compare (size_type pos1, size_type count1, byte_string const & s) const
//     {
//         return _d.compare(pos1, count1, s._d);
//     }

//     int compare (size_type pos1, size_type count1
//             , byte_string const & s
//             , size_type pos2, size_type count2 = npos) const
//     {
//         return _d.compare(pos1, count1, s._d, pos2, count2);
//     }

//     int compare (const_pointer s) const
//     {
//         return _d.compare(s);
//     }

//     int compare (const char * s) const
//     {
//         return compare(reinterpret_cast<const_pointer> (s));
//     }

//     int compare (size_type pos1, size_type count1, const_pointer s) const
//     {
//         return _d.compare(pos1, count1, s);
//     }

//     int compare (size_type pos1, size_type count1, const char * s) const
//     {
//         return compare(pos1, count1, reinterpret_cast<const_pointer> (s));
//     }

//     int compare (size_type pos1, size_type count1, const_pointer s, size_type count2) const
//     {
//         return _d.compare(pos1, count1, s, count2);
//     }

//     int compare (size_type pos1, size_type count1, const char * s, size_type count2) const
//     {
//         return compare(pos1, count1, reinterpret_cast<const_pointer> (s), count2);
//     }

//     byte_string & replace (size_type pos, size_type count, byte_string const & s)
//     {
//         _d.replace(pos, count, s._d);
//         return *this;
//     }

//     //	byte_string & replace (const_iterator first, const_iterator last, byte_string const & s)
//     //	{
//     //		_d.replace(first, last, s._d);
//     //		return *this;
//     //	}

//     byte_string & replace (size_type pos, size_type count
//             , byte_string const & s
//             , size_type pos2, size_type count2 = npos)
//     {
//         _d.replace(pos, count, s._d, pos2, count2);
//         return *this;
//     }

//     template <typename InputIterator>
//     byte_string & replace (const_iterator first, const_iterator last
//             , InputIterator first2, InputIterator last2)
//     {
//         _d.replace<InputIterator>(first, last, first2, last2);
//         return *this;
//     }

//     byte_string & replace (size_type pos, size_type count, const_pointer s, size_type count2)
//     {
//         _d.replace(pos, count, s, count2);
//         return *this;
//     }

//     //	byte_string & replace (const_iterator first, const_iterator last, const_pointer s, size_type count2)
//     //	{
//     //		_d.replace(first, last, s, count2);
//     //		return *this;
//     //	}

//     byte_string & replace (size_type pos, size_type count, const_pointer s)
//     {
//         _d.replace(pos, count, s);
//         return *this;
//     }

//     // FIXME
//     //	byte_string & replace (const_iterator first, const_iterator last, const_pointer s)
//     //	{
//     //		_d.replace(first, last, s);
//     //		return *this;
//     //	}

//     byte_string & replace (size_type pos, size_type count, size_type count2, value_type ch)
//     {
//         _d.replace(pos, count, count2, ch);
//         return *this;
//     }

//     // FIXME
//     //	byte_string & replace (const_iterator first, const_iterator last, size_type count2, value_type ch)
//     //	{
//     //		_d.replace(first, last, count2, ch);
//     //		return *this;
//     //	}

//     byte_string substr (size_type pos = 0, size_type count = npos) const
//     {
//         PFS_ASSERT(pos <= size());
//         return byte_string(_d.substr(pos, count));
//     }

//     byte_string & substr (byte_string & result, size_type pos = 0, size_type count = npos) const
//     {
//         PFS_ASSERT(pos <= size());
//         return result.assign(_d.substr(pos, count));
//     }

//     size_type copy (pointer dest, size_type count, size_type pos = 0) const
//     {
//         return _d.copy(dest, count, pos);
//     }

//     size_type copy (char * dest, size_type count, size_type pos = 0) const
//     {
//         return copy(reinterpret_cast<pointer> (dest), count, pos);
//     }

//     void resize (size_type count)
//     {
//         _d.resize(count);
//     }

//     void resize (size_type count, value_type ch)
//     {
//         _d.resize(count, ch);
//     }

//     void swap (byte_string & other)
//     {
//         _d.swap(other._d);
//     }
};

// template <int N>
// struct byte_string_ref_n
// {
//     typedef typename size_type_n<N>::type size_type;
//     byte_string * p;
//
//     byte_string_ref_n (byte_string * ptr)
//         : p (ptr)
//     {}
//
//     size_type max_size () const
//     {
//         return numeric_limits<size_type>::max();
//     }
// };

// struct byte_string_ref
// {
//     byte_string * p;
//     byte_string::size_type max_size;
//
//     byte_string_ref (byte_string * ptr)
//         : p(ptr)
//         , max_size(ptr->size())
//     {}
//
//     byte_string_ref (byte_string * ptr, byte_string::size_type sz)
//         : p(ptr)
//         , max_size(sz)
//     {}
// };

// template <typename T>
// struct buffer_wrapper
// {
//     T * p;
//     size_t max_size;
//
//     buffer_wrapper (T * ptr, byte_string::size_type n)
//         : p(ptr)
//         , max_size(n)
//     {}
// };

// inline byte_string operator + (byte_string const & lhs, byte_string const & rhs)
// {
//     byte_string r(lhs);
//     return r += rhs;
// }
//
// inline byte_string operator + (byte_string const & lhs, byte_string::value_type ch)
// {
//     byte_string r(lhs);
//     return r += ch;
// }
//
// inline byte_string operator + (byte_string::value_type ch, byte_string const & rhs)
// {
//     byte_string r(1, ch);
//     return r += rhs;
// }
//
// inline byte_string operator + (char ch, byte_string const & rhs)
// {
//     byte_string r(1, ch);
//     return r += rhs;
// }
//
// inline byte_string operator + (byte_string const & lhs, byte_string::const_pointer rhs)
// {
//     byte_string r(lhs);
//     return r += rhs;
// }
//
// inline byte_string operator + (byte_string::const_pointer lhs, byte_string const & rhs)
// {
//     byte_string r(lhs);
//     return r += rhs;
// }
//
// inline byte_string operator + (byte_string const & lhs, char const * rhs)
// {
//     byte_string r(lhs);
//     return r += rhs;
// }
//
// inline byte_string operator + (char const * lhs, byte_string const & rhs)
// {
//     byte_string r(lhs);
//     return r += rhs;
// }

byte_string & base64_encode (byte_string const & src, byte_string & result);
byte_string & base64_decode (byte_string const & src, byte_string & result);

// inline bool operator == (byte_string const & lhs, byte_string const & rhs)
// {
//     return lhs.compare(rhs) == 0;
// }
//
// inline bool operator != (byte_string const & lhs, byte_string const & rhs)
// {
//     return lhs.compare(rhs) != 0;
// }
//
// inline bool operator < (byte_string const & lhs, byte_string const & rhs)
// {
//     return lhs.compare(rhs) < 0;
// }
//
// inline bool operator <= (byte_string const & lhs, byte_string const & rhs)
// {
//     return lhs.compare(rhs) <= 0;
// }
//
// inline bool operator > (byte_string const & lhs, byte_string const & rhs)
// {
//     return lhs.compare(rhs) > 0;
// }
//
// inline bool operator >= (byte_string const & lhs, byte_string const & rhs)
// {
//     return lhs.compare(rhs) >= 0;
// }
//
// inline bool operator == (byte_string const & lhs, const char * rhs)
// {
//     return lhs.compare(rhs) == 0;
// }
//
// inline bool operator != (byte_string const & lhs, const char * rhs)
// {
//     return lhs.compare(rhs) != 0;
// }
//
// inline bool operator < (byte_string const & lhs, const char * rhs)
// {
//     return lhs.compare(rhs) < 0;
// }
//
// inline bool operator <= (byte_string const & lhs, const char * rhs)
// {
//     return lhs.compare(rhs) <= 0;
// }
//
// inline bool operator > (byte_string const & lhs, const char * rhs)
// {
//     return lhs.compare(rhs) > 0;
// }
//
// inline bool operator >= (byte_string const & lhs, const char * rhs)
// {
//     return lhs.compare(rhs) >= 0;
// }
//
// inline bool operator == (const char * lhs, byte_string const & rhs)
// {
//     return rhs.compare(lhs) == 0;
// }
//
// inline bool operator != (const char * lhs, byte_string const & rhs)
// {
//     return rhs.compare(lhs) != 0;
// }
//
// inline bool operator < (const char * lhs, byte_string const & rhs)
// {
//     return rhs.compare(lhs) > 0;
// }
//
// inline bool operator <= (const char * lhs, byte_string const & rhs)
// {
//     return rhs.compare(lhs) >= 0;
// }
//
// inline bool operator > (const char * lhs, byte_string const & rhs)
// {
//     return rhs.compare(lhs) < 0;
// }
//
// inline bool operator >= (const char * lhs, byte_string const & rhs)
// {
//     return rhs.compare(lhs) <= 0;
// }

// template <typename Device>
// inline ssize_t write_binary (Device & dev, endian order, byte_string const & v)
// {
//     return details::sequence::write_binary<Device>(dev, order, v.c_str(), v.size());
// }
//
// template <typename Device>
// ssize_t read_binary (Device & dev, endian order, byte_string & v)
// {
//     char * buffer = 0;
//     size_t size = 0;
//
//     ssize_t result = details::sequence::read_binary<Device>(dev, order, & buffer, & size);
//
//     if (buffer) {
//         v.append(buffer, size);
//         delete buffer;
//     }
//
//     return result;
// }

// TODO Must be deprecated

template <typename StringType>
inline StringType to_string (byte_string const & s)
{
    return StringType(s.c_str());
}

} // pfs
