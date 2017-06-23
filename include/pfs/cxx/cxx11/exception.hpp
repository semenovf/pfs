/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on October 20, 2016, 1:33 PM
 */

#ifndef __PFS_CXX11_EXCEPTION_HPP__
#define __PFS_CXX11_EXCEPTION_HPP__

#include <stdexcept>

namespace pfs {

using bad_alloc        = std::bad_alloc;
using logic_error      = std::logic_error;
using domain_error     = std::domain_error;
using invalid_argument = std::invalid_argument;
using length_error     = std::length_error;
using out_of_range     = std::out_of_range;
using runtime_error    = std::runtime_error;
using range_error      = std::range_error;
using overflow_error   = std::overflow_error;
using underflow_error  = std::underflow_error;

} // pfs


#endif /* __PFS_CXX11_EXCEPTION_HPP__ */

