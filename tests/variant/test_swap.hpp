#pragma once
#include <string>
#include <pfs/utility.hpp>

namespace swap1 {

struct Obj
{
    Obj (size_t * dtor_count) : dtor_count_(dtor_count) {}
    ~Obj () { ++(*dtor_count_); }
    size_t * dtor_count_;
};

} // namespace swap1

namespace swap2 {

struct Obj
{
    Obj (size_t * dtor_count) : dtor_count_(dtor_count) {}
    ~Obj () { ++(*dtor_count_); }
    size_t * dtor_count_;
};

static void swap (Obj &lhs, Obj &rhs) noexcept
{
    std::swap(lhs.dtor_count_, rhs.dtor_count_);
}

} // namespace swap2

namespace swap3 {

struct V
{
    V (size_t *dtor_count) : dtor_count_(dtor_count) {}
    ~V () { ++(*dtor_count_); }
    size_t * dtor_count_;
};

struct W
{
    W (size_t *dtor_count) : dtor_count_(dtor_count) {}
    ~W () { ++(*dtor_count_); }
    size_t * dtor_count_;
};

} // namespace swap2


void test_swap ()
{
    ADD_TESTS(20);

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                  Swap                                 //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    // Same
    {
        pfs::variant<int, std::string> v("hello");
        pfs::variant<int, std::string> w("world");
        // Check `v`.
        TEST_OK("hello" == pfs::get<std::string>(v));
        // Check `w`.
        TEST_OK("world" == pfs::get<std::string>(w));
        // Swap.
        using pfs::swap;
        swap(v, w);
        // Check `v`.
        TEST_OK("world" == pfs::get<std::string>(v));
        // Check `w`.
        TEST_OK("hello" == pfs::get<std::string>(w));
    }

    // Different
    {
        pfs::variant<int, std::string> v(42);
        pfs::variant<int, std::string> w("hello");
        // Check `v`.
        TEST_OK(42 == pfs::get<int>(v));
        // Check `w`.
        TEST_OK("hello" == pfs::get<std::string>(w));
        // Swap.
        using pfs::swap;
        swap(v, w);
        // Check `v`.
        TEST_OK("hello" == pfs::get<std::string>(v));
        // Check `w`.
        TEST_OK(42 == pfs::get<int>(w));
    }

    // DtorsSame
    {
        size_t v_count = 0;
        size_t w_count = 0;
        {
            pfs::variant<swap1::Obj> v(&v_count), w(&w_count);
            // Calls `std::swap(Obj &lhs, Obj &rhs)`, with which we perform:
            // ```
            // {
            //   Obj temp(move(lhs));
            //   lhs = move(rhs);
            //   rhs = move(temp);
            // }  `++v_count` from `temp::~Obj()`.
            // ```
            using pfs::swap;
            swap(v, w);
            TEST_OK(1u == v_count);
            TEST_OK(0u == w_count);
        }
        TEST_OK(2u == v_count);
        TEST_OK(1u == w_count);
    }

    // DtorsSameWithSwap
    {
        size_t v_count = 0;
        size_t w_count = 0;
        {
            pfs::variant<swap2::Obj> v(& v_count), w(& w_count);
            using pfs::swap;
            swap(v, w);
            // Calls `swap2::swap(Obj &lhs, Obj &rhs)`, with which doesn't call any destructors.
            TEST_OK(0u == v_count);
            TEST_OK(0u == w_count);
        }
        TEST_OK(1u == v_count);
        TEST_OK(1u == w_count);
    }

    // DtorsDifferent
    {
        size_t v_count = 0;
        size_t w_count = 0;
        {
            pfs::variant<swap3::V, swap3::W> v(pfs::in_place_type_t<swap3::V>(), & v_count);
            pfs::variant<swap3::V, swap3::W> w(pfs::in_place_type_t<swap3::W>(), & w_count);
            using pfs::swap;
            swap(v, w);
            TEST_OK(1u == v_count);
            TEST_OK(2u == w_count);
        }
        TEST_OK(2u == v_count);
        TEST_OK(3u == w_count);
    }
}
