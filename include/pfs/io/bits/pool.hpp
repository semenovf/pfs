#pragma once

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
