/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:58 PM
 */

#ifndef __PFS_TRAITS_STDCXX_LIST_HPP__
#define __PFS_TRAITS_STDCXX_LIST_HPP__

#include <list>
#include <pfs/traits/list.hpp>

namespace stdcxx {

template <typename T>
struct list
{
    typedef std::list<T> type;
};

} // stdcxx

namespace pfs {
namespace traits {

template <typename T>
struct list_traits<T, ::stdcxx::list>
{
    typedef typename ::stdcxx::list<T>::type             native_type;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::difference_type        difference_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    typedef native_type                                  data_type;
};

template <typename T>
class basic_list<T, ::stdcxx::list>
    : public details::basic_list<T, ::stdcxx::list>
{
    typedef details::basic_list<T, ::stdcxx::list> base_type;
    typedef basic_list self_type;
    
protected:
    typedef typename base_type::native_type            native_type;
    typedef typename base_type::value_type             value_type;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::difference_type        difference_type;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;

    virtual iterator xbegin ()
    {
        return this->_d.begin();
    }
    
    virtual const_iterator xbegin () const
    {
        return this->_d.begin();
    }
    
    virtual iterator xend ()
    {
        return this->_d.end();
    }
    
    virtual const_iterator xend () const
    {
        return this->_d.end();
    }
    
    virtual reverse_iterator xrbegin ()
    {
        return this->_d.rbegin();
    }
    
    virtual const_reverse_iterator xrbegin () const
    {
        return this->_d.rbegin();
    }
    
    virtual reverse_iterator xrend ()
    {
        return this->_d.rend();
    }
    
    virtual const_reverse_iterator xrend () const
    {
        return this->_d.rend();
    }
    
    virtual size_type xsize () const
    {
        return this->_d.size();
    }
    
    virtual void xclear ()
    {
        this->_d.clear();
    }

    virtual iterator xerase (iterator position)
    {
#if __cplusplus >= 201103
        return _d.erase(position);
#else
        this->_d.erase(position++);
        return position;
#endif        
    }

    virtual iterator xerase (iterator first, iterator last)
    {
#if __cplusplus >= 201103
        return _d.erase(first, last);
#else
        this->_d.erase(first, last);
        return last;
#endif        
    }

    virtual void xpush_back (T const & value)
    {
        this->_d.push_back(value);
    }

    virtual void xpop_back ()
    {
        this->_d.pop_back();
    }

    virtual void xpush_front (T const & value)
    {
        this->_d.push_front(value);
    }

    virtual void xpop_front ()
    {
        this->_d.pop_front();
    }

    virtual void xswap (base_type & rhs)
    {
        this->_d.swap(rhs._d);
    }

    virtual void xresize (size_type count, value_type const & value)
    {
        this->_d.resize(count, value);
    }
    
    virtual void xsplice (iterator pos, base_type & rhs)
    {
        this->_d.splice(pos, rhs.native());
    }
    
    virtual iterator xinsert (iterator pos, value_type const & value)
    {
#if __cplusplus >= 201103L
        return this->_d.insert(pos, value);
#else
        this->_d.insert(pos++, value);
        return pos;
#endif
    }

public:
    virtual native_type & native ()
    {
        return this->_d;
    }

    virtual native_type const & native () const
    {
        return this->_d;
    }
};

}} // pfs::traits

#endif /* __PFS_TRAITS_STDCXX_LIST_HPP__ */

