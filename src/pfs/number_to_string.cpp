#include "pfs/assert.hpp"
#include "pfs/exception.hpp"

namespace pfs {
namespace details {
namespace fp {

/**
 * @brief Convert floating point number to string.
 *
 * @param n Floating point number.
 * @param f Floating point format for output.
 * @param prec Floating point precision.
 * @param buf Pointer to buffer @c buf.
 * @param bufsz Buffer @c buf size.
 * @return Pointer to @c buf if conversion is successful
 *         or NULL if not enough space to store string
 *         representation of floating point number.
 *         In case of failure *bufsz contains final buffer size
 *         or actual number of characters written in success.
 */
char * double_to_cstr (double num, char f, int prec, char * buf, size_t * n)
{
	PFS_ASSERT(n);

	char fmt[32];

	if (prec >= 0) {
		PFS_ASSERT(sprintf(fmt, "%%.%d%c", prec, f) > 0);
	} else {
		PFS_ASSERT(sprintf(fmt, "%%%c", f) > 0);
	}

	/* The functions snprintf() and vsnprintf() do not write  more  than  size  bytes
	 * (including the terminating null byte ('\0')).  If the output was truncated due
	 * to this limit then the return value is the number of characters (excluding the
	 * terminating  null  byte)  which would have been written to the final string if
	 * enough space had been available. Thus, a return value of size or  more  means
	 * that the output was truncated.
	 *
	 * The glibc implementation of the functions snprintf() and vsnprintf()  conforms
	 * to  the C99 standard, that is, behaves as described above, since glibc version
	 * 2.1.  Until glibc 2.0.6 they would return -1 when the output was truncated.
	 */

	// FIXME Need the recognition of GLIBC version.
	// Supporting modern behavior only now.
	int sz = snprintf(buf, *n, fmt, num);

	// Truncated
	//
	if (sz >= *n) {
		*n = sz;
		return 0;
	}

	*n = sz;

//#ifdef PFS_CC_MSC
//	// FIXME make checks as below code for PFS_CC_GNUC
//	PFS_ASSERT(_snprintf(buf, bufsz - 1, fmt, n) > 0);
//#endif

	return buf;
}

}}} // pfs::details::fp
