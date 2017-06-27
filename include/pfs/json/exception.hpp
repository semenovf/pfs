/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on June 27, 2017, 3:45 PM
 */

#ifndef __PFS_JSON_EXCEPTION_HPP__
#define __PFS_JSON_EXCEPTION_HPP__

#include <pfs/exception.hpp>

namespace pfs {
namespace json {

class exception : public logic_error
{
public:
    enum id {
        BAD_NUMBER = 1001
    };
    
public:
    explicit exception (id i)
        : logic_error("")
        , _id(i)
    {}

    virtual ~exception() {}
    
private:
    id _id;
};

}}

#endif /* __PFS_JSON_EXCEPTION_HPP__ */

