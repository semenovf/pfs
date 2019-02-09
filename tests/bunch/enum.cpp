#include <pfs/string.hpp>
#include <pfs/voc.hpp>
#include "../catch.hpp"

enum counter_enum {
      ZERO
    , ONE
    , TWO
    , THREE
};

// Enum struct declaration
struct counter_enum_struct
{
    enum value_enum {
          ZERO
        , ONE
        , TWO
        , THREE
    };
};

#if __cplusplus >= 201103L
enum class counter_enum_class : int
{
      ZERO
    , ONE
    , TWO
    , THREE
};
#endif

static pfs::string STRINGS[] = {
      pfs::string("Zero")
    , pfs::string("One")
    , pfs::string("Two")
    , pfs::string("Three")
};

//
// Specializations
//
namespace pfs {

// to_string() specialization
template <>
string to_string<counter_enum> (counter_enum value)
{
    return STRINGS[static_cast<int>(value)];
}

template <>
counter_enum make_enum<counter_enum> (string const & s, bool & ok)
{
    ok = true;

    for (int i = ZERO; i <= THREE; i++) {
        if (STRINGS[i] == s)
            return static_cast<counter_enum>(i);
    }

    ok = false;
    return ZERO;
}

template <>
counter_enum make_enum<counter_enum> (enum_helper<counter_enum>::underlying_type value
        , bool & ok)
{
    ok = true;

    if (value >= ZERO && value <= THREE)
        return static_cast<counter_enum>(value);

    ok = false;
    return ZERO;
}

template <>
string to_string<counter_enum_struct> (counter_enum_struct::value_enum value)
{
    return STRINGS[static_cast<int>(value)];
}

template <>
counter_enum_struct::value_enum make_enum<counter_enum_struct> (string const & s, bool & ok)
{
    return static_cast<counter_enum_struct::value_enum>(make_enum<counter_enum>(s, ok));
}

template <>
counter_enum_struct::value_enum make_enum<counter_enum_struct> (enum_helper<counter_enum_struct>::underlying_type value
        , bool & ok)
{
    return static_cast<counter_enum_struct::value_enum>(make_enum<counter_enum>(value, ok));
}

#if __cplusplus >= 201103L

template <>
string to_string<counter_enum_class> (counter_enum_class value)
{
    return STRINGS[static_cast<int>(value)];
}

template <>
counter_enum_class make_enum<counter_enum_class> (string const & s, bool & ok)
{
    return static_cast<counter_enum_class>(make_enum<counter_enum>(s, ok));
}

template <>
counter_enum_class make_enum<counter_enum_class> (enum_helper<counter_enum_class>::underlying_type value
        , bool & ok)
{
    return static_cast<counter_enum_class>(make_enum<counter_enum>(value, ok));
}

#endif

} // namespace pfs

inline std::ostream & operator << (std::ostream & os, counter_enum value)
{
    os << static_cast<int>(value);
    return os;
}

inline std::ostream & operator << (std::ostream & os, counter_enum_struct::value_enum value)
{
    os << static_cast<int>(value);
    return os;
}

#if __cplusplus >= 201103L
inline std::ostream & operator << (std::ostream & os, counter_enum_class value)
{
    os << static_cast<int>(value);
    return os;
}
#endif

template <typename EnumT>
void check_makers ()
{
    REQUIRE_NOTHROW(pfs::make_enum<EnumT>(0));
    REQUIRE_NOTHROW(pfs::make_enum<EnumT>(1));
    REQUIRE_NOTHROW(pfs::make_enum<EnumT>(2));
    REQUIRE_NOTHROW(pfs::make_enum<EnumT>(3));
    CHECK_THROWS(pfs::make_enum<EnumT>(-1));
    CHECK_THROWS(pfs::make_enum<EnumT>(4));

#if __cplusplus >= 201103L
    CHECK(pfs::make_enum<EnumT>(0) == EnumT::ZERO);
    CHECK(pfs::make_enum<EnumT>(1) == EnumT::ONE);
    CHECK(pfs::make_enum<EnumT>(2) == EnumT::TWO);
    CHECK(pfs::make_enum<EnumT>(3) == EnumT::THREE);
#else
    CHECK(static_cast<counter_enum>(pfs::make_enum<EnumT>(0)) == ZERO);
    CHECK(static_cast<counter_enum>(pfs::make_enum<EnumT>(1)) == ONE);
    CHECK(static_cast<counter_enum>(pfs::make_enum<EnumT>(2)) == TWO);
    CHECK(static_cast<counter_enum>(pfs::make_enum<EnumT>(3)) == THREE);
#endif

    CHECK_THROWS(pfs::make_enum<EnumT>("-1"));
    CHECK_THROWS(pfs::make_enum<EnumT>("Bad Value"));

    REQUIRE_NOTHROW(pfs::make_enum<EnumT>("Zero"));
    REQUIRE_NOTHROW(pfs::make_enum<EnumT>("One"));
    REQUIRE_NOTHROW(pfs::make_enum<EnumT>("Two"));
    REQUIRE_NOTHROW(pfs::make_enum<EnumT>("Three"));

#if __cplusplus >= 201103L
    CHECK(pfs::make_enum<EnumT>("Zero") == EnumT::ZERO);
    CHECK(pfs::make_enum<EnumT>("One") == EnumT::ONE);
    CHECK(pfs::make_enum<EnumT>("Two") == EnumT::TWO);
    CHECK(pfs::make_enum<EnumT>("Three") == EnumT::THREE);
#else
    CHECK(static_cast<counter_enum>(pfs::make_enum<EnumT>("Zero")) == ZERO);
    CHECK(static_cast<counter_enum>(pfs::make_enum<EnumT>("One")) == ONE);
    CHECK(static_cast<counter_enum>(pfs::make_enum<EnumT>("Two")) == TWO);
    CHECK(static_cast<counter_enum>(pfs::make_enum<EnumT>("Three")) == THREE);
#endif
}

template <typename EnumT>
void to_string ()
{
#if __cplusplus >= 201103L
    CHECK(pfs::to_string<EnumT>(EnumT::ZERO) == "Zero");
    CHECK(pfs::to_string<EnumT>(EnumT::ONE) == "One");
    CHECK(pfs::to_string<EnumT>(EnumT::TWO) == "Two");
    CHECK(pfs::to_string<EnumT>(EnumT::THREE) == "Three");
#else
    CHECK(pfs::to_string<EnumT>(EnumT::ZERO) == "Zero");
    CHECK(pfs::to_string<EnumT>(EnumT::ONE) == "One");
    CHECK(pfs::to_string<EnumT>(EnumT::TWO) == "Two");
    CHECK(pfs::to_string<EnumT>(EnumT::THREE) == "Three");
#endif
}

TEST_CASE("Makers") {
    check_makers<counter_enum>();
    check_makers<counter_enum_struct>();
#if __cplusplus >= 201103L
    check_makers<counter_enum_class>();
#endif
}

TEST_CASE("To string conversions") {
    //to_string<counter_enum>();
    to_string<counter_enum_struct>();
#if __cplusplus >= 201103L
    to_string<counter_enum_class>();
#endif
}
