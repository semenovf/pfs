#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>

namespace pfs {

void base64_encode (char const * first, char const * last, byte_string & result);
void base64_encode (char const * first, char const * last, string & result);
void base64_encode (byte_string const & src, byte_string & result);
void base64_encode (byte_string const & src, string & result);
void base64_decode (char const * first, char const * last, byte_string & result);
void base64_decode (byte_string const & src, byte_string & result);
void base64_decode (string const & src, byte_string & result);

#if __cplusplus >= 201103L

inline byte_string base64_encode (byte_string const & src)
{
    byte_string result;
    base64_encode(src, result);
    return result;
}

inline string base64_encode (byte_string const & src)
{
    string result;
    base64_encode(src, result);
    return result;
}

inline byte_string base64_decode (byte_string const & src)
{
    byte_string result;
    base64_decode(src, result);
    return result;
}

inline byte_string base64_decode (string const & src)
{
    byte_string result;
    base64_decode(src, result);
    return result;
}

#endif

} // namespace pfs
