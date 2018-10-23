#include <iostream>
#include <pfs/type_traits.hpp>

using std::cout;
using std::endl;

namespace t1 {

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

void test ()
{
    A<char> a1;
    a1.foo("aaa"); // equals

    A<int> a2;
    int * ints;
    a2.foo(ints); // equals

    A<int> a3;
    double * floats;
    a3.foo(floats); // non-equals
}

} // namespace t1

namespace t2 {

//
// [SFINAE in class template constructors](https://stackoverflow.com/questions/16300828/sfinae-in-class-template-constructors)
//

template <typename T> struct A { A () {} };
template <typename T> struct B { B () {} };

//
// Use dummy function parameter
//
template <typename T>
class C {
public:
    template <typename U>
    C (A<U> const & a, typename pfs::enable_if<pfs::is_same<A<U>, T>::value, U>::type * = 0) { cout << "C(A)\n"; }

    template <typename U>
    C (B<U> const & b, typename pfs::enable_if<pfs::is_same<B<U>, T >::value, U>::type * = 0) { cout << "C(B)\n"; }
};

//
// Use dummy template parameter
//
template <typename T>
class D {
public:
#if __cplusplus >= 201103L
    // gcc error: default template arguments may not be used in function templates without -std=c++11 or -std=gnu++11
    template <typename U, typename E = typename pfs::enable_if<pfs::is_same<A<U>, T>::value, U>::type>
    D (A<U> const & a) { cout << "D(A)\n"; }

    // gcc error: default template arguments may not be used in function templates without -std=c++11 or -std=gnu++11
    template <typename U, typename E = typename pfs::enable_if<pfs::is_same<B<U>, T>::value, U>::type>
    D (B<U> const & b) { cout << "D(B)\n"; }
#endif
};
void test ()
{
    C<A<int> >(A<int>()); // C(A)
    C<B<int> >(B<int>()); // C(B)

#if __cplusplus >= 201103L
    D<A<int> >(A<int>()); // D(A)
    D<B<int> >(B<int>()); // D(B)
#endif
}

} // namespace t2

int main (int, char *[])
{
    t1::test();
    t2::test();

    return 0;
}
