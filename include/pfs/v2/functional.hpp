#pragma once

#include <pfs/types.hpp>
#include <pfs/v2/byte_string.hpp>
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(functional)

namespace pfs {

/**
 * @brief Calculates the CRC32 checksum for the given array of bytes
 * 	using crc32 polynomial:
 * 	x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 +
 *  x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1
 *
 * @param pdata data for checksum calculation
 * @param nbytes data length in bytes
 * @param initial initial value for checksum
 * @return CRC32 checksum value
 *
 * @note Compatible with java.util.zip.CRC32;
 *
 * @see http://en.wikipedia.org/wiki/Cyclic_redundancy_check
 */
int32_t crc32 (void const * pdata, size_t nbytes, int32_t initial = 0);

inline int32_t crc32 (byte_string const & pdata, int32_t initial = 0)
{
    return crc32(pdata.data(), pdata.size(), initial);
}


/**
 * @brief Calculates the CRC64 checksum for the given array of bytes.
 *
 * @param pdata data for checksum calculation
 * @param nbytes data length in bytes
 * @param initial initial value for checksum
 * @return CRC64 checksum value
 *
 * @see http://en.wikipedia.org/wiki/Cyclic_redundancy_check
 */
int64_t crc64 (void const * pdata, size_t nbytes, int64_t initial = 0);

inline int64_t crc64 (byte_string const & pdata, int64_t initial = 0)
{
    return crc64(pdata.data(), pdata.size(), initial);
}

} // pfs
