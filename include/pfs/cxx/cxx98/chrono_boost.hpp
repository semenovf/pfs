#pragma once
#include <boost/chrono.hpp>
#include <pfs/cxxlang.hpp>
#include "ratio_boost.hpp"

namespace pfs {
namespace chrono {

template <typename Rep, typename Period = ::boost::ratio<1> > 
class duration : public ::boost::chrono::duration<Rep, Period>
{
    typedef ::boost::chrono::duration<Rep, Period> base_class;

public:
    pfs_constexpr duration () : base_class() {}

    template <typename Rep2>
    pfs_constexpr explicit duration (Rep2 const & r)
        : base_class(r)
    {}

    template <typename Rep2, typename Period2>
    pfs_constexpr duration (duration<Rep2, Period2> const & d)
        : base_class(d)
    {}

    duration & operator = (duration const & rhs)
    {
        base_class::operator = (rhs);
        return *this;
    }
};

template <typename Clock, typename Duration = typename Clock::duration>
class time_point : public ::boost::chrono::time_point<Clock, Duration>
{
    typedef ::boost::chrono::time_point<Clock, Duration> base_class;

public:
    pfs_constexpr time_point () : base_class() {}
    
    pfs_constexpr explicit time_point (Duration const & d)
        : base_class(d)
    {}

    template <typename Duration2>
    pfs_constexpr time_point (time_point<Clock, Duration2> const & t)
        : base_class(t)
    {}
};


typedef duration<int64_t, nano> 	    nanoseconds;
typedef duration<int64_t, micro> 	    microseconds;
typedef duration<int64_t, milli> 	    milliseconds;
typedef duration<int64_t> 		        seconds;
typedef duration<int64_t, ratio< 60> >  minutes;
typedef duration<int64_t, ratio<3600> > hours;

}} // pfs::chrono
