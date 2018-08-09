#include "pfs/test.hpp"

extern void sqlite3_test0 ();
extern void sqlite3_test1 ();
//extern void sqlite3_statement ();

int main ()
{
    BEGIN_TESTS(0);

    sqlite3_test0();
    sqlite3_test1();
    //sqlite3_statement();

    return END_TESTS;
}
