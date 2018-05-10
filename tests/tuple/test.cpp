#include <pfs/test.hpp>
#include <pfs/tuple.hpp>

struct Foo
{
    int x;
    Foo() : x(10) {}
};

int main ()
{
    BEGIN_TESTS(65);

    ///////////////////////////////////////////////////////////////////////////
    // Basic tests                                                           //
    ///////////////////////////////////////////////////////////////////////////

    typedef pfs::tuple<> tuple0_t;
    tuple0_t t0;
    TEST_OK(pfs::tuple_size<tuple0_t>::value == 0);
    TEST_OK(pfs::tuple_size_f(t0) == 0);

    typedef pfs::tuple<int> tuple1_t;
    tuple1_t t1(10);
    TEST_OK(pfs::tuple_size<tuple1_t>::value == 1);
    TEST_OK(pfs::tuple_size_f(t1) == 1);
     TEST_OK(pfs::get<0>(t1) == 10);

    typedef pfs::tuple<int, char> tuple2_t;
    tuple2_t t2(10, 'w');
    TEST_OK(pfs::tuple_size<tuple2_t>::value == 2);
    TEST_OK(pfs::tuple_size_f(t2) == 2);
    TEST_OK(pfs::get<0>(t2) == 10);
    TEST_OK(pfs::get<1>(t2) == 'w');

    typedef pfs::tuple<int, char, double> tuple3_t;
    tuple3_t t3(10, 'w', 3.14f);
    TEST_OK(pfs::tuple_size<tuple3_t>::value == 3);
    TEST_OK(pfs::tuple_size_f(t3) == 3);
    TEST_OK(pfs::get<0>(t3) == 10);
    TEST_OK(pfs::get<1>(t3) == 'w');
    TEST_OK(pfs::get<2>(t3) == 3.14f);

    typedef pfs::tuple<int, char, double, unsigned char> tuple4_t;
    tuple4_t t4(10, 'w', 3.14f, 255);
    TEST_OK(pfs::tuple_size<tuple4_t>::value == 4);
    TEST_OK(pfs::tuple_size_f(t4) == 4);
    TEST_OK(pfs::get<0>(t4) == 10);
    TEST_OK(pfs::get<1>(t4) == 'w');
    TEST_OK(pfs::get<2>(t4) == 3.14f);
    TEST_OK(pfs::get<3>(t4) == 255);

    typedef pfs::tuple<int, char, double, unsigned char, Foo> tuple5_t;
    tuple5_t t5(10, 'w', 3.14f, 255, Foo());
    TEST_OK(pfs::tuple_size<tuple5_t>::value == 5);
    TEST_OK(pfs::tuple_size_f(t5) == 5);
    TEST_OK(pfs::get<1>(t5) == 'w');
    TEST_OK(pfs::get<2>(t5) == 3.14f);
    TEST_OK(pfs::get<0>(t5) == 10);
    TEST_OK(pfs::get<3>(t5) == 255);
    TEST_OK(pfs::get<4>(t5).x == 10);

    typedef pfs::tuple<int, char, double, unsigned char, Foo, int> tuple6_t;
    tuple6_t t6(10, 'w', 3.14f, 255, Foo(), 10);
    TEST_OK(pfs::tuple_size<tuple6_t>::value == 6);
    TEST_OK(pfs::tuple_size_f(t6) == 6);
    TEST_OK(pfs::get<0>(t6) == 10);
    TEST_OK(pfs::get<1>(t6) == 'w');
    TEST_OK(pfs::get<2>(t6) == 3.14f);
    TEST_OK(pfs::get<3>(t6) == 255);
    TEST_OK(pfs::get<4>(t6).x == 10);
    TEST_OK(pfs::get<5>(t6) == 10);

    typedef pfs::tuple<int, char, double, unsigned char, Foo, int, char> tuple7_t;
    tuple7_t t7(10, 'w', 3.14f, 255, Foo(), 10, 'a');
    TEST_OK(pfs::tuple_size<tuple7_t>::value == 7);
    TEST_OK(pfs::tuple_size_f(t7) == 7);
    TEST_OK(pfs::get<0>(t7) == 10);
    TEST_OK(pfs::get<1>(t7) == 'w');
    TEST_OK(pfs::get<2>(t7) == 3.14f);
    TEST_OK(pfs::get<3>(t7) == 255);
    TEST_OK(pfs::get<4>(t7).x == 10);
    TEST_OK(pfs::get<5>(t7) == 10);
    TEST_OK(pfs::get<6>(t7) == 'a');

    typedef pfs::tuple<int, char, double, unsigned char, Foo, int, char, double> tuple8_t;
    tuple8_t t8(10, 'w', 3.14f, 255, Foo(), 10, 'a', 3.14f);
    TEST_OK(pfs::tuple_size<tuple8_t>::value == 8);
    TEST_OK(pfs::tuple_size_f(t8) == 8);
    TEST_OK(pfs::get<0>(t8) == 10);
    TEST_OK(pfs::get<1>(t8) == 'w');
    TEST_OK(pfs::get<2>(t8) == 3.14f);
    TEST_OK(pfs::get<3>(t8) == 255);
    TEST_OK(pfs::get<4>(t8).x == 10);
    TEST_OK(pfs::get<5>(t8) == 10);
    TEST_OK(pfs::get<6>(t8) == 'a');
    TEST_OK(pfs::get<7>(t8) == 3.14f);

    typedef pfs::tuple<int, char, double, unsigned char, Foo, int, char, double, signed char> tuple9_t;
    tuple9_t t9(10, 'w', 3.14f, 255, Foo(), 10, 'a', 3.14f, -128);
    TEST_OK(pfs::tuple_size<tuple9_t>::value == 9);
    TEST_OK(pfs::tuple_size_f(t9) == 9);
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
