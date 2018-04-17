#include <pfs/test.hpp>
#include <pfs/thread.hpp>
#include <pfs/atomic.hpp>
#include <pfs/active_queue.hpp>
#include <pfs/memory.hpp>
#include <iostream>

namespace test {
namespace active_queue {
namespace consumer_producer {

using std::cout;
using std::endl;

typedef pfs::active_queue<pfs::deque
    , pfs::mutex
    , 256> active_queue_type;

static int const COUNT          = 10000;
static int const PRODUCER_COUNT = 10;
static int const CONSUMER_COUNT = 15;

static active_queue_type callbackq;
static pfs::atomic_int quit_flag(PRODUCER_COUNT);
static pfs::atomic_int counter(0);

void func1 ()
{
    ++counter;
}

class producer
{
public:
    producer () {}

    void run ()
    {
        for (int i = 0; i < COUNT; ++i) {
            callbackq.push_function(& func1);
        }

        --quit_flag;
    }
};

class consumer
{
public:
    consumer () {}

    void run ()
    {
        while (not (quit_flag.load() == 0 /*&& callbackq.empty()*/)) {
            callbackq.call_all();
        }
    }
};


void test ()
{
    ADD_TESTS(1)

    producer producers[PRODUCER_COUNT];
    consumer consumers[CONSUMER_COUNT];

    pfs::unique_ptr<pfs::thread> consumer_threads[CONSUMER_COUNT];
    pfs::unique_ptr<pfs::thread> producer_threads[PRODUCER_COUNT];

    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumer_threads[i] = pfs::make_unique<pfs::thread>(& consumer::run, & consumers[i]);
    }

    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        producer_threads[i] = pfs::make_unique<pfs::thread>(& producer::run, & producers[i]);
    }

    pfs::this_thread::sleep_for(pfs::chrono::milliseconds(10));


    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumer_threads[i]->join();
    }

    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        producer_threads[i]->join();
    }

    TEST_OK(counter.load() == COUNT * PRODUCER_COUNT);
}

}}} // test::active_queue::consumer_producer

