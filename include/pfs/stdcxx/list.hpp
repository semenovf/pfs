#pragma once
#include <list>

namespace pfs {

namespace stdcxx {

template <typename T, typename DerivedT>
class list : public std::list<T>
{
    typedef std::list<T> base_class;

public:
    typedef typename base_class::value_type       value_type;
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

    using base_class::erase;
    using base_class::insert;
    using base_class::merge;

    inline iterator const_cast_iterator (const_iterator it)
    {
        // TODO Need optimization
        if (it == this->cbegin())
            return this->begin();
        if (it == this->cend())
            return this->end();
        return this->find(it->first);
    }

public:
    ///////////////////////////////////////////////////////////////////////////
    // Constructors                                                          //
    ///////////////////////////////////////////////////////////////////////////

    list () : base_class() {}

    list (size_type count, const_reference value)
        : base_class(count, value)
    {}

    explicit list (size_type count)
        : base_class(count)
    {}

    template <typename InputIt>
    list (InputIt first, InputIt last)
        : base_class(first, last)
    {}

    list (list const & rhs)
        : base_class(rhs)
    {}

#if __cplusplus >= 201103L
    list (list && rhs)
        : base_class(std::forward<list>(rhs))
    {}

    list (std::initializer_list<T> ilist)
        : base_class(ilist.begin(), ilist.end())
    {}
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Assign operators and methods                                          //
    ///////////////////////////////////////////////////////////////////////////

    DerivedT & operator = (DerivedT const & rhs)
    {
        base_class::operator = (rhs);
        return *this;
    }

#if __cplusplus >= 201103L
    DerivedT & operator = (DerivedT && rhs)
    {
        base_class::operator = (std::forward<list>(rhs));
        return *this;
    }

    DerivedT & operator = (std::initializer_list<T> ilist)
    {
        base_class::operator = (ilist);
        return *this;
    }
#endif

    //
    // Note: std::list:::assign() methods has no return value.
    //

    DerivedT & assign (size_type count, const_reference value)
    {
        base_class::assign(count, value);
        return *this;
    }

    template <typename InputIt>
    DerivedT & assign (InputIt first, InputIt last)
    {
        base_class::template assign<InputIt>(first, last);
        return *this;
    }

#if __cplusplus >= 201103L
    DerivedT & assign (std::initializer_list<T> ilist)
    {
        base_class::assign(ilist);
        return *this;
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Element access                                                        //
    ///////////////////////////////////////////////////////////////////////////

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
     * @fn iterator rbegin ()
     */

    /**
     * @fn const_iterator rbegin () const
     */

#if __cplusplus < 201103L
    const_iterator crbegin () const
    {
        return this->rbegin();
    }
#endif

    /**
     * @fn iterator rend ()
     */

    /**
     * @fn const_iterator rend () const
     */

#if __cplusplus < 201103L
    const_iterator crend () const
    {
        return this->rend();
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Capacity                                                              //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn bool empty () const
     */

    /**
     * @fn size_type size () const
     */

    /**
     * @fn size_type max_size () const
     */

    ///////////////////////////////////////////////////////////////////////////
    // Modifiers                                                             //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn void clear ()
     */

    /**
     * @fn iterator insert (const_iterator pos, T const & value)
     */

#if __cplusplus < 201103L
    iterator insert (const_iterator pos, T const & value)
    {
        return base_class::erase(const_cast_iterator(pos), value);
    }
#endif

#if __cplusplus >= 201103L
    /**
     * @fn iterator insert (const_iterator pos, T && value)
     */
#endif

    /**
     * @fn iterator insert (const_iterator pos, size_type count, T const & value)
     *
     * @return Iterator pointing to the first element inserted, or @a pos if count==0.
     */

    /**
     * @fn template <typename InputIt>
     *     iterator insert (const_iterator pos, InputIt first, InputIt last)
     */

#if __cplusplus < 201103L
    iterator insert (const_iterator pos, size_type count, const_reference value)
    {
        //
        // C++ <  C++11: void insert( iterator pos, size_type count, const T& value );
        // C++ >= C++11: iterator insert( const_iterator pos, size_type count, const T& value );
        //
        // Note: No iterators or references are invalidated
        //
        iterator result = pos;

        if (count > 0) {
            result = this->insert(pos, value); //  first inserted value

            if (--count > 0)
                base_class::insert(const_cast_iterator(pos), count, value);
        }
        return result;
    }

    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        return base_class::template insert<InputIt>(
                const_cast_iterator(pos), first, last);
    }
#endif

#if __cplusplus >= 201103L
    /**
     * @fn iterator insert (const_iterator pos, std::initializer_list<T> ilist)
     */
#endif

#if __cplusplus >= 201103L
    /**
     * @fn template <typename... Args>
     *     iterator emplace (const_iterator pos, Args &&... args)
     */
#endif

    /**
     * @fn iterator erase (const_iterator pos)
     */

    /**
     * @fn iterator erase (const_iterator first, const_iterator last)
     */

#if __cplusplus < 201103L
    //
    // C++98 has erase() methods signatures like this once:
    //      iterator erase(iterator pos)
    //      iterator erase(iterator first, iterator last)
    //

    inline iterator erase (const_iterator pos)
    {
        return base_class::erase(const_cast_iterator(pos));
    }

    inline iterator erase (const_iterator first, const_iterator last)
    {
        return base_class(const_cast_iterator(first)
                , const_cast_iterator(last));
    }
#endif

    /**
     * @fn void push_back (const_reference value)
     */

#if __cplusplus >= 201103L
    /**
     * @fn void push_back (T && value)
     */
#endif

#if __cplusplus >= 201103L
    // TODO Implement to meet C++17:
    // reference emplace_back (Args &&... args)
    //
    /**
     * @fn template <typename... Args>
     *     void emplace_back (Args &&... args)
     */

    /**
     * @fn template <typename... Args>
     *     void emplace_front (Args &&... args )
     */
#endif

    /**
     * @fn void pop_back ()
     */

    /**
    * @fn void push_front (const_reference value)
    */

#if __cplusplus >= 201103L
    /**
    * @fn void push_front (T && value)
    */
#endif

    /**
     * @fn void pop_front ()
     */

    void resize (size_type count)
    {
        base_class::resize(count, T());
    }

    void resize (size_type count, const_reference value)
    {
        base_class::resize(count, value);
    }

    /**
     * @fn  void swap (list & rhs)
     */

    ///////////////////////////////////////////////////////////////////////////
    // Operations                                                            //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn void merge (list & rhs)
     */

#if __cplusplus >= 201103L
    /**
     * @fn void merge (list && rhs)
     */
#endif

    /**
     * @fn template <typename Compare>
     *     void merge (list & rhs, Compare comp)
     */

#if __cplusplus >= 201103L
    /**
     * @fn void merge (list && rhs, Compare comp)
     */
#endif

    /**
     * @fn void splice (const_iterator pos, list & rhs)
     */

#if __cplusplus >= 201103L
    /**
     * @fn void splice (const_iterator pos, list && rhs )
     */
#endif

    /**
     * @fn void splice (const_iterator pos, list & rhs, const_iterator it)
     */

    /**
     * @fn void splice (const_iterator pos, list && rhs, const_iterator it)
     */

    /**
     * @fn void splice (const_iterator pos, list & rhs, const_iterator first, const_iterator last)
     */

#if __cplusplus >= 201103L
    /**
     * @fn void splice (const_iterator pos, list && rhs, const_iterator first, const_iterator last)
     */
#endif

     /**
      * @fn void remove (const_reference value)
      */

     /**
      * @fn template <typename UnaryPredicate>
      *     void remove_if (UnaryPredicate p)
      */

     /**
      * @fn void reverse ()
      */

     /**
      * @fn void unique ()
      */

     /**
      * @fn template <typename BinaryPredicate>
      *     void unique (BinaryPredicate p)
      */

     /**
      * @fn void sort ()
      */

     /**
      * @fn template <typename Compare>
      *     void sort (Compare comp)
      */

    ///////////////////////////////////////////////////////////////////////////
    // Compare operators                                                     //
    ///////////////////////////////////////////////////////////////////////////
//     template <typename T>
//     friend inline bool operator == (DerivedT const & lhs, DerivedT const & rhs )
//     {
//     }
};

}} // pfs::stdcxx

