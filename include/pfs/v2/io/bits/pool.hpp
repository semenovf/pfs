#ifndef __PFS_IO_BITS_POOL_HPP__
#define __PFS_IO_BITS_POOL_HPP__

namespace pfs {
namespace io {
namespace bits {

struct pool {};

struct pool_iterator
{
	virtual ~pool_iterator () {}

	virtual void next () = 0;
};

}}}

#endif /* __PFS_IO_BITS_POOL_HPP__ */
