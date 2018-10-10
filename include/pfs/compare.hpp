#pragma once

template <typename T, typename U>
inline bool operator != (T const & a, U const & b)
{
    return !(a == b);
}

template <typename T, typename U>
inline bool operator <= (T const & a, U const & b)
{
    return !(b < a);
}

template <typename T, typename U>
inline bool operator > (T const & a, U const & b)
{
    return b < a;
}

template <typename T, typename U>
inline bool operator >= (T const & a, U const & b)
{
    return !(a < b);
}
