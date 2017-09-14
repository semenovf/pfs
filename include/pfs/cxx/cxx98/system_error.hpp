/* 
 * File:   system_error.hpp
 * Author: wladt
 *
 * Created on June 21, 2017, 3:22 PM
 */

#ifndef __PFS_CXX_CXX98_SYSTEM_ERROR_HPP__
#define __PFS_CXX_CXX98_SYSTEM_ERROR_HPP__

#include <pfs/cxxlang.hpp>

#if HAVE_BOOST_SYSTEM

#include <boost/system/error_code.hpp>

namespace pfs {

typedef ::boost::system::error_code      error_code;
typedef ::boost::system::error_category  error_category;
typedef ::boost::system::error_condition error_condition;
//typedef ::boost::system::errc::errc_t    errc;

struct errc 
{
    //typedef ::boost::system::errc::errc_t value_enum;
    enum value_enum {
          success                            = ::boost::system::errc::success
        , address_family_not_supported       = ::boost::system::errc::address_family_not_supported
        , address_in_use                     = ::boost::system::errc::address_in_use
        , address_not_available              = ::boost::system::errc::address_not_available
        , already_connected                  = ::boost::system::errc::already_connected
        , argument_list_too_long             = ::boost::system::errc::argument_list_too_long
        , argument_out_of_domain             = ::boost::system::errc::argument_out_of_domain
        , bad_address                        = ::boost::system::errc::bad_address
        , bad_file_descriptor                = ::boost::system::errc::bad_file_descriptor
        , bad_message                        = ::boost::system::errc::bad_message
        , broken_pipe                        = ::boost::system::errc::broken_pipe
        , connection_aborted                 = ::boost::system::errc::connection_aborted
        , connection_already_in_progress     = ::boost::system::errc::connection_already_in_progress
        , connection_refused                 = ::boost::system::errc::connection_refused
        , connection_reset                   = ::boost::system::errc::connection_reset
        , cross_device_link                  = ::boost::system::errc::cross_device_link
        , destination_address_required       = ::boost::system::errc::destination_address_required
        , device_or_resource_busy            = ::boost::system::errc::device_or_resource_busy
        , directory_not_empty                = ::boost::system::errc::directory_not_empty
        , executable_format_error            = ::boost::system::errc::executable_format_error
        , file_exists                        = ::boost::system::errc::file_exists
        , file_too_large                     = ::boost::system::errc::file_too_large
        , filename_too_long                  = ::boost::system::errc::filename_too_long
        , function_not_supported             = ::boost::system::errc::function_not_supported
        , host_unreachable                   = ::boost::system::errc::host_unreachable
        , identifier_removed                 = ::boost::system::errc::identifier_removed
        , illegal_byte_sequence              = ::boost::system::errc::illegal_byte_sequence
        , inappropriate_io_control_operation = ::boost::system::errc::inappropriate_io_control_operation
        , interrupted                        = ::boost::system::errc::interrupted
        , invalid_argument                   = ::boost::system::errc::invalid_argument
        , invalid_seek                       = ::boost::system::errc::invalid_seek
        , io_error                           = ::boost::system::errc::io_error
        , is_a_directory                     = ::boost::system::errc::is_a_directory
        , message_size                       = ::boost::system::errc::message_size
        , network_down                       = ::boost::system::errc::network_down
        , network_reset                      = ::boost::system::errc::network_reset
        , network_unreachable                = ::boost::system::errc::network_unreachable
        , no_buffer_space                    = ::boost::system::errc::no_buffer_space
        , no_child_process                   = ::boost::system::errc::no_child_process
        , no_link                            = ::boost::system::errc::no_link
        , no_lock_available                  = ::boost::system::errc::no_lock_available
        , no_message_available               = ::boost::system::errc::no_message_available
        , no_message                         = ::boost::system::errc::no_message
        , no_protocol_option                 = ::boost::system::errc::no_protocol_option
        , no_space_on_device                 = ::boost::system::errc::no_space_on_device
        , no_stream_resources                = ::boost::system::errc::no_stream_resources
        , no_such_device_or_address          = ::boost::system::errc::no_such_device_or_address
        , no_such_device                     = ::boost::system::errc::no_such_device
        , no_such_file_or_directory          = ::boost::system::errc::no_such_file_or_directory
        , no_such_process                    = ::boost::system::errc::no_such_process
        , not_a_directory                    = ::boost::system::errc::not_a_directory
        , not_a_socket                       = ::boost::system::errc::not_a_socket
        , not_a_stream                       = ::boost::system::errc::not_a_stream
        , not_connected                      = ::boost::system::errc::not_connected
        , not_enough_memory                  = ::boost::system::errc::not_enough_memory
        , not_supported                      = ::boost::system::errc::not_supported
        , operation_canceled                 = ::boost::system::errc::operation_canceled
        , operation_in_progress              = ::boost::system::errc::operation_in_progress
        , operation_not_permitted            = ::boost::system::errc::operation_not_permitted
        , operation_not_supported            = ::boost::system::errc::operation_not_supported
        , operation_would_block              = ::boost::system::errc::operation_would_block
        , owner_dead                         = ::boost::system::errc::owner_dead
        , permission_denied                  = ::boost::system::errc::permission_denied
        , protocol_error                     = ::boost::system::errc::protocol_error
        , protocol_not_supported             = ::boost::system::errc::protocol_not_supported
        , read_only_file_system              = ::boost::system::errc::read_only_file_system
        , resource_deadlock_would_occur      = ::boost::system::errc::resource_deadlock_would_occur
        , resource_unavailable_try_again     = ::boost::system::errc::resource_unavailable_try_again
        , result_out_of_range                = ::boost::system::errc::result_out_of_range
        , state_not_recoverable              = ::boost::system::errc::state_not_recoverable
        , stream_timeout                     = ::boost::system::errc::stream_timeout
        , text_file_busy                     = ::boost::system::errc::text_file_busy
        , timed_out                          = ::boost::system::errc::timed_out
        , too_many_files_open_in_system      = ::boost::system::errc::too_many_files_open_in_system
        , too_many_files_open                = ::boost::system::errc::too_many_files_open
        , too_many_links                     = ::boost::system::errc::too_many_links
        , too_many_symbolic_link_levels      = ::boost::system::errc::too_many_symbolic_link_levels
        , value_too_large                    = ::boost::system::errc::value_too_large
        , wrong_protocol_type                = ::boost::system::errc::wrong_protocol_type
    } v;
    
    errc (value_enum x)
        : v(x)
    {}

    errc & operator = (value_enum x)
    {
        v = x;
        return *this;
    }
    
    operator int () const
    {
        return static_cast<int>(v);
    }
};

inline error_category const & system_category () pfs_noexcept
{
    return ::boost::system::system_category();
}

inline error_category const & generic_category () pfs_noexcept
{
    return ::boost::system::generic_category();
}

inline error_code make_error_code (errc e) pfs_noexcept
{
    return error_code(e, system_category());
}

template <typename StringT>
inline StringT to_string (error_code const & ec)
{
    return StringT(ec.message());
}

// [Construct std::error_code from errno on POSIX and GetLastError() on Windows]
// (https://stackoverflow.com/questions/13950938/construct-stderror-code-from-errno-on-posix-and-getlasterror-on-windows)
//
inline error_code get_last_system_error ()
{
#if PFS_CC_MSC
    return error_code(::GetLastError(), system_category());
#else // POSIX
    return error_code(errno, system_category());
#endif // !PFS_CC_MSC
}

template <typename ErrorCode>
ErrorCode lexical_cast (pfs::error_code const & ec);

template <>
inline ::boost::system::error_code 
lexical_cast< ::boost::system::error_code> (::boost::system::error_code const & ec)
{
    return ec;
}

} // pfs

#else

#   error "Need to implement system_error for C++98"

#endif // HAVE_BOOST_SYSTEM

#endif /* __PFS_CXX_CXX98_SYSTEM_ERROR_HPP__ */
