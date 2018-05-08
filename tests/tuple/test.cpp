#include <pfs/test.hpp>
#include <pfs/tuple.hpp>


struct Foo
{
    int x;
    Foo() : x(10) {}
};

int main ()
{
    BEGIN_TESTS(55);

    ///////////////////////////////////////////////////////////////////////////
    // Basic tests                                                           //
    ///////////////////////////////////////////////////////////////////////////

    pfs::tuple<> t0;
    TEST_OK(t0.count() == 0);

    pfs::tuple<int> t1(10);
    TEST_OK(t1.count() == 1);
    TEST_OK(pfs::get<0>(t1) == 10);

    pfs::tuple<int, char> t2(10, 'w');
    TEST_OK(t2.count() == 2);
    TEST_OK(pfs::get<0>(t2) == 10);
    TEST_OK(pfs::get<1>(t2) == 'w');

    pfs::tuple<int, char, double> t3(10, 'w', 3.14f);
    TEST_OK(t3.count() == 3);
    TEST_OK(pfs::get<0>(t3) == 10);
    TEST_OK(pfs::get<1>(t3) == 'w');
    TEST_OK(pfs::get<2>(t3) == 3.14f);

    pfs::tuple<int, char, double, unsigned char> t4(10, 'w', 3.14f, 255);
    TEST_OK(t4.count() == 4);
    TEST_OK(pfs::get<0>(t4) == 10);
    TEST_OK(pfs::get<1>(t4) == 'w');
    TEST_OK(pfs::get<2>(t4) == 3.14f);
    TEST_OK(pfs::get<3>(t4) == 255);

    pfs::tuple<int, char, double, unsigned char, Foo> t5(10, 'w', 3.14f, 255, Foo());
    TEST_OK(t5.count() == 5);
    TEST_OK(pfs::get<0>(t5) == 10);
    TEST_OK(pfs::get<1>(t5) == 'w');
    TEST_OK(pfs::get<2>(t5) == 3.14f);
    TEST_OK(pfs::get<3>(t5) == 255);
    TEST_OK(pfs::get<4>(t5).x == 10);

    pfs::tuple<int, char, double, unsigned char, Foo, int> t6(10, 'w', 3.14f, 255, Foo(), 10);
    TEST_OK(t6.count() == 6);
    TEST_OK(pfs::get<0>(t6) == 10);
    TEST_OK(pfs::get<1>(t6) == 'w');
    TEST_OK(pfs::get<2>(t6) == 3.14f);
    TEST_OK(pfs::get<3>(t6) == 255);
    TEST_OK(pfs::get<4>(t6).x == 10);
    TEST_OK(pfs::get<5>(t6) == 10);

    pfs::tuple<int, char, double, unsigned char, Foo, int, char> t7(10, 'w', 3.14f, 255, Foo(), 10, 'a');
    TEST_OK(t7.count() == 7);
    TEST_OK(pfs::get<0>(t7) == 10);
    TEST_OK(pfs::get<1>(t7) == 'w');
    TEST_OK(pfs::get<2>(t7) == 3.14f);
    TEST_OK(pfs::get<3>(t7) == 255);
    TEST_OK(pfs::get<4>(t7).x == 10);
    TEST_OK(pfs::get<5>(t7) == 10);
    TEST_OK(pfs::get<6>(t7) == 'a');

    pfs::tuple<int, char, double, unsigned char, Foo, int, char, double> t8(10, 'w', 3.14f, 255, Foo(), 10, 'a', 3.14f);
    TEST_OK(t8.count() == 8);
    TEST_OK(pfs::get<0>(t8) == 10);
    TEST_OK(pfs::get<1>(t8) == 'w');
    TEST_OK(pfs::get<2>(t8) == 3.14f);
    TEST_OK(pfs::get<3>(t8) == 255);
    TEST_OK(pfs::get<4>(t8).x == 10);
    TEST_OK(pfs::get<5>(t8) == 10);
    TEST_OK(pfs::get<6>(t8) == 'a');
    TEST_OK(pfs::get<7>(t8) == 3.14f);

    pfs::tuple<int, char, double, unsigned char, Foo, int, char, double, signed char> t9(10, 'w', 3.14f, 255, Foo(), 10, 'a', 3.14f, -128);
    TEST_OK(t9.count() == 9);
    TEST_OK(pfs::get<0>(t9) == 10);
    TEST_OK(pfs::get<1>(t9) == 'w');
    TEST_OK(pfs::get<2>(t9) == 3.14f);
    TEST_OK(pfs::get<3>(t9) == 255);
    TEST_OK(pfs::get<4>(t9).x == 10);
    TEST_OK(pfs::get<5>(t9) == 10);
    TEST_OK(pfs::get<6>(t9) == 'a');
    TEST_OK(pfs::get<7>(t9) == 3.14f);
    TEST_OK(pfs::get<8>(t9) == -128);

    return END_TESTS;
}
