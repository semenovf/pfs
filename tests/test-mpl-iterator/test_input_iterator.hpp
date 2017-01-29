#include <pfs/iterator.hpp>

struct A 
{
    int x;
};

struct input_iterator : public pfs::iterator<
          pfs::input_iterator_tag
        , input_iterator
        , A, A *, A &>
{
    A * _p;

    input_iterator (input_iterator const & rhs)
        : _p(rhs._p)
    {}
    
    input_iterator (A * p)
        : _p(p)
    {}

    static const reference ref (input_iterator const & it)
    {
        return *it._p;
    }
    
    static pointer ptr (input_iterator & it)
    {
        return it._p;
    }
    
    static void increment (input_iterator & it)
    {
        ++it._p;
    }
    
    static void decrement (input_iterator & it)
    {
        --it._p;
    }
    
    static bool equals (input_iterator const & it1, input_iterator const & it2)
    {
        return it1._p == it2._p;
    }
};

void test_input_iterator ()
{
    ADD_TESTS(12);
    
    A data[] = { {1}, {2}, {3}, {4} };
    
    {
        input_iterator it(data);

        TEST_OK((*it++).x == 1);
        TEST_OK((*it++).x == 2);
        TEST_OK((*it++).x == 3);
        TEST_OK((*it++).x == 4);
    }

    // Test prefix increment
    {
        input_iterator it(data);
        TEST_OK((*it).x == 1);
        TEST_OK((*++it).x == 2);
        TEST_OK((*++it).x == 3);
        TEST_OK((*++it).x == 4);
    }
    
    // Test equality and `->` operator
    {
        input_iterator it1(data);
        input_iterator it2(data);

        TEST_OK(it1 == it2++);
        TEST_OK(it1 != it2);

        TEST_OK(it1->x == 1);
        TEST_OK(it2->x == 2);
    }
}
