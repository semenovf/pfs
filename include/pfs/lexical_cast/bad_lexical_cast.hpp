/* 
 * File:   bad_lexical_cast.hpp
 * Author: wladt
 *
 * Created on June 28, 2017, 8:28 AM
 */

#ifndef __PFS_LEXICAL_CAST_BAD_LEXICAL_CAST_HPP__
#define __PFS_LEXICAL_CAST_BAD_LEXICAL_CAST_HPP__

#include <pfs/exception.hpp>

namespace pfs {

class bad_lexical_cast : public logic_error
{
public:
    explicit bad_lexical_cast ()
        : logic_error("")
    {}

    explicit bad_lexical_cast (char const * what)
        : logic_error(what)
    {}
        
    virtual ~bad_lexical_cast() {}
};

} // pfs

#endif /* __PFS_LEXICAL_CAST_BAD_LEXICAL_CAST_HPP__ */

