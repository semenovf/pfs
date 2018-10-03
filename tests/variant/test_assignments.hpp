#pragma once
#include <string>

void test_assignments ()
{
    ADD_TESTS(15);

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Assignments                               //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    // Same types
    {
        struct Obj {
            Obj () {}
            Obj (Obj const &) { TEST_FAIL(false); }
#if __cplusplus >= 201103L
            Obj(Obj &&) = default;
#endif
            Obj & operator = (Obj const &) { TEST_OK2(true, "Applied copy assignment operator"); return *this; }
#if __cplusplus >= 201103L
            Obj & operator = (Obj &&) = delete;
#endif
        };
        // `v`, `w`.
        pfs::variant<Obj, int> v, w;
        // copy assignment.
        v = w;
    }

    // Different types
    {
        struct Obj {
            Obj () {}
            Obj (Obj const &) { TEST_OK2(true, "Applied copy constructor"); }
#if __cplusplus >= 201103L
            Obj (Obj &&) = default;
#endif
            Obj & operator = (Obj const &) noexcept { TEST_FAIL(false); return *this; }
#if __cplusplus >= 201103L
            Obj & operator = (Obj &&) = delete;
#endif
        };

        // `v`, `w`.
        pfs::variant<Obj, int> v(42), w;
        // copy assignment.
        v = w;
    }

    //
    // Forwarding
    //

    // SameType
    {
        pfs::variant<int, std::string> v(101);
        TEST_OK(101 == pfs::get<int>(v));
        v = 202;
        TEST_OK(202 == pfs::get<int>(v));
    }

    // SameTypeFwd
    {
        pfs::variant<int, std::string> v(1.1);
        TEST_OK(1 == pfs::get<int>(v));
        v = 2.2;
        TEST_OK(2 == pfs::get<int>(v));
    }

    // DiffType
    {
        pfs::variant<int, std::string> v(42);
        TEST_OK(42 == pfs::get<int>(v));
        v = "42";
        TEST_OK("42" == pfs::get<std::string>(v));
    }

    // DiffTypeFwd
    {
        pfs::variant<int, std::string> v(42);
        TEST_OK(42 == pfs::get<int>(v));
        v = "42";
        TEST_OK("42" == pfs::get<std::string>(v));
    }

    // ExactMatch
    {
        pfs::variant<const char *, std::string> v;
        v = std::string("hello");
        TEST_OK("hello" == pfs::get<std::string>(v));
    }

    // BetterMatch
    {
        pfs::variant<int, double> v;
        // `char` -> `int` is better than `char` -> `double`
        v = 'x';
        TEST_OK(static_cast<int>('x') == pfs::get<int>(v));
    }

    // NoMatch
    {
#if __cplusplus >= 201103L
        struct x {};
        static_assert(!std::is_assignable<pfs::variant<int, std::string>, x> {},
                      "variant<int, std::string> v; v = x;");
#endif
    }

    // Ambiguous
    {
#if __cplusplus >= 201103L
        static_assert(!std::is_assignable<pfs::variant<short, long>, int> {},
                      "variant<short, long> v; v = 42;");
#endif
    }

    // SameTypeOptimization
    {
        pfs::variant<int, std::string> v("hello world!");
        // Check `v`.
        std::string const & x = pfs::get<std::string>(v);
        TEST_OK("hello world!" == x);

        // Save the "hello world!"'s capacity.
        int capacity = x.capacity();

        // Use `std::string::operator=(const char *)` to assign into `v`.
        v = "hello";

        // Check `v`.
        std::string const & y = pfs::get<std::string>(v);

        TEST_OK("hello" == y);

        // Since "hello" is shorter than "hello world!", we should have preserved the
        // existing capacity of the string!.
        TEST_OK(capacity == y.capacity());
    }

    //
    // Movable assignments
    //

#if __cplusplus >= 201103L
    ADD_TESTS(2);

    // SameType
    {
        struct Obj {
            constexpr Obj () {}
            Obj (Obj const &) = delete;
            Obj (Obj &&) noexcept { TEST_FAIL(false); }
            Obj & operator = (Obj const &) = delete;
            Obj & operator = (Obj &&) noexcept { TEST_OK2(true, "Applied movable assignment operator"); return *this; }
        };
        // `v`, `w`.
        pfs::variant<Obj, int> v, w;
        // move assignment.
        v = std::move(w);
    }

    // DiffType
    {
        struct Obj {
            constexpr Obj () {}
            Obj (Obj const &) = delete;
            Obj (Obj &&) noexcept { TEST_OK2(true, "Applied movable constructor"); }
            Obj & operator = (Obj const &) = delete;
            Obj & operator = (Obj &&) noexcept { TEST_FAIL(false); return *this; }
        };
        // `v`, `w`.
        pfs::variant<Obj, int> v(42), w;
        // move assignment.
        v = std::move(w);
    }
#endif
}
