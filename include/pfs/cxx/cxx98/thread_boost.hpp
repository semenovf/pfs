#ifndef __PFS_CXX_CXX98_THREAD_BOOST_HPP__
#define __PFS_CXX_CXX98_THREAD_BOOST_HPP__

#include <boost/thread.hpp>
#include <pfs/cxxlang.hpp>
#include "chrono.hpp"

namespace pfs {

class thread : public ::boost::thread
{
    typedef ::boost::thread base_class;
    
private:
    thread (thread const &);
    thread & operator = (thread const &);
    
public:
    thread () noexcept
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

namespace this_thread {

inline ::boost::thread::id get_id () noexcept
{
    return ::boost::this_thread::get_id();
}

inline void yield () noexcept
{
    ::boost::this_thread::yield();
}

template <typename Rep, typename Period>
inline void sleep_for (pfs::chrono::duration<Rep, Period> const & sleep_duration)
{
    ::boost::this_thread::sleep_for<Rep, Period>(sleep_duration);
}

template <typename Clock, typename Duration>
inline void sleep_until (pfs::chrono::time_point<Clock, Duration> const & sleep_time)
{
    ::boost::this_thread::sleep_until<Clock, Duration>(sleep_time);
}

} // this_thread

} // pfs

#endif /* __PFS_CXX_CXX98_THREAD_BOOST_HPP__ */

