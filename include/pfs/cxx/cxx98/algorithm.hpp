#pragma once
#include <algorithm>
#include <pfs/cxxlang.hpp>
#include <pfs/utility.hpp>

namespace pfs {

template <typename T>
inline pfs_constexpr T const & min (const T & a, const T & b)
{
    return std::min(a, b);
}

template <typename T, typename Compare>
inline pfs_constexpr T const & min (T const & a, T const & b, Compare comp)
{
    return std::min(a, b, comp);
}

template <typename T>
inline pfs_constexpr T const & max (const T & a, const T & b)
{
    return std::max(a, b);
}

template <typename T, typename Compare>
inline pfs_constexpr T const & max (T const & a, T const & b, Compare comp)
{
    return std::max(a, b, comp);
}

template <typename InputIt, typename OutputIt>
inline OutputIt copy (InputIt first, InputIt last, OutputIt d_first)
{
    return std::copy(first, last, d_first);
}

template <typename RandomIt>
inline void sort (RandomIt first, RandomIt last)
{
    std::sort(first, last);
}

/**
 * The signature ot the comparison function should be equivalent to the following:
 *
 * bool cmp (Type1 const & a, Type2 const & b);
 */
template <typename RandomIt, typename Compare>
inline void sort (RandomIt first, RandomIt last, Compare comp)
{
    std::sort(first, last, comp);
}

template <typename ForwardIt, typename T>
inline void replace (ForwardIt first
            , ForwardIt last
            , T const & old_value
            , T const & new_value)
{
    std::replace(first, last, old_value, new_value);
}

template <typename InputIt, typename UnaryPredicate>
bool all_of (InputIt first, InputIt last, UnaryPredicate p)
{
    for (; first != last; ++first) {
        if (!p(*first)) return false;
    }
    return true;
}

template <typename InputIt, typename UnaryPredicate>
bool any_of (InputIt first, InputIt last, UnaryPredicate p)
{
    for (; first != last; ++first) {
        if (p(*first)) return true;
    }
    return false;
}

template <typename InputIt, typename UnaryPredicate>
bool none_of (InputIt first, InputIt last, UnaryPredicate p)
{
    for (; first != last; ++first) {
        if (p(*first)) return false;
    }
    return true;
}

template <typename ForwardIt>
inline ForwardIt max_element (ForwardIt first, ForwardIt last)
{
    return std::max_element(first, last);
}

template <typename ForwardIt, typename Compare>
inline ForwardIt max_element (ForwardIt first, ForwardIt last, Compare comp)
{
    return std::max_element(first, last, comp);
}

template <typename ForwardIt>
inline ForwardIt min_element (ForwardIt first, ForwardIt last)
{
    return std::min_element(first, last);
}

template <typename ForwardIt, typename Compare>
inline ForwardIt min_element (ForwardIt first, ForwardIt last, Compare comp)
{
    return std::min_element(first, last, comp);
}

#if __cplusplus < 201103L

//
// https://en.cppreference.com/w/cpp/algorithm/minmax_element
//
template <typename ForwardIt>
pfs::pair<ForwardIt,ForwardIt> minmax_element (ForwardIt first, ForwardIt last)
{
    pfs::pair<ForwardIt, ForwardIt> result(first, first);

    if (first == last) return result;
    if (++first == last) return result;

    if (*first < *result.first) {
        result.first = first;
    } else {
        result.second = first;
    }

    while (++first != last) {
        ForwardIt i = first;

        if (++first == last) {
            if (*i < *result.first) result.first = i;
            else if (!(*i < *result.second)) result.second = i;
            break;
        } else {
            if (*first < *i) {
                if (*first < *result.first) result.first = first;
                if (!(*i < *result.second)) result.second = i;
            } else {
                if (*i < *result.first) result.first = i;
                if (!(*first < *result.second)) result.second = first;
            }
        }
    }

    return result;
}

//
// https://en.cppreference.com/w/cpp/algorithm/minmax_element
//
template <typename ForwardIt, typename Compare>
pfs::pair<ForwardIt,ForwardIt> minmax_element (ForwardIt first, ForwardIt last, Compare comp)
{
    pfs::pair<ForwardIt, ForwardIt> result(first, first);

    if (first == last) return result;
    if (++first == last) return result;

    if (comp(*first, *result.first)) {
        result.first = first;
    } else {
        result.second = first;
    }

    while (++first != last) {
        ForwardIt i = first;
        if (++first == last) {
            if (comp(*i, *result.first)) result.first = i;
            else if (!(comp(*i, *result.second))) result.second = i;
            break;
        } else {
            if (comp(*first, *i)) {
                if (comp(*first, *result.first)) result.first = first;
                if (!(comp(*i, *result.second))) result.second = i;
            } else {
                if (comp(*i, *result.first)) result.first = i;
                if (!(comp(*first, *result.second))) result.second = first;
            }
        }
    }

    return result;
}

#endif

} // namespace pfs
