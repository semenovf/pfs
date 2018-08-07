#pragma once
#include <set>
#include <pfs/type_traits.hpp>

namespace pfs {
namespace stdcxx {

template <typename Key, typename DerivedT>
class set : public std::set<Key>
{
    typedef std::set<Key> base_class;

public:
    typedef typename base_class::key_type         key_type;
    typedef typename base_class::value_type       mapped_type;
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

    using base_class::find;
    using base_class::erase;
    using base_class::insert;

    iterator const_cast_iterator (const_iterator it)
    {
        // TODO Need optimization if possible
        if (it == this->cbegin())
            return this->begin();
        if (it == this->cend())
            return this->end();
        return this->find(*it);
    }

public:
    ///////////////////////////////////////////////////////////////////////////
    // Constructors                                                          //
    ///////////////////////////////////////////////////////////////////////////

    set () : base_class() {}

    template <typename InputIt>
    set (InputIt first, InputIt last)
        : base_class(first, last)
    {}

    set (set const & rhs)
        : base_class(rhs)
    {}

#if __cplusplus >= 201103L
    set (set && rhs)
        : base_class(std::forward<set>(rhs))
    {}

    set (std::initializer_list<value_type> ilist)
        : base_class(ilist)
    {}
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Assign operators and methods                                          //
    ///////////////////////////////////////////////////////////////////////////

    DerivedT & operator = (set const & rhs)
    {
        base_class::operator = (rhs);
        return *this;
    }

#if __cplusplus >= 201103L
    DerivedT & operator = (set && rhs)
    {
        base_class::operator = (std::forward<set>(rhs));
        return *this;
    }

    DerivedT & operator = (std::initializer_list<value_type> ilist)
    {
        base_class::operator = (ilist);
        return *this;
    }
#endif

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
     * @fn std::pair<iterator,bool> insert (value_type const & value)
     */

#if __cplusplus >= 201103L
    /**
     * @fn std::pair<iterator,bool> insert (value_type && value)
     */
#endif

#if __cplusplus < 201103L
    iterator insert (const_iterator hint, value_type const & value)
    {
        return base_class::insert(const_cast_iterator(hint), value);
    }
#endif

#if __cplusplus >= 201103L
    /**
    * @fn iterator insert (const_iterator hint, value_type && value);
    */
#endif

    /**
     * @fn template <typename InputIt>
     *     void insert (InputIt first, InputIt last);
     */

#if __cplusplus >= 201103L
    /**
     * @fn void insert (std::initializer_list<value_type> ilist)
     */
#endif

#if __cplusplus >= 201103L
    /**
     * @fn template <typename... Args>
     *     std::pair<iterator,bool> emplace (Args &&... args)
     */

    /**
     * @fn template <class... Args>
     *     iterator emplace_hint (const_iterator hint, Args&&... args)
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
    //      void erase(iterator pos)
    //      void erase(iterator first, iterator last)
    //

    inline iterator erase (const_iterator pos)
    {
        // Other iterators are not affected
        iterator it = const_cast_iterator(pos);
        iterator result = it;
        ++result;
        base_class::erase(it);
        return result;
    }

    inline iterator erase (const_iterator first, const_iterator last)
    {
        // Other iterators are not affected
        iterator result = const_cast_iterator(last);
        base_class(const_cast_iterator(first), result);
        return result;
    }
#endif

    // TODO Since C++17
    // iterator erase (iterator pos);

    /**
     * @fn size_type erase (key_type const & key)
     */

    /**
     * @fn void swap (map & rhs)
     */

    ///////////////////////////////////////////////////////////////////////////
    // Lookup                                                                //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn size_type count (Key const & key) const
     *
     * @details Return the number of elements with key that compares equivalent
     *          to the specified argument, which is either 1 or 0 since this
     *          container does not allow duplicates.
     *
     * @return The number of elements with key @a key.
     */

    /**
     * @fn iterator find (Key const & key)
     */

    /**
     * @fn const_iterator find (Key const & key) const
     */

    /**
     * @fn std::pair<iterator,iterator> equal_range (Key const & key)
     */

    /**
     * @fn std::pair<const_iterator,const_iterator> equal_range (Key const & key) const
     */

    /**
     * @fn iterator lower_bound (Key const & key)
     */

    /**
     * @fn const_iterator lower_bound (Key const & key) const
     */

    /**
     * @fn iterator upper_bound (Key const & key)
     */

    /**
     * @fn const_iterator upper_bound (Key const & key) const
     */

    template <typename Iter>
    static inline
    typename enable_if<is_same<Iter, iterator>::value && !is_same<iterator, const_iterator>::value, mapped_type &>::type
    mapped_reference (Iter it)
    {
        return *it;
    }

    template <typename Iter>
    static inline
    typename enable_if<is_same<Iter, const_iterator>::value, mapped_type const &>::type
    mapped_reference (const_iterator it)
    {
        return *it;
    }

    static inline key_type const & key_reference (const_iterator it)
    {
        return *it;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Compare operators                                                     //
    ///////////////////////////////////////////////////////////////////////////
//     template <typename T>
//     friend inline bool operator == (DerivedT const & lhs, DerivedT const & rhs )
//     {
//     }
};

}} // pfs::stdcxx

