/* 
 * File:   functional.hpp
 * Author: wladt
 *
 * Created on May 2, 2017, 4:16 PM
 */

#ifndef __PFS_CXX_CXX98_FUNCTIONAL_HPP__
#define __PFS_CXX_CXX98_FUNCTIONAL_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/cxx/cxx98/memory.hpp>

namespace pfs {

template <typename T>
class reference_wrapper
{
    T * _ptr;

public:
    typedef T type;

public:
    reference_wrapper () pfs_noexcept
        : _ptr(0)
    {}
    
    reference_wrapper (T & ref) pfs_noexcept
        : _ptr(addressof(ref))
    {}
    
//    reference_wrapper (reference_wrapper const & x) pfs_noexcept = default;
//    reference_wrapper & operator = (reference_wrapper const & x) pfs_noexcept = default;

    operator T & () const pfs_noexcept
    {
        return *_ptr;
    }

    T & get () const pfs_noexcept
    {
        return *_ptr;
    }

//    template< class... ArgTypes >
//    typename std::result_of<T & (ArgTypes && ...)>::type
//    
//    operator () (ArgTypes &&... args) const
//    {
//        return std::invoke(get(), std::forward<ArgTypes>(args)...);
//    }
};

} // pfs

#endif /* __PFS_CXX_CXX98_FUNCTIONAL_HPP__ */

