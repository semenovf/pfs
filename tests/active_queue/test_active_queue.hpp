/* 
 * File:   test_active_queue.hpp
 * Author: wladt
 *
 * Created on October 26, 2016, 4:19 PM
 */

#ifndef __TEST_ACTIVE_QUEUE_HPP__
#define __TEST_ACTIVE_QUEUE_HPP__

#include <cstdlib>
#include <pfs/test/test.hpp>
#include <pfs/types.hpp>
#include <pfs/limits.hpp>
#include <pfs/atomic.hpp>
#include <pfs/thread.hpp>
#include <pfs/traits/stdcxx/deque.hpp>
#include <pfs/active_queue.hpp>

#include <iostream>

using std::cout;
using std::endl;

struct null_ostream
{
    template <typename T>
    null_ostream & operator << (T a) { return *this; }
};

static null_ostream nout;

#define COUT nout
//#define COUT std::cout

namespace test {
namespace active_queue {

namespace test0 {

typedef pfs::active_queue<pfs::stdcxx::deque
    , pfs::mutex
    , 256> active_queue_type;

static int counter = 0;

void func ()
{
    ++counter;
}

void test ()
{
    ADD_TESTS(2);
    
    active_queue_type q;
    
    size_t max_count = 100;
    
    for (size_t i = 0; i < max_count; ++i) {
        q.push_function(& func);
    }
    
    max_count = q.count();
    q.call_all();
    
    TEST_OK(max_count > 0 && counter == max_count);
            //, _Sf("counter(%u) == max_count(%u)")(counter)(max_count).str().c_str());
    
    max_count = 100;
    
    active_queue_type q1;
    
    for (size_t i = 0; i < max_count; ++i) {
        q1.push_function(& func);
    }
    
    TEST_OK(q1.count() == max_count)
        //, _Sf("q1.count()=> %u == %u")(q1.count())(max_count).str().c_str());
}

} // test0

namespace test1 {

struct A 
{
    static size_t max_count;
    size_t counter;
    
    A () : counter(0) {}
    
    void method ()
    {
        ++counter;
    }
};

typedef pfs::active_queue<pfs::stdcxx::deque
    , pfs::mutex
    , 256> active_queue_type;

size_t A::max_count = 100;

void test ()
{
    ADD_TESTS(2);
    
    active_queue_type q;
    active_queue_type q1;
    A a;
    
    for (size_t i = 0; i < A::max_count; ++i) {
        q.push_method(& A::method, & a);
    }
    
    A::max_count = q.count();
    q.call_all();
    
    TEST_OK(A::max_count > 0 && a.counter == A::max_count);
            //, _Sf("counter(%u) == max_count(%u)")(a.counter)(A::max_count).str().c_str());

    A::max_count = 100;
    
    for (size_t i = 0; i < A::max_count; ++i) {
        q1.push_method(& A::method, & a);
    }
    
    TEST_OK(q1.count() == A::max_count);
        //, _Sf("q1.count()=> %u == %u")(q1.count())(A::max_count).str().c_str());
}

} // test1

namespace test2 {

typedef pfs::active_queue<pfs::stdcxx::deque
    , pfs::mutex
    , 256> active_queue_type;
active_queue_type q;

static int counter1 = 0;
static int counter2 = 0;
static int counter3 = 0;

void func1 ()
{
    ++counter1;
}

void func2 (int i)
{
    COUT << "func2(" << i << ")\n";
    ++counter2;
}

void func3 (int a, char b)
{
    COUT << "func3(" << a << ", " << b << ")\n";
    ++counter3;
}

static size_t random ()
{
    return double(std::rand())/double(pfs::numeric_limits<uint32_t>::max()) * 100;
}

void test ()
{
    ADD_TESTS(1);
    
    std::srand(0xACCABEAF);
    size_t limit = random();
    int max = pfs::numeric_limits<uint16_t>::max();
    
    for (int i = 0; i < max; ++i) {
        //COUT << "Push: " << i << endl;
        q.push_function(& func1);
        q.push_function(& func2, i);
        q.push_function(& func3, i, 'W');
        
        if (q.size() > limit) {
            q.call_all();
            limit = random();
        }
    }
    
    q.call_all();
    
    TEST_OK2(counter1 == max
                && counter2 == max
                && counter3 == max
            , "test::active_queue::test2 is ok");
}

} // test2

namespace test3 {

typedef pfs::active_queue<pfs::stdcxx::deque
    , pfs::mutex
    , 256> active_queue_type;

active_queue_type q;

static pfs::atomic_int is_finish(0);
static pfs::atomic_int result(0);

static pfs::atomic_size_t prod1_counter(0);
static pfs::atomic_size_t prod2_counter(0);
static pfs::atomic_size_t prod3_counter(0);

static pfs::atomic_size_t cons1_counter(0);
static pfs::atomic_size_t cons2_counter(0);
static pfs::atomic_size_t cons3_counter(0);

void func1 ()
{
    COUT << "func1()\n";
    ++cons1_counter;
}

void func2 (int i)
{
    COUT << "func2(" << i << ")\n";
    ++cons2_counter;
}

void func3 (int a, char b)
{
    COUT << "func3(" << a << ", " << b << ")\n";
    ++cons3_counter;
}

struct consumer
{
public:
	consumer () {} 

	void run()
	{
		while (! is_finish) {
            q.call_all();
        }
        
        // Wait some time to complete producers
        pfs::this_thread::sleep_for(pfs::chrono::milliseconds(5));
        q.call_all();
	}
};

class producer1
{
public:
	producer1 () {}

	void run()
	{
		while (! is_finish) {
            q.push_function(& func1);
            ++prod1_counter;
        }
	}
};

class producer2
{
public:
	producer2 () {}

	void run()
	{
        int i = 0;
        
		while (! is_finish) {
            q.push_function(& func2, i++);
            ++prod2_counter;
        }
	}
};

class producer3
{
public:
	producer3 () {}

	void run()
	{
        int i = 0;
        
		while (! is_finish) {
            q.push_function(& func3, i++, 'W');
            ++prod3_counter;
        }
	}
};


void test ()
{
    ADD_TESTS(1);
    
    consumer cons;
    producer1 prod1;
    producer2 prod2;
    producer3 prod3;

    pfs::thread t(& consumer::run, & cons);
    pfs::thread t1(& producer1::run, & prod1);
    pfs::thread t2(& producer2::run, & prod2);
    pfs::thread t3(& producer3::run, & prod3);

    pfs::this_thread::sleep_for(pfs::chrono::milliseconds(10));
    
    is_finish = 1;

    t.join();
    t1.join();
    t2.join();
    t3.join();
    
    TEST_OK2(result == 0
                && q.count() == 0
                && cons1_counter == prod1_counter
                && cons2_counter == prod2_counter
                && cons3_counter == prod3_counter
            , "test::active_queue::test3 is ok");
    
    cout << "\tresult = 0x" << std::hex << result.load() << std::dec << " (must be 0x0)" << endl;
    cout << "\tcons1_counter(" << cons1_counter.load() << ") "
         << (cons1_counter == prod1_counter ? "==" : "!=")
         << " prod1_counter(" << prod1_counter.load() << ")" << endl;

    cout << "\tcons2_counter(" << cons2_counter.load() << ") "
         << (cons2_counter == prod2_counter ? "==" : "!=")
         << " prod2_counter(" << prod2_counter.load() << ")" << endl;

    cout << "\tcons3_counter(" << cons3_counter.load() << ") "
         << (cons3_counter == prod3_counter ? "==" : "!=")
         << " prod3_counter(" << prod3_counter.load() << ")" << endl;
    
    cout << "\tq.count() == " << q.count() << " (must be 0)" << endl;
}

} // test3

namespace test4 {

typedef pfs::active_queue<pfs::stdcxx::deque
    , pfs::mutex
    , 256> active_queue_type;

static active_queue_type q;

static pfs::atomic_int is_finish(0);
static pfs::atomic_int result(0);

void func1 ()
{}

void func2 ()
{
    q.push_function(& func1);
}

class consumer
{
public:
	consumer () {}

	void run()
	{
		while (! is_finish) {
            q.call_all();
        }
	}
};

class producer
{
public:
	producer () {}

	void run()
	{
		while (! is_finish) {
            q.push_function(& func2);
        }
	}
};

void test ()
{
    ADD_TESTS(1);
    
    consumer cons;
    producer prod;

    pfs::thread t(& consumer::run, & cons);
    pfs::thread t1(& producer::run, & prod);
    
    pfs::this_thread::sleep_for(pfs::chrono::milliseconds(10));
    
    is_finish = 1;
    
    t1.join();
    t.join();
    
    if (result != 0)
        cout << "result = 0x" << std::hex << result.load() << endl;
    
    TEST_OK2(result == 0
            , "test::active_queue::test4 is ok");
}

} // test4

}} //test::active_queue

#endif /* __TEST_ACTIVE_QUEUE_HPP__ */
