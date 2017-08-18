#include <iostream>
#include <pfs/type_traits.hpp>

using std::cout;
using std::endl;

template <typename CharT>
struct A
{
    template <typename CharU>
    typename pfs::enable_if<pfs::is_same<CharU, CharT>::value, void>::type foo (CharU const * a)
    {
        cout << "equals" << endl;
    }

    template <typename CharU>
    typename pfs::enable_if<! pfs::is_same<CharU, CharT>::value, void>::type foo (CharU const * a)
    {
        cout << "non-equals" << endl;
    }
};


int main (int, char *[])
{
    A<char> a1;
    a1.foo("aaa");

    A<int> a2;
    int * ints;
    a2.foo(ints);

    return 0;
}
