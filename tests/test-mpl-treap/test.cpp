/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <cstdlib>
#include <vector>
#include <pfs/test/test.hpp>
#include <pfs/experimental/implicit_treap.hpp>
#include <iostream>

using std::cout;
using std::endl;

struct priority_traits
{
    typedef long int value_type;
    
    static value_type generate ()
    {
        return ::random();
    }
};

template <typename T>
class vector : public std::vector<T>
{
public:
    vector () 
    {}
};

typedef pfs::implicit_treap<std::pair<int, int>, priority_traits, vector> implicit_treap;

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(2);

    implicit_treap tree;
    tree.insert(0, std::make_pair(0, 1));
    tree.insert(1, std::make_pair(2, 2));
            
//	ADD_TESTS(2);
//	TEST_OK2(true == true, "true == true");
//	TEST_FAIL2(true == true, "true == true");

	return END_TESTS;
}

