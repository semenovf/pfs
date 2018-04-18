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

}} // pfs::io
