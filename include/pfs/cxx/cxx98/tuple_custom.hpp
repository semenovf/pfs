#pragma once
#include <pfs/types.hpp>

namespace pfs {

template <typename A1 = void
        , typename A2 = void
        , typename A3 = void
        , typename A4 = void
        , typename A5 = void
        , typename A6 = void
        , typename A7 = void
        , typename A8 = void
        , typename A9 = void
        , typename A10 = void>
struct tuple;
//{
//     tuple (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) : _a1(a1), _a2(a2), _a3(a3), _a4(a4), _a5(a5), _a6(a6), _a7(a7), _a8(a8), _a9(a9) {}
//     static const int count () { return 9; }
//     A1 _a1; A2 _a2; A3 _a3; A4 _a4; A5 _a5; A6 _a6; A7 _a7; A8 _a8; A9 _a9;
//};

template <size_t N
        , typename A1 = void
        , typename A2 = void
        , typename A3 = void
        , typename A4 = void
        , typename A5 = void
        , typename A6 = void
        , typename A7 = void
        , typename A8 = void
        , typename A9 = void>
struct tuple_element;

///////////////////////////////////////////////////////////////////////////
// tuple_getter and specializations                                      //
///////////////////////////////////////////////////////////////////////////

template <size_t N, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter
{
    typename tuple_element<N, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> &);

    typename tuple_element<N, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const &) const;

};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<0, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a1;
    }

    typename tuple_element<0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a1;
    }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<1, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<1, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a2;
    }

    typename tuple_element<1, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a2;
    }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<2, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<2, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a3;
    }

    typename tuple_element<2, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a3;
    }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<3, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<3, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a4;
    }

    typename tuple_element<3, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a4;
    }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<4, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<4, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a5;
    }

    typename tuple_element<4, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a5;
    }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<5, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<5, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a6;
    }

    typename tuple_element<5, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a6;
    }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<6, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<6, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a7;
    }

    typename tuple_element<6, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a7;
    }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<7, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<7, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a8;
    }

    typename tuple_element<7, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a8;
    }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_getter<8, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typename tuple_element<8, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & operator () (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
    {
        return t._a9;
    }

    typename tuple_element<8, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & operator () (
            tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t) const
    {
        return t._a9;
    }
};

///////////////////////////////////////////////////////////////////////////
// get (tuple &)                                                         //
// get (tuple const &)                                                   //
///////////////////////////////////////////////////////////////////////////

template <size_t N, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
inline typename tuple_element<N, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type & get (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> & t)
{
    return tuple_getter<N, A1, A2, A3, A4, A5, A6, A7, A8, A9>()(t);
}

template <size_t N, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
inline typename tuple_element<N, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type const & get (
        tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9> const & t)
{
    return tuple_getter<N, A1, A2, A3, A4, A5, A6, A7, A8, A9>()(t);
}

///////////////////////////////////////////////////////////////////////////
// tuple element specializations                                         //
///////////////////////////////////////////////////////////////////////////

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<0, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A1 type;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<1, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A2 type;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<2, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A3 type;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<3, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A4 type;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<4, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A5 type;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<5, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A6 type;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<6, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A7 type;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<7, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A8 type;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple_element<8, A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    typedef A9 type;
};

///////////////////////////////////////////////////////////////////////////
// tuple specializations                                                 //
///////////////////////////////////////////////////////////////////////////

template <>
struct tuple<>
{
    static int const SIZE = 0;
    static const int count () { return 0; }
};

template <typename A1>
struct tuple<A1>
{
    static int const SIZE = 1;
    tuple (A1 a1) : _a1(a1) {}
    static const int count () { return 1; }
    A1 _a1;
};

template <typename A1, typename A2>
struct tuple<A1, A2>
{
    static int const SIZE = 2;
    tuple (A1 a1, A2 a2) : _a1(a1), _a2(a2) {}
    static const int count () { return 2; }
    A1 _a1; A2 _a2;
};

template <typename A1, typename A2, typename A3>
struct tuple<A1, A2, A3>
{
    static int const SIZE = 3;
    tuple (A1 a1, A2 a2, A3 a3) : _a1(a1), _a2(a2), _a3(a3) {}
    static const int count () { return 3; }
    A1 _a1; A2 _a2; A3 _a3;
};

template <typename A1, typename A2, typename A3, typename A4>
struct tuple<A1, A2, A3, A4>
{
    static int const SIZE = 4;
    tuple (A1 a1, A2 a2, A3 a3, A4 a4) : _a1(a1), _a2(a2), _a3(a3), _a4(a4) {}
    static const int count () { return 4; }
    A1 _a1; A2 _a2; A3 _a3; A4 _a4;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5>
struct tuple<A1, A2, A3, A4, A5>
{
    static int const SIZE = 5;
    tuple (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) : _a1(a1), _a2(a2), _a3(a3), _a4(a4), _a5(a5) {}
    static const int count () { return 5; }
    A1 _a1; A2 _a2; A3 _a3; A4 _a4; A5 _a5;;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct tuple<A1, A2, A3, A4, A5, A6>
{
    static int const SIZE = 6;
    tuple (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) : _a1(a1), _a2(a2), _a3(a3), _a4(a4), _a5(a5), _a6(a6) {}
    static const int count () { return 6; }
    A1 _a1; A2 _a2; A3 _a3; A4 _a4; A5 _a5; A6 _a6;;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct tuple<A1, A2, A3, A4, A5, A6, A7>
{
    static int const SIZE = 7;
    tuple (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) : _a1(a1), _a2(a2), _a3(a3), _a4(a4), _a5(a5), _a6(a6), _a7(a7) {}
    static const int count () { return 7; }
    A1 _a1; A2 _a2; A3 _a3; A4 _a4; A5 _a5; A6 _a6; A7 _a7;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
struct tuple<A1, A2, A3, A4, A5, A6, A7, A8>
{
    static int const SIZE = 8;
    tuple (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) : _a1(a1), _a2(a2), _a3(a3), _a4(a4), _a5(a5), _a6(a6), _a7(a7), _a8(a8) {}
    static const int count () { return 8; }
    A1 _a1; A2 _a2; A3 _a3; A4 _a4; A5 _a5; A6 _a6; A7 _a7; A8 _a8;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct tuple<A1, A2, A3, A4, A5, A6, A7, A8, A9>
{
    static int const SIZE = 9;
    tuple (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) : _a1(a1), _a2(a2), _a3(a3), _a4(a4), _a5(a5), _a6(a6), _a7(a7), _a8(a8), _a9(a9) {}
    static const int count () { return 9; }
    A1 _a1; A2 _a2; A3 _a3; A4 _a4; A5 _a5; A6 _a6; A7 _a7; A8 _a8; A9 _a9;
};

template <typename T>
struct tuple_size
{
    static int const value = T::SIZE;
};

} // namespace pfs

#include "tuple_ext.hpp"
