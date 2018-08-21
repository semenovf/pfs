#pragma once
#include <pfs/exception.hpp>

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
        targetref = error_code_cast<OrigType, TargetType>(origref);
    }
};

} // pfs

#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(system_error)

namespace pfs {

inline bool is_error (error_code const & ec)
{
    return static_cast<bool>(ec);
}

//
// This a base class for exceptions that supports error codes.
// Do not use it directly.
//
class error_code_exception : public exception
{
public:
    error_code_exception (error_code const & ec)
        : exception(ec.message())
    {}

    explicit error_code_exception (error_code const & ec, char const * s)
        : exception(s == 0 ? ec.message() : ec.message() + ": " + s)
    {}

    explicit error_code_exception (error_code const & ec, std::string const & s)
        : exception(s.empty() ? ec.message() : ec.message() + ": " + s)
    {}
};

} // namespace pfs
