#include "pfs/algo/compare.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include "../catch.hpp"

int a[] = {1, 2, 3, 4, 5, 0};
int b[] = {1, 2, 3, 4, 0};
int c[] = {2, 3, 4, 5, 0};

#define X_COUNT(x) (sizeof(x)/sizeof(x[0]))

TEST_CASE("Algo / compare") {
    CHECK((pfs::compare(a, a + X_COUNT(a)
            , a, a + X_COUNT(a)
            , std::less<int>()) == 0));

    CHECK((pfs::compare(a, a + X_COUNT(a)
            , b, b + X_COUNT(b)
            , std::less<int>()) > 0));

    CHECK((pfs::compare(a, a + X_COUNT(a)
            , c, c + X_COUNT(c)
            , std::less<int>()) < 0));
}

template <typename T>
struct until_zero
{
    bool operator () (T * x) { return *x == 0; }
};

TEST_CASE("Algo / compare until") {
    CHECK((pfs::compare_until(a, a + X_COUNT(a) -1
            , a, until_zero<int>()
            , std::less<int>()) == 0));

    CHECK((pfs::compare_until(a, a + X_COUNT(a) - 1
            , b, until_zero<int>()
            , std::less<int>()) > 0));

    CHECK((pfs::compare_until(a, a + X_COUNT(a) - 1
            , c, until_zero<int>()
            , std::less<int>()) < 0));

    CHECK((pfs::compare_until(a, until_zero<int>()
            , a, until_zero<int>()
            , std::less<int>()) == 0));

    CHECK((pfs::compare_until(a, until_zero<int>()
            , b, until_zero<int>()
            , std::less<int>()) > 0));

    CHECK((pfs::compare_until(a, until_zero<int>()
            , c, until_zero<int>()
            , std::less<int>()) < 0));
}
