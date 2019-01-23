#pragma once
#include <pfs/type_traits.hpp>
#include <pfs/limits.hpp>
#include <pfs/string.hpp>
#include <pfs/compare.hpp>
#include <pfs/exception.hpp>

namespace pfs {

/**
 */
template <typename StructEnumT>
class voc
{
public:
    typedef typename size_type_n<sizeof(typename StructEnumT::value_enum)>::signed_type native_type;

private:
    static const native_type bad_value ()
    {
        return numeric_limits<native_type>::max();
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

    explicit voc (typename StructEnumT::value_enum value)
        : _value(static_cast<native_type>(value))
    {}

    voc & operator = (typename StructEnumT::value_enum value)
    {
        _value = static_cast<native_type>(value);
        return *this;
    }

    typename StructEnumT::value_enum value () const
    {
        PFS_THROW_EXPR(valid());
        return static_cast<typename StructEnumT::value_enum>(_value);
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
    static bool valid (native_type value);

private:
    native_type _value;
};

template <typename StructEnumT>
inline bool operator == (voc<StructEnumT> const & lhs, voc<StructEnumT> const & rhs)
{
    return lhs.value() == rhs.value();
}

template <typename StructEnumT>
inline bool operator == (voc<StructEnumT> const & lhs, typename StructEnumT::value_enum rhs)
{
    return lhs.value() == rhs;
}

template <typename StructEnumT>
inline bool operator == (typename StructEnumT::value_enum lhs, voc<StructEnumT> const & rhs)
{
    return lhs == rhs.value();
}

template <typename StructEnumT>
inline bool operator < (voc<StructEnumT> const & lhs, voc<StructEnumT> const & rhs)
{
    return lhs.value() < rhs.value();
}

template <typename StructEnumT>
inline bool operator < (voc<StructEnumT> const & lhs, typename StructEnumT::value_enum rhs)
{
    return lhs.value() < rhs;
}

template <typename StructEnumT>
inline bool operator < (typename StructEnumT::value_enum lhs, voc<StructEnumT> const & rhs)
{
    return lhs < rhs.value();
}

template <typename StructEnumT>
inline voc<StructEnumT> make_voc (typename voc<StructEnumT>::native_type value)
{
    return voc<StructEnumT>::valid(value)
            ? voc<StructEnumT>(static_cast<typename StructEnumT::value_enum>(value))
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

