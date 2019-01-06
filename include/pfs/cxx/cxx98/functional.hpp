#pragma once
#include <pfs/config.h>
#include <pfs/cxxlang.hpp>
#include <pfs/cxx/cxx98/memory.hpp>
#include <pfs/cxx/cxx98/type_traits.hpp>

#if HAVE_BOOST_BIND
#   include "bind_boost.hpp"
#   include "function_boost.hpp"
#else
#   error "Custom functional is not implemented, use Boost::Bind"
#endif

namespace pfs {

template <typename T, bool IsFunctionType = is_function<T>::value>
struct function_to_function_pointer
{
    typedef T type;
};

template <typename T>
struct function_to_function_pointer<T, true>
{
    typedef T * type;
};

template <typename T>
class reference_wrapper
    //: public _Reference_wrapper_base<typename remove_cv<_Tp>::type>
{
    // If T is a function type, we can't form result_of<_Tp(...)>,
    // so turn it into a function pointer type.
    typedef typename function_to_function_pointer<T>::type func_type;

    T * _ptr;

public:
    typedef T type;

public:
    explicit reference_wrapper (T & ref)
        : _ptr (& ref)
    {}

    reference_wrapper (reference_wrapper<T> const & ref)
        :_ptr(ref._ptr)
    {}

    reference_wrapper & operator = (reference_wrapper<T> const & ref)
    {
        _ptr = ref._ptr;
        return *this;
    }

    operator T & () const noexcept
    {
        return this->get();
    }

    T & get () const noexcept
    {
        return *_ptr;
    }

//    typename result_of<func_type()>::type
//    operator () () const
//    {
//        return (*get())();
//    }
//
//    template <typename A1>
//    typename result_of<func_type(A1)>::type
//    operator () (A1 a1) const
//    {
//        return (*get())(a1);
//    }
};

template <typename T>
inline reference_wrapper<T> ref (T & ref)
{
    return reference_wrapper<T>(ref);
}

template <typename T>
inline reference_wrapper<const T> cref (T const & ref)
{
    return reference_wrapper<const T>(ref);
}

template <typename T>
inline reference_wrapper<T> ref (reference_wrapper<T> ref)
{
    return ref(ref.get());
}

template <typename T>
inline reference_wrapper<const T> cref (reference_wrapper<T> ref)
{
    return cref(ref.get());
}

//    template< class... ArgTypes >
//    typename std::result_of<T & (ArgTypes && ...)>::type
//
//    operator () (ArgTypes &&... args) const
//    {
//        return std::invoke(get(), std::forward<ArgTypes>(args)...);
//    }

template <typename T = void>
struct greater : std::greater<T> {};

template <typename T = void>
struct less : std::less<T> {};


} // pfs
