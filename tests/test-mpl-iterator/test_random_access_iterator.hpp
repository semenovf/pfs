#include <pfs/iterator.hpp>

#include <iostream>

struct E
{
    int x;
};

struct random_access_iterator : public pfs::iterator_facade<
          pfs::random_access_iterator_tag
        , random_access_iterator
        , E, E *, E &>
{
    E * _p;

    random_access_iterator (E * p)
        : _p(p)
    {}

    static reference ref (random_access_iterator & it)
    {
        return *it._p;
    }
    
    static pointer ptr (random_access_iterator & it)
    {
        return it._p;
    }
    
    static void increment (random_access_iterator & it, difference_type n)
    {
        it._p += n;
    }

    static int compare (random_access_iterator const & it1, random_access_iterator const & it2)
    {
        return it1._p - it2._p;
    }
    
    static void decrement (random_access_iterator & it, difference_type n)
    {
        it._p -= n;
    }
    
    static reference subscript (random_access_iterator & it, difference_type n)
    {
        return *(it._p + n);
    }
    
    static difference_type diff (random_access_iterator const & lhs
            , random_access_iterator const & rhs)
    {
        return lhs._p - rhs._p;
    }
};

void test_random_access_iterator ()
{
    ADD_TESTS_X(51, "Test Random Access Iterator");
    
    E data[] = { {1}, {2}, {3}, {4} };

    // Test postfix increment
    {
        random_access_iterator it(data);

        TEST_OK((*it++).x == 1);
        TEST_OK((*it++).x == 2);
        TEST_OK((*it++).x == 3);
        TEST_OK((*it++).x == 4);
    }

    // Test prefix increment
    {
        random_access_iterator it(data);
        TEST_OK((*it).x == 1);
        TEST_OK((*++it).x == 2);
        TEST_OK((*++it).x == 3);
        TEST_OK((*++it).x == 4);
    }

    // Test postfix decrement
    {
        random_access_iterator it(data + sizeof(data)/sizeof(data[0]));

        it--;
        TEST_OK((*it--).x == 4);
        TEST_OK((*it--).x == 3);
        TEST_OK((*it--).x == 2);
        TEST_OK((*it--).x == 1);
    }

    // Test prefix decrement
    {
        random_access_iterator it(data + sizeof(data)/sizeof(data[0]));

        TEST_OK((*--it).x == 4);
        TEST_OK((*--it).x == 3);
        TEST_OK((*--it).x == 2);
        TEST_OK((*--it).x == 1);
    }
    
    // Test equality and `->` operator
    {
        random_access_iterator it1(data);
        random_access_iterator it2(data);

        TEST_OK(it1 == it2++);
        TEST_OK(it1 != it2);

        TEST_OK(it1->x == 1);
        TEST_OK(it2->x == 2);
    }

    // Test subscript
    {
        random_access_iterator it(data);
        TEST_OK(it[0].x == 1);
        TEST_OK(it[1].x == 2);
        TEST_OK(it[2].x == 3);
        TEST_OK(it[3].x == 4);
    }

    // Test +=, -=
    {
        random_access_iterator it(data);
        
        it += 0;
        TEST_OK(it->x == 1);

        it -= 0;
        TEST_OK(it->x == 1);

        it += 1;
        TEST_OK(it->x == 2);

        it -= 1;
        TEST_OK(it->x == 1);

        it += 2;
        TEST_OK(it->x == 3);

        it -= 2;
        TEST_OK(it->x == 1);
        
        it += 3;
        TEST_OK(it->x == 4);

        it -= 3;
        TEST_OK(it->x == 1);
        
        random_access_iterator it1(data + sizeof(data)/sizeof(data[0]));
        
        it1 -= sizeof(data)/sizeof(data[0]);
        TEST_OK(it1->x == 1);
        
        TEST_OK((it1 + 0)->x == 1);
        TEST_OK((it1 + 1)->x == 2);
        TEST_OK((it1 + 2)->x == 3);
        TEST_OK((it1 + 3)->x == 4);
        
        it1 += sizeof(data)/sizeof(data[0]);
        TEST_OK((it1 - 1)->x == 4);
        TEST_OK((it1 - 2)->x == 3);
        TEST_OK((it1 - 3)->x == 2);
        TEST_OK((it1 - 4)->x == 1);
    }
    
    // Test difference
    {
        random_access_iterator it1(data);
        random_access_iterator it2(data + sizeof(data)/sizeof(data[0]));
        
        TEST_OK(it2 - it1 == sizeof(data)/sizeof(data[0]));
        TEST_OK(it1 - it2 == - static_cast<int>(sizeof(data)/sizeof(data[0])));
    }

    // Test compare
    {
        random_access_iterator it1(data);
        random_access_iterator it2(data);
        random_access_iterator it3(data + sizeof(data)/sizeof(data[0]));
        
        TEST_OK(it1 == it2);
        TEST_OK(it1 != it3);
        TEST_OK(it1 <= it2);
        TEST_OK(it1 <= it3);
        TEST_OK(it1 < it3);
        TEST_OK(it2 >= it1);
        TEST_OK(it3 >= it1);
        TEST_OK(it3 > it1);
    }
}
