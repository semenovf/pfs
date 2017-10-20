/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on October 20, 2016, 1:33 PM
 */

#ifndef __PFS_CXX98_EXCEPTION_HPP__
#define __PFS_CXX98_EXCEPTION_HPP__

#include <stdexcept>

namespace pfs {

typedef std::bad_alloc        bad_alloc;
typedef std::invalid_argument invalid_argument;
typedef std::length_error     length_error;
typedef std::out_of_range     out_of_range;
typedef std::runtime_error    runtime_error;
typedef std::range_error      range_error;
typedef std::overflow_error   overflow_error;
typedef std::underflow_error  underflow_error;


class logic_error : public std::logic_error 
{
    typedef std::logic_error base_class;

public:
    explicit logic_error (std::string const & s) : base_class(s) {}

#if __cplusplus >= 201103L
    explicit logic_error (char const * s) : base_class(s) {}
#else
    explicit logic_error (char const * s) : base_class(std::string(s)) {}
#endif
};

class domain_error : public std::domain_error
{
    typedef std::domain_error base_class;
public:
    explicit domain_error (std::string const & s) : base_class(s) {}
#if __cplusplus >= 201103L
    explicit domain_error (char const * s) : base_class(s) {}
#else
    explicit domain_error (char const * s) : base_class(std::string(s)) {}
#endif
};

} // pfs

#endif /* __PFS_CXX98_EXCEPTION_HPP__ */

