#include <pfs/string.hpp>
#include <pfs/voc.hpp>
#include "../catch.hpp"

// Enum struct declaration
struct counter_enum
{
    enum value_enum {
          ZERO
        , ONE
        , TWO
        , THREE
        , __BAD_VALUE__
    };
};

#if __cplusplus >= 201103L
enum class counter_enum_class : int
{
      ZERO
    , ONE
    , TWO
    , THREE
    , __BAD_VALUE__
};
#endif

static pfs::string ACRONYMS[] = {
      pfs::string("0")
    , pfs::string("1")
    , pfs::string("2")
    , pfs::string("3")
    , pfs::string("?")
};

static pfs::string STRINGS[] = {
      pfs::string("Zero")
    , pfs::string("One")
    , pfs::string("Two")
    , pfs::string("Three")
    , pfs::string("?")
};

//
// Specializations
//
namespace pfs {

// to_acronym() function specialization
template <>
pfs::string to_acronym<counter_enum> (voc<counter_enum> const & voc)
{
    return ACRONYMS[voc.value()];
}

// to_string() function specialization
template <>
pfs::string to_string<counter_enum> (voc<counter_enum> const & voc)
{
    return STRINGS[voc.value()];
}

// voc::valid() function specialization
template <>
bool voc<counter_enum>::valid (int value)
{
    return value >= counter_enum::ZERO && value < counter_enum::__BAD_VALUE__;
}

template <>
voc<counter_enum> make_voc (pfs::string const & s)
{
    if (!s.empty()) {
        for (int i = counter_enum::ZERO; i < counter_enum::__BAD_VALUE__; i++) {
            if (ACRONYMS[i] == s || STRINGS[i] == s)
                return make_voc<counter_enum>(i);
        }
    }

    return voc<counter_enum>();
}

#if __cplusplus >= 201103L

// to_acronym() function specialization
template <>
pfs::string to_acronym<counter_enum_class> (voc<counter_enum_class> const & voc)
{
    return ACRONYMS[static_cast<int>(voc.value())];
}

// to_string() function specialization
template <>
pfs::string to_string<counter_enum_class> (voc<counter_enum_class> const & voc)
{
    return STRINGS[static_cast<int>(voc.value())];
}

// voc::valid() function specialization
template <>
bool voc<counter_enum_clas>::valid (int value)
{
    return value >= static_cast<int>(counter_enum_clas::ZERO)
            && value < static_cast<int>(counter_enum_clas::__BAD_VALUE__);
}

template <>
voc<counter_enum_class> make_voc (pfs::string const & s)
{
    if (!s.empty()) {
        for (int i = static_cast<int>(counter_enum_class::ZERO)
                    ; i < static_cast<int>(counter_enum_class)::__BAD_VALUE__
                    ; i++) {
            if (ACRONYMS[i] == s || STRINGS[i] == s)
                return make_voc<counter_enum_clas>(i);
        }
    }

    return voc<counter_enum_class>();
}

#endif

} // namespace pfs

typedef pfs::voc<counter_enum> voc_t;

inline std::ostream & operator << (std::ostream & os, voc_t const & value)
{
    os << value.value();
    return os;
}

#if __cplusplus >= 201103L
typedef pfs::voc<counter_enum_class> voc_class_t;

inline std::ostream & operator << (std::ostream & os, voc_class_t const & value)
{
    os << static_cast<int>(value.value());
    return os;
}
#endif


TEST_CASE("Default constructor") {
    voc_t voc;
    CHECK_FALSE(voc.valid());
}

TEST_CASE("Constructors and assign operators") {
    voc_t zero(counter_enum::ZERO);
    voc_t zero_clone(zero);
    CHECK(zero.valid());
    CHECK(zero_clone.valid());

    voc_t one(counter_enum::ONE);
    voc_t one_clone(one);
    CHECK(one.valid());
    CHECK(one_clone.valid());

    voc_t two(counter_enum::TWO);
    voc_t two_clone(two);
    CHECK(two.valid());
    CHECK(two_clone.valid());

    voc_t three(counter_enum::THREE);
    voc_t three_clone(three);
    CHECK(three.valid());
    CHECK(three_clone.valid());

    voc_t bad(counter_enum::__BAD_VALUE__);
    voc_t bad_clone(bad);
    CHECK_FALSE(bad.valid());
    CHECK_FALSE(bad_clone.valid());

    voc_t voc;
    CHECK_FALSE(voc.valid());

    voc = zero;
    CHECK(voc.value() == counter_enum::ZERO);

    voc = one;
    CHECK(voc.value() == counter_enum::ONE);

    voc = two;
    CHECK(voc.value() == counter_enum::TWO);

    voc = three;
    CHECK(voc.value() == counter_enum::THREE);

    voc = bad;
    CHECK(voc.value() == counter_enum::__BAD_VALUE__);
}

TEST_CASE("Makers") {
    voc_t bad_voc = pfs::make_voc<counter_enum>(-1);
    CHECK(bad_voc.value() == counter_enum::__BAD_VALUE__);

    bad_voc = pfs::make_voc<counter_enum>(100);
    CHECK(bad_voc.value() == counter_enum::__BAD_VALUE__);

    voc_t zero = pfs::make_voc<counter_enum>(0);
    CHECK(zero.value() == counter_enum::ZERO);

    voc_t one = pfs::make_voc<counter_enum>(1);
    CHECK(one.value() == counter_enum::ONE);

    voc_t two = pfs::make_voc<counter_enum>(2);
    CHECK(two.value() == counter_enum::TWO);

    voc_t three = pfs::make_voc<counter_enum>(3);
    CHECK(three.value() == counter_enum::THREE);


    CHECK(pfs::make_voc<counter_enum>("-1").value() == counter_enum::__BAD_VALUE__);
    CHECK(pfs::make_voc<counter_enum>("Bad Value").value() == counter_enum::__BAD_VALUE__);

    CHECK(pfs::make_voc<counter_enum>("0").value() == 0);
    CHECK(pfs::make_voc<counter_enum>("Zero").value() == 0);

    CHECK(pfs::make_voc<counter_enum>("1").value() == 1);
    CHECK(pfs::make_voc<counter_enum>("One").value() == 1);

    CHECK(pfs::make_voc<counter_enum>("2").value() == 2);
    CHECK(pfs::make_voc<counter_enum>("Two").value() == 2);

    CHECK(pfs::make_voc<counter_enum>("3").value() == 3);
    CHECK(pfs::make_voc<counter_enum>("Three").value() == 3);
}

TEST_CASE("To string conversions") {
    CHECK(pfs::to_acronym(voc_t()) == "?");
    CHECK(pfs::to_string(voc_t()) == "?");

    CHECK(pfs::to_acronym(voc_t(counter_enum::ZERO)) == "0");
    CHECK(pfs::to_string(voc_t(counter_enum::ZERO)) == "Zero");

    CHECK(pfs::to_acronym(voc_t(counter_enum::ONE)) == "1");
    CHECK(pfs::to_string(voc_t(counter_enum::ONE)) == "One");

    CHECK(pfs::to_acronym(voc_t(counter_enum::TWO)) == "2");
    CHECK(pfs::to_string(voc_t(counter_enum::TWO)) == "Two");

    CHECK(pfs::to_acronym(voc_t(counter_enum::THREE)) == "3");
    CHECK(pfs::to_string(voc_t(counter_enum::THREE)) == "Three");
}

TEST_CASE("Comparisons") {
    voc_t zero(counter_enum::ZERO);
    voc_t one(counter_enum::ONE);
    voc_t two(counter_enum::TWO);
    voc_t three(counter_enum::THREE);
    voc_t bad(counter_enum::__BAD_VALUE__);

    CHECK(zero == zero);
    CHECK(one == one);
    CHECK(two == two);
    CHECK(three == three);
    CHECK(bad == bad);

    CHECK(zero != one);
    CHECK(zero < one);
    CHECK(zero <= one);
    CHECK(zero <= zero);
    CHECK(one > zero);
    CHECK(one >= zero);
    CHECK(one >= one);

    CHECK(zero != counter_enum::ONE);
    CHECK(zero < counter_enum::ONE);
    CHECK(zero <= counter_enum::ONE);
    CHECK(zero <= counter_enum::ZERO);
    CHECK(one > counter_enum::ZERO);
    CHECK(one >= counter_enum::ZERO);
    CHECK(one >= counter_enum::ZERO);

    CHECK(counter_enum::ZERO != one);
    CHECK(counter_enum::ZERO < one);
    CHECK(counter_enum::ZERO <= one);
    CHECK(counter_enum::ZERO <= zero);
    CHECK(counter_enum::ONE > zero);
    CHECK(counter_enum::ONE >= zero);
    CHECK(counter_enum::ONE >= one);
}
