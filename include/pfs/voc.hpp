#pragma once
#include <pfs/type_traits.hpp>
#include <pfs/limits.hpp>
#include <pfs/string.hpp>
#include <pfs/compare.hpp>
#include <pfs/exception.hpp>

namespace pfs {

#if __cplusplus >= 201103L
template <typename StructEnumT, bool = std::is_enum<StructEnumT>::value>
struct voc_helper
{
    using underlying_type = typename std::underlying_type<StructEnumT>::type;
    using type = StructEnumT;
};

template <typename StructEnumT>
struct voc_helper<StructEnumT, false>
{
    using underlying_type = typename std::underlying_type<typename StructEnumT::value_enum>::type;
    using type = typename StructEnumT::value_enum;
};
#else
template <typename StructEnumT>
struct voc_helper
{
    typedef typename size_type_n<sizeof(typename StructEnumT::value_enum)>::signed_type underlying_type;
    typedef typename StructEnumT::value_enum type;
};
#endif

template <typename StructEnumT>
class voc
{
public:
    typedef typename voc_helper<StructEnumT>::underlying_type underlying_type;
    typedef typename voc_helper<StructEnumT>::type type;

private:
    static underlying_type const bad_value ()
    {
        return pfs::numeric_limits<underlying_type>::max();
    }

public:
    /**
     * @fn voc (voc const & other);
     * @brief Copy constructor.
     */

    /**
     * @fn voc & operator = (voc const & other);
     * @brief Copy assignable operator.
     */

    /**
     */
    voc () : _value(bad_value()) {}

    explicit voc (type value)
        : _value(static_cast<underlying_type>(value))
    {}

    voc & operator = (type value)
    {
        _value = static_cast<underlying_type>(value);
        return *this;
    }

    type value () const
    {
        PFS_THROW_EXPR(valid());
        return static_cast<type>(_value);
    }

    bool valid () const
    {
        return _value != bad_value();
    }

    //
    // Candidates for specialization
    //
    string to_string () const;
    string to_acronym () const;

    static string name ();
    static bool valid (underlying_type value);

private:
    underlying_type _value;
};

template <typename StructEnumT>
inline bool operator == (voc<StructEnumT> const & lhs, voc<StructEnumT> const & rhs)
{
    return lhs.value() == rhs.value();
}

template <typename StructEnumT>
inline bool operator == (voc<StructEnumT> const & lhs, typename voc<StructEnumT>::type rhs)
{
    return lhs.value() == rhs;
}

template <typename StructEnumT>
inline bool operator == (typename voc<StructEnumT>::type lhs, voc<StructEnumT> const & rhs)
{
    return lhs == rhs.value();
}

template <typename StructEnumT>
inline bool operator < (voc<StructEnumT> const & lhs, voc<StructEnumT> const & rhs)
{
    return lhs.value() < rhs.value();
}

template <typename StructEnumT>
inline bool operator < (voc<StructEnumT> const & lhs, typename voc<StructEnumT>::type rhs)
{
    return lhs.value() < rhs;
}

template <typename StructEnumT>
inline bool operator < (typename voc<StructEnumT>::type lhs, voc<StructEnumT> const & rhs)
{
    return lhs < rhs.value();
}

template <typename StructEnumT>
inline voc<StructEnumT> make_voc (typename voc<StructEnumT>::underlying_type value)
{
    return voc<StructEnumT>::valid(value)
            ? voc<StructEnumT>(static_cast<typename voc<StructEnumT>::type>(value))
            : voc<StructEnumT>();
}

template <typename StructEnumT>
voc<StructEnumT> make_voc (string const & s);

template <typename StructEnumT>
inline string to_string (voc<StructEnumT> const & voc)
{
    return voc.to_string();
}

} // namespace pfs
