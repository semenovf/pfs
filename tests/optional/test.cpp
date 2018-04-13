#include <pfs/test.hpp>
#include <pfs/optional.hpp>

int main ()
{
    BEGIN_TESTS(7);

    pfs::optional<int> o1,       // empty
                       o2 = 1,   // init from rvalue
                       o3 = o2;  // copy-constructor

    TEST_OK(!o1.has_value()); // [1]
    TEST_OK(!o1);            // [2] indentical with [1]
    TEST_OK(o1.value_or(10) == 10);
    TEST_OK(o2.has_value());
    TEST_OK(*o2 == 1);
    TEST_OK(o3.has_value());
    TEST_OK(*o3 == 1);


    return END_TESTS;
}
