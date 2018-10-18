#pragma once
#include <numeric>
#include <pfs/type_traits.hpp>

namespace pfs {

#if __cplusplus < 201103L

template <typename ForwardIt, typename T>
inline void iota (ForwardIt first, ForwardIt last, T value)
{
    while (first != last)
        *first++ = value++;
}

#endif

struct naive_summation
{
    naive_summation () {}

    template <typename InputIt, typename T>
    T summate (InputIt first, InputIt last, T init)
    {
        return std::accumulate<InputIt, T>(first, last, init);
    }
};

struct compensated_summation
{
    compensated_summation () {}

    template <typename InputIt, typename T>
    T summate (InputIt first, InputIt last
            , typename pfs::enable_if<pfs::is_integral<T>::value, T>::type init)
    {
        return std::accumulate<InputIt, T>(first, last, init);
    }

    //
    // https://www.drdobbs.com/floating-point-summation
    //
    template <typename InputIt, typename T>
    T summate (InputIt first, InputIt last
            , typename pfs::enable_if<pfs::is_floating_point<T>::value, T>::type init)
    {
        double result = static_cast<double>(init);
        double correction = 0;

        for (; first != last; ++first) {
            double corrected_next_term = *first - correction;
            double new_result = result + corrected_next_term;
            correction = (new_result - result) - corrected_next_term;
            result = new_result;
        }

        return result;
    }
};

typedef compensated_summation kahan_summation;

template <typename SummationPolicy, typename InputIt, typename T>
inline T summate (InputIt first, InputIt last, T init)
{
    return SummationPolicy().summate<InputIt, T>(first, last, init);
}

template <typename InputIt, typename T>
inline T summate (InputIt first, InputIt last, T init)
{
    return summate<naive_summation, InputIt, T>(first, last, init);
}

} // pfs
