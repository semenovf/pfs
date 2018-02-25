#include <pfs/iterator.hpp>

struct B
{
    int x;
};

struct output_iterator : public pfs::iterator_facade<
          pfs::output_iterator_tag
        , output_iterator
        , B, B *, int &>
{
    B * _p;

    output_iterator (output_iterator const & rhs)
        : _p(rhs._p)
    {}

    output_iterator (B * p)
        : _p(p)
    {}

    static reference ref (output_iterator & it)
    {
        reference r = it._p->x;
        it._p++;
        return r;
    }
};

void test_output_iterator ()
{
    ADD_TESTS_X(4, "Test Output Iterator");

    B data[4];

    {
        output_iterator it(data);
        *it++ = 1;
        *it++ = 2;
        *it++ = 3;
        *it++ = 4;
        TEST_OK(data[0].x == 1);
        TEST_OK(data[1].x == 2);
        TEST_OK(data[2].x == 3);
        TEST_OK(data[3].x == 4);
    }
}
