#pragma once
#include <cstring>
#include <string>
#include <iterator>
#include <pfs/types.hpp>
#include <pfs/memory.hpp>

namespace pfs {

class byte_string : public std::basic_string<uint8_t>
{
    typedef std::basic_string<uint8_t> base_class;

public:
    using base_class::assign;
    using base_class::compare;
    using base_class::copy;
    using base_class::erase;
    using base_class::push_back;
    using base_class::substr;

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
     */
    inline const_pointer data () const
    {
        return base_class::data();
    }

    // There is no `pointer data ()` GCC-implementation
    inline pointer data ()
    {
        return & operator [] (0);
    }

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
     * @brief Inserts @c count copies of character @c ch at the position @c index.
     * @param index Position at which the content will be inserted.
     * @param count Number of characters to insert.
     * @param ch Character to insert.
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, size_type count, value_type ch)
    {
        base_class::insert(index, count, ch);
        return *this;
    }

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
     * @brief Inserts null-terminated character string pointed to by @c s at the position @c index.
     *        The length of the string is determined by the first null character.
     * @param index Position at which the content will be inserted.
     * @param s Pointer to the character string to insert.
     * @return @code *this @endcode
     */
    byte_string & insert (size_type index, const_pointer s)
    {
        base_class::insert(index, s);
        return *this;
    }

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
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, const_pointer s, size_type count)
    {
        base_class::insert(index, s, count);
        return *this;
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, char const * s, size_type count)
    {
        base_class::insert(index, reinterpret_cast<const_pointer>(s), count);
        return *this;
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, byte_string const & s)
    {
        base_class::insert(index, s);
        return *this;
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, std::string const & s)
    {
        base_class::insert(index, reinterpret_cast<const_pointer>(s.data()), s.size());
        return *this;
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, basic_string const & str
                   , size_type index_str, size_type count = npos)
    {
        base_class::insert(index, str, index_str, count);
        return *this;
    }

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
    //
    // http://en.cppreference.com/w/cpp/string/basic_string/insert
    // says
    //      iterator insert (iterator pos, CharT ch ); (until C++11)
    // but gcc implements it as meet C++11 standard
    //      iterator insert (iterator pos, CharT ch ); (since C++11)
    // so need checks with various versions of standard C++ library
    //
    inline iterator insert (const_iterator pos, value_type ch)
    {
#if __cplusplus < 201103L
        size_type index = std::distance(cbegin(), pos);
        byte_string::insert(index, 1, ch);
        return iterator(begin() + index);
#else
        return base_class::insert(pos, ch);
#endif
    }

    /**
     */
    inline iterator insert (const_iterator pos, char ch)
    {
        // Do not use base_class::insert here.
        // Will be used above method for C++ prior to C++11
        return byte_string::insert(pos, value_type(ch));
    }

#if __cplusplus < 201103L
    inline iterator insert (const_iterator pos, size_type count, value_type ch)
    {
        size_type index = std::distance(cbegin(), pos);
        base_class::insert(begin() + index, count, ch);
        return begin() + index;
    }
#else
    inline iterator insert (const_iterator pos, size_type count, value_type ch)
    {
        return base_class::insert(pos, count, ch);
    }
#endif

    inline iterator insert (const_iterator pos, size_type count, char ch)
    {
        return byte_string::insert(pos, count, value_type(ch));
    }

#if __cplusplus < 201103L
    template <typename InputIt>
    inline iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        size_type index = std::distance(cbegin(), pos);
        base_class::insert(begin() + index, first, last);
        return begin() + index;
    }
#else
    template <typename InputIt>
    inline iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        return base_class::insert<InputIt>(pos, first, last);
    }
#endif

#if __cplusplus >= 201103L
    /**
    * @fn iterator insert (const_iterator pos, std::initializer_list<value_type> ilist)
    */

    /**
     */
    inline iterator insert (const_iterator pos, std::initializer_list<char> ilist)
    {
        return base_class::insert(pos, ilist.begin(), ilist.end());
    }
#endif

#if __cplusplus > 201402L
    /**
     * @return @code *this @endcode
     */
    template <typename T>
    inline byte_string & insert (size_type pos, T const & t)
    {
        base_class::insert<T>(pos, t);
        return *this;
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & insert (size_type index, T const & t
                , size_type index_str, size_type count = npos)
    {
        base_class::insert<T>(index, t, index_str, count);
        return *this;
    }
#endif

    /**
     * @return @code *this @endcode
     */
    inline byte_string & erase (size_type index = 0, size_type count = npos)
    {
        base_class::erase(index, count);
        return *this;
    }

#if __cplusplus < 201103L
    iterator erase (const_iterator pos)
    {
// || PFS_CC_GCC_VERSION <= 40800 // TODO Check for valid version
        size_type index = std::distance(cbegin(), pos);
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
        size_type pos1 = std::distance(cbegin(), first);
        size_type pos2 = std::distance(cbegin(), last);
        if (pos1 < pos2) {
            erase(pos1, pos2 - pos1);
            return begin() + pos1;
        }
        return end();
    }
#endif

    /**
     * @fn void push_back (value_type ch)
     */

    /**
     * @fn void push_back (char ch)
     */

#if __cplusplus < 201103L
    inline void pop_back ()
    {
        if (!empty())
            erase(size() - 1, 1);
    }
#else
    // C++11 version has no check if string is empty
    inline void pop_back ()
    {
        if (!empty())
            base_class::pop_back();
    }
#endif

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (size_type count, value_type ch)
     {
         base_class::append(count, ch);
         return *this;
     }

    /**
     * @fn byte_string & append (size_type count, char ch)
     * @return @code *this @endcode
     */

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (byte_string const & s)
     {
         base_class::append(s);
         return *this;
     }

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (std::string const & s)
     {
         base_class::append(reinterpret_cast<const_pointer>(s.data()), s.size());
         return *this;
     }

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (byte_string const & s
             , size_type pos
             , size_type count = npos)
     {
         base_class::append(s, pos, count);
         return *this;
     }

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
     * @return @code *this @endcode
     */
     inline byte_string & append (const_pointer s, size_type count)
     {
         base_class::append(s, count);
         return *this;
     }

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (const char * s, size_type count)
     {
         base_class::append(reinterpret_cast<const_pointer>(s), count);
         return *this;
     }

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (const_pointer s)
     {
         base_class::append(s);
         return *this;
     }

    /**
     * @return @code *this @endcode
     */
     inline byte_string & append (const char * s)
     {
         base_class::append(reinterpret_cast<const_pointer>(s));
         return *this;
     }

    /**
     * @return @code *this @endcode
     */
    template <typename InputIterator>
    byte_string & append (InputIterator first, InputIterator last)
    {
        base_class::append<InputIterator>(first, last);
        return *this;
    }

#if __cplusplus >= 201103L
    /**
     * @return @code *this @endcode
    */
    inline byte_string & append (std::initializer_list<value_type> ilist)
    {
        base_class::append(ilist);
        return *this;
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & append (std::initializer_list<char> ilist)
    {
        base_class::append(ilist.begin(), ilist.end());
        return *this;
    }
#endif

#if __cplusplus > 201402L
    /**
     * @return @code *this @endcode
     */
    template <typename T>
    inline byte_string & append (T const & t)
    {
        base_class::append<T>(pos, t);
        return *this;
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & append (T const & t, size_type index_str, size_type count = npos)
    {
        base_class::append<T>(t, index_str, count);
        return *this;
    }
#endif

    /**
     * @return @code *this @endcode
     */
    inline byte_string & operator += (byte_string const & s)
    {
        return append(s);
    }

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
    inline byte_string & operator += (value_type ch)
    {
        return append(1, ch);
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & operator += (char ch)
    {
        return append(1, ch);
    }

    /**
     * @return @code *this @endcode
     */
    inline byte_string & operator += (const_pointer s)
    {
        return append(s);
    }

    /**
     * @return @code *this @endcode
     */
    byte_string & operator += (const char * s)
    {
        return append(s);
    }

#if __cplusplus >= 201103L
    /**
     * @return @code *this @endcode
     */
    inline byte_string & operator += (std::initializer_list<value_type> ilist)
    {
        return append(ilist);
    }

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
     */
    int compare (size_type pos1, size_type count1
            , byte_string const & s
            , size_type pos2, size_type count2 = npos) const
    {
        return base_class::compare(pos1, count1, s, pos2, count2);
    }

    /**
     * @fn int compare (const_pointer s) const
     */

    /**
     * @fn int compare (size_type pos1, size_type count1, const_pointer s) const
     */

    /**
     * @fn int compare (size_type pos1, size_type count1, const_pointer s, size_type count2) const
     */

    /**
     */
    inline bool starts_with (byte_string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;

        return compare(0, rhs.size(), rhs) == 0;
    }

    inline bool starts_with (value_type ch) const
    {
        return empty() ? false : (front() == ch);
    }

    inline bool starts_with (const_pointer s) const
    {
        return compare(0, std::strlen(reinterpret_cast<char const *>(s)), s) == 0;
    }

    inline bool ends_with (byte_string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;

        return compare(size() - rhs.size()
                , rhs.size(), rhs, 0, rhs.size()) == 0;
    }

    inline bool ends_with (value_type ch) const
    {
        return empty() ? false : (back() == ch);
    }

    inline bool ends_with (const_pointer s) const
    {
        size_type n = std::strlen(reinterpret_cast<char const *>(s));

        if (n > this->size())
            return false;

        return compare(size() - n, n, s, 0, n) == 0;
    }

    /**
     */
    inline byte_string & replace (size_type pos, size_type count, byte_string const & s)
    {
        base_class::replace(pos, count, s);
        return *this;
    }

    /**
     */
    inline byte_string & replace (const_iterator first, const_iterator last, byte_string const & s)
    {
#if __cplusplus < 201103L
        // basic_string implementation may use COW,
        // so
        // base_class::replace(begin() + std::distance(cbegin(), first)
        //      , begin() + std::distance(cbegin(), last), s);
        // may cause UB (in  worst case Seg Fault)
        //
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        iterator f = begin() + i1; // begin() unshares data
        iterator e = begin() + i2;
        base_class::replace(f, e, s);
#else
        base_class::replace(first, last, s);
#endif
        return *this;
    }

    /**
     */
    inline byte_string & replace (size_type pos, size_type count
            , byte_string const & s
            , size_type pos2, size_type count2 = npos)
    {
        base_class::replace(pos, count, s, pos2, count2);
        return *this;
    }

    template <typename InputIterator>
    inline byte_string & replace (const_iterator first, const_iterator last
            , InputIterator first2, InputIterator last2)
    {

#if __cplusplus < 201103L
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        iterator f = begin() + i1;
        iterator e = begin() + i2;
        base_class::replace<InputIterator>(f, e, first2, last2);
#else
        base_class::replace<InputIterator>(first, last, first2, last2);
#endif
        return *this;
    }

    /**
     */
    inline byte_string & replace (size_type pos, size_type count, const_pointer s, size_type count2)
    {
        base_class::replace(pos, count, s, count2);
        return *this;
    }

    /**
     */
    inline byte_string & replace (const_iterator first, const_iterator last
            , const_pointer s, size_type count2)
    {
#if __cplusplus < 201103L
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        iterator f = begin() + i1;
        iterator e = begin() + i2;
        base_class::replace(f, e, s, count2);
#else
        base_class::replace(first, last, s, count2);
#endif
        return *this;
    }

    /**
     */
    inline byte_string & replace (size_type pos, size_type count, const_pointer s)
    {
        base_class::replace(pos, count, s);
        return *this;
    }

    /**
     */
    inline byte_string & replace (const_iterator first, const_iterator last, const_pointer s)
    {
#if __cplusplus < 201103L
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        iterator f = begin() + i1;
        iterator e = begin() + i2;
        base_class::replace(f, e, s);
#else
        base_class::replace(first, last, s);
#endif
        return *this;
    }

    /**
     */
    inline byte_string & replace (size_type pos, size_type count
            , size_type count2, value_type ch)
    {
        base_class::replace(pos, count, count2, ch);
        return *this;
    }

    /**
     */
    inline byte_string & replace (const_iterator first, const_iterator last
            , size_type count2, value_type ch)
    {
#if __cplusplus < 201103L
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        base_class::iterator f = begin() + i1;
        base_class::iterator e = begin() + i2;
        base_class::replace(f, e, count2, ch);
#else
        base_class::replace(first, last, count2, ch);
#endif
        return *this;
    }

#if __cplusplus >= 201103L
      /**
      */
      inline byte_string & replace (const_iterator first, const_iterator last
                , std::initializer_list<value_type> ilist)
      {
          base_class::replace(first, last, ilist.begin(), ilist.end());
          return *this;
      }
#endif

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

} // pfs
