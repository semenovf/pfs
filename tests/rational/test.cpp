#include <iostream>
#include <pfs/test.hpp>
#include <pfs/rational.hpp>

typedef pfs::rational<int> rational;

int main ()
{
    BEGIN_TESTS(80);

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
    std::cout << "//                             Assignment                                //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    {
        rational a;
        TEST_OK((a = rational(7, 11)) == rational(7, 11));
        TEST_OK((a.assign(8, 12)) == rational(8, 12));
    }

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
    std::cout << "//                             Absolute value                            //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(rational(1).abs()       == rational(1));
    TEST_OK(rational(-1).abs()      == rational(1));
    TEST_OK(rational( 1, 1).abs()   == rational(1));
    TEST_OK(rational(-1, 1).abs()   == rational(1));
    TEST_OK(rational( 1, -1).abs()  == rational(1));

    TEST_OK(rational( 7, 11).abs()  == rational(7, 11));
    TEST_OK(rational( 7, -11).abs() == rational(7, 11));
    TEST_OK(rational(-7, 11).abs()  == rational(7, 11));
    TEST_OK(rational(-7,-11).abs()  == rational(7, 11));

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                 Reduce                                //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(rational( 1, 2).reduce()  == rational(1, 2));
    TEST_OK(rational( 2, 4).reduce()  == rational(1, 2));
    TEST_OK(rational(-8, 16).reduce()  == rational(-1, 2));

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                Invert                                 //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(rational( 1, 2).invert()  == rational(2));
    TEST_OK(rational( 4, 2).invert()  == rational(2, 4));
    TEST_OK(rational(-8, 16).invert()  == rational(-16, 8));

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                Is decimal                             //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(!rational( 1, 2).is_decimal());
    TEST_OK(!rational( 1, 110).is_decimal());
    TEST_OK(rational( 3, 10).is_decimal());
    TEST_OK(rational( 3, 100000).is_decimal());

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                Compare                                //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    // Denominators are equals
    TEST_OK(rational(-7, 11).compare(rational(-7, 11)) == 0);
    TEST_OK(rational(-7, 11).compare(rational( 7, 11)) <  0);
    TEST_OK(rational( 7, 11).compare(rational(-7, 11)) >  0);

    // Different signs
    TEST_OK(rational(-7, 11).compare(rational( 7, 12)) <  0);
    TEST_OK(rational( 7, 11).compare(rational(-7, 12)) >  0);

    // Different Quotients (q = num / den)
    TEST_OK(rational( 12, 11).compare(rational( 26, 12)) <  0);
    TEST_OK(rational( 26, 12).compare(rational( 12, 11)) >  0);
    TEST_OK(rational(-12, 11).compare(rational(-26, 12)) >  0);
    TEST_OK(rational(-26, 12).compare(rational(-12, 11)) <  0);

    TEST_OK(rational( 555, 5).compare(rational( 447, 4)) <  0);
    TEST_OK(rational( 447, 4).compare(rational( 555, 5)) >  0);
    TEST_OK(rational(-555, 5).compare(rational(-447, 4)) >  0);
    TEST_OK(rational(-447, 4).compare(rational(-555, 5)) <  0);
    TEST_OK(rational( 555, 5).compare(rational( 222, 2)) ==  0);
    TEST_OK(rational(-555, 5).compare(rational(-222, 2)) ==  0);

    TEST_OK(rational(2, pfs::numeric_limits<rational::int_type>::max())
            .compare(rational( 4, 5)) <  0);

    TEST_OK(rational(pfs::numeric_limits<rational::int_type>::max() - 2, pfs::numeric_limits<rational::int_type>::max())
            .compare(rational(pfs::numeric_limits<rational::int_type>::max() - 1, pfs::numeric_limits<rational::int_type>::max())) <  0);

    TEST_OK(rational(-(pfs::numeric_limits<rational::int_type>::max() - 2), pfs::numeric_limits<rational::int_type>::max())
            .compare(-(rational(pfs::numeric_limits<rational::int_type>::max() - 1), pfs::numeric_limits<rational::int_type>::max())) >  0);

    TEST_OK(rational(pfs::numeric_limits<rational::int_type>::min(), pfs::numeric_limits<rational::int_type>::max())
            .compare(rational(pfs::numeric_limits<rational::int_type>::min(), pfs::numeric_limits<rational::int_type>::max())) == 0);

    TEST_OK(rational(pfs::numeric_limits<rational::int_type>::min() + 1, pfs::numeric_limits<rational::int_type>::max())
            .compare(rational(pfs::numeric_limits<rational::int_type>::min() + 2, pfs::numeric_limits<rational::int_type>::max())) <  0);

    TEST_OK(rational(pfs::numeric_limits<rational::int_type>::min() + 2, pfs::numeric_limits<rational::int_type>::max())
            .compare(rational(pfs::numeric_limits<rational::int_type>::min() + 1, pfs::numeric_limits<rational::int_type>::max())) >  0);

    TEST_OK(rational(-7, 11) == rational(-7, 11));
    TEST_OK(rational(-7, 11) != rational( 7, 11));
    TEST_OK(rational(-7, 11) < rational( 7, 11));
    TEST_OK(rational(-7, 11) <= rational( 7, 11));
    TEST_OK(rational( 7, 11) > rational(-7, 11));
    TEST_OK(rational( 7, 11) >= rational(-7, 11));

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                       Arithmetic operators                            //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK((rational(-7, 11) += rational(0)) == rational(-7, 11));
    TEST_OK((rational(-7, 11) += rational( 7, 11)) == rational(0));
    TEST_OK((rational( 7, 11) += rational( 7, 11)) == rational( 14, 11));
    TEST_OK((rational(-7, 11) += rational(-7, 11)) == rational(-14, 11));

    TEST_OK((rational(-7, 11) -= rational(0)) == rational(-7, 11));
    TEST_OK((rational(-7, 11) -= rational(-7, 11)) == rational(0));
    TEST_OK((rational( 7, 11) -= rational(-7, 11)) == rational( 14, 11));
    TEST_OK((rational(-7, 11) -= rational( 7, 11)) == rational(-14, 11));

    TEST_OK((rational(-7, 11) *= rational(0)) == rational(0));
    TEST_OK((rational(-7, 11) *= rational(-7, 11)) == rational( 49, 121));
    TEST_OK((rational( 7, 11) *= rational(-7, 11)) == rational(-49, 121));
    TEST_OK((rational(-7, 11) *= rational( 7, 11)) == rational(-49, 121));

    TEST_OK((rational(0) /= rational(-7, 11)) == rational(0));
    TEST_OK((rational( 49, 121) /= rational(-7, 11)) == rational(-7, 11));
    TEST_OK((rational(-49, 121) /= rational(-7, 11)) == rational( 7, 11));
    TEST_OK((rational(-49, 121) /= rational( 7, 11)) == rational(-7, 11));

    TEST_OK((rational(-7, 11) += 0) == rational(-7, 11));
    TEST_OK((rational(-7, 11) += 7) == rational( 70, 11));
    TEST_OK((rational( 7, 11) += 7) == rational( 84, 11));
    TEST_OK((rational(-7, 11) +=-7) == rational(-84, 11));

    TEST_OK((rational(-7, 11) -= 0) == rational(-7, 11));
    TEST_OK((rational(-7, 11) -= 7) == rational(-84, 11));
    TEST_OK((rational( 7, 11) -= 7) == rational(-70, 11));
    TEST_OK((rational(-7, 11) -=-7) == rational( 70, 11));

    TEST_OK((rational(-7, 11) *= 0) == rational(0));
    TEST_OK((rational(-7, 11) *= 7) == rational(-49, 11));
    TEST_OK((rational( 7, 11) *= 7) == rational( 49, 11));
    TEST_OK((rational(-7, 11) *=-7) == rational( 49, 11));

    TEST_OK((rational(-7, 11) *= 7) == rational(-49, 11));
    TEST_OK((rational( 7, 11) *= 7) == rational( 49, 11));
    TEST_OK((rational(-7, 11) *=-7) == rational( 49, 11));

    TEST_OK((rational(0) /= -7) == rational(0));
    TEST_OK((rational(-7, 11) /= 11) == rational(-7, 121));
    TEST_OK((rational(-7, 11) /=-11) == rational( 7, 121));
    TEST_OK((rational(-7, 11) /= 11) == rational(-7, 121));

    //
    TEST_OK((rational(-7, 11) + rational(0)) == rational(-7, 11));
    TEST_OK((rational(-7, 11) + rational( 7, 11)) == rational(0));
    TEST_OK((rational( 7, 11) + rational( 7, 11)) == rational( 14, 11));
    TEST_OK((rational(-7, 11) + rational(-7, 11)) == rational(-14, 11));

    TEST_OK((rational(-7, 11) - rational(0)) == rational(-7, 11));
    TEST_OK((rational(-7, 11) - rational(-7, 11)) == rational(0));
    TEST_OK((rational( 7, 11) - rational(-7, 11)) == rational( 14, 11));
    TEST_OK((rational(-7, 11) - rational( 7, 11)) == rational(-14, 11));

    TEST_OK((rational(-7, 11) * rational(0)) == rational(0));
    TEST_OK((rational(-7, 11) * rational(-7, 11)) == rational( 49, 121));
    TEST_OK((rational( 7, 11) * rational(-7, 11)) == rational(-49, 121));
    TEST_OK((rational(-7, 11) * rational( 7, 11)) == rational(-49, 121));

    TEST_OK((rational(0) / rational(-7, 11)) == rational(0));
    TEST_OK((rational( 49, 121) / rational(-7, 11)) == rational(-7, 11));
    TEST_OK((rational(-49, 121) / rational(-7, 11)) == rational( 7, 11));
    TEST_OK((rational(-49, 121) / rational( 7, 11)) == rational(-7, 11));

    TEST_OK((rational(-7, 11) + 0) == rational(-7, 11));
    TEST_OK((rational(-7, 11) + 7) == rational( 70, 11));
    TEST_OK((rational( 7, 11) + 7) == rational( 84, 11));
    TEST_OK((rational(-7, 11) +-7) == rational(-84, 11));

    TEST_OK((rational(-7, 11) - 0) == rational(-7, 11));
    TEST_OK((rational(-7, 11) - 7) == rational(-84, 11));
    TEST_OK((rational( 7, 11) - 7) == rational(-70, 11));
    TEST_OK((rational(-7, 11) - -7) == rational( 70, 11));

    TEST_OK((rational(-7, 11) * 0) == rational(0));
    TEST_OK((rational(-7, 11) * 7) == rational(-49, 11));
    TEST_OK((rational( 7, 11) * 7) == rational( 49, 11));
    TEST_OK((rational(-7, 11) *-7) == rational( 49, 11));

    TEST_OK((rational(-7, 11) *= 7) == rational(-49, 11));
    TEST_OK((rational( 7, 11) *= 7) == rational( 49, 11));
    TEST_OK((rational(-7, 11) *=-7) == rational( 49, 11));

    TEST_OK((rational(0) /= -7) == rational(0));
    TEST_OK((rational(-7, 11) /= 11) == rational(-7, 121));
    TEST_OK((rational(-7, 11) /=-11) == rational( 7, 121));
    TEST_OK((rational(-7, 11) /= 11) == rational(-7, 121));

    TEST_OK(( 0 + rational(-7, 11)) == rational(-7, 11));
    TEST_OK(( 7 + rational(-7, 11)) == rational( 70, 11));
    TEST_OK(( 7 + rational( 7, 11)) == rational( 84, 11));
    TEST_OK((-7 + rational(-7, 11)) == rational(-84, 11));

    TEST_OK(( 0 - rational(-7, 11)) == rational( 7, 11));
    TEST_OK(( 7 - rational(-7, 11)) == rational( 84, 11));
    TEST_OK(( 7 - rational( 7, 11)) == rational( 70, 11));
    TEST_OK((-7 - rational(-7, 11)) == rational(-70, 11));

    TEST_OK(( 0 * rational(-7, 11)) == rational(0));
    TEST_OK(( 7 * rational(-7, 11)) == rational(-49, 11));
    TEST_OK(( 7 * rational( 7, 11)) == rational( 49, 11));
    TEST_OK((-7 * rational(-7, 11)) == rational( 49, 11));

    TEST_OK(( 0 / rational(-7, 11)) == rational(0));
    TEST_OK(( 7 / rational(-7, 11)) == rational(-11));
    TEST_OK(( 7 / rational( 7, 11)) == rational( 11));
    TEST_OK((-7 / rational(-7, 11)) == rational( 11));

    return END_TESTS;
}

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
