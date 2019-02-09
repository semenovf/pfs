#pragma once
#include <pfs/type_traits.hpp>
#include <pfs/limits.hpp>
#include <pfs/enum.hpp>
#include <pfs/string.hpp>
#include <pfs/compare.hpp>
#include <pfs/exception.hpp>

namespace pfs {

template <typename EnumT>
class voc
{
public:
    typedef typename enum_helper<EnumT>::underlying_type underlying_type;
    typedef typename enum_helper<EnumT>::type type;

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

template <typename EnumT>
inline bool operator == (voc<EnumT> const & lhs, voc<EnumT> const & rhs)
{
    return lhs.value() == rhs.value();
}

template <typename EnumT>
inline bool operator == (voc<EnumT> const & lhs, typename voc<EnumT>::type rhs)
{
    return lhs.value() == rhs;
}

template <typename EnumT>
inline bool operator == (typename voc<EnumT>::type lhs, voc<EnumT> const & rhs)
{
    return lhs == rhs.value();
}

template <typename EnumT>
inline bool operator < (voc<EnumT> const & lhs, voc<EnumT> const & rhs)
{
    return lhs.value() < rhs.value();
}

template <typename EnumT>
inline bool operator < (voc<EnumT> const & lhs, typename voc<EnumT>::type rhs)
{
    return lhs.value() < rhs;
}

template <typename EnumT>
inline bool operator < (typename voc<EnumT>::type lhs, voc<EnumT> const & rhs)
{
    return lhs < rhs.value();
}

template <typename EnumT>
inline voc<EnumT> make_voc (typename voc<EnumT>::underlying_type value)
{
    return voc<EnumT>::valid(value)
            ? voc<EnumT>(static_cast<typename voc<EnumT>::type>(value))
            : voc<EnumT>();
}

template <typename EnumT>
voc<EnumT> make_voc (string const & s);

template <typename EnumT>
inline string to_string (voc<EnumT> const & voc)
{
    return voc.to_string();
}

} // namespace pfs
