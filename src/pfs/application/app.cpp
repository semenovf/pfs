/*
 * app.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#include "pfs/application/instance.hpp"

namespace pfs {
namespace application {

int app::exec (dispatcher & d)
{
	int r = exit_status::failure;

    d.connect_all();

    if (d.start()) {
   		r = d.exec();
    }

    d.finalize();

	return r;
}

}} // pfs
