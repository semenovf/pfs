/*
 * device.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "pfs/compiler.hpp"
#include "pfs/io/file.hpp"
#include "posix_utils.hpp"

namespace pfs { namespace io { namespace details {

struct file : public bits::device
{
	bits::device::native_handle_type _fd;
	filesystem::path path;
	int      oflags;
	mode_t   omode;

	file ()
		: _fd(-1)
		, oflags(0)
		, omode(0)
	{}

//	file (const file & other)
//		: _fd(other._fd)
//		, path(other.path)
//		, oflags(other.oflags)
//		, omode(other.omode)
//	{}

	~file ()
	{
		close();
	}

	error_code open (filesystem::path const & path, int native_oflags, mode_t native_mode);

    virtual error_code reopen ()
    {
    	close();
    	return open(path, oflags, omode);
    }

    virtual open_mode_flags open_mode () const;

    virtual size_t bytes_available () const;

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);

    virtual error_code close ();

    virtual bool opened () const
    {
    	return _fd >= 0;
    }

    virtual void flush ()
    {
#if PFS_CC_GCC
    	::fsync(_fd);
#else
#   error "Don't know how to sync file"
#endif
    }

    virtual bool set_nonblocking (bool on)
    {
        int flags = fcntl(_fd, F_GETFL, 0);
        if (on)
        	flags |= O_NONBLOCK;
        else
        	flags &= ~O_NONBLOCK;
        return fcntl(_fd, F_SETFL, flags) >= 0;
    }

    virtual bool is_nonblocking () const
    {
        return pfs::io::is_nonblocking(_fd);
    }
    
    virtual native_handle_type native_handle () const
    {
    	return _fd;
    }

    virtual device_type type () const
    {
    	return device_file;
    }
    
    virtual system_string url () const
    {
        system_string r("file:/");
        r.append(path.native());
        return r;
    }
};

error_code file::open (filesystem::path const & path, int oflags, mode_t omode)
{
	int fd = ::open(path.native().c_str(), oflags, omode);

	if (fd < 0) {
        return error_code(errno, pfs::generic_category());
	}

	this->_fd    = fd;
	this->path   = path;
	this->oflags = oflags;
	this->omode  = omode;

	return error_code();
}


bits::device::open_mode_flags file::open_mode () const
{
	bits::device::open_mode_flags r = 0;
	char buf[1] = { 0 };

	if (::read(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= read_only;

	if (::write(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= write_only;

	return r;
}

size_t file::bytes_available () const
{
    PFS_ASSERT(_fd  >= 0);

    off_t cur = ::lseek(_fd, off_t(0), SEEK_CUR);
    PFS_ASSERT(cur >= off_t(0));

    off_t total = ::lseek(_fd, off_t(0), SEEK_END);
    PFS_ASSERT(total >= off_t(0));

    PFS_ASSERT(::lseek(_fd, cur, SEEK_SET) >= off_t(0));
    PFS_ASSERT(total >= cur);

    return (size_t)(total - cur);
}

ssize_t file::read (byte_t * bytes, size_t n, error_code * pex)
{
    ssize_t sz = ::read(_fd, bytes, n);
    
    if (sz < 0 && pex) {
        *pex = error_code(errno, pfs::generic_category());
    }
    
    return sz;
}

ssize_t file::write (const byte_t * bytes, size_t n, error_code * pex)
{
    ssize_t sz = ::write(_fd, bytes, n);
    
    if( sz < 0 && pex) {
        *pex = error_code(errno, pfs::generic_category());
    }
    
    return sz;
}

error_code file::close ()
{
    error_code ec;

    if (_fd > 0) {
        if (::close(_fd) < 0) {
        	ec = error_code(errno, pfs::generic_category());
        }
    }

    _fd = -1;
    return ec;
}

}}} // pfs::io::details

namespace pfs { namespace io {

static int __convert_to_native_perms (filesystem::perms perms)
{
	int r = 0;

	if ((perms & filesystem::perms::owner_read)   != filesystem::perms::none) r |= S_IRUSR;
	if ((perms & filesystem::perms::owner_write)  != filesystem::perms::none) r |= S_IWUSR;
	if ((perms & filesystem::perms::owner_exec)   != filesystem::perms::none) r |= S_IXUSR;
	if ((perms & filesystem::perms::group_read)   != filesystem::perms::none) r |= S_IRGRP;
	if ((perms & filesystem::perms::group_write)  != filesystem::perms::none) r |= S_IWGRP;
	if ((perms & filesystem::perms::group_exec)   != filesystem::perms::none) r |= S_IXGRP;
	if ((perms & filesystem::perms::others_read)  != filesystem::perms::none) r |= S_IROTH;
	if ((perms & filesystem::perms::others_write) != filesystem::perms::none) r |= S_IWOTH;
	if ((perms & filesystem::perms::others_exec)  != filesystem::perms::none) r |= S_IXOTH;

	return r;
}

template <>
device open_device<file> (const open_params<file> & op, error_code & ex)
{
	device result;
	int native_oflags = 0;
	mode_t native_mode = 0;

    if ((op.oflags & write_only) && (op.oflags & read_only)) {
    	native_oflags |= O_RDWR;
    	native_oflags |= O_CREAT;
    	native_mode   |= __convert_to_native_perms(op.permissions);
    } else if (op.oflags & write_only) {
    	native_oflags |= O_WRONLY;
    	native_oflags |= O_CREAT;
    	native_mode   |= __convert_to_native_perms(op.permissions);
    } else if (op.oflags & read_only) {
    	native_oflags |= O_RDONLY;
    }

	if (op.oflags & non_blocking)
		native_oflags |= O_NONBLOCK;

	if (op.oflags & truncate)
		native_oflags |= O_TRUNC;


	details::file * f = new details::file;

	ex = f->open(op.path, native_oflags, native_mode);

	if (!ex) {
	    shared_ptr<bits::device> d(f);
	    result._d.swap(d);
	} else {
		delete f;
	}

	return result;
}

}} // pfs::io
