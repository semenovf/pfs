#include <iostream>
#include <pfs/test.hpp>
#include <pfs/algorithm.hpp>
#include <pfs/rational.hpp>

typedef pfs::rational<int> rational;

int main ()
{
    BEGIN_TESTS(209);

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

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                           Sort algorithm                              //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    {
        rational rationals[] = {
              rational(1, 2)
            , rational(1, 3)
            , rational(1, 4)
            , rational(1, 5)
            , rational(1, 6)
        };

        pfs::sort(rationals, rationals + sizeof(rationals)/sizeof(rationals[0]), std::less<rational>());

        TEST_OK(rationals[0] == rational(1, 6));
        TEST_OK(rationals[1] == rational(1, 5));
        TEST_OK(rationals[2] == rational(1, 4));
        TEST_OK(rationals[3] == rational(1, 3));
        TEST_OK(rationals[4] == rational(1, 2));

        pfs::sort(rationals, rationals + sizeof(rationals)/sizeof(rationals[0]), std::greater<rational>());

        TEST_OK(rationals[0] == rational(1, 2));
        TEST_OK(rationals[1] == rational(1, 3));
        TEST_OK(rationals[2] == rational(1, 4));
        TEST_OK(rationals[3] == rational(1, 5));
        TEST_OK(rationals[4] == rational(1, 6));
    }

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                Floor                                  //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(floor(rational(-4, 2)) == -2);
    TEST_OK(floor(rational(-3, 2)) == -2);
    TEST_OK(floor(rational(-2, 2)) == -1);
    TEST_OK(floor(rational(-1, 2)) == -1);
    TEST_OK(floor(rational( 0, 2)) == 0);
    TEST_OK(floor(rational( 1, 2)) == 0);
    TEST_OK(floor(rational( 2, 2)) == 1);
    TEST_OK(floor(rational( 3, 2)) == 1);
    TEST_OK(floor(rational( 4, 2)) == 2);
    TEST_OK(floor(rational( 5, 2)) == 2);

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                                Ceil                                   //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    TEST_OK(ceil(rational(-4, 2)) == -2);
    TEST_OK(ceil(rational(-3, 2)) == -1);
    TEST_OK(ceil(rational(-2, 2)) == -1);
    TEST_OK(ceil(rational(-1, 2)) ==  0);
    TEST_OK(ceil(rational( 0, 2)) ==  0);
    TEST_OK(ceil(rational( 1, 2)) ==  1);
    TEST_OK(ceil(rational( 2, 2)) ==  1);
    TEST_OK(ceil(rational( 3, 2)) ==  2);
    TEST_OK(ceil(rational( 4, 2)) ==  2);
    TEST_OK(ceil(rational( 5, 2)) ==  3);

    return END_TESTS;
}
