namespace pfs {

template <typename T>
inline typename unique_ptr<T>::rval make_unique() { return typename unique_ptr<T>::rval(new T); }

template <class T, class Arg1>
inline typename unique_ptr<T>::rval make_unique(Arg1 a1) { return typename unique_ptr<T>::rval(new T(a1)); }

template <class T, class Arg1, class Arg2>
inline typename unique_ptr<T>::rval make_unique(Arg1 a1, Arg2 a2) { return typename unique_ptr<T>::rval(new T(a1, a2)); }

template <class T, class Arg1, class Arg2, class Arg3>
inline typename unique_ptr<T>::rval make_unique(Arg1 a1, Arg2 a2, Arg3 a3) { return typename unique_ptr<T>::rval(new T(a1, a2, a3)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
inline typename unique_ptr<T>::rval make_unique(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) { return typename unique_ptr<T>::rval(new T(a1, a2, a3, a4)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline typename unique_ptr<T>::rval make_unique(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) { return typename unique_ptr<T>::rval(new T(a1, a2, a3, a4, a5)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline typename unique_ptr<T>::rval make_unique(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) { return typename unique_ptr<T>::rval(new T(a1, a2, a3, a4, a5, a6)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline typename unique_ptr<T>::rval make_unique(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7) { return typename unique_ptr<T>::rval(new T(a1, a2, a3, a4, a5, a6, a7)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline typename unique_ptr<T>::rval make_unique(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) { return typename unique_ptr<T>::rval(new T(a1, a2, a3, a4, a5, a6, a7, a8));}

} // namespace pfs

