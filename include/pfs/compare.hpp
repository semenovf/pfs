#pragma once

namespace pfs {

template <typename T, typename U = T>
struct compare_op
{
    // TODO Implement
//     friend bool operator != (T const & a, U const & b)
//     {
//         return !(a == b);
//     }
//
//     friend bool operator <= (T const & a, U const & b)
//     {
//         return !(b < a);
//     }
//
//     friend bool operator > (T const & a, U const & b)
//     {
//         return b < a;
//     }
//
//     friend bool operator >= (T const & a, U const & b)
//     {
//         return !(a < b);
//     }
};

template <typename T>
struct compare_op<T,T>
{
    friend bool operator != (T const & a, T const & b)
    {
        return !(a == b);
    }

    friend bool operator <= (T const & a, T const & b)
    {
        return !(b < a);
    }

    friend bool operator > (T const & a, T const & b)
    {
        return b < a;
    }

    friend bool operator >= (T const & a, T const & b)
    {
        return !(a < b);
    }
};

//
// Applicable inside pfs namespace
//

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

} // namespace pfs
