#ifndef __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__

#include <pfs/config.h>

#if HAVE_BOOST_FILESYSTEM

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <pfs/system_error.hpp>

namespace pfs {
namespace filesystem {

class path : public ::boost::filesystem::path
{
    typedef ::boost::filesystem::path base_class;

public:
    typedef typename base_class::string_type string_type;

public:
    path ()
        : base_class()
    {}

    path (base_class const & other)
        : base_class(other)
    {}

    path (path const & other)
        : base_class(other)
    {}

    path const & operator = (path const & other)
    {
        base_class::operator = (other);
        return *this;
    }

    template <typename Source>
    explicit path (Source const & source)
        : base_class(source)
    {}

    template <typename InputIt>
    path (InputIt first, InputIt last)
        : base_class(first, last)
    {}

//    template <typename Source>
//    path (Source const & source, std::locale const & loc)
//        : base_class(source, loc)
//    {}

//    // FIXME fmt unused yet
//    template <typename InputIt>
//    path (InputIt first, InputIt last, std::locale const & loc)
//        : base_class(first, last, loc)
//    {}

    template <typename Source>
    path & operator = (Source const & source)
    {
        base_class::operator = (source);
        return *this;
    }

    operator string_type () const
    {
        return base_class::native();
    }

    // TODO Implement file_status
    //bool exists (filesystem::file_status s) noexcept;

    bool exists (error_code & ec) const pfs_noexcept
    {
        ::boost::system::error_code boost_ec;
        error_code_converter_helper< ::boost::system::error_code, error_code> ch(boost_ec, ec);
        return ::boost::filesystem::exists(*this, boost_ec);
    }

    bool remove (error_code & ec) const pfs_noexcept
    {
        ::boost::system::error_code boost_ec;
        error_code_converter_helper< ::boost::system::error_code, error_code> ch(boost_ec, ec);
        return ::boost::filesystem::remove(*this, boost_ec);
    }

    path & replace_filename (path const & replacement)
    {
        remove_filename();
        base_class::operator /= (replacement);
        return *this;
    }
};

inline path temp_directory_path ()
{
    return path(::boost::filesystem::temp_directory_path());
}

inline path temp_directory_path (error_code & ec)
{
    ::boost::system::error_code boost_ec;
    error_code_converter_helper< ::boost::system::error_code, error_code> ch(boost_ec, ec);
    return ::boost::filesystem::temp_directory_path(boost_ec);
}

inline path current_path ()
{
    return ::boost::filesystem::current_path();
}

inline path current_path (error_code & ec)
{
    ::boost::system::error_code boost_ec;
    error_code_converter_helper< ::boost::system::error_code, error_code> ch(boost_ec, ec);
    return ::boost::filesystem::current_path(boost_ec);
}

inline void current_path (path const & p)
{
    ::boost::filesystem::current_path(p);
}

inline void current_path (path const & p, error_code & ec)
{
    ::boost::system::error_code boost_ec;
    error_code_converter_helper< ::boost::system::error_code, error_code> ch(boost_ec, ec);
    ::boost::filesystem::current_path(p, boost_ec);
}

}}

#else
#   error "Filesystem implementation not found"
#endif

#endif /* __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__ */
