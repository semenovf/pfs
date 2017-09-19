/*
 * This tests based on loki::SafeFormat tests
 */

#include <iostream>
#include <sstream>
//#include <utility>
//#include <cstdio>

#include "pfs/test/test.hpp"
#include "pfs/test/profiler.hpp"
//#include "pfs/compiler.hpp"
//#include "pfs/string.hpp"
#include "pfs/typeinfo.hpp"
//#include "pfs/limits.hpp"
#include "pfs/traits/stdcxx/string.hpp"
#include "pfs/safeformat.hpp"

#ifdef HAVE_QT
#   include <QString>
#endif

template <typename T, typename StringImplType>
bool testCase (char const * fmt1, std::string const & fmt2, T value)
{
    typedef pfs::string<StringImplType>  string_type;

#ifdef PFS_CC_MSC
    typedef pfs::safeformat<string_type, string_type, pfs::safeformat_compat_msc> fmt;
#else
    typedef pfs::safeformat<string_type, string_type, pfs::safeformat_compat_gcc> fmt;
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

template <typename T, typename StringImplType>
void testCaseIntegral (std::string const & length_modifier)
{
	ADD_TESTS(30);
	std::cout << "\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL((testCase<T, StringImplType>("%o" , std::string("%" ) + length_modifier + 'o' , pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%+o", std::string("%+") + length_modifier + 'o', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%o" , std::string("%" ) + length_modifier + 'o' , pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%+o", std::string("%+") + length_modifier + 'o', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%o" , std::string("%" ) + length_modifier + 'o' , 0)));
	TEST_FAIL((testCase<T, StringImplType>("%+o", std::string("%+") + length_modifier + 'o', 0)));

	TEST_FAIL((testCase<T, StringImplType>("%x" , std::string("%" ) + length_modifier + 'x', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%+x", std::string("%+") + length_modifier + 'x', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%x" , std::string("%" ) + length_modifier + 'x', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%+x", std::string("%+") + length_modifier + 'x', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%x" , std::string("%" ) + length_modifier + 'x', 0)));
	TEST_FAIL((testCase<T, StringImplType>("%+x", std::string("%+") + length_modifier + 'x', 0)));

	TEST_FAIL((testCase<T, StringImplType>("%d" , std::string("%" ) + length_modifier + 'd', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%+d", std::string("%+") + length_modifier + 'd', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%d" , std::string("%" ) + length_modifier + 'd', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%+d", std::string("%+") + length_modifier + 'd', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%d" , std::string("%" ) + length_modifier + 'd', 0)));
	TEST_FAIL((testCase<T, StringImplType>("%+d", std::string("%+") + length_modifier + 'd', 0)));

  	TEST_FAIL((testCase<T, StringImplType>("%i" , std::string("%" ) + length_modifier + 'i', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%+i", std::string("%+") + length_modifier + 'i', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%i" , std::string("%" ) + length_modifier + 'i', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%+i", std::string("%+") + length_modifier + 'i', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%i" , std::string("%" ) + length_modifier + 'i', 0)));
	TEST_FAIL((testCase<T, StringImplType>("%+i", std::string("%+") + length_modifier + 'i', 0)));

	TEST_FAIL((testCase<T, StringImplType>("%u" , std::string("%" ) + length_modifier + 'u', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%+u", std::string("%+") + length_modifier + 'u', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%u" , std::string("%" ) + length_modifier + 'u', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%+u", std::string("%+") + length_modifier + 'u', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%u" , std::string("%" ) + length_modifier + 'u', 0)));
	TEST_FAIL((testCase<T, StringImplType>("%+u", std::string("%+") + length_modifier + 'u', 0)));
}

template <typename T, typename StringImplType>
void testCaseFloat (std::string const & length_modifier)
{
	ADD_TESTS(10);
	std::cout << "\nTesting with [T = " << pfs::type_name<T>() << "]\n";
    
	TEST_FAIL((testCase<T, StringImplType>("%e", std::string("%") + length_modifier + 'e', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%e", std::string("%") + length_modifier + 'e', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%E", std::string("%") + length_modifier + 'E', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%E", std::string("%") + length_modifier + 'E', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%f", std::string("%") + length_modifier + 'f', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%f", std::string("%") + length_modifier + 'f', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%g", std::string("%") + length_modifier + 'g', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%g", std::string("%") + length_modifier + 'g', pfs::numeric_limits<T>::max())));
	TEST_FAIL((testCase<T, StringImplType>("%G", std::string("%") + length_modifier + 'G', pfs::numeric_limits<T>::min())));
	TEST_FAIL((testCase<T, StringImplType>("%G", std::string("%") + length_modifier + 'G', pfs::numeric_limits<T>::max())));
}

template <typename StringImplType>
void testCaseString ()
{
    typedef pfs::string<StringImplType>  string_type;
    
	ADD_TESTS(1);
	std::cout << "\nTesting with [T = " << pfs::type_name<string_type>() << "]\n";
    
	TEST_FAIL((testCase<char const *, StringImplType>("%s", "%s", "Hello")));
}

template <typename StringImplType>
void test0 ()
{
	testCaseIntegral<char          , StringImplType>(pfs::printf_length_modifier<char>::value());
	testCaseIntegral<signed char   , StringImplType>(pfs::printf_length_modifier<signed char>::value());
	testCaseIntegral<unsigned char , StringImplType>(pfs::printf_length_modifier<unsigned char>::value());
	testCaseIntegral<short         , StringImplType>(pfs::printf_length_modifier<short>::value());
	testCaseIntegral<unsigned short, StringImplType>(pfs::printf_length_modifier<unsigned short>::value());
	testCaseIntegral<int           , StringImplType>(pfs::printf_length_modifier<int>::value());
	testCaseIntegral<unsigned int  , StringImplType>(pfs::printf_length_modifier<unsigned int>::value());
	testCaseIntegral<long          , StringImplType>(pfs::printf_length_modifier<long>::value());
	testCaseIntegral<unsigned long , StringImplType>(pfs::printf_length_modifier<unsigned long>::value());

#ifdef PFS_HAVE_LONG_LONG
	testCaseIntegral<long long, StringImplType>(pfs::printf_length_modifier<long long>::value());
	testCaseIntegral<unsigned long long, StringImplType>(pfs::printf_length_modifier<unsigned long long>::value());
#endif

	testCaseFloat<float, StringImplType>(pfs::printf_length_modifier<float>::value());
	testCaseFloat<double, StringImplType>(pfs::printf_length_modifier<double>::value());

#ifdef PFS_HAVE_LONG_DOUBLE
	testCaseFloat<long double, StringImplType>(pfs::printf_length_modifier<long double>::value());
#endif
    
    testCaseString<StringImplType>();
}

template <typename StringImplType>
void test1 ()
{
	ADD_TESTS(4);
	std::cout << "\n\nTesting with pointer\n";

    TEST_FAIL((testCase<void *  , StringImplType>("%p" , "%p", (void *)0xabcd)));
    TEST_FAIL((testCase<int *   , StringImplType>("%p" , "%p", (int *)0xabcd)));
    TEST_FAIL((testCase<long *  , StringImplType>("%p" , "%p", (long *)0xabcd)));
    TEST_FAIL((testCase<double *, StringImplType>("%p" , "%p", (double *)0xabcd)));
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

void test2 (unsigned limit)
{
    typedef pfs::stdcxx::string string_impl_type;
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
        	TEST_FAIL2((testCase<int, string_impl_type>(formatSpec.c_str(), formatSpec, randomInt(1, 127))), formatSpec.c_str());
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
            TEST_FAIL2((testCase<int, string_impl_type>(formatSpec.c_str(), formatSpec, randomInt(-10000 * (sizeof(size_t) > 4 ? 0 : 1) , 10000)))
            		, formatSpec.c_str());
            break;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
        	TEST_FAIL2((testCase<double, string_impl_type>(formatSpec.c_str()
                            , formatSpec
                            , randomInt(-10000, 10000) / double(randomInt(1, 100))))
        			, formatSpec.c_str());
            break;
        case 'n':
            break;
        case 'p':
            {
                void * p = malloc(randomInt(1, 1000));
                TEST_FAIL2((testCase<void *, string_impl_type>(formatSpec.c_str(), formatSpec, p))
                		, formatSpec.c_str());
                free(p);
            }
            break;
        case 's':
        	TEST_FAIL2((testCase<char const *, string_impl_type>(formatSpec.c_str(), formatSpec, randomString(100).c_str()))
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
void test3 ()
{
    typedef pfs::string<pfs::stdcxx::string> string_type;
    
	char buf[512];
	using pfs::test::profiler;
	profiler sw;

	int loop = 100;

	double ellapsed_sprintf;
	double ellapsed_safeformat;
	double ellapsed_sstream;
#ifdef HAVE_QT
	double ellapsed_qstring;
#endif

	sw.start();
	for (int i = loop; i > 0; --i)
		sprintf(buf, "Hey, %u frobnicators and %u twiddlicators\n", i, i);
	ellapsed_sprintf = sw.ellapsed();

	sw.start();
	for (int i = loop; i > 0; --i)
		pfs::safeformat<string_type>("Hey, %u frobnicators and %u twiddlicators\n")(i)(i);
	ellapsed_safeformat = sw.ellapsed();

	sw.start();
	for (int i = loop; i > 0; --i)
		std::stringstream() << "Hey, " << i << " frobnicators and " << i <<" twiddlicators\n";
	ellapsed_sstream = sw.ellapsed();

#ifdef HAVE_QT
	sw.start();
	for (int i = loop; i > 0; --i)
		QString("Hey, %1 frobnicators and %2 twiddlicators\n").arg(i).arg(i);
	ellapsed_qstring = sw.ellapsed();
#endif

	std::cout << std::endl << "Elapsed time for " << loop << " outputs:" << std::endl
	     << "\tprintf       = " << ellapsed_sprintf    << std::endl
	     << "\tsafeformat   = " << ellapsed_safeformat << std::endl
		 << "\tstringstream = " << ellapsed_sstream    << std::endl
#ifdef HAVE_QT
		 << "\tQString      = " << ellapsed_qstring    << std::endl
#endif
		 ;
}

int main (int argc, char * [])
{
	BEGIN_TESTS(0);
    
//#
//#ifdef PFS_CC_MSC
//	safeformat::set_global_compat(safeformat::compat_msc);
//#else
//	safeformat::set_global_compat(safeformat::compat_gcc);
//#endif

    test0<pfs::stdcxx::string>();
    test1<pfs::stdcxx::string>();
    test2(1000);
    test3();

    if (argc > 1) {
        test2(pfs::numeric_limits<unsigned>::max());
    }

    return END_TESTS;
}
