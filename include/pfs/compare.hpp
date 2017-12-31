#ifndef __PFS_COMPARE_HPP__
#define __PFS_COMPARE_HPP__

namespace pfs {

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

} // pfs

#endif /* __PFS_COMPARE_HPP__ */

