#pragma once
#include <pfs/cxxlang.hpp>
#include <boost/version.hpp>
#include <boost/optional.hpp>

namespace pfs {

typedef boost::none_t nullopt_t;

template <typename T>
class optional : public boost::optional<T>
{
    typedef boost::optional<T> base_class;

public:
    typedef T value_type;

public:
    //
    // C++17 constructors
    //
    pfs_constexpr optional () pfs_noexcept : base_class() {}
    pfs_constexpr optional (nullopt_t none) pfs_noexcept : base_class(none) {}
    pfs_constexpr optional (optional const & rhs) : base_class(rhs) {}

#if __cplusplus >= 201103L
    pfs_constexpr optional (optional && rhs) pfs_noexcept
        : base_class(std::forward<optional>(rhs))
    {}
#endif

    template <typename U>
    explicit optional (optional<U> const & rhs)
        : base_class(rhs)
    {}

#if __cplusplus >= 201103L
    template <typename U>
    explicit optional (optional<U> && rhs)
        : base_class(rhs)
    {}
#endif

//     template <typename... Args>
//     pfs_constexpr explicit optional( std::in_place_t, Args &&... args );
//
//     template <typename U, typename ...Args >
//     pfs_constexpr explicit optional (std::in_place_t,
//                              std::initializer_list<U> ilist,
//                              Args &&... args );
//     template <class U = value_type>
//     pfs_constexpr explicit optional (U && value);


    optional & operator = (nullopt_t none) pfs_noexcept
    {
        base_class::operator = (none);
        return *this;
    }

    optional & operator = (optional const & rhs)
    {
        base_class::operator = (rhs);
        return *this;
    }

#if __cplusplus >= 201103L
    optional & operator = (optional && rhs) pfs_noexcept
    {
        base_class::operator = (std::forward<optional>(rhs));
        return *this;
    }
#endif

#if __cplusplus >= 201103L
    template <typename U = T>
    optional & operator = (U && value)
    {
        base_class::operator = (std::forward<U>(value));
        return *this;
    }
#endif

    template <typename U>
    optional & operator = (optional<U> const & rhs)
    {
        base_class::operator = (rhs);
        return *this;
    }

#if __cplusplus >= 201103L
    template <typename U>
    optional & operator = (optional<U> && rhs)
    {
        base_class::operator = (std::forward<U>(rhs));
        return *this;
    }
#endif

    //
    // Boost specific constructors and copy operators
    //
    optional (T const & v) : base_class(v) {}
    optional & operator = (T const & v)
    {
        base_class::operator = (v);
        return *this;
    }

    pfs_constexpr operator bool () const pfs_noexcept
    {
        return base_class::is_initialized();
    }

    pfs_constexpr bool has_value () const pfs_noexcept
    {
        return bool(*this);
    }

#if __cplusplus >= 201103L
    /**
     * @fn template<typename U>
     *     pfs_constexpr T value_or (U && default_value) const &;
     * @note for >= C++11
     */

    /**
     * @fn template <typename U>
     *     constexpr T value_or (U && default_value) &&;
     * @note for >= C++11
     */
#endif

#if __cplusplus < 201103L
    /**
     * @note for prior to C++11
     */
    template <typename U>
    pfs_constexpr T value_or (U & default_value) const
    {
        // TODO Check BOOST from version has `value_or` method
#if BOOST_VERSION >= 105800
        return base_class::template value_or<U>(default_value);
#else
        return base_class::template get_value_or<U>(default_value);
#endif
    }

    /**
     * @note for prior to C++11
     */
    // TODO Check BOOST from version has `value_or` method
     template<typename U>
     pfs_constexpr T value_or (U const & default_value) const
     {
#if BOOST_VERSION >= 105800
         return base_class::template value_or<U>(default_value);
#else
         return base_class::template get_value_or<U>(default_value);
#endif
     }
#endif
};

// template <typename T>
// constexpr std::optional<std::decay_t<T>> make_optional( T&& value );
//
// template< class T, class... Args >
// constexpr std::optional<T> make_optional( Args&&... args );
//
// template< class T, class U, class... Args >
// constexpr std::optional<T> make_optional( std::initializer_list<U> il, Args&&... args );

} // pfs
