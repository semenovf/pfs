#include "pfs/cxxlang.hpp"
#include "pfs/system_error.hpp"

#if __cplusplus >= 201103L && HAVE_BOOST_SYSTEM_ERROR

namespace pfs {
namespace boost {
namespace detail {

class generic_category : public std::error_category
{
public:
    virtual char const * name () const pfs_noexcept pfs_override
    {
        return ::boost::system::generic_category().name();
    }
    virtual std::string message (int value) const pfs_override
    {
        return ::boost::system::generic_category().message(value);
    }
};

class system_category : public std::error_category
{
public:
    virtual char const * name () const pfs_noexcept pfs_override
    {
        return ::boost::system::system_category().name();
    }
    virtual std::string message (int value) const pfs_override
    {
        return ::boost::system::system_category().message(value);
    }
};

} // detail

pfs::error_category const & generic_category ()
{
    static detail::generic_category instance;
    return instance;
}

pfs::error_category const & system_category ()
{
    static detail::system_category instance;
    return instance;
}

}} // pfs::boost

#endif
