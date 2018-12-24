#include "pfs/iterator.hpp"
#include "pfs/base64.hpp"

//
// [The Base16, Base32, and Base64 Data Encodings](https://tools.ietf.org/html/rfc4648)
//

namespace pfs {

template <typename Container>
void __base64_encode (byte_string const & src, Container & result)
{
    static const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
                    "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
    static const char padchar = '=';
    int padlen = 0;
    size_t sz = src.size();

    result.clear();
    result.reserve((sz * 4) / 3 + 3);
    back_insert_iterator<Container> out = pfs::back_inserter(result);

    size_t i = 0;
    byte_string::const_pointer d = src.data();

    while (i < sz) {
        int chunk = 0;
            chunk |= int(d[i++]) << 16;
        if (i == sz) {
            padlen = 2;
        } else {
            chunk |= int(d[i++]) << 8;

            if (i == sz)
                padlen = 1;
            else
                chunk |= int(d[i++]);
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

template <typename Container>
void __base64_decode (Container const & src, byte_string & result)
{
   size_t sz = src.size();

    result.clear();
    result.reserve((sz * 3) / 4);

    unsigned int buf = 0;
    int nbits = 0;

    for (size_t i = 0; i < sz; ++i) {
        int ch = src.at(i);
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

    result.resize(result.size());
}

void base64_encode (byte_string const & src, byte_string & result)
{
    __base64_encode(src, result);
}

void base64_encode (byte_string const & src, string & result)
{
    __base64_encode(src, result);
}

void base64_decode (byte_string const & src, byte_string & result)
{
    __base64_decode(src, result);
}

void base64_decode (string const & src, byte_string & result)
{
    __base64_decode(src, result);
}

} // namespace pfs
