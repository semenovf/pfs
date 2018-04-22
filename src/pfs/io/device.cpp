#include "pfs/io/device.hpp"
#include <iostream>

namespace pfs {
namespace io {

static const ssize_t DEFAULT_READ_BUFSZ = 256;

bool device::read (byte_string & bytes, ssize_t n)
{
    byte_t buffer[DEFAULT_READ_BUFSZ];
    ssize_t sz = 0;
    ssize_t total = 0;
    ssize_t chunk_size = DEFAULT_READ_BUFSZ;

    do {
        if (n - total < chunk_size)
            chunk_size = n - total;

        sz = _d->read(buffer, chunk_size);

        std::cout << "device::read: sz = " << sz << std::endl;
        if (sz > 0) {
            bytes.append(buffer, size_t(sz));
            std::cout << "device::read: size_t(sz) = " << size_t(sz) << std::endl;

            total += sz;
        }
    } while (sz > 0 && total < n);

    std::cout << "device::read: total = " << total << std::endl;

     return this->errorcode() == error_code();
}

bool device::close ()
{
    if (_d)
        return _d->close();
    return true;
}

/**
 * @brief Copy data between source @a src an destination @a dest devices.
 *
 * @param dest Destination (write-to) device.
 * @param src Source (read-from) device.
 * @param chunkSize Size of temporary buffer to chunk
 * @param ex Pointer to store error code.
 * @return @li -1 and @a *ex != 0 if error occurred while read from @a src or write to @a dest;
 *         @li -1 and @a *ex == 0 if size of written data is not equals to size of read data;
 *         @li >=0 if data is succesfull.
 */
ssize_t copy (device & dest, device & src, size_t chunk_size, error_code * ec)
{
    byte_t buffer[DEFAULT_READ_BUFSZ];
    ssize_t r = 0;

    if (ec)
    	ec->clear();

    while (r < static_cast<ssize_t>(chunk_size)) {
    	ssize_t r1 = src.read(buffer, DEFAULT_READ_BUFSZ);

    	if (r1 < 0)
    		return -1;

    	if (r1 == 0)
    		break;

    	ssize_t r2 = dest.write(buffer, static_cast<size_t>(r1));

    	if (r2 <= 0)
    		return -1;

    	if (r2 != r1)
    		return -1;

    	r += r1;
    }

    return r;
}

// TODO Move functions below (compress/uncompress) to another place
#if __TODO__
bool compress (device & dest, device & src, zlib::compression_level level, size_t chunkSize, error_code * pex)
{
	if (!src.opened()) {
		// FIXME assign error code
		return false;
	}

    if (!dest.opened()) {
   		// FIXME assign error code
   		return false;
   	}

	if (chunkSize < 32)
		chunkSize = 0x4000;

	int flush;
	z_stream strm;
	pfs::scoped_array_ptr<unsigned char> in(new unsigned char[chunkSize]);
	pfs::scoped_array_ptr<unsigned char> out(new unsigned char[chunkSize]);

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	int rc = deflateInit(& strm, level);

	if (rc != Z_OK) {
		//FIXME Need to notify error
//		_nx.append(zlib::strerror(rc));
		return false;
	}

	error_code ex;

	/* compress until end of file */
	do {
		strm.avail_in = src.read(in.get(), chunkSize, & ex);

		if (ex) {
			(void)deflateEnd(& strm);

			if (pex)
				*pex = ex;

			return false;
		}

		flush = src.at_end() ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in.get();

		/* run deflate() on input until output buffer not full, finish
		   compression if all of source has been read in */
        do {
        	strm.avail_out = chunkSize;
        	strm.next_out = out.get();
        	rc = deflate(& strm, flush);    /* no bad return value */
        	PFS_ASSERT(rc != Z_STREAM_ERROR);  /* state not clobbered */
        	size_t have = chunkSize - strm.avail_out;

        	if (dest.write(out.get(), have, & ex) != ssize_t(have) || ex) {
        		(void)deflateEnd(& strm);

    			if (pex)
    				*pex = ex;

        		return false;
        	}
        } while (strm.avail_out == 0);

        PFS_ASSERT(strm.avail_in == 0);     /* all input will be used */

	  /* done when last data in file processed */
	} while (flush != Z_FINISH);

	PFS_ASSERT(rc == Z_STREAM_END);        /* stream will be complete */

	/* clean up and return */
	deflateEnd(& strm);
	return true;
}

bool uncompress (device & dest, device & src, size_t chunkSize, error_code * pex)
{
	if (!src.opened()) {
		// FIXME assgn error code
		return false;
	}

    if (!dest.opened()) {
   		// FIXME assgn error code
   		return false;
   	}

    z_stream strm;
	pfs::scoped_array_ptr<unsigned char> in(new unsigned char[chunkSize]);
	pfs::scoped_array_ptr<unsigned char> out(new unsigned char[chunkSize]);

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    int rc = inflateInit(& strm);

	if (rc != Z_OK) {
		//FIXME Need to notify error
		//_nx.append(zlib::strerror(rc));
		return false;
	}

	error_code ex;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = src.read(in.get(), chunkSize, & ex);

        if (ex) {
            (void)inflateEnd(&strm);

            if (pex)
				*pex = ex;

            return false;
        }

        if (strm.avail_in == 0)
            break;

        strm.next_in = in.get();

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = chunkSize;
            strm.next_out = out.get();
            rc = inflate(& strm, Z_NO_FLUSH);
            PFS_ASSERT(rc != Z_STREAM_ERROR);  /* state not clobbered */

            switch (rc) {
            case Z_NEED_DICT:
                rc = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(& strm);
                return rc;
            }

            size_t have = chunkSize - strm.avail_out;

            if (dest.write(out.get(), have, & ex) != ssize_t(have) || ex) {
                inflateEnd(& strm);

                if (pex)
    				*pex = ex;

                return false;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (rc != Z_STREAM_END);

    /* clean up and return */
    inflateEnd(& strm);

    if (rc != Z_STREAM_END) {
    	//FIXME Need to notify error
    	//_nx.append(zlib::strerror(Z_DATA_ERROR));
    	return false;
    }

    return true;
}

#endif

}} // pfs::io
