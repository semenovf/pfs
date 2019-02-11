#include <pfs/io/exception.hpp>
#include "pfs/io/file.hpp"

namespace pfs {
namespace io {

filesystem::perms const open_params<file>::default_create_perms = filesystem::perms::owner_read
        | filesystem::perms::owner_write
        | filesystem::perms::group_read
        | filesystem::perms::others_read;

// TODO
//void file_appender::print (pfs::string const & msg)
//{
//	error_code ex;
//	_d.write(msg.c_str(), msg.size(), & ex);
//	_d.write("\n", 1);
//
//    PFS_ERROR2(ex == 0, string("Failed to write log message: ").append(to_string(ex)).c_str());
//}

string u8_read_all (filesystem::path const & path, error_code & ec)
{
    device_ptr d = open_device(open_params<file>(path, pfs::io::read_only), ec);
    
    if (ec)
        return string();
    
    ssize_t available = d->available();
    
    if (available <= 0)
        return string();
    
    string result;
    size_t sz = integral_cast_check<size_t>(available);
    result.resize(sz);

    ssize_t n = d->read(result.data(), sz, ec);
    
    if (n < 0)
        return string();
    
    result.resize(pfs::integral_cast_check<size_t>(n));
    return result;
}

}} // pfs::io
