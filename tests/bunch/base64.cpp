#include <cstring>
#include <pfs/base64.hpp>
#include "../catch.hpp"

template <typename Container>
bool compare_raw_encoded (char const * src, char const * result)
{
    Container out;
    pfs::base64_encode(src, src + std::strlen(src), out);
    return out == Container(result);
}

bool compare_raw_decoded (char const * src, char const * result)
{
    pfs::byte_string out;
    pfs::base64_decode(src, src + std::strlen(src), out);
    return out == pfs::byte_string(result);
}

template <typename Container>
bool compare_encoded (char const * src, char const * result)
{
    pfs::byte_string in(src);
    Container out;

    pfs::base64_encode(in, out);
    return out == Container(result);
}

template <typename Container>
bool compare_decoded (char const * src, char const * result)
{
    Container in(src);
    pfs::byte_string out;

    pfs::base64_decode(in, out);
    return out == pfs::byte_string(result);
}

template <typename Container>
bool check_codec (char const * src, char const * result)
{
    return compare_raw_encoded<Container>(src, result)
            && compare_raw_decoded(result, src)
            && compare_encoded<Container>(src, result)
            && compare_decoded<Container>(result, src);
}


TEST_CASE("Test base64 encoding/decoding") {
   CHECK(check_codec<pfs::byte_string>("", ""));
   CHECK(check_codec<pfs::byte_string>("f", "Zg=="));
   CHECK(check_codec<pfs::byte_string>("fo", "Zm8="));
   CHECK(check_codec<pfs::byte_string>("foo", "Zm9v"));
   CHECK(check_codec<pfs::byte_string>("foob", "Zm9vYg=="));
   CHECK(check_codec<pfs::byte_string>("fooba", "Zm9vYmE="));
   CHECK(check_codec<pfs::byte_string>("foobar", "Zm9vYmFy"));
   CHECK(check_codec<pfs::string>("", ""));
   CHECK(check_codec<pfs::string>("f", "Zg=="));
   CHECK(check_codec<pfs::string>("fo", "Zm8="));
   CHECK(check_codec<pfs::string>("foo", "Zm9v"));
   CHECK(check_codec<pfs::string>("foob", "Zm9vYg=="));
   CHECK(check_codec<pfs::string>("fooba", "Zm9vYmE="));
   CHECK(check_codec<pfs::string>("foobar", "Zm9vYmFy"));

   CHECK_FALSE(check_codec<pfs::string>("fooba", "Zm9vYmE"));
   CHECK_FALSE(check_codec<pfs::string>("foobar", "Ym9vYmFy"));
}
