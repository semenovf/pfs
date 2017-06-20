/* 
 * File:   chrono.hpp
 * Author: wladt
 *
 * Created on June 19, 2017, 9:05 AM
 */

#ifndef __PFS_CXX_CXX98_CHRONO_HPP__
#define __PFS_CXX_CXX98_CHRONO_HPP__

#include <pfs/types.hpp>
#include <pfs/type_traits.hpp>
#include "ratio.hpp"

namespace pfs {
namespace chrono {

template <typename Rep>
struct duration_values
{
    static Rep zero ()
    {
        return Rep(0);
    }

    static Rep max ()
    {
        return numeric_limits<Rep>::max();
    }

    static Rep min ()
    {
        return numeric_limits<Rep>::lowest();
    }
};

template <typename Rep, typename Period>
struct duration
{
    typedef Rep    rep_type;
    typedef Period period_type;

//    static_assert (!__is_duration<_Rep>::value, "rep cannot be a duration");
//    static_assert (__is_ratio<_Period>::value,
//            "period must be a specialization of ratio");
//    static_assert (_Period::num > 0, "period must be positive");
//
    duration ()
    {}

//    duration (duration const & rhs)
//        : _r(rhs._r)
//    {}

//    template <typename Rep2, typename = typename enable_if<is_convertible<Rep2, rep_type>::value
//            && (treat_as_floating_point<rep_type>::value
//            || !treat_as_floating_point<Rep2>::value)>::type>
    template <typename Rep2>
    explicit duration (Rep2 const & rep)
        : _r(static_cast<rep_type>(rep))
    {}
    
//    template<typename _Rep2, typename _Period2, typename = typename
//    enable_if<treat_as_floating_point<rep>::value
//    || (ratio_divide<_Period2, period>::den == 1
//            && !treat_as_floating_point<_Rep2>::value)>::type>
    template <typename Rep2, typename Period2>
    duration (duration<Rep2, Period2> const & d)
        : _r(d.count())
    {}

    ~duration ()
    {}
    
    duration & operator = (duration const & rhs)
    {
        _r = rhs._r;
        return *this;
    }

    rep_type count () const
    {
        return _r;
    }

    duration operator + () const
    {
        return *this;
    }

    duration operator- () const
    {
        return duration(-_r);
    }

    duration & operator ++ ()
    {
        ++_r;
        return *this;
    }

    duration operator ++ (int)
    {
        return duration(_r++);
    }

    duration & operator -- ()
    {
        --_r;
        return *this;
    }

    duration operator -- (int)
    {
        return duration(_r--);
    }

    duration & operator += (duration const & d)
    {
        _r += d.count();
        return *this;
    }

    duration & operator -= (duration const & d)
    {
        _r -= d.count();
        return *this;
    }

    duration & operator *= (rep_type const & rhs)
    {
        _r *= rhs;
        return *this;
    }

    duration & operator /= (rep_type const & rhs)
    {
        _r /= rhs;
        return *this;
    }

//    template<typename _Rep2 = rep>
//    typename enable_if<!treat_as_floating_point<_Rep2>::value,
//    duration&>::type
    duration & operator %= (rep_type const & rhs)
    {
        _r %= rhs;
        return *this;
    }

//    template<typename _Rep2 = rep>
//    typename enable_if<!treat_as_floating_point<_Rep2>::value,
//    duration&>::type
    duration & operator %= (duration const & d)
    {
        _r %= d.count();
        return *this;
    }

    static duration zero ()
    {
        return duration(duration_values<rep_type>::zero());
    }

    static duration min ()
    {
        return duration(duration_values<rep_type>::min());
    }

    static duration max ()
    {
        return duration(duration_values<rep_type>::max());
    }

private:
    rep_type _r;
};

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline duration<Rep1, Period1> operator + (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return lhs.count() + rhs.count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline duration<Rep1, Period1> operator - (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return lhs.count() - rhs.count();
}

template <typename Rep1, typename Period, typename Rep2>
inline duration<Rep1, Period> operator * (duration<Rep1, Period> const & d
        , Rep2 const & n)
{
    return d.count() * n;
}

template <typename Rep1, typename Period, typename Rep2>
inline duration<Rep1, Period> operator * (Rep2 const & n
        , duration<Rep1, Period> const & d)
{
    return d * n;
}

template <typename Rep1, typename Period, typename Rep2>
inline duration<Rep1, Period> operator / (duration<Rep1, Period> const & d
        , Rep2 const & n)
{
    return d.count() / n;
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline duration<Rep1, Period1> operator / (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return lhs.count() / rhs.count();
}

template <typename Rep1, typename Period, typename Rep2>
inline duration<Rep1, Period> operator % (duration<Rep1, Period> const & d
        , Rep2 const & n)
{
    return d.count() % n;
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline duration<Rep1, Period1> operator % (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return lhs.count() % rhs.count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline bool operator == (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return lhs.count() == rhs.count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline bool operator < (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return lhs.count() < rhs.count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline bool operator != (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return !(lhs == rhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline bool operator <= (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return !(rhs < lhs);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline bool operator > (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return rhs < lhs;
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
inline bool operator >= (duration<Rep1, Period1> const & lhs
        , duration<Rep2, Period2> const & rhs)
{
    return !(lhs < rhs);
}

typedef duration<int64_t, nano> 	    nanoseconds;
typedef duration<int64_t, micro> 	    microseconds;
typedef duration<int64_t, milli> 	    milliseconds;
typedef duration<int64_t, ratio<1,1> > 	seconds;
typedef duration<int64_t, ratio<60> >   minutes;
typedef duration<int64_t, ratio<3600> > hours;

//template <typename Clock, typename Duration = typename Clock::duration>
//using time_point = std::chrono::time_point<Clock, Duration>;
//

}} // pfs::chrono

#endif /* __PFS_CXX_CXX98_CHRONO_HPP__ */

