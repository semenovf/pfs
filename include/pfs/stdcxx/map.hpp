#pragma once
#include <map>

namespace pfs {
namespace stdcxx {

//
// Trick to convert const_iterator to iterator (>= C++11) described at:
// [How to remove constness of const_iterator?](https://stackoverflow.com/questions/765148/how-to-remove-constness-of-const-iterator)
//
// Original at:
// [Hat tip to Howard Hinnant and Jon Kalb for this trick](https://twitter.com/_JonKalb/status/202815932089896960)
//

#if __cplusplus >= 201103L
    template <typename Key, typename T>
#else
    template <typename Key, typename T, typename DerivedT>
#endif
class map : public std::map<Key, T>
{
    typedef std::map<Key, T> base_class;

public:
#if __cplusplus >= 201103L
    using DerivedT = map;
#endif
    typedef typename base_class::key_type         key_type;
    typedef typename base_class::mapped_type      mapped_type;
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

    map () : base_class() {}

    template <typename InputIt>
    map (InputIt first, InputIt last)
        : base_class(first, last)
    {}

    map (map const & rhs)
        : base_class(rhs)
    {}

#if __cplusplus >= 201103L
    map (map && rhs)
        : base_class(std::forward<map>(rhs))
    {}

    map (std::initializer_list<std::pair<Key const, T> > ilist)
        : base_class(ilist)
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
        base_class::operator = (std::forward<map>(rhs));
        return *this;
    }

    DerivedT & operator = (std::initializer_list<std::pair<Key const, T> > ilist)
    {
        base_class::operator = (ilist);
        return *this;
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Element access                                                        //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn T & at (key_type const & key)
     */

    /**
     * @fn T const & at (key_type const & key) const
     */

    /**
     * @fn T & operator [] (key_type const & key)
     */

#if __cplusplus >= 201103L
    /**
     * @fn T & operator [] (Key && key)
     */
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
     */
    pfs::pair<iterator,bool> insert (key_type const & key, mapped_type const & value)
    {
        std::pair<iterator,bool> result = base_class::insert(std::make_pair(key, value));
        return pfs::make_pair(result.first, result.second);
    }

#if __cplusplus >= 201103L
    pfs::pair<iterator,bool> insert (key_type && key, mapped_type && value)
    {
        std::pair<iterator,bool> result = base_class::insert(
                std::make_pair(std::forward<key_type>(key)
                        , std::forward<mapped_type>(value)));
        return pfs::make_pair(result.first, result.second);
    }
#endif

    /**
     * @fn std::pair<iterator,bool> insert (value_type const & value)
     */

#if __cplusplus >= 201103L
    /**
     * @fn template <typename... Args>
     *     std::pair<iterator,bool> emplace (Args &&... args)
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
        base_class::erase(const_cast_iterator(first), result);
        return result;
    }
#endif

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

    static inline mapped_type & mapped_reference (iterator it)
    {
        return it->second;
    }

    static inline mapped_type const & mapped_reference (const_iterator it)
    {
        return it->second;
    }

    static inline key_type const & key_reference (const_iterator it)
    {
        return it->first;
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
