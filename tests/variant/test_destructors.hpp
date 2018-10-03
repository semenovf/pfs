#pragma once
#include <string>

void test_destructors ()
{
    ADD_TESTS(1);

    std::cout << "///////////////////////////////////////////////////////////////////////////\n";
    std::cout << "//                             Destructors                               //\n";
    std::cout << "///////////////////////////////////////////////////////////////////////////\n";

    struct Obj {
        Obj(bool & dtor_called) : dtor_called_(dtor_called) {}

        ~Obj() {
            dtor_called_ = true;
        }
        bool & dtor_called_;
    };  // Obj

    bool dtor_called = false;

    {
        pfs::variant<Obj> v(pfs::in_place_type_t<Obj>(), dtor_called);
    }

    TEST_OK2(dtor_called, "Destructor called");
}

