#pragma once
#include <pfs/types.hpp>

namespace pfs {

template <typename T, T v>
struct integral_constant
{
    static T const value = v;

    typedef T value_type;

    typedef integral_constant<T,v> type;

    operator value_type () const { return value; }
};

//template <typename T, T v>
//T integral_constant<T, v>::value;

//template <bool v>
//struct integral_constant<bool, v>
//{
//    static bool const value = v;
//
//    typedef bool value_type;
//
//    typedef integral_constant<bool, v> type;
//
//    operator value_type () const { return value; }
//};

typedef integral_constant<bool, true> true_type;

typedef integral_constant<bool, false> false_type;

template <bool, typename T1, typename>
struct conditional
{
    typedef T1 type;
};

template <typename T1, typename T2>
struct conditional<false, T1, T2>
{
    typedef T2 type;
};

namespace details {

template <typename T1, typename T2>
struct and_op2
    : public conditional<T1::value, T2, T1>::type
{ };

template <typename T>
struct not_op
    : public integral_constant<bool, !T::value>
{ };

} // details

template<typename T>
struct remove_const
{
    typedef T type;
};

template<typename T>
struct remove_const<T const>
{
    typedef T type;
};

template<typename T>
struct remove_volatile
{
    typedef T type;
};

template<typename T>
struct remove_volatile<T volatile>
{
    typedef T type;
};

template<typename T>
struct remove_cv
{
	typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};


template <typename T>
struct remove_reference
{
    typedef T type;
};

template <typename T>
struct remove_reference<T &>
{
    typedef T type;
};

template <typename T>
struct remove_pointer
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T *>
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T const *>
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T volatile *>
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T const volatile *>
{
    typedef T type;
};

//struct true_type
//{
//	enum { value = 1 };
//};
//
//struct false_type
//{
//	enum { value = 0 };
//};

// Compare for equality of types.
template <typename, typename>
struct is_same
    : public false_type
{ };

template <typename T>
struct is_same<T, T>
    : public true_type
{ };

template <bool, typename>
struct enable_if
{};

template <typename T>
struct enable_if<true, T>
{
	typedef T type;
};

template <typename T>
struct is_void
    : public is_same<void, typename remove_cv<T>::type>
{};

template <typename T>
struct is_bool
    : public is_same<bool, typename remove_cv<T>::type>
{};

namespace details {

template <typename>
struct is_integral
    : public false_type
{ };

template <>
struct is_integral<bool>
    : public true_type
{ };

template <>
struct is_integral<char>
    : public true_type
{ };

template <>
struct is_integral<signed char>
    : public true_type
{ };

template <>
struct is_integral<unsigned char>
    : public true_type
{ };

template <>
struct is_integral<wchar_t>
    : public true_type { };


template <>
struct is_integral<short>
    : public true_type
{ };

template <>
struct is_integral<unsigned short>
    : public true_type
{ };

template <>
struct is_integral<int>
    : public true_type
{ };

template <>
struct is_integral<unsigned int>
    : public true_type
{ };

template <>
struct is_integral<long>
    : public true_type
{ };

template <>
struct is_integral<unsigned long>
    : public true_type
{ };

#if PFS_HAVE_LONGLONG

template <>
struct is_integral<long long>
    : public true_type
{ };

template <>
struct is_integral<unsigned long long>
    : public true_type
{ };

#endif // PFS_HAVE_LONGLONG

} // details

template <typename T>
struct is_integral
    : public details::is_integral<typename remove_cv<T>::type>::type
{ };

namespace details {

template <typename>
struct is_floating_point
    : public false_type
{ };

template <>
struct is_floating_point<float>
    : public true_type
{ };

template <>
struct is_floating_point<double>
    : public true_type
{ };

#if PFS_HAVE_LONG_DOUBLE

template <>
struct is_floating_point<long double>
    : public true_type
{ };

#endif

} // details

template <typename T>
struct is_floating_point
    : public details::is_floating_point<typename remove_cv<T>::type>::type
{ };

/**
 * If T is an arithmetic type (that is, an integral type or a floating-point type),
 * provides the member constant value equal @c true. For any other type, value is @c false.
 */
template <typename T>
struct is_arithmetic
    : public integral_constant<bool
        , is_integral<T>::value
        || is_floating_point<T>::value>
{ };

/**
 * If T is a fundamental type (that is, arithmetic type, void),
 * provides the member constant value equal @c true.
 * For any other type, value is @c false.
 */
template <typename T>
struct is_fundamental
    : public integral_constant<bool
        , is_arithmetic<T>::value
        || is_void<T>::value>
{};


namespace details {

template <typename>
struct is_unsigned
    : public false_type
{ };

template <>
struct is_unsigned<bool>
    : public true_type
{ };

template <>
struct is_unsigned<unsigned char>
    : public true_type
{ };

template <>
struct is_unsigned<unsigned short>
    : public true_type
{ };

template <>
struct is_unsigned<unsigned int>
    : public true_type
{ };

template <>
struct is_unsigned<unsigned long>
    : public true_type
{ };

#if PFS_HAVE_LONGLONG

template <>
struct is_unsigned<unsigned long long>
    : public true_type
{ };

#endif

} // details

template <typename T>
struct is_unsigned
    : public details::is_unsigned<typename remove_cv<T>::type>::type
{ };

template <typename T>
struct is_signed
    : public details::and_op2<is_arithmetic<T>, details::not_op<is_unsigned<T> > >::type
{ };


template <typename T>
struct is_const
    : public false_type
{};

template <typename T>
struct is_const<const T>
    : public true_type
{};

template <typename>
struct is_volatile
    : public false_type
{};

template <typename T>
struct is_volatile<T volatile>
    : public true_type
{};

template <typename>
struct is_reference : public false_type
{};

template <typename T>
struct is_reference<T &> : public true_type
{};

// is_function

template <typename>
struct is_function : false_type {};

template <typename R>
struct is_function<R()> : true_type {};

template <typename R, typename A1>
struct is_function<R(A1)> : true_type {};

template <typename R, typename A1, typename A2>
struct is_function<R(A1, A2)> : true_type {};

template <typename R, typename A1, typename A2, typename A3>
struct is_function<R(A1, A2, A3)> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4>
struct is_function<R(A1, A2, A3, A4)> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct is_function<R(A1, A2, A3, A4, A5)> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct is_function<R(A1, A2, A3, A4, A5, A6)> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct is_function<R(A1, A2, A3, A4, A5, A6, A7)> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
struct is_function<R(A1, A2, A3, A4, A5, A6, A7, A8)> : true_type {};

// cost
template <typename R>
struct is_function<R() const> : true_type {};

template <typename R, typename A1>
struct is_function<R(A1) const> : true_type {};

template <typename R, typename A1, typename A2>
struct is_function<R(A1, A2) const> : true_type {};

template <typename R, typename A1, typename A2, typename A3>
struct is_function<R(A1, A2, A3) const> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4>
struct is_function<R(A1, A2, A3, A4) const> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct is_function<R(A1, A2, A3, A4, A5) const> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct is_function<R(A1, A2, A3, A4, A5, A6) const> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct is_function<R(A1, A2, A3, A4, A5, A6, A7) const> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
struct is_function<R(A1, A2, A3, A4, A5, A6, A7, A8) const> : true_type {};

// volatile
template <typename R>
struct is_function<R() volatile> : true_type {};

template <typename R, typename A1>
struct is_function<R(A1) volatile> : true_type {};

template <typename R, typename A1, typename A2>
struct is_function<R(A1, A2) volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3>
struct is_function<R(A1, A2, A3) volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4>
struct is_function<R(A1, A2, A3, A4) volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct is_function<R(A1, A2, A3, A4, A5) volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct is_function<R(A1, A2, A3, A4, A5, A6) volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct is_function<R(A1, A2, A3, A4, A5, A6, A7) volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
struct is_function<R(A1, A2, A3, A4, A5, A6, A7, A8) volatile> : true_type {};

// const volatile
template <typename R>
struct is_function<R() const volatile> : true_type {};

template <typename R, typename A1>
struct is_function<R(A1) const volatile> : true_type {};

template <typename R, typename A1, typename A2>
struct is_function<R(A1, A2) const volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3>
struct is_function<R(A1, A2, A3) const volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4>
struct is_function<R(A1, A2, A3, A4) const volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct is_function<R(A1, A2, A3, A4, A5) const volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct is_function<R(A1, A2, A3, A4, A5, A6) const volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct is_function<R(A1, A2, A3, A4, A5, A6, A7) const volatile> : true_type {};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
struct is_function<R(A1, A2, A3, A4, A5, A6, A7, A8) const volatile> : true_type {};

template <typename T>
struct make_unsigned;

template <> struct make_unsigned<char> { typedef unsigned char type; };
template <> struct make_unsigned<char const> { typedef unsigned char const type; };
template <> struct make_unsigned<char volatile> { typedef unsigned char volatile type; };
template <> struct make_unsigned<char const volatile> { typedef unsigned char const volatile type; };
template <> struct make_unsigned<signed char> { typedef unsigned char type; };
template <> struct make_unsigned<signed char const> { typedef unsigned char const type; };
template <> struct make_unsigned<signed char volatile> { typedef unsigned char volatile type; };
template <> struct make_unsigned<signed char const volatile> { typedef unsigned char const volatile type; };
template <> struct make_unsigned<unsigned char> { typedef unsigned char type; };
template <> struct make_unsigned<unsigned char const> { typedef unsigned char const type; };
template <> struct make_unsigned<unsigned char volatile> { typedef unsigned char volatile type; };
template <> struct make_unsigned<unsigned char const volatile> { typedef unsigned char const volatile type; };

template <> struct make_unsigned<short> { typedef unsigned short type; };
template <> struct make_unsigned<short const> { typedef unsigned short const type; };
template <> struct make_unsigned<short volatile> { typedef unsigned short volatile type; };
template <> struct make_unsigned<short const volatile> { typedef unsigned short const volatile type; };
template <> struct make_unsigned<unsigned short> { typedef unsigned short type; };
template <> struct make_unsigned<unsigned short const> { typedef unsigned short const type; };
template <> struct make_unsigned<unsigned short volatile> { typedef unsigned short volatile type; };
template <> struct make_unsigned<unsigned short const volatile> { typedef unsigned short const volatile type; };

template <> struct make_unsigned<int> { typedef unsigned int type; };
template <> struct make_unsigned<int const> { typedef unsigned int const type; };
template <> struct make_unsigned<int volatile> { typedef unsigned int volatile type; };
template <> struct make_unsigned<int const volatile> { typedef unsigned int const volatile type; };
template <> struct make_unsigned<unsigned int> { typedef unsigned int type; };
template <> struct make_unsigned<unsigned int const> { typedef unsigned int const type; };
template <> struct make_unsigned<unsigned int volatile> { typedef unsigned int volatile type; };
template <> struct make_unsigned<unsigned int const volatile> { typedef unsigned int const volatile type; };

template <> struct make_unsigned<long> { typedef unsigned long type; };
template <> struct make_unsigned<long const> { typedef unsigned long const type; };
template <> struct make_unsigned<long volatile> { typedef unsigned long volatile type; };
template <> struct make_unsigned<long const volatile> { typedef unsigned long const volatile type; };
template <> struct make_unsigned<unsigned long> { typedef unsigned long type; };
template <> struct make_unsigned<unsigned long const> { typedef unsigned long const type; };
template <> struct make_unsigned<unsigned long volatile> { typedef unsigned long volatile type; };
template <> struct make_unsigned<unsigned long const volatile> { typedef unsigned long const volatile type; };

#if PFS_HAVE_LONGLONG
template <> struct make_unsigned<long long> { typedef unsigned long long type; };
template <> struct make_unsigned<long long const> { typedef unsigned long long const type; };
template <> struct make_unsigned<long long volatile> { typedef unsigned long long volatile type; };
template <> struct make_unsigned<long long const volatile> { typedef unsigned long long const volatile type; };
template <> struct make_unsigned<unsigned long long> { typedef unsigned long long type; };
template <> struct make_unsigned<unsigned long long const> { typedef unsigned long long const type; };
template <> struct make_unsigned<unsigned long long volatile> { typedef unsigned long long volatile type; };
template <> struct make_unsigned<unsigned long long const volatile> { typedef unsigned long long const volatile type; };
#endif

template <typename T>
struct make_signed;

template <> struct make_signed<char> { typedef signed char type; };
template <> struct make_signed<char const> { typedef signed char const type; };
template <> struct make_signed<char volatile> { typedef signed char volatile type; };
template <> struct make_signed<char const volatile> { typedef signed char const volatile type; };
template <> struct make_signed<signed char> { typedef signed char type; };
template <> struct make_signed<signed char const> { typedef signed char const type; };
template <> struct make_signed<signed char volatile> { typedef signed char volatile type; };
template <> struct make_signed<signed char const volatile> { typedef signed char const volatile type; };
template <> struct make_signed<unsigned char> { typedef signed char type; };
template <> struct make_signed<unsigned char const> { typedef signed char const type; };
template <> struct make_signed<unsigned char volatile> { typedef signed char volatile type; };
template <> struct make_signed<unsigned char const volatile> { typedef signed char const volatile type; };

template <> struct make_signed<short> { typedef signed short type; };
template <> struct make_signed<short const> { typedef signed short const type; };
template <> struct make_signed<short volatile> { typedef signed short volatile type; };
template <> struct make_signed<short const volatile> { typedef signed short const volatile type; };
template <> struct make_signed<unsigned short> { typedef signed short type; };
template <> struct make_signed<unsigned short const> { typedef signed short const type; };
template <> struct make_signed<unsigned short volatile> { typedef signed short volatile type; };
template <> struct make_signed<unsigned short const volatile> { typedef signed short const volatile type; };

template <> struct make_signed<int> { typedef signed int type; };
template <> struct make_signed<int const> { typedef signed int const type; };
template <> struct make_signed<int volatile> { typedef signed int volatile type; };
template <> struct make_signed<int const volatile> { typedef signed int const volatile type; };
template <> struct make_signed<unsigned int> { typedef signed int type; };
template <> struct make_signed<unsigned int const> { typedef signed int const type; };
template <> struct make_signed<unsigned int volatile> { typedef signed int volatile type; };
template <> struct make_signed<unsigned int const volatile> { typedef signed int const volatile type; };

template <> struct make_signed<long> { typedef signed long type; };
template <> struct make_signed<long const> { typedef signed long const type; };
template <> struct make_signed<long volatile> { typedef signed long volatile type; };
template <> struct make_signed<long const volatile> { typedef signed long const volatile type; };
template <> struct make_signed<unsigned long> { typedef signed long type; };
template <> struct make_signed<unsigned long const> { typedef signed long const type; };
template <> struct make_signed<unsigned long volatile> { typedef signed long volatile type; };
template <> struct make_signed<unsigned long const volatile> { typedef signed long const volatile type; };

#if PFS_HAVE_LONGLONG
template <> struct make_signed<long long> { typedef signed long long type; };
template <> struct make_signed<long long const> { typedef signed long long const type; };
template <> struct make_signed<long long volatile> { typedef signed long long volatile type; };
template <> struct make_signed<long long const volatile> { typedef signed long long const volatile type; };
template <> struct make_signed<unsigned long long> { typedef signed long long type; };
template <> struct make_signed<unsigned long long const> { typedef signed long long const type; };
template <> struct make_signed<unsigned long long volatile> { typedef signed long long volatile type; };
template <> struct make_signed<unsigned long long const volatile> { typedef signed long long const volatile type; };
#endif

} // pfs
