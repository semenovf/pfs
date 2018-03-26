#ifndef __PFS_SYSTEM_ERROR_HPP__
#define __PFS_SYSTEM_ERROR_HPP__

namespace pfs {

template <typename OrigType, typename TargetType>
TargetType error_code_cast (OrigType const & ec);

template <typename OrigType, typename TargetType>
struct error_code_converter_helper
{
    OrigType   & origref;
    TargetType & targetref;
    
    error_code_converter_helper (OrigType & orig, TargetType & target)
        : origref(orig)
        , targetref(target)
    {}
    
    ~error_code_converter_helper ()
    {
        targetref = error_code_cast<OrigType, TargetType>();
    }
};

} // pfs

#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(system_error)

#endif /* __PFS_SYSTEM_ERROR_HPP__ */
