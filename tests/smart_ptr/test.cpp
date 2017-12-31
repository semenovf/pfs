#include <cstdio>
#include <pfs/test/test.hpp>
#include <pfs/memory.hpp>
#include <pfs/debug.hpp>
#include <pfs/assert.hpp>
//
// This unit tests base on BOOST's shared_ptr unit tests.
//
struct Foo
{
	Foo(int aVal) :
        m_val(aVal)
    {
        ++g_instances;
    }
    ~Foo(void)
    {
        --g_instances;
    }
    void incr(void)
    {
        ++m_val;
    }
    void decr(void)
    {
        --m_val;
    }

    int m_val;
    static int g_instances;
};

int Foo::g_instances = 0;


void test_empty_ptr()
{
    ADD_TESTS(10);

    // Create an empty (ie. NULL) shared_ptr
    pfs::shared_ptr<Foo> xPtr;

    //TEST_OK(false == xPtr);
    TEST_OK(false == xPtr.unique());
    TEST_OK(0 == xPtr.use_count());
    TEST_OK(NULL == xPtr.get());

    TEST_FAIL(!xPtr); // bool cast operator error";

    // Reset to NULL (ie. do nothing)
    xPtr.reset();

    //TEST_OK(false == xPtr);
    TEST_OK(false == xPtr.unique());
    TEST_OK(0 == xPtr.use_count());
    TEST_OK(NULL == xPtr.get());

    // sub-scope
    {
        ADD_TESTS(12);
        // Copy construct the empty (ie. NULL) shared_ptr
    	pfs::shared_ptr<Foo> yPtr(xPtr);

        //TEST_OK(false == xPtr);
        TEST_OK(false == xPtr.unique());
        TEST_OK(0 == xPtr.use_count());
        TEST_OK(NULL == xPtr.get());
        TEST_OK(false == yPtr.unique());
        TEST_OK(0 == yPtr.use_count());
        TEST_OK(NULL == yPtr.get());

        // Assign the empty (ie. NULL) shared_ptr
        pfs::shared_ptr<Foo> zPtr;
        zPtr = xPtr;

        //TEST_OK(false == xPtr);
        TEST_OK(false == xPtr.unique());
        TEST_OK(0 == xPtr.use_count());
        TEST_OK(NULL == xPtr.get());
        TEST_OK(false == zPtr.unique());
        TEST_OK(0 == zPtr.use_count());
        TEST_OK(NULL == zPtr.get());
    }
    // end of scope

    //TEST_OK(false == xPtr);
    TEST_OK(false == xPtr.unique());
    TEST_OK(0 == xPtr.use_count());
    TEST_OK(NULL == xPtr.get());
}

void test_basic_ptr()
{
    ADD_TESTS(1);

    {
        ADD_TESTS(9);

        // Create a shared_ptr
    	pfs::shared_ptr<Foo> xPtr(new Foo(123));

        //TEST_OK(true == xPtr);
        TEST_OK(true == xPtr.unique());
        TEST_OK(1 == xPtr.use_count());
        TEST_OK(NULL != xPtr.get());

        if (xPtr) {
            ADD_TESTS(21);

            TEST_OK(123 == xPtr->m_val);
            TEST_OK(1 == xPtr->g_instances);
            TEST_OK(1 == Foo::g_instances);

            // call a function
            xPtr->incr();
            TEST_OK(124 == xPtr->m_val);
            (*xPtr).incr();
            TEST_OK(125 == (*xPtr).m_val);
            xPtr->decr();
            xPtr->decr();

            // Copy construct the shared_ptr
            pfs::shared_ptr<Foo> yPtr(xPtr);

            TEST_OK(xPtr == yPtr);
            //TEST_OK(true == xPtr);
            TEST_OK(false == xPtr.unique());
            TEST_OK(2 == xPtr.use_count());
            TEST_OK(NULL != xPtr.get());
            TEST_OK(123 == xPtr->m_val);
            TEST_OK(1 == Foo::g_instances);
            //TEST_OK(true == yPtr);
            TEST_OK(false == yPtr.unique());
            TEST_OK(2 == yPtr.use_count());
            TEST_OK(NULL != yPtr.get());
            TEST_OK(123 == yPtr->m_val);
            TEST_OK(1 == Foo::g_instances);

            if (yPtr) {
                ADD_TESTS(16);

                // Assign the shared_ptr
            	pfs::shared_ptr<Foo> zPtr;
                zPtr = xPtr;

                TEST_OK(xPtr == zPtr);
                //TEST_OK(true == xPtr);
                TEST_OK(false == xPtr.unique());
                TEST_OK(3 == xPtr.use_count());
                TEST_OK(NULL != xPtr.get());
                TEST_OK(123 == xPtr->m_val);
                TEST_OK(1 == Foo::g_instances);
                //TEST_OK(true == yPtr);
                TEST_OK(false == yPtr.unique());
                TEST_OK(3 == yPtr.use_count());
                TEST_OK(NULL != yPtr.get());
                TEST_OK(123 == yPtr->m_val);
                TEST_OK(1 == Foo::g_instances);
                //TEST_OK(true == zPtr);
                TEST_OK(false == zPtr.unique());
                TEST_OK(3 == zPtr.use_count());
                TEST_OK(NULL != zPtr.get());
                TEST_OK(123 == zPtr->m_val);
                TEST_OK(1 == Foo::g_instances);
            }

            //TEST_OK(true == xPtr);
            TEST_OK(false == xPtr.unique());
            TEST_OK(2 == xPtr.use_count());
            TEST_OK(NULL != xPtr.get());
            TEST_OK(123 == xPtr->m_val);
            TEST_OK(1 == Foo::g_instances);
        }

        TEST_FAIL(xPtr); // bool cast operator error

        //TEST_OK(true == xPtr);
        TEST_OK(true == xPtr.unique());
        TEST_OK(1 == xPtr.use_count());
        TEST_OK(NULL != xPtr.get());
        TEST_OK(123 == xPtr->m_val);
        TEST_OK(1 == Foo::g_instances);
    }

    TEST_OK(0 == Foo::g_instances);
}

void test_reset_ptr()
{
    ADD_TESTS(15);

    // Create an empty (ie. NULL) shared_ptr
    pfs::shared_ptr<Foo> xPtr;

    // Reset it with a new pointer
    xPtr.reset(new Foo(123));

    //TEST_OK(true == xPtr);
    TEST_OK(true == xPtr.unique());
    TEST_OK(1 == xPtr.use_count());
    TEST_OK(NULL != xPtr.get());
    TEST_OK(123 == xPtr->m_val);
    TEST_OK(1 == Foo::g_instances);
    Foo* pX = xPtr.get();

    // Reset it with another new pointer
    xPtr.reset(new Foo(234));

    //TEST_OK(true == xPtr);
    TEST_OK(true == xPtr.unique());
    TEST_OK(1 == xPtr.use_count());
    TEST_OK(NULL != xPtr.get());
    TEST_OK(234 == xPtr->m_val);
    TEST_OK(1 == Foo::g_instances);
    TEST_OK(pX != xPtr.get());

    // Reset to NULL
    xPtr.reset();

    TEST_OK(false == xPtr.unique());
    TEST_OK(0 == xPtr.use_count());
    TEST_OK(NULL == xPtr.get());
    TEST_OK(0 == Foo::g_instances);
}

void test_compare_ptr()
{
    ADD_TESTS(33);

    // Create a shared_ptr
    pfs::shared_ptr<Foo> xPtr(new Foo(123));

    //TEST_OK(true == xPtr);
    TEST_OK(true == xPtr.unique());
    TEST_OK(1 == xPtr.use_count());
    TEST_OK(NULL != xPtr.get());
    TEST_OK(123 == xPtr->m_val);
    TEST_OK(1 == Foo::g_instances);
    Foo* pX = xPtr.get();

    // Create another shared_ptr
    pfs::shared_ptr<Foo> yPtr(new Foo(234));

    //TEST_OK(true == xPtr);
    TEST_OK(true == xPtr.unique());
    TEST_OK(1 == xPtr.use_count());
    TEST_OK(NULL != xPtr.get());
    TEST_OK(123 == xPtr->m_val);
    TEST_OK(2 == Foo::g_instances);

    //TEST_OK(true == yPtr);
    TEST_OK(true == yPtr.unique());
    TEST_OK(1 == yPtr.use_count());
    TEST_OK(NULL != yPtr.get());
    TEST_OK(234 == yPtr->m_val);
    Foo* pY = yPtr.get();

    TEST_OK(xPtr.get() != pY);
    TEST_OK(xPtr != yPtr);
    if (pX < pY)
    {
        TEST_OK(xPtr.get() < pY);
        TEST_OK(xPtr < yPtr);
        TEST_OK(xPtr.get() <= pY);
        TEST_OK(xPtr <= yPtr);
        TEST_OK(yPtr.get() > pX);
        TEST_OK(yPtr > xPtr);
        TEST_OK(yPtr.get() >= pX);
        TEST_OK(yPtr >= xPtr);
    }
    else // (pX > pY)
    {
        TEST_OK(xPtr.get() > pY);
        TEST_OK(xPtr > yPtr);
        TEST_OK(xPtr.get() >= pY);
        TEST_OK(xPtr >= yPtr);
        TEST_OK(yPtr.get() < pX);
        TEST_OK(yPtr < xPtr);
        TEST_OK(yPtr.get() <= pX);
        TEST_OK(yPtr <= xPtr);
    }

    // Copy a shared_ptr
    pfs::shared_ptr<Foo> zPtr = xPtr;
    Foo* pZ = zPtr.get();

    TEST_OK(pX == pZ);
    TEST_OK(xPtr.get() == pZ);
    TEST_OK(xPtr == zPtr);
    TEST_OK(zPtr.get() == pX);
    TEST_OK(zPtr == xPtr);
    TEST_OK(xPtr.get() >= pZ);
    TEST_OK(xPtr >= zPtr);
    TEST_OK(xPtr.get() <= pZ);
    TEST_OK(xPtr <= zPtr);
}

void test_swap_ptr()
{
    ADD_TESTS(12);

    // Create a shared_ptr
    pfs::shared_ptr<Foo> xPtr(new Foo(123));

    //TEST_OK(true == xPtr);
    TEST_OK(true == xPtr.unique());
    TEST_OK(1 == xPtr.use_count());
    TEST_OK(NULL != xPtr.get());
    TEST_OK(123 == xPtr->m_val);
    TEST_OK(1 == Foo::g_instances);
    Foo* pX = xPtr.get();

    // Create another shared_ptr
    pfs::shared_ptr<Foo> yPtr(new Foo(234));

    //TEST_OK(true == yPtr);
    TEST_OK(true == yPtr.unique());
    TEST_OK(1 == yPtr.use_count());
    TEST_OK(NULL != yPtr.get());
    TEST_OK(234 == yPtr->m_val);
    TEST_OK(2 == Foo::g_instances);
    Foo* pY = yPtr.get();

    if (pX < pY)
    {
        TEST_OK(xPtr < yPtr);
        xPtr.swap(yPtr);
        TEST_OK(xPtr > yPtr);
        //TEST_OK(true == xPtr);
        //TEST_OK(true == yPtr);
    }
    else // (pX > pY)
    {
        TEST_OK(xPtr > yPtr);
        xPtr.swap(yPtr);
        TEST_OK(xPtr < yPtr);
        //TEST_OK(true == xPtr);
        //TEST_OK(true == yPtr);
    }
}

void test_unique()
{
    ADD_TESTS(2);

    pfs::shared_ptr<int> sp1(new int(5));
    TEST_OK(sp1.unique() == true);

    pfs::shared_ptr<int> sp2(sp1);
    TEST_OK(sp1.unique() == false);

}

void f1 (pfs::shared_ptr<int> p)
{
	pfs::shared_ptr<int> d(new int(4321));
	p.swap(d);
	std::cout << *p << std::endl;
}

void f2 (pfs::shared_ptr<int> p)
{
	f1(p);
	std::cout << *p << std::endl;
}

void f3 (pfs::shared_ptr<int> p)
{
	f2(p);
	std::cout << *p << std::endl;
}

void test_deep_swap_ptr ()
{
	ADD_TESTS(2);

	pfs::shared_ptr<int> p(new int(1234));
	TEST_OK(*p == 1234)

	f3(p);

	TEST_OK(*p == 4321)
}

void test_cast ()
{
    ADD_TESTS(4);

    pfs::shared_ptr<int> p1(new int);
    pfs::shared_ptr<int> p2 = pfs::static_pointer_cast<int>(p1);

    TEST_OK(p1.use_count() == 2);
    TEST_OK(p2.use_count() == 2);

    pfs::shared_ptr<int> p3(new int);
    pfs::shared_ptr<int> p4 = pfs::reinterpret_pointer_cast<int>(p3);

    TEST_OK(p3.use_count() == 2);
    TEST_OK(p4.use_count() == 2);
}


int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(0);

	test_empty_ptr();
	test_basic_ptr();
	test_reset_ptr();
	test_compare_ptr();
	test_swap_ptr();
	test_unique();
    test_cast();
//	test_deep_swap_ptr();

	return END_TESTS;
}
