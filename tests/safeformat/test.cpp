#include <iostream>
#include <sstream>
#include "pfs/test.hpp"
#include "pfs/typeinfo.hpp"
#include "pfs/string.hpp"
#include "pfs/safeformat.hpp"

#ifdef HAVE_QT_CORE
#   include <QString>
#endif

/*
 * This tests based on loki::SafeFormat tests
 */

template <typename T, typename StringType>
bool testCase (char const * fmt1, std::string const & fmt2, T value)
{
    typedef StringType string_type;

#ifdef PFS_CC_MSC
    typedef pfs::safeformat<pfs::safeformat_compat_msc> fmt;
#else
    typedef pfs::safeformat fmt;
#endif

    char buf[5020];
    string_type s = fmt(string_type(fmt1)).arg(value).str();

#ifdef PFS_CC_MSC
    int i2 =_snprintf(buf, sizeof(buf), fmt2.c_str(), value);
#else
    int i2 = snprintf(buf, sizeof(buf), fmt2.c_str(), value);
#endif

    if (s.size() != static_cast<size_t>(i2) || s != string_type(buf)) {
    	std::cout << "\nReference : " << i2 << "; Actual: " << s.size() << ", Difference = " << i2 - int(s.size())
				  << "\nvalue            : [" << value << "]\n"
				  <<   "safeformat format: [" << string_type(fmt1) << "]\n"
				  <<   "snprintf format  : [" << fmt2 << "]\n"
				  <<   "expected result  : [" << buf << "]\n"
				  <<   "actual result    : [" << s << "]\n";
        return false;
    }
    return true;
}

template <typename T, typename StringType>
void testCaseIntegral (std::string const & length_modifier)
{
    typedef StringType string_type;

    ADD_TESTS(30);

    std::cout << "\nTesting with [T = " << pfs::type_name<T>() << "]\n";

    TEST_FAIL((testCase<T, string_type>("%o" , std::string("%" ) + length_modifier + 'o' , pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%+o", std::string("%+") + length_modifier + 'o', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%o" , std::string("%" ) + length_modifier + 'o' , pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%+o", std::string("%+") + length_modifier + 'o', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%o" , std::string("%" ) + length_modifier + 'o' , 0)));
    TEST_FAIL((testCase<T, string_type>("%+o", std::string("%+") + length_modifier + 'o', 0)));

    TEST_FAIL((testCase<T, string_type>("%x" , std::string("%" ) + length_modifier + 'x', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%+x", std::string("%+") + length_modifier + 'x', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%x" , std::string("%" ) + length_modifier + 'x', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%+x", std::string("%+") + length_modifier + 'x', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%x" , std::string("%" ) + length_modifier + 'x', 0)));
    TEST_FAIL((testCase<T, string_type>("%+x", std::string("%+") + length_modifier + 'x', 0)));

    TEST_FAIL((testCase<T, string_type>("%d" , std::string("%" ) + length_modifier + 'd', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%+d", std::string("%+") + length_modifier + 'd', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%d" , std::string("%" ) + length_modifier + 'd', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%+d", std::string("%+") + length_modifier + 'd', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%d" , std::string("%" ) + length_modifier + 'd', 0)));
    TEST_FAIL((testCase<T, string_type>("%+d", std::string("%+") + length_modifier + 'd', 0)));

    TEST_FAIL((testCase<T, string_type>("%i" , std::string("%" ) + length_modifier + 'i', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%+i", std::string("%+") + length_modifier + 'i', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%i" , std::string("%" ) + length_modifier + 'i', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%+i", std::string("%+") + length_modifier + 'i', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%i" , std::string("%" ) + length_modifier + 'i', 0)));
    TEST_FAIL((testCase<T, string_type>("%+i", std::string("%+") + length_modifier + 'i', 0)));

    TEST_FAIL((testCase<T, string_type>("%u" , std::string("%" ) + length_modifier + 'u', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%+u", std::string("%+") + length_modifier + 'u', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%u" , std::string("%" ) + length_modifier + 'u', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%+u", std::string("%+") + length_modifier + 'u', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%u" , std::string("%" ) + length_modifier + 'u', 0)));
    TEST_FAIL((testCase<T, string_type>("%+u", std::string("%+") + length_modifier + 'u', 0)));
}

template <typename T, typename StringType>
void testCaseFloat (std::string const & length_modifier)
{
    typedef StringType string_type;

    ADD_TESTS(10);
    std::cout << "\nTesting with [T = " << pfs::type_name<T>() << "]\n";

    TEST_FAIL((testCase<T, string_type>("%e", std::string("%") + length_modifier + 'e', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%e", std::string("%") + length_modifier + 'e', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%E", std::string("%") + length_modifier + 'E', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%E", std::string("%") + length_modifier + 'E', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%f", std::string("%") + length_modifier + 'f', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%f", std::string("%") + length_modifier + 'f', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%g", std::string("%") + length_modifier + 'g', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%g", std::string("%") + length_modifier + 'g', pfs::numeric_limits<T>::max())));
    TEST_FAIL((testCase<T, string_type>("%G", std::string("%") + length_modifier + 'G', pfs::numeric_limits<T>::min())));
    TEST_FAIL((testCase<T, string_type>("%G", std::string("%") + length_modifier + 'G', pfs::numeric_limits<T>::max())));
}

template <typename StringType>
void testCaseString ()
{
    typedef StringType string_type;

    ADD_TESTS(1);
    std::cout << "\nTesting with [T = " << pfs::type_name<string_type>() << "]\n";

    TEST_FAIL((testCase<char const *, string_type>("%s", "%s", "Hello")));
}

template <typename StringType>
void test0 ()
{
    typedef StringType string_type;

    testCaseIntegral<char          , string_type>(pfs::printf_length_modifier<char>::value());
    testCaseIntegral<signed char   , string_type>(pfs::printf_length_modifier<signed char>::value());
    testCaseIntegral<unsigned char , string_type>(pfs::printf_length_modifier<unsigned char>::value());
    testCaseIntegral<short         , string_type>(pfs::printf_length_modifier<short>::value());
    testCaseIntegral<unsigned short, string_type>(pfs::printf_length_modifier<unsigned short>::value());
    testCaseIntegral<int           , string_type>(pfs::printf_length_modifier<int>::value());
    testCaseIntegral<unsigned int  , string_type>(pfs::printf_length_modifier<unsigned int>::value());
    testCaseIntegral<long          , string_type>(pfs::printf_length_modifier<long>::value());
    testCaseIntegral<unsigned long , string_type>(pfs::printf_length_modifier<unsigned long>::value());

#ifdef PFS_HAVE_LONG_LONG
    testCaseIntegral<long long, string_type>(pfs::printf_length_modifier<long long>::value());
    testCaseIntegral<unsigned long long, string_type>(pfs::printf_length_modifier<unsigned long long>::value());
#endif

    testCaseFloat<float, string_type>(pfs::printf_length_modifier<float>::value());
    testCaseFloat<double, string_type>(pfs::printf_length_modifier<double>::value());

#ifdef PFS_HAVE_LONG_DOUBLE
    testCaseFloat<long double, string_type>(pfs::printf_length_modifier<long double>::value());
#endif

    testCaseString<string_type>();
}

template <typename StringType>
void test1 ()
{
    typedef StringType string_type;

    ADD_TESTS(4);

    std::cout << "\n\nTesting with pointer\n";

    TEST_FAIL((testCase<void *  , string_type>("%p" , "%p", reinterpret_cast<void *>(0xabcd))));
    TEST_FAIL((testCase<int *   , string_type>("%p" , "%p", reinterpret_cast<int *>(0xabcd))));
    TEST_FAIL((testCase<long *  , string_type>("%p" , "%p", reinterpret_cast<long *>(0xabcd))));
    TEST_FAIL((testCase<double *, string_type>("%p" , "%p", reinterpret_cast<double *>(0xabcd))));
}


template <class Integral1, class Integral2>
Integral2 randomInt (Integral1 low, Integral2 up)
{
    // From ``Accelerated C++'', page 135:
    // random integer in the range [0, n)
    // We adjust to generate in the range [0, n]
    const Integral2
    low2 = low,
           n = up - low;

    PFS_ASSERT(n > 0);

    const size_t bucket_size = RAND_MAX / n;
    PFS_ASSERT(bucket_size > 0);

    Integral2 r;
    do
        r = Integral2(rand() / bucket_size);
    while (r > n);

    r = r + low2;
    PFS_ASSERT(r >= low2 && r <= up);
    return r;
}

std::string randomString (unsigned int maxSize)
{
    size_t i = 0;
    size_t n = randomInt(0, maxSize);
    std::string result;

    for (; i < n; ++i) {
        result.push_back(char(randomInt('a', 'z')));
    }

    return result;
}

template <typename StringType>
void test2 (unsigned limit)
{
    typedef StringType string_type;

    srand(0);

    if (limit != pfs::numeric_limits<unsigned>::max())
        ADD_TESTS(limit);

    for (unsigned i = 0; i < limit; ++i) {
        printf("%u\r", i);

        // Generate a random string for the head
        std::string lead = randomString(100);

        // This string will hold a random format specification
        std::string formatSpec(lead + "|%");

        // Generate a random set of flags
        static std::string const flags("-+0 #");
        size_t maxFlags = randomInt(0u, flags.length() - 1);

        for (size_t j = 0; j != maxFlags; ++j) {
            formatSpec += flags[randomInt(0u, flags.length() - 1)];
        }

        // Generate an optional random width
        if (randomInt(0, 1)) {
            const unsigned int width = randomInt(0, 100);
            char buf[4];
            sprintf(buf, "%u", width);
            formatSpec += buf;
        }

        // Generate an optional random precision
        if (randomInt(0, 1)) {
            const unsigned int prec = randomInt(0, 100);
            char buf[4];
            sprintf(buf, "%u", prec);
            formatSpec += '.';
            formatSpec += buf;
        }

        // Generate a random type character
        static std::string const type("cdeEfgGiopsuxX");
        char typeSpec = type.at(randomInt(0u, type.length() - 1));

        // Generate an optional type prefix
//            static const string prefix("hl");
//            if (typeSpec != 's' && randomInt(0, 1)) {
//                formatSpec += prefix[randomInt(0u, prefix.size() - 1)];
//            }

        formatSpec += typeSpec;
        formatSpec += '|';
        formatSpec += randomString(100);

        switch (typeSpec) {
        case 'c':
        	TEST_FAIL2((testCase<int, string_type>(formatSpec.c_str(), formatSpec, randomInt(1, 127))), formatSpec.c_str());
            break;
        case 'd':
        case 'i':
        case 'o':
        case 'u':
        case 'x':
        case 'X':
            //TestCase(formatSpec, RandomInt(-10000, 10000));
            // don't test negative values on 64bit systems, because
            // snprintf does not support 64 Bit values
            TEST_FAIL2((testCase<int, string_type>(formatSpec.c_str(), formatSpec, randomInt(-10000 * (sizeof(size_t) > 4 ? 0 : 1) , 10000)))
                    , formatSpec.c_str());
            break;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
            TEST_FAIL2((testCase<double, string_type>(formatSpec.c_str()
                            , formatSpec
                            , randomInt(-10000, 10000) / double(randomInt(1, 100))))
                    , formatSpec.c_str());
            break;
        case 'n':
            break;
        case 'p':
            {
                void * p = malloc(randomInt(1, 1000));
                TEST_FAIL2((testCase<void *, string_type>(formatSpec.c_str(), formatSpec, p))
                        , formatSpec.c_str());
                free(p);
            }
            break;
        case 's':
            TEST_FAIL2((testCase<char const *, string_type>(formatSpec.c_str(), formatSpec, randomString(100).c_str()))
                    , formatSpec.c_str());
            break;
        default:
            PFS_ASSERT(false);
            break;
        }
    }
}

// speed test
//
template <typename StringType>
void test3 ()
{
    typedef StringType string_type;

    char buf[512];
    pfs::test::profiler sw;

    int loop = 100;

    double ellapsed_sprintf;
    double ellapsed_safeformat;
    double ellapsed_sstream;
#ifdef HAVE_QT_CORE
    double ellapsed_qstring;
#endif

    sw.start();
    for (int i = loop; i > 0; --i)
        sprintf(buf, "Hey, %u frobnicators and %u twiddlicators\n", i, i);
    ellapsed_sprintf = sw.ellapsed();

    sw.start();
    for (int i = loop; i > 0; --i)
        pfs::safeformat("Hey, %u frobnicators and %u twiddlicators\n")(i)(i);
    ellapsed_safeformat = sw.ellapsed();

    sw.start();
    for (int i = loop; i > 0; --i)
        std::stringstream() << "Hey, " << i << " frobnicators and " << i <<" twiddlicators\n";
    ellapsed_sstream = sw.ellapsed();

#ifdef HAVE_QT_CORE
    sw.start();
    for (int i = loop; i > 0; --i) {
        QString s = QString("Hey, %1 frobnicators and %2 twiddlicators\n").arg(i).arg(i);
        (void)s;
    }
    ellapsed_qstring = sw.ellapsed();
#endif

    std::cout << std::endl << "Elapsed time for " << loop << " outputs:" << std::endl
            << "\tprintf       = " << ellapsed_sprintf    << std::endl
            << "\tsafeformat   = " << ellapsed_safeformat << std::endl
            << "\tstringstream = " << ellapsed_sstream    << std::endl
#ifdef HAVE_QT_CORE
            << "\tQString      = " << ellapsed_qstring    << std::endl
#endif
    ;
}

int main (int argc, char **)
{
    BEGIN_TESTS(0);

//#
//#ifdef PFS_CC_MSC
//  safeformat::set_global_compat(safeformat::compat_msc);
//#else
//  safeformat::set_global_compat(safeformat::compat_gcc);
//#endif

    test0<pfs::string>();
    test1<pfs::string>();
    test2<pfs::string>(1000);
    test3<pfs::string>();

    if (argc > 1) {
        test2<pfs::string>(pfs::numeric_limits<unsigned>::max());
    }

    return END_TESTS;
}
