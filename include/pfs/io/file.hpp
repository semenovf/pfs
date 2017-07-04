/*
 * file.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_FILE_HPP__
#define __PFS_IO_FILE_HPP__

//#include <pfs/logger.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/io/device.hpp>

namespace pfs { namespace io {

/**
 * @struct pfs::io::file
 * @brief File device implementation.
 * @see pfs::io::device.
 */
struct file {};

template <>
struct open_params<file>
{
	static filesystem::perms const default_create_perms = filesystem::perms::owner_read
			| filesystem::perms::owner_write
			| filesystem::perms::group_read
			| filesystem::perms::others_read;

	filesystem::path path;
	device::open_mode_flags oflags;
	filesystem::perms permissions;

	open_params (filesystem::path const & s, device::open_mode_flags of, filesystem::perms perms)
		: path(s)
		, oflags(of)
		, permissions(perms)
	{}

	open_params (filesystem::path const & s, device::open_mode_flags of)
		: path(s)
		, oflags(of)
		, permissions(default_create_perms)
	{}

	open_params (filesystem::path const & s)
		: path(s)
		, oflags(read_write)
		, permissions(default_create_perms)
	{}
};


/**
 * @fn bool open_device<file> (device & d, const open_params<file> & op)
 *
 * @brief Open file device.
 *
 * @param d File device to open.
 * @param op Open device parameters.
 * 		@li open_params(size_t n, uint32_t oflags)
 * 		@li open_params(byte_t * p, size_t n, uint32_t oflags)
 * 		@li open_params(char * p, size_t n, uint32_t oflags)
 *
 * @return @c true if open is successful, @c false otherwise
 *         (i.e. file device is already opened).
 */
template <>
device open_device<file> (const open_params<file> & op, error_code & ec);

}} // pfs::io

//namespace pfs {
//
//class file_appender : public pfs::logger_appender
//{
//	io::device _d;
//
//public:
//	file_appender (fs::path const & path);
//
//	file_appender (io::device d)
//		: _d(d)
//	{}
//
//	~file_appender ()
//	{
//		_d.close();
//	}
//
//protected:
//	virtual void print (string const & msg);
//};
//
//}

#endif /* __PFS_IO_FILE_HPP__ */
