#include "pfs/test.hpp"

extern void sqlite3_test ();

int main ()
{
    BEGIN_TESTS(0);

    sqlite3_test();

    return END_TESTS;
}
