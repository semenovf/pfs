#include "pfs/algo/advance.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include "../catch.hpp"

int a[] = {1, 2, 3, 4, 5, 0};
int b[] = {1, 2, 3, 4, 0};
int c[] = {2, 3, 4, 5, 0};

#define X_COUNT(x) (sizeof(x)/sizeof(x[0]))

TEST_CASE("Algo / advance") {
    int * p = a;
    pfs::advance_sequence(p, a + X_COUNT(a), a, a + X_COUNT(a));
    CHECK(p == a + X_COUNT(a));

    p = a;
    pfs::advance_sequence(p, a + X_COUNT(a), b, b + X_COUNT(b));
    CHECK(p == a + X_COUNT(b));

    p = a;
    pfs::advance_sequence(p, a + X_COUNT(a), c, c + X_COUNT(c));
    CHECK(p == a);
}

template <typename T>
struct until_zero
{
    bool operator () (T * x) { return *x == 0; }
};

TEST_CASE("Algo / advance until") {
    int * p = a;
    pfs::advance_sequence_until(p, a + X_COUNT(a), a, until_zero<int>());
    CHECK(p == a + X_COUNT(a) - 1);

    p = a;
    pfs::advance_sequence_until(p, a + X_COUNT(a), b, until_zero<int>());
    CHECK(p == a + X_COUNT(b) - 1);
    CHECK(*p == 5);

    p = a;
    pfs::advance_sequence_until(p, a + X_COUNT(a), c, until_zero<int>());
    CHECK(p == a);

    p = a;
    pfs::advance_sequence_until(p, until_zero<int>(), a, until_zero<int>());
    CHECK(p == a + X_COUNT(a) - 1);

    p = a;
    pfs::advance_sequence_until(p, until_zero<int>(), b, until_zero<int>());
    CHECK(p == a + X_COUNT(b) - 1);
    CHECK(*p == 5);

    p = a;
    pfs::advance_sequence_until(p, until_zero<int>(), c, until_zero<int>());
    CHECK(p == a);

}

