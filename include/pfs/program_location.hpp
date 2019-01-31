#pragma once
#include <pfs/filesystem.hpp>
#include <pfs/system_error.hpp>

namespace pfs {

// TODO Implement for different platforms (there is GNU-specific implementation only)
// see https://www.boost.org/doc/libs/1_64_0/boost/dll/detail/posix/program_location_impl.hpp
// for implementations for different platforms.
filesystem::path program_location (error_code & ec);
filesystem::path program_location ();

} // namespace pfs
