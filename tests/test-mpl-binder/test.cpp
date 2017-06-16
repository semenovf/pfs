#include "pfs/test/test.hpp"
#include "pfs/cxx/cxx98/binder.hpp"
#include <iostream>

static int func1_var = 0;

static void func1 ()
{
	func1_var = 1234;
}

static int func2 (int a1, int a2)
{
	return a1 + a2;
}

class A
{
public:
	int num (int a)
	{
		return a;
	}
};


class B
{
public:
	int sum (int a, int b)
	{
		return a + b;
	}
};

void check1 ()
{
	pfs::binder_function0<void> f1(& func1);
	f1();
	TEST_OK(func1_var == 1234);

	pfs::binder_function2<int, int, int> f2(& func2, 10, 20);
	TEST_OK(f2() == 30);

	A a;
	pfs::binder_method1<A, int, int> f3(& A::num, & a, 1234);
	TEST_OK(f3() == 1234);
}

int main (int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(3);

	check1();

	return END_TESTS;
}
