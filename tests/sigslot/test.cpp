//#include <cstring>
#include <iostream>
#include <pfs/test/test.hpp>
#include <pfs/traits/stdcxx/deque.hpp>
#include <pfs/active_queue.hpp>
#include <pfs/sigslot.hpp>

typedef pfs::active_queue<pfs::stdcxx::deque> active_queue_type;

struct sync_slot_class : public pfs::has_slots<>
{
    sync_slot_class () : pfs::has_slots<>() {}

    void slot1 (int x)
    {
        std::cout << "Arg: " << x << std::endl;
    }
};

struct async_slot_class : public pfs::has_async_slots<active_queue_type>
{
    async_slot_class () : pfs::has_async_slots<active_queue_type>() {}
    
    void slot1 (int x)
    {
        std::cout << "Arg: " << x << std::endl;
    }
    
    void run () 
    {
        _queue.call_all();
    }
};

int main ()
{
    BEGIN_TESTS(0);
    
    pfs::signal1<int> sig1;
    async_slot_class async_slot;
    sync_slot_class sync_slot;
    sig1.connect(& async_slot, & async_slot_class::slot1);
    sig1.connect(& sync_slot, & sync_slot_class::slot1);
    
    std::cout << "Emit signal and call sync slot" << std::endl;
    sig1(10);
    
    std::cout << "Real call async slot now" << std::endl;
    async_slot.run();

    return END_TESTS;
}
