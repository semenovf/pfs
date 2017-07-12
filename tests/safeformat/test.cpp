/*
 * This tests based on loki::SafeFormat tests
 */

//#include <iostream>
//#include <sstream>
//#include <utility>
//#include <cstdio>

#include "pfs/test/test.hpp"
//#include "pfs/test/profiler.hpp"
//#include "pfs/compiler.hpp"
//#include "pfs/string.hpp"
//#include "pfs/typeinfo.hpp"
//#include "pfs/limits.hpp"
#include "pfs/traits/stdcxx/string.hpp"
#include "pfs/safeformat.hpp"

#include "test_iterator.hpp"


//#ifdef PFS_HAVE_QT
//#include <QString>
//#endif

using std::cout;
using std::endl;

//typedef pfs::traits::stdcxx::string  string_impl;
//typedef pfs::string<string_impl>     string;
//typedef pfs::safeformat<string_impl> safeformat;

#if __TODO__

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

string randomString (unsigned int maxSize)
{
    size_t i = 0;
    size_t n = randomInt(0, maxSize);
    string result;

    for (; i < n; ++i) {
        result.append(pfs::to_string<string>(randomInt('a', 'z')));
    }

    return result;
}

template <class T>
bool testCase (const string & fmt1, const string & fmt2, T value)
{
    char buf[5020];
    string s = safeformat(fmt1)(value).str();

#ifdef PFS_CC_MSC
    int i2 =_snprintf(buf, sizeof(buf), reinterpret_cast<const char *>(fmt2.c_str()), value);
#else
    int i2 = snprintf(buf, sizeof(buf), reinterpret_cast<const char *>(fmt2.data()), value);
#endif

    if (s.size() != static_cast<size_t>(i2) || s != string(buf)) {
    	cout << endl
    			<< "Reference: " << i2 << "; Actual: " << s.length() << ", Difference = " << i2 - int(s.length()) << endl
				<< "V: ["  << value << "]\n"
				<< "F1: [" << fmt1 << "]\n"
				<< "F2: [" << fmt2 << "]\n"
				<< "R: ["  << buf << "]\n"
				<< "A: ["  << s.data() << "]\n";
        return false;
    }
    return true;
}

#if __COMMENT__
void test1 ()
{
    //srand(time(0));
    srand(0);
    for (unsigned i = 0; ; ++i) {
        printf("%u\r", i);

        // Generate a random string for the head
        string lead = randomString(100);

        // This string will hold a random format specification
        string formatSpec(lead + "|%");

        // Generate a random set of flags
        static const string flags("-+0 #");
        size_t maxFlags = randomInt(0u, flags.length() - 1);

        for (size_t i = 0; i != maxFlags; ++i) {
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
        static const string type("cdeEfgGiopsuxX");
        pfs::ucchar typeSpec = type.charAt(randomInt(0u, type.length() - 1));

        // Generate an optional type prefix
//            static const string prefix("hl");
//            if (typeSpec != 's' && randomInt(0, 1)) {
//                formatSpec += prefix[randomInt(0u, prefix.size() - 1)];
//            }
        formatSpec += typeSpec;
        formatSpec += '|';
        formatSpec += randomString(100);

        switch (typeSpec.value()) {
        case 'c':
        	TEST_FAIL2(testCase(formatSpec, randomInt(1, 127))
        			, formatSpec.c_str());
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
            TEST_FAIL2(testCase(formatSpec, randomInt( -10000 * (sizeof(size_t)>4 ? 0 : 1) , 10000))
            		, formatSpec.c_str());
            break;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
        	TEST_FAIL2(testCase(formatSpec,randomInt(-10000, 10000) / double(randomInt(1, 100)))
        			, formatSpec.c_str());
            break;
        case 'n':
            break;
        case 'p':
            {
                void * p = malloc(randomInt(1, 1000));
                TEST_FAIL2(testCase(formatSpec, p)
                		, formatSpec.c_str());
                free(p);
            }
            break;
        case 's':
        	TEST_FAIL2(testCase(formatSpec, randomString(100).c_str())
        			, formatSpec.c_str());
            break;
        default:
            PFS_ASSERT(false);
            break;
        }
    }
}
#endif

template <typename T>
void testCaseIntegral()
{
	ADD_TESTS(25);
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>("%o" , "%o", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+o", "%+o", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%o" , "%o" , pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+o", "%+o", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%o" , "%o" , 0));
	TEST_FAIL(testCase<T>("%+o", "%+o", 0));

	TEST_FAIL(testCase<T>("%x" , "%x" , pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+x", "%+x", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%x" , "%x" , pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+x", "%+x", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%x" , "%x" , 0));
	TEST_FAIL(testCase<T>("%+x", "%+x", 0));

	TEST_FAIL(testCase<T>("%d" , "%d" , pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+d", "%+d", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%d" , "%d" , pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+d", "%+d", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%d" , "%d" , 0));
	TEST_FAIL(testCase<T>("%+d", "%+d", 0));

	TEST_FAIL(testCase<T>("%u" , "%u" , pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+u", "%+u", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%u" , "%u" , pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+u", "%+u", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%u" , "%u" , 0));
	TEST_FAIL(testCase<T>("%+u", "%+u", 0));

	TEST_FAIL(testCase<T>("%c" , "%c", T('W')));
}

template <typename T>
void testCaseLongIntegral()
{
	ADD_TESTS(25);
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>("%o" , "%lo" , pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+o", "%+lo", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%o" , "%lo" , pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+o", "%+lo", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%o" , "%lo" , 0));
	TEST_FAIL(testCase<T>("%+o", "%+lo", 0));

	TEST_FAIL(testCase<T>("%x"  , "%lx" , pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+x" , "%+lx", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%x"  , "%lx" , pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+x" , "%+lx", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%x"  , "%lx" , 0));
	TEST_FAIL(testCase<T>("%+x" , "%+lx", 0));

	TEST_FAIL(testCase<T>("%d"  , "%ld" , pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+d" , "%+ld", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%d"  , "%ld" , pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+d" , "%+ld", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%d"  , "%ld" , 0));
	TEST_FAIL(testCase<T>("%+d" , "%+ld", 0));

	TEST_FAIL(testCase<T>("%u" , "%lu" , pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+u", "%+lu", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%u" , "%lu" , pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+u", "%+lu", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%u" , "%lu" , 0));
	TEST_FAIL(testCase<T>("%+u", "%+lu", 0));

	TEST_FAIL(testCase<T>("%c", "%lc", T('W')));
}

#ifdef PFS_HAVE_LONGLONG
template <typename T>
void testCaseLongLongIntegral()
{
	ADD_TESTS(25);
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>("%o", "%llo", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+o", "%+llo", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%o", "%llo", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+o", "%+llo", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%o", "%llo", 0));
	TEST_FAIL(testCase<T>("%+o", "%+llo", 0));

	TEST_FAIL(testCase<T>("%x", "%llx", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+x", "%+llx", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%x", "%llx", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+x", "%+llx", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%x", "%llx", 0));
	TEST_FAIL(testCase<T>("%+x", "%+llx", 0));

	TEST_FAIL(testCase<T>("%d", "%lld", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+d", "%+lld", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%d", "%lld", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+d", "%+lld", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%d", "%lld", 0));
	TEST_FAIL(testCase<T>("%+d", "%+lld", 0));

	TEST_FAIL(testCase<T>("%u", "%llu", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%+u", "%+llu", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%u", "%llu", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%+u", "%+llu", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%u", "%llu", 0));
	TEST_FAIL(testCase<T>("%+u", "%+llu", 0));

	TEST_FAIL(testCase<T>("%c", "%llc", T('W')));
}
#endif // PFS_HAVE_LONGLONG

template <typename T>
void testCaseFloat()
{
	ADD_TESTS(10);
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>("%e", "%e", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%e", "%e", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%E", "%E", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%E", "%E", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%f", "%f", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%f", "%f", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%g", "%g", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%g", "%g", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%G", "%G", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%G", "%G", pfs::numeric_limits<T>::max()));
}

template <typename T>
void testCaseLongDouble ()
{
	ADD_TESTS(10);
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>("%e", "%Le", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%e", "%Le", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%E", "%LE", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%E", "%LE", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%f", "%Lf", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%f", "%Lf", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%g", "%Lg", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%g", "%Lg", pfs::numeric_limits<T>::max()));
	TEST_FAIL(testCase<T>("%G", "%LG", pfs::numeric_limits<T>::min()));
	TEST_FAIL(testCase<T>("%G", "%LG", pfs::numeric_limits<T>::max()));
}

void test0 ()
{
	string s = safeformat("%f")(pfs::numeric_limits<double>::max())();
	cout << s << endl;

	testCaseIntegral<char>();
	testCaseIntegral<signed char>();
	testCaseIntegral<unsigned char>();
	testCaseIntegral<short>();
	testCaseIntegral<unsigned short>();
	testCaseIntegral<int>();
	testCaseIntegral<unsigned int>();
	testCaseLongIntegral<long>();
	testCaseLongIntegral<unsigned long>();

#ifdef PFS_HAVE_LONGLONG
	testCaseLongLongIntegral<long long>();
	testCaseLongLongIntegral<unsigned long long>();
#endif

	testCaseFloat<float>();
	testCaseFloat<double>();

#ifdef PFS_HAVE_LONG_DOUBLE
	testCaseLongDouble<long double>();
#endif
}

// test speed
//
void test2 ()
{
	char buf[512];
	using pfs::test::profiler;
	profiler sw;

	int loop = 100;

	double ellapsed_sprintf;
	double ellapsed_safeformat;
	double ellapsed_sstream;
#ifdef PFS_HAVE_QT
	double ellapsed_qstring;
#endif

	sw.start();
	for (int i = loop; i > 0; --i)
		sprintf(buf, "Hey, %u frobnicators and %u twiddlicators\n", i, i);
	ellapsed_sprintf = sw.ellapsed();

	sw.start();
	for (int i = loop; i > 0; --i)
		safeformat("Hey, %u frobnicators and %u twiddlicators\n")(i)(i);
	ellapsed_safeformat = sw.ellapsed();

	sw.start();
	for (int i = loop; i > 0; --i)
		std::stringstream() << "Hey, " << i << " frobnicators and " << i <<" twiddlicators\n";
	ellapsed_sstream = sw.ellapsed();

#ifdef PFS_HAVE_QT
	sw.start();
	for (int i = loop; i > 0; --i)
		QString("Hey, %1 frobnicators and %2 twiddlicators\n").arg(i).arg(i);
	ellapsed_qstring = sw.ellapsed();
#endif

	cout << endl << "Elapsed time for " << loop << " outputs:" << endl
	     << "\tprintf       = " << ellapsed_sprintf    << endl
	     << "\tsafeformat   = " << ellapsed_safeformat << endl
		 << "\tstringstream = " << ellapsed_sstream    << endl
#ifdef PFS_HAVE_QT
		 << "\tQString      = " << ellapsed_qstring    << endl
#endif
		 ;
}

void test3 ()
{
	ADD_TESTS(4);
	cout << "\n\nTesting with pointer\n";

    TEST_FAIL(testCase<void *>("%p"  , "%p", (void *)0xabcd));
    TEST_FAIL(testCase<int *>("%p"   , "%p", (int *)0xabcd));
    TEST_FAIL(testCase<long *>("%p"  , "%p", (long *)0xabcd));
    TEST_FAIL(testCase<double *>("%p", "%p", (double *)0xabcd));
}

#endif

int main (int , char * [])
{
	BEGIN_TESTS(0);
    
//#
//#ifdef PFS_CC_MSC
//	safeformat::set_global_compat(safeformat::compat_msc);
//#else
//	safeformat::set_global_compat(safeformat::compat_gcc);
//#endif

    test_iterator<pfs::traits::stdcxx::string>();
    
#if __TODO__
	test0();
	test2();
    test3();
#endif    

//	if (argc > 1) {
//		if (1) test2();
//	} else {
//		if (1) test1();
//	}

	return END_TESTS;
}
