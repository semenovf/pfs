#include <pfs/test/test.hpp>
#include <pfs/algo/between.hpp>

static void test_int_defaults ()
{
    ADD_TESTS(22);

    TEST_OK(pfs::between_inclusive( 5, 0, 10));
    TEST_OK(pfs::between_inclusive( 0, 0, 10));
    TEST_OK(pfs::between_inclusive(10, 0, 10));
    TEST_OK(!pfs::between_inclusive(-1, 0, 10));
    TEST_OK(!pfs::between_inclusive(11, 0, 10));

    TEST_OK(pfs::between_exclusive( 1, 0, 10));
    TEST_OK(pfs::between_exclusive( 9, 0, 10));
    TEST_OK(!pfs::between_exclusive(0, 0, 10));
    TEST_OK(!pfs::between_exclusive(10, 0, 10));
    TEST_OK(!pfs::between_exclusive(-1, 0, 10));
    TEST_OK(!pfs::between_exclusive(11, 0, 10));

    TEST_OK(pfs::between_exclusive_min( 1, 0, 10));
    TEST_OK(pfs::between_exclusive_min( 9, 0, 10));
    TEST_OK(pfs::between_exclusive_min(10, 0, 10));
    TEST_OK(!pfs::between_exclusive_min(0, 0, 10));
    TEST_OK(!pfs::between_exclusive_min(-1, 0, 10));
    TEST_OK(!pfs::between_exclusive_min(11, 0, 10));

    TEST_OK(pfs::between_exclusive_max( 0, 0, 10));
    TEST_OK(pfs::between_exclusive_max( 1, 0, 10));
    TEST_OK(pfs::between_exclusive_max( 9, 0, 10));
    TEST_OK(!pfs::between_exclusive_max(10, 0, 10));
    TEST_OK(!pfs::between_exclusive_max(11, 0, 10));
}

struct IntLessEq
{
    bool operator () (int const & x, int const & y) const
    {
        return x <= y;
    }
};

struct IntLess
{
    bool operator () (int const & x, int const & y) const
    {
        return x < y;
    }
};

static void test_int_custom ()
{
    ADD_TESTS(22);

    TEST_OK((pfs::between_inclusive<int, IntLessEq>( 5, 0, 10)));
    TEST_OK((pfs::between_inclusive<int, IntLessEq>( 0, 0, 10)));
    TEST_OK((pfs::between_inclusive<int, IntLessEq>(10, 0, 10)));
    TEST_OK(!(pfs::between_inclusive<int, IntLessEq>(-1, 0, 10)));
    TEST_OK(!(pfs::between_inclusive<int, IntLessEq>(11, 0, 10)));

    TEST_OK((pfs::between_exclusive<int, IntLess>( 1, 0, 10)));
    TEST_OK((pfs::between_exclusive<int, IntLess>( 9, 0, 10)));
    TEST_OK(!(pfs::between_exclusive<int, IntLess>(0, 0, 10)));
    TEST_OK(!(pfs::between_exclusive<int, IntLess>(10, 0, 10)));
    TEST_OK(!(pfs::between_exclusive<int, IntLess>(-1, 0, 10)));
    TEST_OK(!(pfs::between_exclusive<int, IntLess>(11, 0, 10)));

    TEST_OK((pfs::between_exclusive_min<int, IntLess, IntLessEq>(1, 0, 10)));
    TEST_OK((pfs::between_exclusive_min<int, IntLess, IntLessEq>(9, 0, 10)));
    TEST_OK((pfs::between_exclusive_min<int, IntLess, IntLessEq>(10, 0, 10)));
    TEST_OK(!(pfs::between_exclusive_min<int, IntLess, IntLessEq>(0, 0, 10)));
    TEST_OK(!(pfs::between_exclusive_min<int, IntLess, IntLessEq>(-1, 0, 10)));
    TEST_OK(!(pfs::between_exclusive_min<int, IntLess, IntLessEq>(11, 0, 10)));

    TEST_OK((pfs::between_exclusive_max<int, IntLess, IntLessEq>( 0, 0, 10)));
    TEST_OK((pfs::between_exclusive_max<int, IntLess, IntLessEq>( 1, 0, 10)));
    TEST_OK((pfs::between_exclusive_max<int, IntLess, IntLessEq>( 9, 0, 10)));
    TEST_OK(!(pfs::between_exclusive_max<int, IntLess, IntLessEq>(10, 0, 10)));
    TEST_OK(!(pfs::between_exclusive_max<int, IntLess, IntLessEq>(11, 0, 10)));
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    BEGIN_TESTS(0);

    test_int_defaults();
    test_int_custom();

    return END_TESTS;
}

