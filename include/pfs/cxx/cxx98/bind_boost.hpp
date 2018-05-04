#pragma once
#include <pfs/cxxlang.hpp>
#include <pfs/type_traits.hpp>
#include <boost/bind.hpp>

namespace pfs {

namespace placeholders {

//#include <boost/bind/placeholders.hpp>

} // placeholders

template <typename R, typename F
        , typename A1 = void
        , typename A2 = void
        , typename A3 = void
        , typename A4 = void
        , typename A5 = void
        , typename A6 = void
        , typename A7 = void
        , typename A8 = void
        , typename A9 = void>
struct binder
{
    typedef boost::_bi::bind_t<R, F
            , typename if_else<is_same<A1, void>::value, boost::_bi::list0
                , typename if_else<is_same<A2, void>::value, typename boost::_bi::list_av_1<A1>::type
                    , typename if_else<is_same<A3, void>::value, typename boost::_bi::list_av_2<A1, A2>::type
                        , typename if_else<is_same<A4, void>::value, typename boost::_bi::list_av_3<A1, A2, A3>::type
                            , typename if_else<is_same<A5, void>::value, typename boost::_bi::list_av_4<A1, A2, A3, A4>::type
                                , typename if_else<is_same<A6, void>::value, typename boost::_bi::list_av_5<A1, A2, A3, A4, A5>::type
                                    , typename if_else<is_same<A7, void>::value, typename boost::_bi::list_av_6<A1, A2, A3, A4, A5, A6>::type
                                        , typename if_else<is_same<A8, void>::value, typename boost::_bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type
                                            , typename if_else<is_same<A9, void>::value, typename boost::_bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type
                                                , typename boost::_bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type
                                            >::type
                                        >::type
                                    >::type
                                >::type
                            >::type
                        >::type
                    >::type
                >::type
            >::type> type;
};

template <typename R, typename F>
inline typename binder<R, F>::type bind (F f)
{
    return boost::bind<R, F>(f);
}

template <typename R, typename F
        , typename A1>
inline typename binder<R, F, A1>::type bind (F f, A1 a1)
{
    return boost::bind<R, F, A1>(f, a1);
}

template <typename R, typename F
        , typename A1
        , typename A2>
inline typename binder<R, F, A1, A2>::type bind (F f, A1 a1, A2 a2)
{
    return boost::bind<R, F, A1, A2>(f, a1, a2);
}

template <typename R, typename F
        , typename A1
        , typename A2
        , typename A3>
inline typename binder<R, F, A1, A2, A3>::type bind (F f, A1 a1, A2 a2, A3 a3)
{
    return boost::bind<R, F, A1, A2, A3>(f, a1, a2, a3);
}

template <typename R, typename F
        , typename A1
        , typename A2
        , typename A3
        , typename A4>
inline typename binder<R, F, A1, A2, A3, A4>::type bind (F f, A1 a1, A2 a2, A3 a3, A4 a4)
{
    return boost::bind<R, F, A1, A2, A3, A4>(f, a1, a2, a3, a4);
}

template <typename R, typename F
        , typename A1
        , typename A2
        , typename A3
        , typename A4
        , typename A5>
inline typename binder<R, F, A1, A2, A3, A4, A5>::type bind (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return boost::bind<R, F, A1, A2, A3, A4, A5>(f, a1, a2, a3, a4, a5);
}

template <typename R, typename F
        , typename A1
        , typename A2
        , typename A3
        , typename A4
        , typename A5
        , typename A6>
inline typename binder<R, F, A1, A2, A3, A4, A5, A6>::type bind (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    return boost::bind<R, F, A1, A2, A3, A4, A5, A6>(f, a1, a2, a3, a4, a5, a6);
}

template <typename R, typename F
        , typename A1
        , typename A2
        , typename A3
        , typename A4
        , typename A5
        , typename A6
        , typename A7>
inline typename binder<R, F, A1, A2, A3, A4, A5, A6, A7>::type bind (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    return boost::bind<R, F, A1, A2, A3, A4, A5, A6, A7>(f, a1, a2, a3, a4, a5, a6, a7);
}

template <typename R, typename F
        , typename A1
        , typename A2
        , typename A3
        , typename A4
        , typename A5
        , typename A6
        , typename A7
        , typename A8>
inline typename binder<R, F, A1, A2, A3, A4, A5, A6, A7, A8>::type bind (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    return boost::bind<R, F, A1, A2, A3, A4, A5, A6, A7, A8>(f, a1, a2, a3, a4, a5, a6, a7, a8);
}

template <typename R, typename F
        , typename A1
        , typename A2
        , typename A3
        , typename A4
        , typename A5
        , typename A6
        , typename A7
        , typename A8
        , typename A9>
inline typename binder<R, F, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type bind (F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{
    return boost::bind<R, F, A1, A2, A3, A4, A5, A6, A7, A8, A9>(f, a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

} // namespace pfs

