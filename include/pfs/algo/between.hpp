/* 
 * File:   between.hpp
 * Author: wladt
 *
 * Created on February 18, 2017, 12:36 PM
 */

#ifndef __PFS_ALGO_BETWEEN_HPP__
#define __PFS_ALGO_BETWEEN_HPP__

namespace pfs {

template <typename T, typename LessEq>
inline bool between_inclusive (T const & value
        , T const & min_val
        , T const & max_val)
{
    return LessEq()(min_val, value) && LessEq()(value, max_val);
}

template <typename T>
inline bool between_inclusive (T const & value
        , T const & min_val
        , T const & max_val)
{
    return (min_val <= value) && (value <= max_val);
}

template <typename T, typename Less>
inline bool between_exclusive (T const & value
        , T const & min_val
        , T const & max_val)
{
    return Less()(min_val, value) && Less()(value, max_val);
}

template <typename T>
inline bool between_exclusive (T const & value
        , T const & min_val
        , T const & max_val)
{
    return min_val < value && value < max_val;
}

template <typename T, typename Less, typename LessEq>
inline bool between_exclusive_min (T const & value
        , T const & min_val
        , T const & max_val)
{
    return Less()(min_val, value) && LessEq()(value, max_val);
}

template <typename T>
inline bool between_exclusive_min (T const & value
        , T const & min_val
        , T const & max_val)
{
    return min_val < value && value <= max_val;
}

template <typename T, typename Less, typename LessEq>
inline bool between_exclusive_max (T const & value
        , T const & min_val
        , T const & max_val)
{
    return LessEq()(min_val, value) && Less()(value, max_val);
}

template <typename T>
inline bool between_exclusive_max (T const & value
        , T const & min_val
        , T const & max_val)
{
    return min_val <= value && value < max_val;
}

} // pfs

#endif /* __PFS_ALGO_BETWEEN_HPP__ */

