#pragma once
#include <cstring>
#include <string>
#include <iterator>
#include <pfs/config.h>
#include <pfs/algorithm.hpp>
#include <pfs/types.hpp>
#include <pfs/stdcxx/basic_string.hpp>
#include <pfs/compare.hpp>

namespace pfs {

class byte_string : public stdcxx::basic_string<uint8_t, byte_string>
{
    typedef stdcxx::basic_string<uint8_t, byte_string> base_class;

public:
    using base_class::append;
    using base_class::assign;
    using base_class::compare;
    using base_class::copy;
    using base_class::erase;
    using base_class::insert;
    using base_class::push_back;
    using base_class::substr;
    using base_class::operator =;
    using base_class::operator +=;

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

    static const size_type npos = base_class::npos;

public:
    byte_string () : base_class () {}

    byte_string (byte_string const & other)
        : base_class(other)
    {}

    byte_string (byte_string const & other
            , size_type pos
            , size_type count = npos)
        : base_class(other, pos, count)
    {}

    byte_string (const_pointer s)
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
     * @fn byte_string & operator = (byte_string const & s)
     *
     * @brief Default copy assignment
     * @details Replaces the contents with the copy of @a s
     */

    /**
     * @fn byte_string & operator = (const_pointer s)
     */

    /**
     */
    byte_string & operator = (char const * s)
    {
        base_class::operator = (reinterpret_cast<const_pointer>(s));
        return *this;
    }

    /**
     * @fn byte_string & operator = (value_type c)
     */

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

    ///////////////////////////////////////////////////////////////////////////
    // Element access                                                        //
    ///////////////////////////////////////////////////////////////////////////

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

    /**
     * @fn reference front ()
     */

    /**
     * @fn const_reference front () const
     */

    /**
     * @fn reference back ()
     */

    /**
     * @fn const_reference back () const
     */

    /**
     */
//     inline const_pointer data () const
//     {
//         return base_class::data();
//     }

    // There is no `pointer data ()` GCC-implementation
//     inline pointer data ()
//     {
//         return & operator [] (0);
//     }

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

    /**
     * @fn const_iterator cbegin () const
     */

    /**
     * @fn iterator end ()
     */

    /**
     * @fn const_iterator end () const
     */

    /**
     * @fn const_iterator cend () const
     */

    /**
     * @fn reverse_iterator rbegin ()
     */

    /**
     * @fn const_reverse_iterator rbegin () const
     */

    /**
     * @fn const_reverse_iterator crbegin () const
     */

    /**
     * @fn reverse_iterator rend ()
     */

    /**
     * @fn const_reverse_iterator rend () const
     */

    /**
     * @fn const_reverse_iterator crend () const
     */

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
     * @fn void reserve (size_type new_cap)
     */

    /**
     * @fn size_type capacity () const
     */

    /**
     * @fn void shrink_to_fit ()
     */

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
    inline byte_string & insert (size_type index, char const * s)
    {
        base_class::insert(index, reinterpret_cast<const_pointer>(s));
        return *this;
    }

    /**
     * @fn byte_string & insert (size_type index, const_pointer s, size_type count)
     *
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, char const * s, size_type count)
    {
        base_class::insert(index, reinterpret_cast<const_pointer>(s), count);
        return *this;
    }

    /**
     * @fn byte_string & insert (size_type index, byte_string const & s)
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, std::string const & s)
    {
        base_class::insert(index, reinterpret_cast<const_pointer>(s.data()), s.size());
        return *this;
    }

    /**
     * @fn byte_string & insert (size_type index, basic_string const & str
     *              , size_type index_str, size_type count = npos)
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, std::string const & str
                   , size_type index_str, size_type count = npos)
    {
        base_class::insert(index
                , reinterpret_cast<const_pointer>(str.data() + index_str)
                , count);
        return *this;
    }

    /**
     * @fn iterator insert (const_iterator pos, value_type ch)
     */

    /**
     */
    inline iterator insert (const_iterator pos, char ch)
    {
        // Do not use base_class::insert here.
        // Will be used above method for C++ prior to C++11
        return byte_string::insert(pos, value_type(ch));
    }

    /**
     * @fn iterator insert (const_iterator pos, size_type count, value_type ch)
     */

    inline iterator insert (const_iterator pos, size_type count, char ch)
    {
        return byte_string::insert(pos, count, value_type(ch));
    }

    /**
     * @fn template <typename InputIt>
     *     iterator insert (const_iterator pos, InputIt first, InputIt last)
     */

#if PFS_COMPILER_CXX_GENERALIZED_INITIALIZERS
    /**
    * @fn iterator insert (const_iterator pos, std::initializer_list<value_type> ilist)
    */

    /**
     */
    inline iterator insert (const_iterator pos, std::initializer_list<char> ilist)
    {
        return base_class::insert<std::initializer_list<char>::const_iterator>(pos
                , ilist.begin(), ilist.end());
    }
#endif


    /**
     * @fn byte_string & erase (size_type index = 0, size_type count = npos)
     *
     * @return @code *this @endcode
     */

    /**
     * @fn iterator erase (const_iterator pos)
     */

    /**
     * @fn iterator erase (const_iterator first, const_iterator last)
     */

    /**
     * @fn void push_back (value_type ch)
     */

    /**
     * @fn void push_back (char ch)
     */

    /**
     * @fn void pop_back ()
     */

    /**
     * @fn byte_string & append (size_type count, value_type ch)
     * @return @code *this @endcode
     */

    /**
     * @fn byte_string & append (size_type count, char ch)
     * @return @code *this @endcode
     */
    byte_string & append (size_type count, char ch)
    {
        base_class::append(count, value_type(ch));
        return *this;
    }

    /**
     * @fn byte_string & append (byte_string const & s)
     *
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (std::string const & s)
     {
         base_class::append(reinterpret_cast<const_pointer>(s.data()), s.size());
         return *this;
     }

    /**
     * @fn byte_string & append (byte_string const & s
//              , size_type pos
//              , size_type count = npos)
     *
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (std::string const & s
             , size_type pos
             , size_type count = npos)
     {
         base_class::append(reinterpret_cast<const_pointer>(s.data()) + pos, count);
         return *this;
     }

    /**
     * @fn byte_string & append (const_pointer s, size_type count)
     *
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (const char * s, size_type count)
     {
         base_class::append(reinterpret_cast<const_pointer>(s), count);
         return *this;
     }

    /**
     * @fn byte_string & append (const_pointer s)
     *
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (const char * s)
     {
         base_class::append(reinterpret_cast<const_pointer>(s));
         return *this;
     }

    /**
     * @fn template <typename InputIterator>
     *     byte_string & append (InputIterator first, InputIterator last)
     *
     * @return @code *this @endcode
     */

#if PFS_COMPILER_CXX_GENERALIZED_INITIALIZERS
    /**
     * @fn byte_string & append (std::initializer_list<value_type> ilist)
     *
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
    inline byte_string & append (std::initializer_list<char> ilist)
    {
        base_class::append(ilist.begin(), ilist.end());
        return *this;
    }
#endif

    /**
     * @fn
     * @return @code *this @endcode
     */
//     inline byte_string & operator += (byte_string const & s)
//     {
//         return append(s);
//     }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & operator += (std::string const & s)
    {
        return append(s);
    }

    /**
     * @return @code *this @endcode
     */
//     inline byte_string & operator += (value_type ch)
//     {
//         return append(1, ch);
//     }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & operator += (char ch)
    {
        return append(size_type(1), value_type(ch));
    }

    /**
     * @return @code *this @endcode
     */
//     inline byte_string & operator += (const_pointer s)
//     {
//         return append(s);
//     }

    /**
     * @return @code *this @endcode
     */
    byte_string & operator += (const char * s)
    {
        return append(s);
    }

#if PFS_COMPILER_CXX_GENERALIZED_INITIALIZERS
    /**
     * @return @code *this @endcode
     */
//     inline byte_string & operator += (std::initializer_list<value_type> ilist)
//     {
//         return append(ilist);
//     }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & operator += (std::initializer_list<char> ilist)
    {
        return append(ilist);
    }
#endif

    /**
     * @fn int compare (byte_string const & s) const
     */

    /**
     * @fn int compare (size_type pos1, size_type count1, byte_string const & s) const
     */

    /**
     * @fn int compare (size_type pos1, size_type count1
     *          , byte_string const & s
     *          , size_type pos2, size_type count2 = npos) const
     */

    /**
     * @fn int compare (const_pointer s) const
     */

    /**
     */
    inline int compare (char const * s) const
    {
        return this->compare(reinterpret_cast<const_pointer>(s));
    }

    /**
     */
    inline int compare (std::string const & s) const
    {
        return this->compare(0, this->size(), reinterpret_cast<const_pointer>(s.data()), s.size());
    }

    /**
     * @fn int compare (size_type pos1, size_type count1, const_pointer s) const
     */

    /**
     * @fn int compare (size_type pos1, size_type count1, const_pointer s, size_type count2) const
     */

    /**
     * @fn byte_string substr (size_type pos = 0, size_type count = npos) const
     */

    /**
     * @fn size_type copy (pointer dest, size_type count, size_type pos = 0) const
     */

    /**
     * @fn void resize (size_type count)
     */

    /**
     * @fn void resize (size_type count, value_type ch)
     */

    /**
     * @fn void swap (byte_string & other)
     */

    ///////////////////////////////////////////////////////////////////////////
    // Search                                                                //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn size_type find (byte_string const & bs, size_type pos = 0) const;
     */

    /**
     * @fn size_type find (const_pointer p, size_type pos, size_type count) const;
     */

    /**
     * @fn size_type find (const_pointer p, size_type pos = 0) const;
     */

    /**
     * @fn size_type find (value_type ch, size_type pos = 0) const;
     */

    /**
     * @fn size_type rfind (byte_string const & bs, size_type pos = npos) const;
     */

    /**
     * @fn size_type rfind (const_pointer p, size_type pos, size_type count) const;
     */

    /**
     * @fn size_type rfind (const_pointer p, size_type pos = npos) const;
     */

    /**
     * @fn size_type rfind (value_type ch, size_type pos = npos) const;
     */

    /**
     * @fn size_type find_first_of (byte_string const & bs, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_of (const_pointer p, size_type pos, size_type count) const
     */

    /**
     * @fn size_type find_first_of (const_pointer p, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_of (value_type ch, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_not_of (byte_string const & bs, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_not_of (const_pointer p, size_type pos, size_type count) const
     */

    /**
     * @fn size_type find_first_not_of (const_pointer p, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_not_of (value_type ch, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_of (byte_string const & bs, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_of (const_pointer p, size_type pos, size_type count) const
     */

    /**
     * @fn size_type find_last_of (const_pointer p, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_of (value_type ch, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_not_of (byte_string const & bs, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_not_of (const_pointer p, size_type pos, size_type count) const
     */

    /**
     * @fn size_type find_last_not_of (const_pointer p, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_not_of (value_type ch, size_type pos = 0) const
     */

    //
    // binary_ostream compliant method
    //
    inline ssize_t write (const_pointer s, size_type n)
    {
        append(s, n);
        return static_cast<ssize_t>(n);
    }

    inline ssize_t write (char const * s, size_type n)
    {
        return write(reinterpret_cast<const_pointer>(s), n);
    }

    //
    // binary_istream compliant method
    //
    inline ssize_t read (pointer s, size_type n)
    {
        size_type n1 = pfs::min(n, size());
        copy(s, n1);
        return static_cast<ssize_t>(n1);
    }

    inline ssize_t read (char * s, size_type n)
    {
        return read(reinterpret_cast<pointer>(s), n);
    }
};

template <typename T>
struct buffer_wrapper
{
    T * p;
    size_t max_size;

    buffer_wrapper (T * ptr, byte_string::size_type n)
        : p(ptr)
        , max_size(n)
    {}
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

inline bool operator == (byte_string const & lhs, byte_string const & rhs)
{
    return lhs.compare(rhs) == 0;
}

inline bool operator < (byte_string const & lhs, byte_string const & rhs)
{
    return lhs.compare(rhs) < 0;
}

inline bool operator == (byte_string const & lhs, const char * rhs)
{
    return lhs.compare(rhs) == 0;
}

inline bool operator == (char const * lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) == 0;
}

inline bool operator == (byte_string const & lhs, byte_string::const_pointer rhs)
{
    return lhs.compare(rhs) == 0;
}

inline bool operator == (byte_string::const_pointer lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) == 0;
}

inline bool operator == (byte_string const & lhs, std::string const & rhs)
{
    return lhs.compare(rhs) == 0;
}

inline bool operator == (std::string const & lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) == 0;
}

inline bool operator < (byte_string const & lhs, const char * rhs)
{
    return lhs.compare(rhs) < 0;
}

inline bool operator < (char const * lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) > 0;
}

inline bool operator < (byte_string const & lhs, byte_string::const_pointer rhs)
{
    return lhs.compare(rhs) < 0;
}

inline bool operator < (byte_string::const_pointer lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) > 0;
}

inline bool operator < (byte_string const & lhs, std::string const & rhs)
{
    return lhs.compare(rhs) < 0;
}

inline bool operator < (std::string const & lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) > 0;
}

} // pfs
