#pragma once

namespace pfs {

template <typename T, typename LessOrEqual>
inline bool between_inclusive (T const & value
        , T const & min_val
        , T const & max_val)
{
    return LessOrEqual()(min_val, value) && LessOrEqual()(value, max_val);
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

template <typename T, typename Less, typename LessOrEqual>
inline bool between_exclusive_min (T const & value
        , T const & min_val
        , T const & max_val)
{
    return Less()(min_val, value) && LessOrEqual()(value, max_val);
}

template <typename T>
inline bool between_exclusive_min (T const & value
        , T const & min_val
        , T const & max_val)
{
    return min_val < value && value <= max_val;
}

template <typename T, typename Less, typename LessOrEqual>
inline bool between_exclusive_max (T const & value
        , T const & min_val
        , T const & max_val)
{
    return LessOrEqual()(min_val, value) && Less()(value, max_val);
}

template <typename T>
inline bool between_exclusive_max (T const & value
        , T const & min_val
        , T const & max_val)
{
    return min_val <= value && value < max_val;
}

} // pfs
