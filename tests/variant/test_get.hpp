#pragma once
#include <string>

void test_get ()
{
    ADD_TESTS(12);

#if __cplusplus >= 201103L
    ADD_TESTS(0);
#endif

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                 Get                                   //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    // HoldsAlternative
    {
        pfs::variant<int, std::string> v(42);
        TEST_OK(pfs::holds_alternative<0>(v));
        TEST_OK(!pfs::holds_alternative<1>(v));
        TEST_OK(pfs::holds_alternative<int>(v));
        TEST_OK(!pfs::holds_alternative<std::string>(v));

#if __cplusplus >= 201103L
        constexpr pfs::variant<int, const char *> cv(42);
        static_assert(pfs::holds_alternative<0>(cv), "");
        static_assert(!pfs::holds_alternative<1>(cv), "");
        static_assert(pfs::holds_alternative<int>(cv), "");
        static_assert(!pfs::holds_alternative<const char *>(cv), "");
#endif
    }


    // MutVarMutType
    {
        pfs::variant<int> v(42);
        TEST_OK(42 == pfs::get<int>(v));
    }

    // MutVarConstType
    {
        pfs::variant<int const> v(42);
        TEST_OK(42 == pfs::get<int const>(v));
    }

    // ConstVarMutType
    {
        pfs::variant<int> const v(42);
        TEST_OK(42 == pfs::get<int>(v));

#if __cplusplus >= 201103L
        constexpr pfs::variant<int> cv(42);
        static_assert(42 == pfs::get<int>(cv), "");
#endif
    }

    // ConstVarConstType
    {
        const pfs::variant<const int> v(42);
        TEST_OK(42 == pfs::get<const int>(v));

#if __cplusplus >= 201103L
        constexpr pfs::variant<const int> cv(42);
        static_assert(42 == pfs::get<const int>(cv), "");
#endif
    }

    // MutVarMutType
    {
        pfs::variant<int> v(42);
        TEST_OK(42 == *pfs::get_if<int>(&v));
    }

    // MutVarConstType
    {
        pfs::variant<const int> v(42);
        TEST_OK(42 == *pfs::get_if<const int>(&v));
    }

    // ConstVarMutType
    {
        const pfs::variant<int> v(42);
        TEST_OK(42 == *pfs::get_if<int>(&v));

#if __cplusplus >= 201103L
        static constexpr pfs::variant<int> cv(42);
        static_assert(42 == *pfs::get_if<int>(&cv), "");
#endif
    }

    // ConstVarConstType
    {
        const pfs::variant<const int> v(42);
        TEST_OK(42 == *pfs::get_if<const int>(&v));

#if __cplusplus >= 201103L
        static constexpr pfs::variant<const int> cv(42);
        static_assert(42 == *pfs::get_if<const int>(&cv), "");
#endif
    }
}


