#include <pfs/test/test.hpp>

extern void test_iterator ();
extern void test_decode ();
extern void test_encode ();

int main (int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(0);

    test_iterator();
    test_decode();
    test_encode();

    return END_TESTS;
}
