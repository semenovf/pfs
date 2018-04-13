#pragma once
#include <string>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <pfs/operationsystem.hpp>
#include <pfs/assert.hpp>

#if PFS_OS_WIN
    // [http://support.microsoft.com/kb/815668](http://support.microsoft.com/kb/815668)
    // [QueryPerformanceCounter - бомба замедленного действия](http://devdoc.web-ide.ru/index.php/content/view/queryPerformanceCounter.htm)
#   include <windows.h>
#else
#   include <sys/time.h>
#endif

#define BEGIN_TESTS(n)   pfs::test::start(n);
#define END_TESTS        pfs::test::finish(false);
#define ABORT_TESTS      pfs::test::finish(true);
#define ADD_TESTS(n)     pfs::test::add_total(n);
#define ADD_TESTS_X(n,t) pfs::test::add_total_x(n,t);

#define TEST_OK(expr) if ((expr)) {                          \
        pfs::test::todo(#expr, true, __FILE__, __LINE__);    \
    } else {                                                 \
        pfs::test::todo(#expr, false, __FILE__, __LINE__);   \
    }

#define TEST_OK_X(expr, stmt) if ((expr)) {                  \
        pfs::test::todo(#expr, true, __FILE__, __LINE__);    \
    } else {                                                 \
        pfs::test::todo(#expr, false, __FILE__, __LINE__);   \
        stmt;                                                \
    }

#define TEST_OK2(expr,desc) if ((expr)) {                    \
        pfs::test::todo(desc, true, __FILE__, __LINE__);     \
    } else {                                                 \
        pfs::test::todo(desc, false, __FILE__, __LINE__);    \
    }

#define TEST_FAIL(expr) if ((expr)) {                        \
        pfs::test::todo(#expr, true, __FILE__, __LINE__);    \
    } else {                                                 \
        pfs::test::todo(#expr, false, __FILE__, __LINE__);   \
        ABORT_TESTS                                          \
    }

#define TEST_FAIL_X(expr,stmt) if ((expr)) {                 \
        pfs::test::todo(#expr, true, __FILE__, __LINE__);    \
    } else {                                                 \
        pfs::test::todo(#expr, false, __FILE__, __LINE__);   \
        stmt;                                                \
        ABORT_TESTS                                          \
    }

#define TEST_FAIL2(expr,desc) if ((expr)) {                  \
        pfs::test::todo(desc, true, __FILE__, __LINE__);     \
    } else {                                                 \
        pfs::test::todo(desc, false, __FILE__, __LINE__);    \
        ABORT_TESTS                                          \
    }

namespace pfs {

struct test
{
    class profiler
    {
        double _sec;

    public:
        profiler () : _sec(0) { start(); }

        void start ()
        {
#if PFS_OS_WIN
            LARGE_INTEGER c;
            PFS_ASSERT(QueryPerformanceCounter(& c));
            _sec = static_cast<double> (c.QuadPart);
#else
            struct timeval now = {0, 0};
            PFS_ASSERT(gettimeofday(& now, 0) == 0);
            _sec = static_cast<double>(now.tv_sec) + static_cast<double>(now.tv_usec)/1000000;
#endif
        }

        double ellapsed () const
        {
#if PFS_OS_WIN
            LARGE_INTEGER c, freq;
            PFS_ASSERT(QueryPerformanceCounter(& c));
            PFS_ASSERT(QueryPerformanceFrequency(& freq));
            return (static_cast<double> (c.QuadPart) - _sec) / static_cast<double> (freq.QuadPart);
#else
            struct timeval now = {0, 0};
            PFS_ASSERT(gettimeofday(& now, 0) == 0);
            return (static_cast<double>(now.tv_sec) + static_cast<double>(now.tv_usec)/1000000) - _sec;
#endif
        }
    };

    class context
    {
    public:
        context ()
            : _total(0)
            , _processed(0)
            , _passed(0)
            , _failed(0)
            , _profiler() // start/stopwatch
        {}

        ~context() {}

        size_t total     () const { return _total; }
        size_t processed () const { return _processed; }
        size_t passed    () const { return _passed; }
        size_t failed    () const { return _failed; }

        void add_total (size_t n) { _total += n; }
        void test_ok ()      { ++_processed; ++_passed; }
        void test_failed ()  { ++_processed; ++_failed; }

        // 0 - success
        // 1 - failed
        // 2 - incomplete
        int  result () const
        {
                return _failed > 0
                    ? 1
                    : _processed == _total ? 0 : 2;
        }

        std::string output_prefix (bool passed) const
        {
            std::stringstream ss;
            ss << stringify_time(_profiler.ellapsed())
                << (passed ? " PASS[" : " FAIL[")
                << std::setw(3) << std::setfill('0') << _processed
                << "]: ";

            return ss.str();
        }

        double elapsed_time() const
        {
            return _profiler.ellapsed();
        }

        static std::string stringify_time (double t)
        {
            std::stringstream ss;
            long sec, usec;

            sec = static_cast<long>(t);
            usec = static_cast<long>((t - long(t)) * 1000000);

            ss << std::setw(6) << std::setfill('0') << sec << '.'
                    << std::setw(6) << std::setfill('0') << usec;

            return ss.str();
        }

    private:
        size_t   _total;
        size_t   _processed;
        size_t   _passed;
        size_t   _failed;
        profiler _profiler;
    };

    static context & get_context ()
    {
        static context * ctx = new context();
        return *ctx;
    }

    static context & free_context ()
    {
        static context * ctx = new context();
        delete & get_context();
    }

    static void start (size_t n)
    {
        get_context().add_total(n);
    }

    static int finish (bool abort)
    {
        context & ctx = get_context();
        double elapsed = ctx.elapsed_time();

        std::cout.precision(6);
        std::cout << "-----------------------------------" << std::endl
                    << "Results: total=" << ctx.total()
                    << ", run="          << ctx.processed()
                    << ", ok="           << ctx.passed()
                    << ", failed="       << ctx.failed() << std::endl;

        if (ctx.total() != ctx.processed())
            std::cerr << "WARN: Incomplete tests" << std::endl;

        std::cout << "Elapsed time: " << elapsed << " seconds" << std::endl;
        std::cout << "Total result: "
                    << (ctx.failed() > 0
                        ? "FAILURE"
                        : (ctx.processed() == ctx.total()
                                ? "SUCCESS"
                                : "INCOMPLETE"))
                    << std::endl;

        int r = ctx.result();
        free_context();

        if (abort)
            ::exit(r);

        return r;

    }

    static void add_total (size_t n)
    {
        get_context().add_total(n);
    }

    static void add_total_x (size_t n, std::string const & caption)
    {
        get_context().add_total(n);
        std::cout << get_context().output_prefix(true) << "*** " << caption << std::endl;
    }

    static void todo (std::string const & expr, bool result, std::string const & filename, int line)
    {
        context & ctx = get_context();
        if (result) {
            ctx.test_ok();
            std::cout << ctx.output_prefix(true) << expr << std::endl;
        } else {
            ctx.test_failed();
            std::cout << ctx.output_prefix(false) << expr << " ["
                    << filename << ':' << line << ']' << std::endl;
        }
    }
}; // test

} // pfs
