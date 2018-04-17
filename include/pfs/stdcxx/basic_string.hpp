#pragma once
#include <cstring>
#include <string>
#include <pfs/type_traits.hpp>

namespace pfs {
namespace stdcxx {

template <typename CharT>
size_t strlen (CharT const *);

template <typename CharT, typename DerivedT>
class basic_string : public std::basic_string<CharT>
{
    typedef std::basic_string<CharT> base_class;

public:
    using base_class::assign;
    using base_class::compare;
    using base_class::copy;
    using base_class::erase;
    using base_class::insert;
    using base_class::push_back;
    using base_class::substr;

    typedef typename base_class::value_type       value_type;
    typedef typename base_class::traits_type      traits_type;
    typedef typename base_class::size_type        size_type;
    typedef typename base_class::difference_type  difference_type;
    typedef typename base_class::reference        reference;
    typedef typename base_class::const_reference  const_reference;
    typedef typename base_class::pointer          pointer;
    typedef typename base_class::const_pointer    const_pointer;
    typedef typename base_class::iterator         iterator;
    typedef typename base_class::const_iterator   const_iterator;
    typedef typename base_class::reverse_iterator reverse_iterator;
    typedef typename base_class::const_reverse_iterator const_reverse_iterator;

    static const size_type npos = base_class::npos;

public:
    basic_string () : base_class () {}

    basic_string (basic_string const & other)
        : base_class(other)
    {}

    basic_string (basic_string const & other
            , size_type pos
            , size_type count = npos)
        : base_class(other, pos, count)
    {}

    basic_string (const_pointer s)
        : base_class(s)
    {}

    basic_string (const_pointer s, size_type n)
        : base_class(s, n)
    {}

    basic_string (size_type n, CharT c)
        : base_class(n, c)
    {}

    template <typename InputIterator>
    basic_string (InputIterator first, InputIterator last)
        : base_class(first, last)
    {}

    /**
     * @fn string::string (std::string const & s).
     *
     * @brief Constructs string from std::string.
     * @param s value.
     */
    template <typename T>
    explicit basic_string (T const & s, typename enable_if<is_same<T, std::string>::value
            && is_same<base_class, std::string>::value, T>::type * = 0)
        : base_class(s)
    {}

    virtual ~basic_string ()
    {}

    /**
     * @brief Default copy assignment
     * @details Replaces the contents with the copy of @a s
     */
    DerivedT & operator = (basic_string const & s)
    {
        base_class::operator = (s);
        return *static_cast<DerivedT *>(this);
    }

    DerivedT & operator = (const_pointer s)
    {
        base_class::operator = (s);
        return *static_cast<DerivedT *>(this);
    }

    DerivedT & operator = (CharT c)
    {
        base_class::operator = (c);
        return *static_cast<DerivedT *>(this);
    }

    /**
     * @fn basic_string & assign (size_type count, CharT c)
     * @brief Replaces the contents with count copies of value @a c.
     */

    /**
     * @fn basic_string & assign (basic_string const & bs)
     * @brief Replaces the contents with a copy of @a bs.
     */

    /**
     * @fn basic_string & assign (basic_string const & bs, size_type pos, size_type count)
     * @details Replaces the contents with a substring [pos, pos + count) of @a bs.
     */

    /**
     * @fn basic_string & assign (const_pointer s, size_type count)
     * @details Replaces the contents with the first count of values from @a s.
     */

    /**
     * @fn basic_string & assign (const_pointer s)
     */

    /**
     * @fn template <typename InputIterator>
     *     basic_string & assign (InputIterator first, InputIterator last)
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
        PFS_ASSERT(!this->empty());
        return this->at(0);
    }
#endif

#if __cplusplus < 201103L
    const_reference front () const
    {
        PFS_ASSERT(!this->empty());
        return this->at(0);
    }
#endif

#if __cplusplus < 201103L
    reference back ()
    {
        PFS_ASSERT(!this->empty());
        return this->at(this->size() - 1);
    }
#endif

#if __cplusplus < 201103L
    const_reference back () const
    {
        PFS_ASSERT(!this->empty());
        return this->at(this->size() - 1);
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
        return & this->operator [] (0);
    }

    //
    // @note `const_pointer c_str () const` is inaccessable
    //
    const_pointer c_str () const
    {
        return base_class::c_str();
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
        return this->begin();
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
        return this->end();
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
     * @brief Checks if string is empty.
     * @return @c true if string is empty (size() == 0) or @c false otherwise.
     */

    /**
     * @fn size_type size () const
     * @brief Return size of string in code units.
     * @return String in code units.
     */

    /**
     * @fn size_type length () const
     * @brief Return size of string.
     * @return Size of string.
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
        if (this->capacity() > this->size()) {
            this->reserve(0);
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
    inline DerivedT & insert (size_type index, size_type count, CharT ch)
    {
        base_class::insert(index, count, ch);
        return *static_cast<DerivedT *>(this);
    }

    /**
     * @fn basic_string & insert (size_type index, size_type count, char ch)
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
    DerivedT & insert (size_type index, const_pointer s)
    {
        base_class::insert(index, s);
        return *static_cast<DerivedT *>(this);
    }

    /**
     * @return @code *this @endcode
     */
    inline DerivedT & insert (size_type index, const_pointer s, size_type count)
    {
        base_class::insert(index, s, count);
        return *static_cast<DerivedT *>(this);
    }

    /**
     * @return @code *this @endcode
     */
    inline DerivedT & insert (size_type index, basic_string const & s)
    {
        base_class::insert(index, s);
        return *static_cast<DerivedT *>(this);
    }

    /**
     * @return @code *this @endcode
     */
    inline DerivedT & insert (size_type index, basic_string const & str
                   , size_type index_str, size_type count = npos)
    {
        base_class::insert(index, str, index_str, count);
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    //
    // http://en.cppreference.com/w/cpp/string/basic_string/insert
    // says
    //      iterator insert (iterator pos, CharT ch ); (until C++11)
    // but gcc implements it as meet C++11 standard
    //      iterator insert (iterator pos, CharT ch ); (since C++11)
    // so need checks with various versions of standard C++ library
    //
    inline iterator insert (const_iterator pos, CharT ch)
    {
#if __cplusplus < 201103L
        size_type index = std::distance(cbegin(), pos);
        basic_string::insert(index, 1, ch);
        return iterator(this->begin() + index);
#else
        // While build on Travis CI under Ubuntu 14.04.05 (gcc 4.8) error occured:
        // --------------------------------------------------------------------------
        // ...
        // no matching function for call to ... insert (const_iterator pos, CharT ch)
        // ...
        // --------------------------------------------------------------------------
        // But under Ubuntu 16.04 (gcc ?) and 17.10 (gcc 7.2.0) build is successfull.
        // So call another method:
        return base_class::insert(pos, 1, ch);
#endif
    }

#if __cplusplus < 201103L
    inline iterator insert (const_iterator pos, size_type count, CharT ch)
    {
        size_type index = std::distance(cbegin(), pos);
        base_class::insert(this->begin() + index, count, ch);
        return this->begin() + index;
    }
#else
    inline iterator insert (const_iterator pos, size_type count, CharT ch)
    {
        return base_class::insert(pos, count, ch);
    }
#endif

#if __cplusplus < 201103L
    template <typename InputIt>
    inline iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        size_type index = std::distance(cbegin(), pos);
        base_class::insert(this->begin() + index, first, last);
        return this->begin() + index;
    }
#else
    template <typename InputIt>
    inline iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        return base_class::template insert<InputIt>(pos, first, last);
    }
#endif

#if __cplusplus >= 201103L
    /**
    * @fn iterator insert (const_iterator pos, std::initializer_list<CharT> ilist)
    */
#endif

    /**
     * @return @code *this @endcode
     */
    inline DerivedT & erase (size_type index = 0, size_type count = npos)
    {
        base_class::erase(index, count);
        return *static_cast<DerivedT *>(this);
    }

#if __cplusplus < 201103L
    iterator erase (const_iterator pos)
    {
        size_type index = std::distance(cbegin(), pos);
        if (index < this->size()) {
            erase(index, 1);
            return this->begin() + index;
        }
        return this->end();
    }
#endif

#if __cplusplus < 201103L
    iterator erase (const_iterator first, const_iterator last)
    {
        size_type pos1 = std::distance(cbegin(), first);
        size_type pos2 = std::distance(cbegin(), last);
        if (pos1 < pos2) {
            erase(pos1, pos2 - pos1);
            return this->begin() + pos1;
        }
        return this->end();
    }
#endif

    /**
     * @fn void push_back (CharT ch)
     */

#if __cplusplus < 201103L
    inline void pop_back ()
    {
        if (!this->empty())
            erase(this->size() - 1, 1);
    }
#else
    // C++11 version has no check if string is empty
    inline void pop_back ()
    {
        if (!this->empty())
            base_class::pop_back();
    }
#endif

    /**
     * @return @code *this @endcode
     */
     inline DerivedT & append (size_type count, CharT ch)
     {
         base_class::append(count, ch);
         return *static_cast<DerivedT *>(this);
     }

    /**
     * @return @code *this @endcode
     */
     inline DerivedT & append (DerivedT const & s)
     {
         base_class::append(s);
         return *static_cast<DerivedT *>(this);
     }

    /**
     * @return @code *this @endcode
     */
     inline DerivedT & append (DerivedT const & s
             , size_type pos
             , size_type count = npos)
     {
         base_class::append(s, pos, count);
         return *static_cast<DerivedT *>(this);
     }

    /**
     * @return @code *this @endcode
     */
     inline DerivedT & append (const_pointer s, size_type count)
     {
         base_class::append(s, count);
         return *static_cast<DerivedT *>(this);
     }

    /**
     * @return @code *this @endcode
     */
     inline DerivedT & append (const_pointer s)
     {
         base_class::append(s);
         return *static_cast<DerivedT *>(this);
     }

    /**
     * @return @code *this @endcode
     */
    template <typename InputIterator>
    DerivedT & append (InputIterator first, InputIterator last)
    {
        base_class::template append<InputIterator>(first, last);
        return *static_cast<DerivedT *>(this);
    }

#if __cplusplus >= 201103L
    /**
     * @return @code *this @endcode
    */
    inline DerivedT & append (std::initializer_list<CharT> ilist)
    {
        base_class::append(ilist);
        return *static_cast<DerivedT *>(this);
    }
#endif

    DerivedT & append (std::string const & s)
    {
        this->append(basic_string(s));
    }

    /**
     * @return @code *this @endcode
     */
    inline DerivedT & operator += (DerivedT const & s)
    {
        return append(s);
    }

    /**
     * @return @code *this @endcode
     */
    inline DerivedT & operator += (CharT ch)
    {
        return append(1, ch);
    }

    /**
     * @return @code *this @endcode
     */
    inline DerivedT & operator += (const_pointer s)
    {
        return append(s);
    }

#if __cplusplus >= 201103L
    /**
     * @return @code *this @endcode
     */
    inline DerivedT & operator += (std::initializer_list<CharT> ilist)
    {
        return append(ilist);
    }
#endif

    /**
     * @fn int compare (basic_string const & s) const
     */

    /**
     * @fn int compare (size_type pos1, size_type count1, basic_string const & s) const
     */

    /**
     * @fn int compare (size_type pos1, size_type count1
     *             , basic_string const & s
     *             , size_type pos2, size_type count2 = npos) const
     */
    int compare (size_type pos1, size_type count1
            , DerivedT const & s
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
    inline bool starts_with (basic_string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;

        return compare(0, rhs.size(), rhs) == 0;
    }

    inline bool starts_with (CharT ch) const
    {
        return this->empty() ? false : (this->front() == ch);
    }

    inline bool starts_with (const_pointer s) const
    {
        return compare(0, strlen(reinterpret_cast<char const *>(s)), s) == 0;
    }

    inline bool ends_with (basic_string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;

        return compare(this->size() - rhs.size()
                , rhs.size(), rhs, 0, rhs.size()) == 0;
    }

    inline bool ends_with (CharT ch) const
    {
        return this->empty() ? false : (this->back() == ch);
    }

    inline bool ends_with (const_pointer s) const
    {
        size_type n = std::strlen(reinterpret_cast<char const *>(s));

        if (n > this->size())
            return false;

        return this->compare(this->size() - n, n, s, n) == 0;
    }

    /**
     */
    inline DerivedT & replace (size_type pos, size_type count, basic_string const & s)
    {
        base_class::replace(pos, count, s);
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    inline DerivedT & replace (const_iterator first, const_iterator last, basic_string const & s)
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
        iterator f = this->begin() + i1; // begin() unshares data
        iterator e = this->begin() + i2;
        base_class::replace(f, e, s);
#else
        base_class::replace(first, last, s);
#endif
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    inline DerivedT & replace (size_type pos, size_type count
            , basic_string const & s
            , size_type pos2, size_type count2 = npos)
    {
        base_class::replace(pos, count, s, pos2, count2);
        return *static_cast<DerivedT *>(this);
    }

    template <typename InputIterator>
    inline DerivedT & replace (const_iterator first, const_iterator last
            , InputIterator first2, InputIterator last2)
    {

#if __cplusplus < 201103L
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        iterator f = this->begin() + i1;
        iterator e = this->begin() + i2;
        base_class::template replace<InputIterator>(f, e, first2, last2);
#else
        base_class::template replace<InputIterator>(first, last, first2, last2);
#endif
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    inline DerivedT & replace (size_type pos, size_type count, const_pointer s, size_type count2)
    {
        base_class::replace(pos, count, s, count2);
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    inline DerivedT & replace (const_iterator first, const_iterator last
            , const_pointer s, size_type count2)
    {
#if __cplusplus < 201103L
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        iterator f = this->begin() + i1;
        iterator e = this->begin() + i2;
        base_class::replace(f, e, s, count2);
#else
        base_class::replace(first, last, s, count2);
#endif
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    inline DerivedT & replace (size_type pos, size_type count, const_pointer s)
    {
        base_class::replace(pos, count, s);
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    inline DerivedT & replace (const_iterator first, const_iterator last, const_pointer s)
    {
#if __cplusplus < 201103L
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        iterator f = this->begin() + i1;
        iterator e = this->begin() + i2;
        base_class::replace(f, e, s);
#else
        base_class::replace(first, last, s);
#endif
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    inline DerivedT & replace (size_type pos, size_type count
            , size_type count2, CharT ch)
    {
        base_class::replace(pos, count, count2, ch);
        return *static_cast<DerivedT *>(this);
    }

    /**
     */
    inline DerivedT & replace (const_iterator first, const_iterator last
            , size_type count2, CharT ch)
    {
#if __cplusplus < 201103L
        difference_type i1 = std::distance(cbegin(), first);
        difference_type i2 = std::distance(cbegin(), last);
        iterator f = this->begin() + i1;
        iterator e = this->begin() + i2;
        base_class::replace(f, e, count2, ch);
#else
        base_class::replace(first, last, count2, ch);
#endif
        return *static_cast<DerivedT *>(this);
    }

#if __cplusplus >= 201103L
      /**
      */
      inline DerivedT & replace (const_iterator first, const_iterator last
                , std::initializer_list<CharT> ilist)
      {
          base_class::replace(first, last, ilist.begin(), ilist.end());
          return *static_cast<DerivedT *>(this);
      }
#endif

    /**
     * @fn basic_string substr (size_type pos = 0, size_type count = npos) const
     */

    /**
     * @fn size_type copy (pointer dest, size_type count, size_type pos = 0) const
     */

    /**
     * @fn void resize (size_type count)
     */

    /**
     * @fn void resize (size_type count, CharT ch)
     */

    /**
     * @fn void swap (basic_string & other)
     */

    ///////////////////////////////////////////////////////////////////////////
    // Search                                                                //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn size_type find (basic_string const & bs, size_type pos = 0) const;
     */

    /**
     * @fn size_type find (const_pointer p, size_type pos, size_type count) const;
     */

    /**
     * @fn size_type find (const_pointer p, size_type pos = 0) const;
     */

    /**
     * @fn size_type find (CharT ch, size_type pos = 0) const;
     */

    /**
     * @fn size_type rfind (basic_string const & bs, size_type pos = npos) const;
     */

    /**
     * @fn size_type rfind (const_pointer p, size_type pos, size_type count) const;
     */

    /**
     * @fn size_type rfind (const_pointer p, size_type pos = npos) const;
     */

    /**
     * @fn size_type rfind (CharT ch, size_type pos = npos) const;
     */

    /**
     * @fn size_type find_first_of (basic_string const & bs, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_of (const_pointer p, size_type pos, size_type count) const
     */

    /**
     * @fn size_type find_first_of (const_pointer p, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_of (CharT ch, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_not_of (basic_string const & bs, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_not_of (const_pointer p, size_type pos, size_type count) const
     */

    /**
     * @fn size_type find_first_not_of (const_pointer p, size_type pos = 0) const
     */

    /**
     * @fn size_type find_first_not_of (CharT ch, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_of (basic_string const & bs, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_of (const_pointer p, size_type pos, size_type count) const
     */

    /**
     * @fn size_type find_last_of (const_pointer p, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_of (CharT ch, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_not_of (basic_string const & bs, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_not_of (const_pointer p, size_type pos, size_type count) const
     */

    /**
     * @fn size_type find_last_not_of (const_pointer p, size_type pos = 0) const
     */

    /**
     * @fn size_type find_last_not_of (CharT ch, size_type pos = 0) const
     */
};

template <>
inline size_t strlen (char const * s)
{
    return std::strlen(s);
}

}} // pfs::stdcxx
