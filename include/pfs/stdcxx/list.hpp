#pragma once
#include <list>
#include <pfs/sequence_container.hpp>

namespace pfs {

namespace stdcxx { 
    
template <typename T, typename DerivedT>
class list : public std::list<T>
{
    typedef std::list<T> base_class;

public:
    typedef typename base_class::value_type       value_type;
    typedef typename base_class::const_pointer    const_pointer;
    typedef typename base_class::reference        reference;
    typedef typename base_class::const_reference  const_reference;
    typedef typename base_class::iterator         iterator;
    typedef typename base_class::const_iterator   const_iterator;
    typedef typename base_class::reverse_iterator reverse_iterator;
    typedef typename base_class::const_reverse_iterator const_reverse_iterator;
    typedef typename base_class::difference_type  difference_type;
    typedef typename base_class::size_type        size_type;

    using base_class::erase;
    using base_class::insert;

    //
    // Apply trick described at:
    // [How to remove constness of const_iterator?](https://stackoverflow.com/questions/765148/how-to-remove-constness-of-const-iterator)
    //
    // Original at:
    // [Hat tip to Howard Hinnant and Jon Kalb for this trick](https://twitter.com/_JonKalb/status/202815932089896960)
    //
    inline iterator const_cast_iterator (const_iterator it)
    {
        return base_class::erase(it, it);
    }
    
public:
    list () : base_class() {}
    
    list (size_type count, const_reference value)
        : base_class(count, value)
    {}

    explicit list (size_type count)
        : base_class(count)
    {}
    
    template <typename InputIt>
    list (InputIt first, InputIt last)
        : base_class<InputIt>(first, last)
    {}
    
    list (list const & other)
        : base_class(other)
    {}
    
//     list (list&& other );
//     list (std::initializer_list<T> init)

    // *************************************************************************
    // BEGIN Requirements for container traits
    // Based on [C++ concepts: Container](http://en.cppreference.com/w/cpp/concept/Container)
    // *************************************************************************

    // *** ITERATORS ***
    //
    /**
     * @fn iterator begin ()
     */

    /**
     * @fn const_iterator begin () const
     */

    /**
     * @fn iterator end ()
     */

    /**
     * @fn const_iterator end () const
     */

#if __cplusplus < 201103L
    const_iterator cbegin () const
    {
        return this->cbegin();
    }

    const_iterator cend () const
    {
        return this->cend();
    }
#endif

    // *** CAPACITY ***
    //

    /**
     * @fn size_type size () const
     */

    /**
     * @fn size_type max_size () const 
     */

    /**
     * @fn bool empty () const
     */

    // *** MODIFIERS ***
    //
    // FIXME
//    void swap (container & rhs)
//    {
//        _p->swap(*rhs._p);
//    }

    // *** NON-MEMBER FUNCTIONS (OPERATORS) ***
    //

//     friend inline bool operator == (list const & lhs
//         , list const & rhs)
//     {
//         return lhs == rhs;
//     }

//     friend inline bool operator != (list_basic const & lhs
//         , list_basic const & rhs)
//     {
//         return *lhs._p != *rhs._p;
//     }

    // *************************************************************************
    // } END Requirements for container traits
    // *************************************************************************

    // *************************************************************************
    // BEGIN Requirements for sequence container traits {
    // *************************************************************************

    // *** ELEMENT ACCESS ***
    //
    
    /**
     * @fn reference back ()
     */

    /**
     * @fn const_reference back () const
     */

    /**
     * @fn reference front ()
     */

    /**
     * @fn const_reference front () const
     */

    // *** MODIFIERS ***
    //

    /**
     * @fn void clear ()
     */

#if __cplusplus >= 201103L
    /**
     * @fn template <typename... Args>
     *     iterator emplace (const_iterator pos, Args &&... args)
     */

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
     * @fn iterator erase (iterator pos)
     */
    
    /**
     * @fn iterator erase (iterator first, iterator last)
     */
    
#if __cplusplus < 201103L
    //
    // C++98 has erase() methods signatures like this once:
    //      iterator erase(iterator pos)
    //      iterator erase(iterator first, iterator last)
    //
    
    inline iterator erase (const_iterator pos)
    {
        return base_class::erase(const_cast_iterator(it));
    }

    inline iterator erase (iterator first, iterator last)
    {
        return base_class(const_cast_iterator(first)
                , const_cast_iterator(last));
    }
#endif

#if __cplusplus < 201103L
    iterator insert (const_iterator pos, T const & value)
    {
        return base_class::erase(const_cast_iterator(pos), value);
    }
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

#if __cplusplus >= 201103L
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
            result = this->insert(pos, value);
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

    /**
    * @fn void push_front (const_reference value)
    */

    /**
     * @fn void push_back (const_reference value)
     */

    /**
     * @fn void pop_front ()
     */

    /**
     * @fn void pop_back ()
     */

    // *************************************************************************
    // } END Requirements for sequence container traits
    // *************************************************************************
};

} //  stdcxx

namespace sequence_container {

template <typename T>
struct type_traits<T, stdcxx::list>
{
    typedef std::list<T> type;
    typedef T value_type;
};

}} // pfs::associative_container

