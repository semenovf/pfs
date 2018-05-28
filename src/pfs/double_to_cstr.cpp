#include <cstdio>
#include "pfs/exception.hpp"

namespace pfs {
namespace details {
namespace fp {


// Code based on safeformat: stringifier::stringify()
//

char * double_to_cstr (double num
        , char f
        , int prec
        , char * buf
        , size_t * n)
{
    static size_t const SPRINTF_CONV_SPEC_LENGTH = 1 /* '%' */
                             + 2 + sizeof(int)       /* precision */
                             + 1                     /* conversion-specifier */ \
                             + 1;                    /*'\x0' */

    char format[SPRINTF_CONV_SPEC_LENGTH];

    char * p = format;

    *p++ = '%';

    // Precision
    //
    if (prec >= 0) {
        *p++ = '.';
        p += sprintf(p, "%d", prec);
    }
    //
    // Conversion specifier
    //
    *p++ = f;

    *p = '\x0';


    // The glibc implementation of the functions snprintf() and vsnprintf()
    // conforms to the C99 standard, that is, behaves as described above,
    // since glibc version 2.1. Until glibc 2.0.6 they would return -1
    // when the output was truncated.

#if PFS_CC_MSC
    int written = _snprintf(buf, *n, format, num);
#else
    // FIXME Need the recognition of GLIBC version.
    // Supporting modern behavior only now.
    int written = snprintf(buf, *n, format, num);
#endif
    if (written < 0)
        PFS_THROW(runtime_error("safeformat: snprintf() error (a negative value is returned)"));

    // A return value of size or more means that the output was truncated.
    if (static_cast<size_t>(written) >= *n) {
        *n = written;
        return 0;
    }

    *n = written;
    return buf;
}

}}} // pfs
