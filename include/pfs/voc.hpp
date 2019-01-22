#pragma once
#include <pfs/type_traits.hpp>
#include <pfs/string.hpp>
#include <pfs/compare.hpp>

namespace pfs {

/**
 */
template <typename StructEnumT>
class voc
{
public:
    /**
     * @fn voc (voc const & other);
     * @brief Copy constructor.
     */

    /**
     * @fn voc & operator = (voc const & other);
     * @brief Copy assignable operator.
     */

    voc (typename StructEnumT::value_enum value = StructEnumT::__BAD_VALUE__)
        : _value(value)
    {}

    voc & operator = (typename StructEnumT::value_enum value)
    {
        _value = value;
        return *this;
    }

    typename StructEnumT::value_enum value () const
    {
        return _value;
    }

    bool valid () const
    {
        return valid(_value);
    }

    static bool valid (int value);

private:
    typename StructEnumT::value_enum _value;
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
string to_string (voc<StructEnumT> const & voc);

template <typename StructEnumT>
string to_acronym (voc<StructEnumT> const & voc);

template <typename StructEnumT>
inline voc<StructEnumT> make_voc (int value)
{
    return voc<StructEnumT>::valid(value)
            ? voc<StructEnumT>(static_cast<typename StructEnumT::value_enum>(value))
            : voc<StructEnumT>();
}

template <typename StructEnumT>
voc<StructEnumT> make_voc (string const & s);

template <typename StructEnumT>
string voc_name ();

} // namespace pfs

