#include <iostream>
#include <pfs/test.hpp>
#include <pfs/deque.hpp>
#include <pfs/active_queue.hpp>
#include <pfs/sigslot.hpp>
#include "../catch.hpp"

// class F
// {
//     void method () {}
//     void method (int) {}
//     void method (int, int) {}
//     void method (int, int, int) {}
//     void method (int, int, int, int) {}
//     void method (int, int, int, int, int) {}
//     void method (int, int, int, int, int, int) {}
//     void method (int, int, int, int, int, int, int) {}
//     void method (int, int, int, int, int, int, int, int) {}
// };
// 
// 
// TEST_CASE("fake_active_queue") {
//     pfs::fake_active_queue faq;
//     F f;
//     faq.push_method(F::method, & f);
//     faq.push_method(F::method, & f, 10);
// }

// typedef pfs::active_queue<pfs::deque> active_queue_type;
// typedef pfs::sigslot<active_queue_type, active_queue_type::mutex_type> sigslot_ns;
// 
// struct sync_slot_class : public sigslot_ns::has_slots
// {
//     sync_slot_class () : sigslot_ns::has_slots() {}
// 
//     void slot1 (int x)
//     {
//         std::cout << "Arg: " << x << std::endl;
//     }
// };
// 
// struct async_slot_class : public sigslot_ns::has_async_slots
// {
//     async_slot_class () : sigslot_ns::has_async_slots() {}
// 
//     void slot1 (int x)
//     {
//         std::cout << "Arg: " << x << std::endl;
//     }
// 
//     void run ()
//     {
//         callback_queue().call_all();
//     }
// };
// 
// int main ()
// {
//     BEGIN_TESTS(0);
// 
//     sigslot_ns::signal1<int> sig1;
//     async_slot_class async_slot;
//     sync_slot_class sync_slot;
//     sig1.connect(& async_slot, & async_slot_class::slot1);
//     sig1.connect(& sync_slot, & sync_slot_class::slot1);
// 
//     std::cout << "Emit signal and call sync slot" << std::endl;
//     sig1(10);
// 
//     std::cout << "Real call async slot now" << std::endl;
//     async_slot.run();
// 
//     return END_TESTS;
// }
