#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include "pfs/compiler.hpp"
#include "pfs/io/file.hpp"
#include "pfs/compiler.hpp"
#include "posix_utils.hpp"

//#include <iostream>

namespace pfs {

static string const STDIN_NAME = "stdin";
static string const STDOUT_NAME = "stdout";
static string const STDERR_NAME = "stderr";

namespace io {
namespace details {

struct basic_file : public details::device
{
    details::device::native_handle_type _fd;

    basic_file () : _fd(-1) {}

    virtual bool opened () const override
    {
        return _fd >= 0;
    }

    virtual void flush () override
    {
#if PFS_CC_GCC
        ::fsync(_fd);
#else
#error "Don't know how to sync file"
#endif
    }

    virtual native_handle_type native_handle () const override
    {
        return _fd;
    }

    virtual bool set_nonblocking (bool on) override
    {
        int flags = fcntl(_fd, F_GETFL, 0);
        if (on)
            flags |= O_NONBLOCK;
        else
            flags &= ~O_NONBLOCK;
        return fcntl(_fd, F_SETFL, flags) >= 0;
    }

    virtual bool is_nonblocking () const override
    {
        return pfs::io::is_nonblocking(_fd);
    }

    virtual ssize_t read (byte_t * bytes, size_t n, error_code & ec) noexcept override
    {
        ssize_t sz = ::read(_fd, bytes, n);

        if (sz < 0)
            ec = get_last_system_error();

        return sz;
    }

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code & ec) noexcept override
    {
        ssize_t sz = ::write(_fd, bytes, n);

        if (sz < 0)
            ec = get_last_system_error();

        return sz;
    }
};

struct standard_stream : basic_file
{
    typedef basic_file base_class;
    int _orig_fd;

    standard_stream ()
       : base_class()
       , _orig_fd(-1)
    {}

    virtual ~standard_stream ()
    {
        this->close();
    }

    error_code open (int orig_fd)
    {
        PFS_ASSERT(orig_fd >= 0);
        int fd = dup(orig_fd);

        if (fd < 0)
           return get_last_system_error();

        _fd = fd;
        _orig_fd = orig_fd;
        return error_code();
    }

    virtual error_code close () override
    {
        error_code ec;

        if (_fd > 0 && _fd != _orig_fd) {
            if (::close(_fd) < 0)
                ec = get_last_system_error();
        }

       _fd = -1;
        return ec;
    }

    virtual error_code reopen () override
    {
        error_code ec;

        if (close())
            ec = open(_orig_fd);

        return ec;
    }

    virtual open_mode_flags open_mode () const override
    {
        // stdin
        if (_orig_fd == 0)
            return read_only;

        if (_orig_fd == 1 || _orig_fd == 2)
            return write_only;

        return not_open;
    }

    virtual ssize_t available () const override
    {
        PFS_ASSERT(_fd >= 0);
        int n = 0;
        int rc = 0;
        rc = ioctl(_fd, FIONREAD, & n);

        PFS_ASSERT_X(rc == 0, error_code(errno, pfs::generic_category()).message().c_str());
        PFS_ASSERT(n >= 0);

        return static_cast<ssize_t> (n);
    }

    virtual device_type type () const override
    {
        return device_stream;
    }

    virtual string url () const override
    {
        string r("stream://");
        if (_orig_fd == 0)
            r.append(STDIN_NAME);
        else if (_orig_fd == 1)
            r.append(STDOUT_NAME);
        else if (_orig_fd == 2)
            r.append(STDERR_NAME);
        else
            r.append("<unknown>");
        return r;
    }
};

struct file : basic_file
{
    typedef basic_file base_class;

    filesystem::path path;
    int oflags;
    mode_t omode;

    file ()
         : base_class()
         , oflags(0)
         , omode(0)
    {}

    virtual ~file ()
    {
        this->close();
    }

    error_code open (filesystem::path const & path, int of, mode_t om)
    {
        int fd = ::open(path.native().c_str(), of, om);
        error_code ec;

        if (fd >= 0) {
            this->_fd = fd;
            this->path = path;
            this->oflags = of;
            this->omode = om;
        } else {
            ec = get_last_system_error();
        }

        return ec;
    }

    virtual error_code close () override
    {
        error_code ec;

        if (_fd > 0) {
            if (::close(_fd) < 0)
                ec = get_last_system_error();
        }

        _fd = -1;
        return ec;
    }

    virtual error_code reopen () override
    {
        error_code ec;

        if (close())
            ec = open(path, oflags, omode);

        return ec;
    }

    virtual open_mode_flags open_mode () const override
    {
        details::device::open_mode_flags r = 0;

        char buf[1] = {0};

        if (::read(_fd, buf, 0) >= 0 && errno != EBADF)
            r |= read_only;

        if (::write(_fd, buf, 0) >= 0 && errno != EBADF)
            r |= write_only;

        return r;
    }

    virtual ssize_t available () const override
    {
        PFS_ASSERT(_fd >= 0);

        off_t cur = ::lseek(_fd, off_t(0), SEEK_CUR);

        if (cur < 0) {
            PFS_ASSERT(errno == ESPIPE);
            return -1;
        }

        off_t total = ::lseek(_fd, off_t(0), SEEK_END);
        PFS_ASSERT(total >= off_t(0));
        //std::cout << "file::_available: total = " << total << std::endl;

        cur = ::lseek(_fd, cur, SEEK_SET);
        PFS_ASSERT(cur >= off_t(0));
        PFS_ASSERT(total >= cur);
        //std::cout << "file::available: cur = " << cur << std::endl;
        //std::cout << "file::available: static_cast<ssize_t>(total - cur) = " << static_cast<ssize_t>(total - cur) << std::endl;

        return static_cast<ssize_t>(total - cur);
    }

    virtual device_type type () const override
    {
        return device_file;
    }

    virtual string url () const override
    {
        string r("file:/");
        r.append(path.native());
        return r;
    }
};

}}} // pfs::io::details

namespace pfs {
namespace io {

static int __convert_to_native_perms (filesystem::perms perms)
{
    int r = 0;

    if ((perms & filesystem::perms::owner_read) != filesystem::perms::none) r |= S_IRUSR;
    if ((perms & filesystem::perms::owner_write) != filesystem::perms::none) r |= S_IWUSR;
    if ((perms & filesystem::perms::owner_exec) != filesystem::perms::none) r |= S_IXUSR;
    if ((perms & filesystem::perms::group_read) != filesystem::perms::none) r |= S_IRGRP;
    if ((perms & filesystem::perms::group_write) != filesystem::perms::none) r |= S_IWGRP;
    if ((perms & filesystem::perms::group_exec) != filesystem::perms::none) r |= S_IXGRP;
    if ((perms & filesystem::perms::others_read) != filesystem::perms::none) r |= S_IROTH;
    if ((perms & filesystem::perms::others_write) != filesystem::perms::none) r |= S_IWOTH;
    if ((perms & filesystem::perms::others_exec) != filesystem::perms::none) r |= S_IXOTH;

    return r;
}

template <>
device_ptr open_device<file> (open_params<file> const & op, error_code & ec)
{
    device_ptr result;
    int native_oflags = 0;
    mode_t native_mode = 0;

    if ((op.oflags & write_only) && (op.oflags & read_only)) {
        native_oflags |= O_RDWR;
        native_oflags |= O_CREAT;
        native_mode |= __convert_to_native_perms(op.permissions);
    } else if (op.oflags & write_only) {
        native_oflags |= O_WRONLY;
        native_oflags |= O_CREAT;
        native_mode |= __convert_to_native_perms(op.permissions);
    } else if (op.oflags & read_only) {
        native_oflags |= O_RDONLY;
    }

    if (op.oflags & non_blocking)
        native_oflags |= O_NONBLOCK;

    if (op.oflags & truncate)
        native_oflags |= O_TRUNC;


    details::file * f = new details::file;

    ec = f->open(op.path, native_oflags, native_mode);

    if (!ec) {
        device_ptr d(f);
        result.swap(d);
    } else {
        delete f;
    }

    return result;
}

static device_ptr open_standard_stream (int fd, error_code & ec)
{
    details::standard_stream * f = new details::standard_stream;

    ec = f->open(fd);

    if (!ec)
        return device_ptr(f);

    return device_ptr();
}

template <>
device_ptr open_device<file_stdin> (open_params<file_stdin> const & op, error_code & ec)
{
    return open_standard_stream(0, ec);
}

template <>
device_ptr open_device<file_stdout> (open_params<file_stdout> const & op, error_code & ec)
{
    return open_standard_stream(1, ec);
}

template <>
device_ptr open_device<file_stderr> (open_params<file_stderr> const & op, error_code & ec)
{
    return open_standard_stream(2, ec);
}

}} // pfs::io
