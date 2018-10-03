#pragma once
#include <string>

#if __cplusplus >= 201103L
struct concat
{
    template <typename... Args>
    std::string operator()(const Args &... args) const
    {
        std::ostringstream strm;
        std::initializer_list<int>({(strm << args, 0)...});
        return std::move(strm).str();
    }
};
#endif

void test_visit ()
{
    ADD_TESTS(8);

#if __cplusplus >= 201103L
    ADD_TESTS(9);
#endif

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                Visit                                  //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    enum Qual { LRef, ConstLRef, RRef, ConstRRef };

    struct get_qual
    {
        constexpr Qual operator () (int &) const      { return LRef; }
        constexpr Qual operator()(const int &) const  { return ConstLRef; }
        constexpr Qual operator()(int &&) const       { return RRef; }
        constexpr Qual operator()(const int &&) const { return ConstRRef; }
    };

    // MutVarMutType
    {
        pfs::variant<int> v(42);
        // Check `v`.
        TEST_OK(42 == pfs::get<int>(v));
        // Check qualifier.
        TEST_OK(LRef == pfs::visit(get_qual(), v));
#if __cplusplus >= 201103L
        TEST_OK(RRef == pfs::visit(get_qual(), std::move(v)));
#endif
    }

    // MutVarConstType
    {
        pfs::variant<const int> v(42);
        TEST_OK(42 == pfs::get<const int>(v));
        // Check qualifier.
        TEST_OK(ConstLRef == pfs::visit(get_qual(), v));
#if __cplusplus >= 201103L
        TEST_OK(ConstRRef == pfs::visit(get_qual(), std::move(v)));
#endif
    }

    // ConstVarMutType
    {
        const pfs::variant<int> v(42);
        TEST_OK(42 == pfs::get<int>(v));
        // Check qualifier.
        TEST_OK(ConstLRef == pfs::visit(get_qual(), v));
#if __cplusplus >= 201103L
        TEST_OK(ConstRRef == pfs::visit(get_qual(), std::move(v)));
#endif
    }

    // ConstVarConstType
    {
        const pfs::variant<const int> v(42);
        TEST_OK(42 == pfs::get<const int>(v));
        // Check qualifier.
        TEST_OK(ConstLRef == pfs::visit(get_qual(), v));
#if __cplusplus >= 201103L
        TEST_OK(ConstRRef == pfs::visit(get_qual(), std::move(v)));
#endif
    }

#if __cplusplus >= 201103L
    // Zero
    {
        TEST_OK("" == pfs::visit(concat{}));
    }

    // Double
    {
        pfs::variant<int, std::string> v("hello"), w("world!");
        TEST_OK("helloworld!" == pfs::visit(concat {}, v, w));
    }

    // Quintuple
    {
        pfs::variant<int, std::string> v(101), w("+"), x(202), y("="), z(303);
        TEST_OK("101+202=303" == pfs::visit(concat {}, v, w, x, y, z));
    }

    // Double
    {
        pfs::variant<int, std::string> v("hello");
        pfs::variant<double, const char *> w("world!");
        TEST_OK("helloworld!" == pfs::visit(concat{}, v, w));
    }

    // Quintuple)
    {
        pfs::variant<int, double> v(101);
        pfs::variant<const char *> w("+");
        pfs::variant<bool, std::string, int> x(202);
        pfs::variant<char, std::string, const char *> y('=');
        pfs::variant<long, short> z(303L);
        TEST_OK("101+202=303" == pfs::visit(concat {}, v, w, x, y, z));
    }
#endif
}



