/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on February 8, 2017, 4:23 PM
 */

#ifndef __PFS_DB_EXCEPTION_HPP__
#define __PFS_DB_EXCEPTION_HPP__

#include <pfs/string.hpp>

namespace pfs {
namespace db {

template <typename StringT>
class exception
{
public:
    typedef pfs::string<StringT>        string_type;
    typedef typename string_type::const_pointer const_pointer;
        
protected:
    string_type _msg;
    
public:
    explicit exception (StringT const & msg)
        : _msg(msg)
    {}

    explicit exception (char const * msg)
        : _msg(msg)
    {}

    explicit exception (wchar_t const * msg)
        : _msg(msg)
    {}

    StringT const & what () const
    {
        return _msg;
    }
};

}} // pfs::db;

#endif /* __PFS_DB_EXCEPTION_HPP__ */

