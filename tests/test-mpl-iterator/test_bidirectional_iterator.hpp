#include <pfs/iterator.hpp>

struct D
{
    int x;
};

struct bidirectional_iterator : public pfs::iterator_facade<
          pfs::bidirectional_iterator_tag
        , bidirectional_iterator
        , D, D *, D &>
{
    D * _p;

    bidirectional_iterator (D * p)
        : _p(p)
    {}

    static reference ref (bidirectional_iterator & it)
    {
        return *it._p;
    }
    
    static pointer ptr (bidirectional_iterator & it)
    {
        return it._p;
    }
    
    static void increment (bidirectional_iterator & it, difference_type)
    {
        ++it._p;
    }

    static int compare (bidirectional_iterator const & it1, bidirectional_iterator const & it2)
    {
        return it1._p == it2._p ? 0 : 1;
    }
    
    static void decrement (bidirectional_iterator & it, difference_type)
    {
        --it._p;
    }
};

void test_bidirectional_iterator ()
{
    ADD_TESTS_X(20, "Test Bidirectional Iterator");
    
    D data[] = { {1}, {2}, {3}, {4} };

    // Test postfix increment
    {
        bidirectional_iterator it(data);

        TEST_OK((*it++).x == 1);
        TEST_OK((*it++).x == 2);
        TEST_OK((*it++).x == 3);
        TEST_OK((*it++).x == 4);
    }

    // Test prefix increment
    {
        bidirectional_iterator it(data);
        TEST_OK((*it).x == 1);
        TEST_OK((*++it).x == 2);
        TEST_OK((*++it).x == 3);
        TEST_OK((*++it).x == 4);
    }

    // Test postfix decrement
    {
        bidirectional_iterator it(data + sizeof(data)/sizeof(data[0]));

        it--;
        TEST_OK((*it--).x == 4);
        TEST_OK((*it--).x == 3);
        TEST_OK((*it--).x == 2);
        TEST_OK((*it--).x == 1);
    }

    // Test prefix decrement
    {
        bidirectional_iterator it(data + sizeof(data)/sizeof(data[0]));

        TEST_OK((*--it).x == 4);
        TEST_OK((*--it).x == 3);
        TEST_OK((*--it).x == 2);
        TEST_OK((*--it).x == 1);
    }
    
    // Test equality and `->` operator
    {
        bidirectional_iterator it1(data);
        bidirectional_iterator it2(data);

        TEST_OK(it1 == it2++);
        TEST_OK(it1 != it2);

        TEST_OK(it1->x == 1);
        TEST_OK(it2->x == 2);
    }
}
