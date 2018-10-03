#pragma once
#include <string>

void test_comparisons ()
{
    ADD_TESTS(48);

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                              Comparisons                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    // SameTypeSameValue
    {
        pfs::variant<int, std::string> v(0), w(0);
        // `v` op `w`
        TEST_OK(v == w);
        TEST_NOK(v != w);
        TEST_NOK(v < w);
        TEST_NOK(v > w);
        TEST_OK(v <= w);
        TEST_OK(v >= w);
        // `w` op `v`
        TEST_OK(w == v);
        TEST_NOK(w != v);
        TEST_NOK(w < v);
        TEST_NOK(w > v);
        TEST_OK(w <= v);
        TEST_OK(w >= v);

#if __cplusplus >= 201103L
        constexpr pfs::variant<int, const char *> cv(0), cw(0);
        // `cv` op `cw`
        static_assert(cv == cw, "");
        static_assert(!(cv != cw), "");
        static_assert(!(cv < cw), "");
        static_assert(!(cv > cw), "");
        static_assert(cv <= cw, "");
        static_assert(cv >= cw, "");
        // `cw` op `cv`
        static_assert(cw == cv, "");
        static_assert(!(cw != cv), "");
        static_assert(!(cw < cv), "");
        static_assert(!(cw > cv), "");
        static_assert(cw <= cv, "");
        static_assert(cw >= cv, "");
#endif
    }

    // SameTypeDiffValue
    {
        pfs::variant<int, std::string> v(0), w(1);
        // `v` op `w`
        TEST_NOK(v == w);
        TEST_OK(v != w);
        TEST_OK(v < w);
        TEST_NOK(v > w);
        TEST_OK(v <= w);
        TEST_NOK(v >= w);
        // `w` op `v`
        TEST_NOK(w == v);
        TEST_OK(w != v);
        TEST_NOK(w < v);
        TEST_OK(w > v);
        TEST_NOK(w <= v);
        TEST_OK(w >= v);

#if __cplusplus >= 201103L
        constexpr pfs::variant<int, const char *> cv(0), cw(1);
        // `cv` op `cw`
        static_assert(!(cv == cw), "");
        static_assert(cv != cw, "");
        static_assert(cv < cw, "");
        static_assert(!(cv > cw), "");
        static_assert(cv <= cw, "");
        static_assert(!(cv >= cw), "");
        // `cw` op `cv`
        static_assert(!(cw == cv), "");
        static_assert(cw != cv, "");
        static_assert(!(cw < cv), "");
        static_assert(cw > cv, "");
        static_assert(!(cw <= cv), "");
        static_assert(cw >= cv, "");
#endif
    }

    // DiffTypeSameValue
    {
        pfs::variant<int, unsigned int> v(0), w(0u);
        // `v` op `w`
        TEST_NOK(v == w);
        TEST_OK(v != w);
        TEST_OK(v < w);
        TEST_NOK(v > w);
        TEST_OK(v <= w);
        TEST_NOK(v >= w);
        // `w` op `v`
        TEST_NOK(w == v);
        TEST_OK(w != v);
        TEST_NOK(w < v);
        TEST_OK(w > v);
        TEST_NOK(w <= v);
        TEST_OK(w >= v);

#if __cplusplus >= 201103L
        constexpr pfs::variant<int, unsigned int> cv(0), cw(0u);
        // `cv` op `cw`
        static_assert(!(cv == cw), "");
        static_assert(cv != cw, "");
        static_assert(cv < cw, "");
        static_assert(!(cv > cw), "");
        static_assert(cv <= cw, "");
        static_assert(!(cv >= cw), "");
        // `cw` op `cv`
        static_assert(!(cw == cv), "");
        static_assert(cw != cv, "");
        static_assert(!(cw < cv), "");
        static_assert(cw > cv, "");
        static_assert(!(cw <= cv), "");
        static_assert(cw >= cv, "");
#endif
    }

    // DiffTypeDiffValue
    {
        pfs::variant<int, unsigned int> v(0), w(1u);
        // `v` op `w`
        TEST_NOK(v == w);
        TEST_OK(v != w);
        TEST_OK(v < w);
        TEST_NOK(v > w);
        TEST_OK(v <= w);
        TEST_NOK(v >= w);
        // `w` op `v`
        TEST_NOK(w == v);
        TEST_OK(w != v);
        TEST_NOK(w < v);
        TEST_OK(w > v);
        TEST_NOK(w <= v);
        TEST_OK(w >= v);

#if __cplusplus >= 201103L
        constexpr pfs::variant<int, unsigned int> cv(0), cw(1u);
        // `cv` op `cw`
        static_assert(!(cv == cw), "");
        static_assert(cv != cw, "");
        static_assert(cv < cw, "");
        static_assert(!(cv > cw), "");
        static_assert(cv <= cw, "");
        static_assert(!(cv >= cw), "");
        // `cw` op `cv`
        static_assert(!(cw == cv), "");
        static_assert(cw != cv, "");
        static_assert(!(cw < cv), "");
        static_assert(cw > cv, "");
        static_assert(!(cw <= cv), "");
        static_assert(cw >= cv, "");
#endif
    }
}
