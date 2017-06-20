/* 
 * File:   thread.hpp
 * Author: wladt
 *
 * Created on June 16, 2017, 2:27 PM
 */

#ifndef __PFS_CXX_CXX11_THREAD_HPP__
#define __PFS_CXX_CXX11_THREAD_HPP__

#include <thread>
#include "chrono.hpp"

namespace pfs {

using thread = std::thread;

namespace this_thread {

inline std::thread::id get_id () noexcept
{
    return std::this_thread::get_id();
}

inline void yield () noexcept
{
    std::this_thread::yield();
}

template <typename Rep, typename Period>
inline void sleep_for (pfs::chrono::duration<Rep, Period> const & sleep_duration)
{
    std::this_thread::sleep_for<Rep, Period>(sleep_duration);
}

template <typename Clock, typename Duration>
inline void sleep_until (pfs::chrono::time_point<Clock, Duration> const & sleep_time)
{
    std::this_thread::sleep_until<Clock, Duration>(sleep_time);
}

} // this_thread

} // pfs

#endif /* __PFS_CXX_CXX11_THREAD_HPP__ */

