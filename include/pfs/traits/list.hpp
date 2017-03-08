/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:08 PM
 */

#ifndef __PFS_TRAITS_LIST_HPP__
#define __PFS_TRAITS_LIST_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/iterator.hpp>

namespace pfs {
namespace traits {

template <typename Foundation, typename T>
struct list_traits
{
    typedef struct __Use_Specialized_Traits__ {} native_type;
    typedef native_type const & const_native_reference;
    typedef size_t              size_type;
    typedef T                   value_type;
    typedef T &                 reference;
    typedef T const &           const_reference;
    typedef T *                 iterator;
    typedef T const *           const_iterator;
    typedef T *                 reverse_iterator;
    typedef T const *           const_reverse_iterator;
    typedef native_type         data_type;
    
    static void xassign (data_type & d, const_native_reference lhs);
    static size_type xsize (data_type const & d);
    static iterator xbegin (data_type & d);
    static const_iterator xbegin (data_type const & d);
    static iterator xend (data_type & d);
    static const_iterator xend (data_type const & d);
    static reverse_iterator xrbegin (data_type & d);
    static const_reverse_iterator xrbegin (data_type const & d);
    static reverse_iterator xrend (data_type & d);
    static const_reverse_iterator xrend (data_type const & d);
    static iterator xerase (data_type & d, iterator first, iterator last);
    static void xclear (data_type & d);
    static void xpush_back (data_type & d, const_reference value);
    static void xpop_back (data_type & d);
    static void xpush_front (data_type & d, const_reference value);
    static void xpop_front (data_type & d);
    static void xresize (data_type & d, size_type count, const_reference value);
    static iterator xinsert (data_type & d, iterator pos, value_type const & value);
    static void xswap (data_type & lhs, data_type & rhs);
    static void xsplice (data_type & lhs, iterator pos, data_type & rhs);

    static const_native_reference xcast (data_type const & d);
};

template <typename Foundation, typename T>
class list
{
public:
    typedef list_traits<Foundation, T>                   traits_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::const_native_reference const_native_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::value_type             value_type;
    typedef typename traits_type::reference              reference;
    typedef typename traits_type::const_reference        const_reference;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::data_type              data_type;
    
private:
    data_type _d;
   
public:
	explicit list ()
		: _d()
	{}

    list (size_type count, T const & value)
        : _d(count, value)
    {}
    
    explicit list (size_type count)
        : _d(count)
    {}
    
    template <typename InputIt>
    list (InputIt first, InputIt last)
    {
        for (; first != last; ++first)
            push_back(*first);
    }
    
    explicit list (const_native_reference rhs)
        : _d(rhs)
    {}

    list & operator = (list const & rhs)
    {
        if (this != & rhs)
            traits_type::xassign(_d, traits_type::xcast(rhs._d));
        return *this;
    }

    list & operator = (const_native_reference rhs)
    {
        traits_type::xassign(_d, rhs);
        return *this;
    }
    
    iterator begin ()
    {
        return traits_type::xbegin(_d);
    }
		
    const_iterator begin () const
    {
        return traits_type::xbegin(_d);
    }
		
    const_iterator cbegin () const
    {
        return traits_type::xbegin(_d);
    }
    
    iterator end ()
    {
        return traits_type::xend(_d);
    }
		
    const_iterator end () const
    {
        return traits_type::xend(_d);
    }
		
    const_iterator cend () const
    {
        return traits_type::xend(_d);
    }
    
    reverse_iterator rbegin ()
    {
        return traits_type::xrbegin(_d);
    }
		
    const_reverse_iterator rbegin () const
    {
        return traits_type::xrbegin(_d);
    }
		
    const_reverse_iterator crbegin () const
    {
        return traits_type::xrbegin(_d);
    }
    
    reverse_iterator rend ()
    {
        return traits_type::xrend(_d);
    }
		
    const_reverse_iterator rend () const
    {
        return traits_type::xrend(_d);
    }
		
    const_reverse_iterator crend () const
    {
        return traits_type::xrend(_d);
    }
    
    reference front ()
    {
        return *begin();
    }
		
    const_reference front () const
    {
        return *begin();
    }
    
    reference back ()
    {
        iterator it = this->end();
        --it; 
        return *it;
    }
		
    const_reference back () const
    {
        const_iterator it = this->end();
        --it; 
        return *it;
    }
    
    size_type size () const pfs_noexcept
    {
        return traits_type::xsize(_d);
    }
	
    bool empty () const pfs_noexcept
    {
        return this->size() == 0;
    }
	
    void clear ()
    {
        traits_type::xclear(_d);
    }
    
    iterator erase (iterator pos)
    {
        iterator last(pos);
        ++last;
        return erase(pos, last);
    }

    iterator erase (iterator first, iterator last)
    {
        return traits_type::xerase(_d, first, last);
    }

    void push_back (const_reference value)
    {
        traits_type::xpush_back(_d, value);
    }
    
    void pop_back ()
    {
        traits_type::xpop_back(_d);
    }
    
    void push_front (T const & value)
    {
        traits_type::xpush_front(_d, value);
    }
		
    void pop_front ()
    {
        traits_type::xpop_front(_d);
    }
	
    void resize (size_type count)
    {
        resize(count, value_type());
    }
    
    void resize (size_type count, value_type const & value)
    {
        traits_type::xresize(_d, count, value);
    }

    void splice (iterator pos, list & rhs)
    {
        traits_type::xsplice(_d, pos, rhs._d);
    }
    
    iterator insert (iterator pos, value_type const & value)
    {
        return traits_type::xinsert(_d, pos, value);
    }
    
    iterator insert (iterator pos, size_type count, value_type const & value)
    {
        if (count) {
            list tmp(count, value);
            this->splice(pos, tmp);
            ++pos;
        }
        return pos;
    }

    template <typename InputIt>
    iterator insert (iterator pos, InputIt first, InputIt last)
    {
        if (first != last) {
            list tmp(first, last);
            this->splice(pos, tmp);
            ++pos;
        }
        return pos;
    }

    void swap (list & rhs)
    {
        traits_type::xswap(_d, rhs._d);
    }
    
    const_native_reference cast () const
    {
        return traits_type::xcast(_d);
    }
};

template <typename Foundation, typename T>
inline bool operator == (list<Foundation, T> const & lhs
        , list<Foundation, T> const & rhs)
{
    return lhs.cast() == rhs.cast();
}

template <typename Foundation, typename T>
inline bool operator != (list<Foundation, T> const & lhs
        , list<Foundation, T> const & rhs)
{
    return ! operator == (lhs, rhs);
}

template <typename Foundation, typename T>
inline void swap (list<Foundation, T> const & lhs
        , list<Foundation, T> const & rhs)
{
    lhs.swap(rhs);
}

}} // pfs::traits

#endif /* __PFS_TRAITS_LIST_HPP__ */
