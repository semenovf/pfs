#pragma once
#include <string>

void test_constructors ()
{
    ADD_TESTS(20)

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Constructors                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    // Default constructor
    {
    pfs::variant<int, std::string> v;
    TEST_OK(0 == pfs::get<0>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int> cv{};
    static_assert(0 == pfs::get<0>(cv), "");
#endif
    }

    // Copy constructor
    {
    // `v`
    pfs::variant<int, std::string> v("hello");
    TEST_OK("hello" == pfs::get<std::string>(v));

    // `w`
    pfs::variant<int, std::string> w(v);
    TEST_OK("hello" == pfs::get<std::string>(w));

    // Check `v`
    TEST_OK("hello" == pfs::get<std::string>(v));

#if __cplusplus >= 201103L
    // `cv`
    constexpr pfs::variant<int, const char *> cv{42};
    static_assert(42 == pfs::get<int>(cv), "");
    // `cw`
    // C++17: constexpr is not usable as variant's copy constructor is defaulted.
    // But in MPARK implementation it is not defaulted.
    /*constexpr*/ pfs::variant<int, const char *> cw{cv};
    /*static_assert*/TEST_OK(42 == pfs::get<int>(cw));
#endif
    }

    // Forwarding constructors
    // Direct
    {
    pfs::variant<int, std::string> v(42);
    TEST_OK(42 == pfs::get<int>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, const char *> cv(42);
    static_assert(42 == pfs::get<int>(cv), "");
#endif
    }

    // DirectConversion
    {
    pfs::variant<int, std::string> v("42");
    TEST_OK("42" == pfs::get<std::string>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, const char *> cv(1.1);
    static_assert(1 == pfs::get<int>(cv), "");
#endif
    }

    // CopyInitialization
    {
    pfs::variant<int, std::string> v = 42;
    TEST_OK(42 == pfs::get<int>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, const char *> cv = 42;
    static_assert(42 == pfs::get<int>(cv), "");
#endif
    }

    // CopyInitializationConversion
    {
    pfs::variant<int, std::string> v = "42";
    TEST_OK("42" == pfs::get<std::string>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, const char *> cv = 1.1;
    static_assert(1 == pfs::get<int>(cv), "");
#endif
    }

    // In-place constructors

    // IndexDirect
    {
    pfs::variant<int, std::string> v(pfs::in_place_index_t<0>(), 42);
    TEST_OK(42 == pfs::get<0>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, const char *> cv(pfs::in_place_index_t<0>{},42);
    static_assert(42 == pfs::get<0>(cv), "");
#endif
    }

    // IndexDirectDuplicate
    {
    pfs::variant<int, int> v(pfs::in_place_index_t<0>(), 42);
    TEST_OK(42 == pfs::get<0>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, int> cv(pfs::in_place_index_t<0>{}, 42);
    static_assert(42 == pfs::get<0>(cv), "");
#endif
    }

    // IndexConversion
    {
    pfs::variant<int, std::string> v(pfs::in_place_index_t<1>(), "42");
    TEST_OK("42" == pfs::get<1>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, const char *> cv(pfs::in_place_index_t<0>{}, 1.1);
    static_assert(1 == pfs::get<0>(cv), "");
#endif
    }

    // IndexInitializerList
    {
    pfs::variant<int, std::string> v(pfs::in_place_index_t<1>(), {'4', '2'});
    TEST_OK("42" == pfs::get<1>(v));
    }

    // TypeDirect
    {
    pfs::variant<int, std::string> v(pfs::in_place_type_t<std::string>(), "42");
    TEST_OK("42" == pfs::get<std::string>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, const char *> cv(
    pfs::in_place_type_t<int>{}, 42);
    static_assert(42 == pfs::get<int>(cv), "");
#endif
    }

    // TypeConversion
    {
    pfs::variant<int, std::string> v(pfs::in_place_type_t<int>(), 42.5);
    TEST_OK(42 == pfs::get<int>(v));

#if __cplusplus >= 201103L
    constexpr pfs::variant<int, const char *> cv(
    pfs::in_place_type_t<int>{}, 42.5);
    static_assert(42 == pfs::get<int>(cv), "");
#endif
    }

    // TypeInitializerList
    {
    pfs::variant<int, std::string> v(pfs::in_place_type_t<std::string>(), {'4', '2'});
    TEST_OK("42" == pfs::get<std::string>(v));
    }

    // Move Constructor
    {
    // `v`
    pfs::variant<int, std::string> v("hello");
    TEST_OK("hello" == pfs::get<std::string>(v));
    // `w`
    pfs::variant<int, std::string> w(std::move(v));
    TEST_OK("hello" == pfs::get<std::string>(w));
    // Check `v`
    TEST_OK(pfs::get<std::string>(v).empty());

#if __cplusplus >= 201103L
    // `cv`
    constexpr pfs::variant<int, const char *> cv(42);
    static_assert(42 == pfs::get<int>(cv), "");
    // `cw`
    /*constexpr*/ pfs::variant<int, const char *> cw(std::move(cv));
    /*static_assert*/TEST_OK(42 == pfs::get<int>(cw));
#endif
    }
}
