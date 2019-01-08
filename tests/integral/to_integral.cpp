#include <pfs/string.hpp>
#include <pfs/integral.hpp>
#include "../catch.hpp"

#include <pfs/debug.hpp>

// FIXME Complete tests

TEST_CASE("Tests to_integral<int32/64>") {
    
//     try {
//         pfs::to_integral<int32_t>("-2147483649", 10);
//     } catch (pfs::out_of_range const & ex) {
//         std::cout << "out_of_range" << std::endl;
//     }
    
    CHECK(pfs::to_integral<int32_t>("0", 10) == 0);
    CHECK(pfs::to_integral<int32_t>("+0", 10) == 0);
    CHECK(pfs::to_integral<int32_t>("-0", 10) == 0);
    CHECK(pfs::to_integral<int32_t>("1", 10) == 1);
    CHECK(pfs::to_integral<int32_t>("+1", 10) == 1);
    CHECK(pfs::to_integral<int32_t>("-1", 10) == -1);
    CHECK(pfs::to_integral<int32_t>("2147483647", 10) == 2147483647L);
    CHECK(pfs::to_integral<int32_t>("+2147483647", 10) == 2147483647L);
    CHECK(pfs::to_integral<int32_t>("-2147483648", 10) == -2147483647L - 1); // use the substract instead of -2147483648L to avoid warning: this decimal constant is unsigned only in ISO C90 [enabled by default] 
    REQUIRE_THROWS_AS(pfs::to_integral<int32_t>("-2147483649", 10) == -2147483648L, pfs::out_of_range);
    REQUIRE_THROWS_AS(pfs::to_integral<int32_t>("2147483648", 10) == 2147483647L, pfs::out_of_range);
    CHECK(pfs::to_integral<int32_t>("7FFFFFFF", 16) == 0x7FFFFFFFL);
    CHECK(pfs::to_integral<int32_t>("80000000", 16) == -2147483648L);
//    CHECK(pfs::to_integral<int32_t>("FFFFFFFF", 16) == 0xFFFFFFFF);
//     CHECK(pfs::to_integral<int32_t>("-FFFFFFFF", 16) == -1 * 0xFFFFFFF);

#if PFS_HAVE_INT64
//     CHECK(pfs::to_integral<int64_t>("0", 10) == 0);
//     CHECK(pfs::to_integral<int64_t>("+0", 10) == 0);
//     CHECK(pfs::to_integral<int64_t>("-0", 10) == 0);
//     CHECK(pfs::to_integral<int64_t>("1", 10) == 1);
//     CHECK(pfs::to_integral<int64_t>("+1", 10) == 1);
//     CHECK(pfs::to_integral<int64_t>("-1", 10) == -1);
//     CHECK(pfs::to_integral<int64_t>("9223372036854775807", 10) == 9223372036854775807LL);
//     CHECK(pfs::to_integral<int64_t>("-9223372036854775808", 10) == -9223372036854775807LL - 1LL);
//     CHECK(pfs::to_integral<int64_t>("9223372036854775808", 10) == 922337203685477580LL);
//     CHECK(pfs::to_integral<int64_t>("-9223372036854775809", 10) == -922337203685477580LL);
#endif
    
//     CHECK(pfs::to_integral<uint32_t>("0", 10, 0, 1));
//     CHECK(pfs::to_integral<uint32_t>("+0", 10, 0, 2));
//     CHECK(pfs::to_integral<uint32_t>("-0", 10, 0, 0));
//     CHECK(pfs::to_integral<uint32_t>("1", 10, 1, 1));
//     CHECK(pfs::to_integral<uint32_t>("+1", 10, 1, 2));
//     CHECK(pfs::to_integral<uint32_t>("-1", 10, 0, 0));
//     CHECK(pfs::to_integral<uint32_t>("4294967295", 10, 4294967295UL, 10));
//     CHECK(pfs::to_integral<uint32_t>("+4294967295", 10, 4294967295UL, 11));
//     CHECK(pfs::to_integral<uint32_t>("  +4294967295", 10, 4294967295UL, 13));
//     CHECK(pfs::to_integral<uint32_t>("-4294967295", 10, 0, 0));
//     CHECK(pfs::to_integral<uint32_t>("4294967296" , 10, 429496729UL, 9));
// 
//     CHECK(pfs::to_integral<uint32_t>( "FFFFFFFF", 16, 4294967295UL, 8));
//     CHECK(pfs::to_integral<uint32_t>( "FFFFFFFFF", 16, 4294967295UL, 8));
//     CHECK(pfs::to_integral<uint32_t>("-FFFFFFFF", 16, 0, 0));
//     CHECK(pfs::to_integral<uint32_t>( "10000000", 16, 268435456UL, 8));

#if PFS_HAVE_INT64
//     CHECK(pfs::to_integral<uint64_t>("0", 10, 0, 1));
//     CHECK(pfs::to_integral<uint64_t>("+0", 10, 0, 2));
//     CHECK(pfs::to_integral<uint64_t>("-0", 10, 0, 0));
//     CHECK(pfs::to_integral<uint64_t>("1", 10, 1, 1));
//     CHECK(pfs::to_integral<uint64_t>("+1", 10, 1, 2));
//     CHECK(pfs::to_integral<uint64_t>("-1", 10, 0, 0));
//     CHECK(pfs::to_integral<uint64_t>("18446744073709551615", 10, 18446744073709551615ULL, 20));
//     CHECK(pfs::to_integral<uint64_t>("18446744073709551616", 10, 1844674407370955161ULL, 19));
//     CHECK(pfs::to_integral<uint64_t>("-18446744073709551615", 10, 0, 0));
//     CHECK(pfs::to_integral<uint64_t>("FFFFFFFFFFFFFFFF", 16, 18446744073709551615ULL, 16));
//     CHECK(pfs::to_integral<uint64_t>("FFFFFFFFFFFFFFFFF", 16, 18446744073709551615ULL, 16));
//     CHECK(pfs::to_integral<uint64_t>("1000000000000000", 16, 1152921504606846976ULL, 16));
#endif    
}
