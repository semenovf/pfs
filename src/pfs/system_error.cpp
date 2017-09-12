#include "pfs/cxxlang.hpp"
#include "pfs/system_error.hpp"

#if HAVE_BOOST_SYSTEM

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

} // detail

pfs::error_category const & generic_category ()
{
    static detail::generic_category instance;
    return instance;
}

}} // pfs::boost

#endif // HAVE_BOOST_SYSTEM