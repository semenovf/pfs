#include <pfs/iterator.hpp>

struct A 
{
    int x;
};

struct output_iterator : public pfs::iterator<pfs::<pfs::output_iterator_tag, output_iterator, A, A *, A &>
{
    A * _p;

    output_iterator (input_iterator const & rhs)
        : _p(rhs._p)
    {}
    
    output_iterator (A * p)
        : _p(p)
    {}

//    static reference ref (output_iterator & it)
//    {
//        return *it._p;
//    }
//    
//    static pointer ptr (output_iterator & it)
//    {
//        return it._p;
//    }
//    
//    static void increment (output_iterator & it)
//    {
//        ++it._p;
//    }
//    
//    static void decrement (output_iterator & it)
//    {
//        --it._p;
//    }
//    
//    static bool equals (output_iterator const & it1, output_iterator const & it2)
//    {
//        return it1._p == it2._p;
//    }
};

void test_input_iterator ()
{
    ADD_TESTS(12);
    
    A data[] = { {1}, {2}, {3}, {4} };
    
    {
        output_iterator it(data);

        TEST_OK((*it++).x == 1);
        TEST_OK((*it++).x == 2);
        TEST_OK((*it++).x == 3);
        TEST_OK((*it++).x == 4);
    }

    // Test prefix increment
    {
        output_iterator it(data);
        TEST_OK((*it).x == 1);
        TEST_OK((*++it).x == 2);
        TEST_OK((*++it).x == 3);
        TEST_OK((*++it).x == 4);
    }
    
    // Test equality and `->` operator
    {
        output_iterator it1(data);
        output_iterator it2(data);

        TEST_OK(it1 == it2++);
        TEST_OK(it1 != it2);

        TEST_OK(it1->x == 1);
        TEST_OK(it2->x == 2);
    }
}
