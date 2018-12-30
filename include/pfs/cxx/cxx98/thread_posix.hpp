#ifndef __PFS_CXX_CXX98_THREAD_POSIX_HPP__
#define __PFS_CXX_CXX98_THREAD_POSIX_HPP__

#include <exception>
#include <unistd.h>
#include <pthread.h>
#include <pfs/cxxlang.hpp>
#include <pfs/cxx/cxx98/binder.hpp>

namespace pfs {

class thread
{
public:
    typedef pthread_t native_handle_type;

    class id
    {
        native_handle_type _value;

    public:
        id () noexcept 
            : _value ()
        {}

        explicit
        id (native_handle_type handle)
            : _value(handle)
        {}

    private:
        friend class thread;

        friend
        bool operator == (thread::id lhs, thread::id rhs) noexcept
        {
            return pthread_equal(lhs._value, rhs._value) != 0
                    ? true
                    : false;
        }

        friend
        bool operator < (thread::id lhs, thread::id rhs) noexcept
        {
            return lhs._value < rhs._value;
        }
    };

private:
    id _id;
    
private:
#if __cplusplus >= 201103L
    thread (thread const &) = delete;
#else
    thread (thread const &);
    thread (thread &);
    thread & operator = (thread const &);
#endif
        
public:
    thread () noexcept;

    ~thread ()
    {
        if (joinable())
            std::terminate();
    }
    
    bool joinable () const noexcept
    {
        return !(_id == id());
    }

    thread::id get_id () const noexcept
    {
        return _id; 
    }

    native_handle_type native_handle () noexcept
    {
        return _id._value;
    }

    /**
     * Blocks the current thread until the thread identified by *this 
     * finishes its execution. 
     */
    void join ();
    
    /**
     * Separates the thread of execution from the thread object, 
     * allowing execution to continue independently. 
     * Any allocated resources will be freed once the thread exits.
     */
    void detach ();

    static unsigned int hardware_concurrency () noexcept
    {
        // Linux, Solaris, AIX and Mac OS X >=10.4 (i.e. Tiger onwards)
        return static_cast<unsigned int>(sysconf(_SC_NPROCESSORS_ONLN));
    }
    
    // TODO
    //void swap (thread & other) noexcept;
};

inline bool operator!= (thread::id lhs, thread::id rhs) noexcept
{
    return !(lhs == rhs);
}

inline bool operator <= (thread::id lhs, thread::id rhs) noexcept
{
    return !(rhs < lhs);
}

inline bool operator > (thread::id lhs, thread::id rhs) noexcept
{
    return rhs < lhs;
}

inline bool operator >= (thread::id lhs, thread::id rhs) noexcept
{
    return !(lhs < rhs);
}

namespace this_thread {

inline thread::id get_id () noexcept
{
    return thread::id(pthread_self());
}

inline void yield () noexcept
{
    pthread_yield();
}

//void __sleep_for (chrono::seconds, chrono::nanoseconds);
//
//template<typename _Rep, typename _Period>
//inline void
//sleep_for (const chrono::duration<_Rep, _Period>& __rtime)
//{
//    if (__rtime <= __rtime.zero())
//        return;
//    auto __s = chrono::duration_cast<chrono::seconds>(__rtime);
//    auto __ns = chrono::duration_cast<chrono::nanoseconds>(__rtime - __s);
//#ifdef _GLIBCXX_USE_NANOSLEEP
//    __gthread_time_t __ts ={
//        static_cast<std::time_t> (__s.count()),
//        static_cast<long> (__ns.count())
//    };
//    ::nanosleep(&__ts, 0);
//#else
//    __sleep_for(__s, __ns);
//#endif
//}

//template< typename _Clock, typename _Duration>
//inline void sleep_until (const chrono::time_point<_Clock, _Duration>& __atime)
//{
//    auto __now = _Clock::now();
//    if (__now < __atime)
//        sleep_for(__atime - __now);
//}

} // this_thread

} // pfs

#endif /* __PFS_CXX_CXX98_THREAD_POSIX_HPP__ */

