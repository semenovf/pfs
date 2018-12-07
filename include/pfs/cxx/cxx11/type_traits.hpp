#pragma once
#include <type_traits>

namespace pfs {

using false_type = std::false_type;
using true_type = std::true_type;

template <typename T, typename U>
using is_same = std::is_same<T, U>;

template <typename T>
using remove_cv = std::remove_cv<T>;

template <typename T>
using remove_const = std::remove_const<T>;

template <typename T>
using remove_volatile = std::remove_volatile<T>;

template <typename T>
using remove_pointer = std::remove_pointer<T>;

template <bool B, typename T = void>
using enable_if = std::enable_if<B, T>;

template <typename T>
using is_void = std::is_void<T>;

template <typename T>
struct is_bool
    : public is_same<bool, typename remove_cv<T>::type>
{};

template <typename T>
using is_integral = std::is_integral<T>;

template <typename T>
using is_signed = std::is_signed<T>;

template <typename T>
using is_unsigned = std::is_unsigned<T>;

template <typename T>
using is_floating_point = std::is_floating_point<T>;

template <typename T>
using make_unsigned = std::make_unsigned<T>;

template <typename T>
using make_signed = std::make_signed<T>;

template <typename T>
using add_pointer = std::add_pointer<T>;

#if __cplusplus == 201103L
template <typename T>
using add_pointer_t = typename add_pointer<T>::type;
#endif

}
