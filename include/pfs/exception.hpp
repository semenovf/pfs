#pragma once
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(exception)

#define PFS_THROW(ex) throw ex.at(__TFILE__, __LINE__).backtrace()
#define PFS_THROW_EXPR(expr) if (!(expr)) { throw pfs::runtime_error(#expr).at(__TFILE__, __LINE__).backtrace(); }
