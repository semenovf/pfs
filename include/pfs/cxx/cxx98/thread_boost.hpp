/* 
 * File:   thread_boost.hpp
 * Author: wladt
 *
 * Created on September 10, 2017, 1:31 PM
 */

#ifndef __PFS_CXX_CXX98_THREAD_BOOST_HPP__
#define __PFS_CXX_CXX98_THREAD_BOOST_HPP__

#include <boost/thread.hpp>
#include <pfs/cxxlang.hpp>

namespace pfs {

class thread : public boost::thread
{
    typedef boost::thread base_class;
    
private:
    thread (thread const &);
    thread & operator = (thread const &);
    
public:
    thread () pfs_noexcept
        : base_class()
    {}

    template <typename Function>
    thread (Function f)
        : base_class(f)
    {}

    template <typename Function, typename Arg1>
    thread (Function f, Arg1 a1)
        : base_class(f, a1)
    {}

    template <typename Function, typename Arg1, typename Arg2>
    thread (Function f, Arg1 a1, Arg2 a2)
        : base_class(f, a1, a2)
    {}

    template <typename Function, typename Arg1, typename Arg2, typename Arg3>
    thread (Function f, Arg1 a1, Arg2 a2, Arg3 a3)
        : base_class(f, a1, a2, a3)
    {}

    template <typename Function, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    thread (Function f, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
        : base_class(f, a1, a2, a3, a4)
    {}

    template <typename Function, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    thread (Function f, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
        : base_class(f, a1, a2, a3, a4, a5)
    {}
};

} // pfs

#endif /* __PFS_CXX_CXX98_THREAD_BOOST_HPP__ */

