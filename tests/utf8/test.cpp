#include <pfs/test.hpp>

extern void test_iterator ();
extern void test_decode ();
extern void test_encode ();

int main ()
{
    BEGIN_TESTS(0);

    test_iterator();
    test_decode();
    test_encode();

    return END_TESTS;
}
