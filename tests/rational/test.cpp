#include <iostream>
#include <pfs/test.hpp>
#include <pfs/rational.hpp>

typedef pfs::rational<int> rational;

int main ()
{
    BEGIN_TESTS(45);

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Constructors                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(to_string(rational())         == pfs::string("0"));
    TEST_OK(to_string(rational(1))        == pfs::string("1"));
    TEST_OK(to_string(rational(-1))       == pfs::string("-1"));
    TEST_OK(to_string(rational(1, 1))     == pfs::string("1"));
    TEST_OK(to_string(rational(-1, 1))    == pfs::string("-1"));
    TEST_OK(to_string(rational(1, -1))    == pfs::string("-1"));
    TEST_OK(to_string(rational(-1, -1))   == pfs::string("1"));
    TEST_OK(to_string(rational(2, 1))     == pfs::string("2"));
    TEST_OK(to_string(rational(7, 1))     == pfs::string("7"));
    TEST_OK(to_string(rational(4, 2))     == pfs::string("2"));
    TEST_OK(to_string(rational(7, 10))    == pfs::string("0.7"));
    TEST_OK(to_string(rational(7, 100))   == pfs::string("0.07"));
    TEST_OK(to_string(rational(7, 1000))  == pfs::string("0.007"));
    TEST_OK(to_string(rational(7, 10000)) == pfs::string("0.0007"));
    TEST_OK(to_string(rational(7, 11))    == pfs::string("7/11"));

    TEST_OK(to_string(rational(pfs::ratio<1, 1>()))     == pfs::string("1"));
    TEST_OK(to_string(rational(pfs::ratio<-1, 1>()))    == pfs::string("-1"));
    TEST_OK(to_string(rational(pfs::ratio<1, -1>()))    == pfs::string("-1"));
    TEST_OK(to_string(rational(pfs::ratio<-1, -1>()))   == pfs::string("1"));
    TEST_OK(to_string(rational(pfs::ratio<2, 1>()))     == pfs::string("2"));
    TEST_OK(to_string(rational(pfs::ratio<7, 1>()))     == pfs::string("7"));
    TEST_OK(to_string(rational(pfs::ratio<4, 2>()))     == pfs::string("2"));
    TEST_OK(to_string(rational(pfs::ratio<7, 10>()))    == pfs::string("0.7"));
    TEST_OK(to_string(rational(pfs::ratio<7, 100>()))   == pfs::string("0.07"));
    TEST_OK(to_string(rational(pfs::ratio<7, 1000>()))  == pfs::string("0.007"));
    TEST_OK(to_string(rational(pfs::ratio<7, 10000>())) == pfs::string("0.0007"));
    TEST_OK(to_string(rational(pfs::ratio<7, 11>()))    == pfs::string("7/11"));

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             unary operator -                          //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(to_string(-rational(1))      == pfs::string("-1"));
    TEST_OK(to_string(-rational(-1))     == pfs::string("1"));
    TEST_OK(to_string(-rational(1, 1))   == pfs::string("-1"));
    TEST_OK(to_string(-rational(-1, 1))  == pfs::string("1"));
    TEST_OK(to_string(-rational(1, -1))  == pfs::string("1"));
    TEST_OK(to_string(-rational(-1, -1)) == pfs::string("-1"));
    TEST_OK(to_string(-rational(7, 11))  == pfs::string("-7/11"));
    TEST_OK(to_string(-rational(7, -11)) == pfs::string("7/11"));
    TEST_OK(to_string(-rational(-7, 11)) == pfs::string("7/11"));

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             unary operator +                          //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(to_string(+rational(1))      == pfs::string("1"));
    TEST_OK(to_string(+rational(-1))     == pfs::string("-1"));
    TEST_OK(to_string(+rational(1, 1))   == pfs::string("1"));
    TEST_OK(to_string(+rational(-1, 1))  == pfs::string("-1"));
    TEST_OK(to_string(+rational(1, -1))  == pfs::string("-1"));
    TEST_OK(to_string(+rational(-1, -1)) == pfs::string("1"));
    TEST_OK(to_string(+rational(7, 11))  == pfs::string("7/11"));
    TEST_OK(to_string(+rational(7, -11)) == pfs::string("-7/11"));
    TEST_OK(to_string(+rational(-7, 11)) == pfs::string("-7/11"));

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                Compare                                //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Absolute value                            //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
//     TEST_OK(pfs::numeric(1).abs()      == pfs::string("-1"));
//     TEST_OK(pfs::numeric(-1).abs()     == pfs::string("1"));
//     TEST_OK(pfs::numeric(1, 1).abs()   == pfs::string("-1"));
//     TEST_OK(pfs::numeric(-1, 1).abs()  == pfs::string("1"));
//     TEST_OK(pfs::numeric(1, -1).abs()  == pfs::string("1"));
//     TEST_OK(pfs::numeric(-1, -1).abs() == pfs::string("-1"));
//     TEST_OK(pfs::numeric(7, 11).abs()  == pfs::string("-7/11"));
//     TEST_OK(pfs::numeric(7, -11).abs() == pfs::string("7/11"));
//     TEST_OK(pfs::numeric(-7, 11).abs() == pfs::string("7/11"));

//     CHECK(to_string(Numeric{1234, 4567}) == string_t{"1234.4567"});
//     CHECK(to_string(Numeric{-1234, 4567}) == string_t{"-1234.4567"});
//     CHECK(hg::to_string(Numeric{1234}) == string_t{"1234"});
//     CHECK(hg::to_string(Numeric{-1234}) == string_t{"-1234"});
//     CHECK(hg::to_string(Numeric{0.0f}) == string_t{"0"});
//     CHECK(hg::to_string(Numeric{1.2f}) == string_t{"1.200000051085312"});
//     CHECK(hg::to_string(Numeric{1234.456f}) == string_t{"1234.4560547135619072"});
//     CHECK(hg::to_string(Numeric{-1.2f}) == string_t{"-1.200000051085312"});
//     CHECK(hg::to_string(Numeric{-1234.456f}) == string_t{"-1234.4560547135619072"});
//     CHECK(hg::to_string(Numeric{double{1.2}}) == string_t{"1.1999999999999999"});
//     CHECK(hg::to_string(Numeric{double{1234.456}}) == string_t{"1234.4559999999999036"});
//     CHECK(hg::to_string(Numeric{double{-1.2}}) == string_t{"-1.1999999999999999"});
//     CHECK(hg::to_string(Numeric{double{-1234.456}}) == string_t{"-1234.4559999999999036"});

    return END_TESTS;
}


//TEST_CASE("Numeric compare") {
//     CHECK(Numeric {1, 0, 0}  == Numeric {-1, 0, 0});
//     CHECK(Numeric {1, 0, 0}  <= Numeric {-1, 0, 0});
//     CHECK(Numeric {1, 0, 0}  >= Numeric {-1, 0, 0});
//     CHECK_FALSE(Numeric {1, 0, 0}  < Numeric {-1, 0, 0});
//     CHECK_FALSE(Numeric {1, 0, 0}  > Numeric {-1, 0, 0});
//
//     CHECK(Numeric {1, 1234, 4567}  == Numeric {1, 1234, 4567});
//     CHECK(Numeric {1, 1234, 4567}  != Numeric {-1, 1234, 4567});
//     CHECK(Numeric {1, 1234, 4567}  != Numeric {1, 1234, 4568});
//     CHECK(Numeric {-1, 1234, 4567} != Numeric {-1, 1234, 4568});
//
//     CHECK(Numeric {1, 1234, 4567}  <  Numeric {1, 1234, 4568});
//     CHECK(Numeric {1, 1234, 4567}  <= Numeric {1, 1234, 4568});
//     CHECK(Numeric {1, 1234, 4567}  <= Numeric {1, 1234, 4567});
//     CHECK(Numeric {-1, 1234, 4567} <  Numeric {1, 1234, 4567});
//     CHECK(Numeric {-1, 1234, 4567} <= Numeric {1, 1234, 4567});
//     CHECK(Numeric {-1, 1234, 4568} <  Numeric {-1, 1234, 4567});
//     CHECK(Numeric {-1, 1234, 4568} <= Numeric {-1, 1234, 4567});
//     CHECK(Numeric {-1, 1234, 4567} <= Numeric {-1, 1234, 4567});
//
//     CHECK(Numeric {1, 1234, 4568}   > Numeric {1, 1234, 4567});
//     CHECK(Numeric {1, 1234, 4568}  >= Numeric {1, 1234, 4567});
//     CHECK(Numeric {1, 1234, 4567}  >= Numeric {1, 1234, 4567});
//     CHECK(Numeric {1, 1234, 4567}  >  Numeric {-1, 1234, 4567});
//     CHECK(Numeric {1, 1234, 4567}  >= Numeric {-1, 1234, 4567});
//     CHECK(Numeric {-1, 1234, 4567} >  Numeric {-1, 1234, 4568});
//     CHECK(Numeric {-1, 1234, 4567} >= Numeric {-1, 1234, 4568});
//     CHECK(Numeric {-1, 1234, 4567} >= Numeric {-1, 1234, 4567});
//}

//TEST_CASE("Numeric sum/substruct") {
//     Numeric {-1, 9, 6} += Numeric {1, 9, 6};
//
//     (Numeric {1, 9, 6} += Numeric {-1, 9, 6}) == Numeric {};
//
//     CHECK((Numeric {} += Numeric {}) == Numeric {});
//     CHECK((Numeric {1, 0} += Numeric {-1, 0}) == Numeric {});
//     CHECK((Numeric {-1, 0} += Numeric {1, 0}) == Numeric {});
//     CHECK((Numeric {-1, 0} += Numeric {-1, 0}) == Numeric {});
//
//     // X + Y
//     CHECK((Numeric {1, 2, 2} += Numeric {1, 2, 3}) == Numeric {1, 4, 5});
//     CHECK((Numeric {1, 9, 5} += Numeric {1, 9, 6}) == Numeric {1,19, 1});
//
//     // X + -Y
//     CHECK((Numeric {1, 9, 6} += Numeric {-1, 9, 6}) == Numeric {});
//     CHECK((Numeric {1, 9, 6} += Numeric {-1, 9, 7}) == Numeric {-1, 0, 1});
//     CHECK((Numeric {1, 9, 6} += Numeric {-1, 9, 765}) == Numeric {-1, 0, 165});
//
//     // -X + Y
//     CHECK((Numeric {-1, 9, 6} += Numeric {1, 9, 6}) == Numeric {});
//     CHECK((Numeric {-1, 9, 6} += Numeric {1, 9, 7}) == Numeric {1, 0, 1});
//     CHECK((Numeric {-1, 9, 6} += Numeric {1, 9, 765}) == Numeric {1, 0, 165});
//
//     // -X + -Y
//}
