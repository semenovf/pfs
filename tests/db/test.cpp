#include "pfs/test.hpp"

extern void sqlite3_test0 ();
extern void sqlite3_test1 ();

int main ()
{
    BEGIN_TESTS(0);

    sqlite3_test0();
    sqlite3_test1();

    return END_TESTS;
}
