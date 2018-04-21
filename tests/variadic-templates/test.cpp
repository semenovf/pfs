#include <cstdio>
#include <pfs/test.hpp>

#if __cplusplus >= 201103L

namespace test1 {

struct executor
{
    executor () {}

    template <typename C, typename ...Args>
    void exec (int (C::*func) (Args...), C * c,  Args... args)
    {
        (c->*func)(args...);
    }
};

struct functor
{
    int f () { std::printf("functor::f()\n"); }
    int f (int a) { std::printf("functor::f(int a = %d)\n", a); }
    int f (int a, int b) { std::printf("functor::f(int a = %d, int b = %d)\n", a, b); }
    int f (int a, int b, int c) { std::printf("functor::f(int a = %d, int b = %d, int c = %d)\n", a, b, c); }
};

} // test1

namespace test2 {

    template <typename ...Args>
    class _connection_base
    {
    public:
        virtual ~_connection_base () {}
        virtual void emit(Args...) = 0;
        virtual _connection_base<Args...> * clone() = 0;
    };

    class _signal_base
    {
    public:
        virtual ~_signal_base() {}
    };

    class basic_has_slots
    {
    public:
        basic_has_slots()
        {}

        void signal_connect(_signal_base *)
        {}

        void signal_disconnect(_signal_base *)
        {}

        virtual ~basic_has_slots()
        {}
    };

    class has_slots : public basic_has_slots
    {
    public:
        has_slots () : basic_has_slots () {}
    };

    class has_async_slots : public basic_has_slots
    {
    public:
        has_async_slots () : basic_has_slots () {}
    };

    template <typename ...Args>
    class _signal_base1 : public _signal_base
    {
    public:
        //typedef std::list<_connection_base1<Args...> *>  connections_list;

        _signal_base1()
        {}

        ~_signal_base1()
        {}
    };

    template <class dest_type, typename ...Args>
    class _connection : public _connection_base<Args...>
    {
    public:
        _connection () {}

        virtual _connection_base<Args...> * clone()
        {
            return new _connection<dest_type, Args...>(*this);
        }

        virtual void emit(Args... args)
        {}
    };

    template <typename ...Args>
    class signal : public _signal_base1<Args...>
    {
        using base_class = _signal_base1<Args...>;

    public:
        signal()
        {}

        signal(signal<Args...> const & s)
            : base_class(s)
        {}

        template <typename Slot>
        void connect (Slot * pslot, void (Slot::*func)(Args...))
        {
            std::printf("Connected slot with %u arguments\n", static_cast<unsigned int>(sizeof...(Args)));
        }
    };

    struct slots : public has_slots
    {
        void f () { std::printf("slots::f()\n"); }
        void f (int a) { std::printf("slots::f(int a = %d)\n", a); }
        void f (int a, int b) { std::printf("slots::f(int a = %d, int b = %d)\n", a, b); }
        void f (int a, int b, int c) { std::printf("slots::f(int a = %d, int b = %d, int c = %d)\n", a, b, c); }
    };
}


int main ()
{
    BEGIN_TESTS(0);

    test1::functor f;
    test1::executor().exec(& test1::functor::f, & f);
    test1::executor().exec(& test1::functor::f, & f, 1);
    test1::executor().exec(& test1::functor::f, & f, 1, 2);
    test1::executor().exec(& test1::functor::f, & f, 1, 2, 3);

    test2::slots slots;
    test2::signal<>().connect(& slots, & test2::slots::f);
    test2::signal<int>().connect(& slots, & test2::slots::f);
    test2::signal<int, int>().connect(& slots, & test2::slots::f);
    test2::signal<int, int, int>().connect(& slots, & test2::slots::f);

    return END_TESTS;
}

#else

int main ()
{
    BEGIN_TESTS(0);
    // Nothing to do
    return END_TESTS;
}

#endif
