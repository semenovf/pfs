#pragma once
#include <pfs/compare.hpp>

namespace pfs {

template <typename T, typename U>
inline bool operator == (shared_ptr<T> const & a, shared_ptr<U> const & b)
{
     return a.get() == b.get();
}

template <typename T, typename U>
inline bool operator < (shared_ptr<T> const & a, shared_ptr<U> const & b)
{
    return a.get() < b.get();
}

template <class T, class T1>
inline shared_ptr<T> static_pointer_cast (const shared_ptr<T1> & r)
{
    return shared_ptr<T>(r, static_cast<T*>(r.get()));
}

template <class T, class T1>
inline shared_ptr<T> dynamic_pointer_cast (const shared_ptr<T1> & r)
{
    T * p = dynamic_cast<T*>(r.get());
    if (p)
        return shared_ptr<T>(r, p);
    return shared_ptr<T>();
}

template <class T, class T1>
inline shared_ptr<T> const_pointer_cast (const shared_ptr<T1> & r)
{
    return shared_ptr<T>(r, const_cast<T*>(r.get()));
}

template <class T, class T1>
inline shared_ptr<T> reinterpret_pointer_cast (const shared_ptr<T1> & r)
{
    return shared_ptr<T>(r, reinterpret_cast<T*>(r.get()));
}

template <typename T>
inline shared_ptr<T> make_shared () { return shared_ptr<T>(new T); }

template <class T, class Arg1>
inline shared_ptr<T> make_shared (Arg1 a1) { return shared_ptr<T>(new T(a1)); }

template <class T, class Arg1, class Arg2>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2) { return shared_ptr<T>(new T(a1, a2)); }

template <class T, class Arg1, class Arg2, class Arg3>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3) { return shared_ptr<T>(new T(a1, a2, a3)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) { return shared_ptr<T>(new T(a1, a2, a3, a4)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8)); }

} // namespace pfs
