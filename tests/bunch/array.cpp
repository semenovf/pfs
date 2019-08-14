#include <cstring>
#include <pfs/array.hpp>
#include "../catch.hpp"

// TODO Complete implementation

typedef pfs::array<int, 1> array_t;
typedef array_t::value_type array_value_type;

TEST_CASE("Test base64 encoding/decoding") {
//     CHECK(check_codec<pfs::byte_string>("", ""));
//     CHECK(check_codec<pfs::byte_string>("f", "Zg=="));
//     CHECK(check_codec<pfs::byte_string>("fo", "Zm8="));
//     CHECK(check_codec<pfs::byte_string>("foo", "Zm9v"));
//     CHECK(check_codec<pfs::byte_string>("foob", "Zm9vYg=="));
//     CHECK(check_codec<pfs::byte_string>("fooba", "Zm9vYmE="));
//     CHECK(check_codec<pfs::byte_string>("foobar", "Zm9vYmFy"));
//     CHECK(check_codec<pfs::string>("", ""));
//     CHECK(check_codec<pfs::string>("f", "Zg=="));
//     CHECK(check_codec<pfs::string>("fo", "Zm8="));
//     CHECK(check_codec<pfs::string>("foo", "Zm9v"));
//     CHECK(check_codec<pfs::string>("foob", "Zm9vYg=="));
//     CHECK(check_codec<pfs::string>("fooba", "Zm9vYmE="));
//     CHECK(check_codec<pfs::string>("foobar", "Zm9vYmFy"));
//
//     CHECK_FALSE(check_codec<pfs::string>("fooba", "Zm9vYmE"));
//     CHECK_FALSE(check_codec<pfs::string>("foobar", "Ym9vYmFy"));
//
//     char const * bins[] = {
//           "\x00"
//         , "\x00\x01"
//         , "\x00\x01\x02"
//         , "\x00\x01\x02\x03"
//         , "fooba"
//         , "\x01\x00\xEF\xFE\x7F\x20"
//     };
//
//     char const * bins_encoded[] = {
//           "AA=="
//         , "AAE="
//         , "AAEC"
//         , "AAECAw=="
//         , "Zm9vYmE="
//         , "AQDv/n8g"
//     };
//
//     for (int i = 0, n = sizeof(bins)/ sizeof(bins[0]); i < n ; i++) {
//         pfs::string s;
//         pfs::byte_string b;
//         pfs::base64_encode(bins[i], bins[i] + i + 1, s);
//         std::cout << "bin" << i << ": [" << s << ']' << std::endl;
//         CHECK(s == pfs::string(bins_encoded[i]));
//
//         pfs::base64_decode(pfs::string(bins_encoded[i]), b);
//         CHECK(b == pfs::byte_string(bins[i], i + 1));
//     }
}

