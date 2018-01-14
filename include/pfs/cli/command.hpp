#ifndef __PFS_CLI_COMMAND_HPP__
#define __PFS_CLI_COMMAND_HPP__

#include <pfs/command.hpp>

namespace pfs {
namespace cli {

template <typename PropertyTree, typename StringIter>
struct command : pfs::command<PropertyTree>
{
    typedef pfs::command<PropertyTree>         base_class;
    typedef StringIter                         iterator;
    typedef typename base_class::request_type  request_type;
    typedef typename base_class::response_type response_type;
    
    virtual bool parse (request_type * rq, iterator first, iterator last) = 0;
    virtual response_type usage () const
    {
        return response_type::make(response_code::USAGE);
            //.push_text("<No help usage>");
    }
};

}} // pfs::cli

#endif /* __PFS_CLI_COMMAND_HPP__ */

