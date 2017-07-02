/* 
 * File:   stack.hpp
 * Author: wladt
 *
 * Created on June 26, 2017, 2:28 PM
 */

#ifndef __PFS_TRAITS_STDCXX_STACK_HPP__
#define __PFS_TRAITS_STDCXX_STACK_HPP__

#include <stack>
#include <pfs/traits/value_ref.hpp>

namespace pfs {
namespace traits {
namespace stdcxx {

template <typename T>
struct stack_wrapper
{
    typedef std::stack<T> type;
};

template <typename T, typename ValueOrReference>
class basic_stack
{
    typedef ValueOrReference internal_type;
    
    template <typename T1, typename ValueOrReference1>
    friend bool
    operator == (basic_stack<T, ValueOrReference1> const &
            , basic_stack<T1, ValueOrReference1> const &);

    template<typename T1, typename ValueOrReference1>
    friend bool
    operator < (basic_stack<T1, ValueOrReference1> const &
            , basic_stack<T1, ValueOrReference1> const &);

public:
    typedef basic_stack<T, container_value<T, stack_wrapper> >     container_value_type;
    typedef basic_stack<T, container_reference<T, stack_wrapper> > container_reference_type;
    
    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;

    typedef typename native_type::value_type      value_type;
    typedef typename native_type::size_type       size_type;
    typedef typename native_type::reference       reference;
    typedef typename native_type::const_reference const_reference;

protected:
    internal_type _p;
    
public:
    basic_stack ()
    {}

    basic_stack (native_reference rhs)
        : _p(rhs)
    {}

    basic_stack (const_native_reference rhs)
        : _p(rhs)
    {}
    
    bool empty () const
    {
        return _p->empty();
    }

    size_type size () const
    {
        return _p->size();
    }

    reference top ()
    {
        //return _p->back();
        return _p->top();
    }

    const_reference top () const
    {
        //return _p->back();
        return _p->top();
    }

    void push (value_type const & x)
    {
        //_p->push_back(x);
        _p->push(x);
    }

#if __cplusplus >= 201103L

    void push (value_type && x)
    {
        //_p->push_back(std::forward(x));
        _p->push(std::forward(x));
    }

    template <typename... Args>
    void emplace (Args &&... args)
    {
        //_p->emplace_back(std::forward<Args>(args)...);
        _p->emplace(std::forward<Args>(args)...);
    }
#endif

    void pop ()
    {
        //_p->pop_back();
        _p->pop();
    }

//    void swap (stack & s) noexcept
//    {
//        pfs::swap(_p, s._p);
//    }
};

template <typename T>
class stack : public basic_stack<T, container_value<T, stack_wrapper> >
{
    typedef basic_stack<T, container_value<T, stack_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    stack ()
        : base_class()
    {}

    stack (native_reference rhs)
        : base_class(rhs)
    {}

    stack (const_native_reference rhs)
        : base_class(rhs)
    {}
};

template <typename T>
class stack_reference : public basic_stack<T, container_reference<T, stack_wrapper> >
{
    typedef basic_stack<T, container_reference<T, stack_wrapper> > base_class;

public:
    typedef typename base_class::native_type            native_type;
    typedef typename base_class::native_reference       native_reference;
    typedef typename base_class::const_native_reference const_native_reference;
    
public:
    stack_reference ()
        : base_class()
    {}

    stack_reference (native_reference rhs)
        : base_class(rhs)
    {}

    stack_reference (const_native_reference rhs)
        : base_class(rhs)
    {}
};

}}} // pfs::traits::stdcxx

#endif /* __PFS_TRAITS_STDCXX_STACK_HPP__ */

