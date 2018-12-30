#include "pfs/assert.hpp"
#include "pfs/iterator.hpp"
#include "pfs/base64.hpp"

//
// [The Base16, Base32, and Base64 Data Encodings](https://tools.ietf.org/html/rfc4648)
//

namespace pfs {

template <typename InputIt, typename OutputIt>
void __base64_encode (InputIt first, InputIt last, OutputIt out)
{
    static const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
                    "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
    static const char padchar = '=';
    int padlen = 0;

    while (first != last) {
        int chunk = 0;
        chunk |= int(static_cast<byte_t>(*first++)) << 16;

        if (first == last) {
            padlen = 2;
        } else {
            chunk |= int(static_cast<byte_t>(*first++)) << 8;

            if (first == last)
                padlen = 1;
            else
                chunk |= int(static_cast<byte_t>(*first++));
        }

        int j = (chunk & 0x00fc0000) >> 18;
        int k = (chunk & 0x0003f000) >> 12;
        int l = (chunk & 0x00000fc0) >> 6;
        int m = (chunk & 0x0000003f);

        *out++ = alphabet[j];      // result.append(1, alphabet[j]);
        *out++ = alphabet[k];      // result.append(1, alphabet[k]);

        if (padlen > 1)
            *out++ = padchar;      // result.append(1, padchar);
        else
            *out++ = alphabet[l];  // result.append(1, alphabet[l]);
        if (padlen > 0)
            *out++ = padchar;      // result.append(1, padchar);
        else
            *out++ = alphabet[m];  // result.append(1, alphabet[m]);
    }
}

template <typename InputIt>
void __base64_decode (InputIt first, InputIt last, byte_string & result)
{
    typename pfs::iterator_traits<InputIt>::difference_type sz
            = pfs::distance(first, last);
    PFS_ASSERT(sz >= 0);

    result.reserve(result.size() + (sz * 3) / 4);

    unsigned int buf = 0;
    int nbits = 0;

    for (; first != last; ++first) {
        int ch = static_cast<int>(*first);
        int d = -1;

        if (ch >= 'A' && ch <= 'Z')
            d = ch - 'A';
        else if (ch >= 'a' && ch <= 'z')
            d = ch - 'a' + 26;
        else if (ch >= '0' && ch <= '9')
            d = ch - '0' + 52;
        else if (ch == '+')
            d = 62;
        else if (ch == '/')
            d = 63;

        if (d != -1) {
            buf = (buf << 6) | d;
            nbits += 6;

            if (nbits >= 8) {
                nbits -= 8;
                result.push_back(buf >> nbits);
                buf &= (1 << nbits) - 1;
            }
        }
    }

    //result.resize(result.size());
}

template <typename InputIt, typename ResultContainer>
void __base64_encode_container (InputIt first, InputIt last, ResultContainer & result)
{
    typename pfs::iterator_traits<InputIt>::difference_type sz
            = pfs::distance(first, last);
    PFS_ASSERT(sz >= 0);
    result.reserve(result.size() + (sz * 4) / 3 + 3);
    back_insert_iterator<ResultContainer> out = pfs::back_inserter(result);

    __base64_encode(first, last, out);
}

void base64_encode (char const * first, char const * last, byte_string & result)
{
    __base64_encode_container(first, last, result);
}

void base64_encode (char const * first, char const * last, string & result)
{
    __base64_encode_container(first, last, result);
}

void base64_encode (byte_string const & src, byte_string & result)
{
    __base64_encode_container(src.cbegin(), src.cend(), result);
}

void base64_encode (byte_string const & src, string & result)
{
    __base64_encode_container(src.cbegin(), src.cend(), result);
}

void base64_decode (char const * first, char const * last, byte_string & result)
{
    __base64_decode(first, last, result);
}

void base64_decode (byte_string const & src, byte_string & result)
{
    __base64_decode(src.cbegin(), src.cend(), result);
}

void base64_decode (string const & src, byte_string & result)
{
    __base64_decode(src.cbegin(), src.cend(), result);
}

} // namespace pfs
