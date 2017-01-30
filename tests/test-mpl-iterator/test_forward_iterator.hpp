#include <pfs/iterator.hpp>

struct C
{
    int x;
};

struct forward_iterator : public pfs::iterator_facade<
          pfs::forward_iterator_tag
        , forward_iterator
        , C, C *, C &>
{
    C * _p;

//    forward_iterator (input_iterator const & rhs)
//        : _p(rhs._p)
//    {}
    
    forward_iterator (C * p)
        : _p(p)
    {}

    static reference ref (forward_iterator & it)
    {
        return *it._p;
    }
    
    static pointer ptr (forward_iterator & it)
    {
        return it._p;
    }
    
    static void increment (forward_iterator & it)
    {
        ++it._p;
    }
    
    static bool equals (forward_iterator const & it1, forward_iterator const & it2)
    {
        return it1._p == it2._p;
    }
};

void test_forward_iterator ()
{
    ADD_TESTS_X(12, "Test Forward Iterator");
    
    C data[] = { {1}, {2}, {3}, {4} };
    
    {
        forward_iterator it(data);

        TEST_OK((*it++).x == 1);
        TEST_OK((*it++).x == 2);
        TEST_OK((*it++).x == 3);
        TEST_OK((*it++).x == 4);
    }

    // Test prefix increment
    {
        forward_iterator it(data);
        TEST_OK((*it).x == 1);
        TEST_OK((*++it).x == 2);
        TEST_OK((*++it).x == 3);
        TEST_OK((*++it).x == 4);
    }
    
    // Test equality and `->` operator
    {
        forward_iterator it1(data);
        forward_iterator it2(data);

        TEST_OK(it1 == it2++);
        TEST_OK(it1 != it2);

        TEST_OK(it1->x == 1);
        TEST_OK(it2->x == 2);
    }
}
