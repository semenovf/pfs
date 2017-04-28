/* 
 * File:   uri_query.hpp
 * Author: wladt
 *
 * Created on April 28, 2017, 5:04 PM
 */

#ifndef __PFS_NET_URI_QUERY_HPP__
#define __PFS_NET_URI_QUERY_HPP__

#include <pfs/net/uri.hpp>
#include <pfs/traits/multimap.hpp>

namespace pfs {
namespace net {

template <typename StringT>
class uri_query
{
public:
    typedef traits::string<StringT>            string_type;
    typedef traits::multimap<StringT, StringT> multimap_type;
    typedef uri<StringT>                       uri_type;

private:
    multimap_type _map;
    
public:
    uri_query ();

    uri_query (uri_type const & uri);

    uri_query (string_type const & querystr);

    uri_query (uri_query const & rhs);

};

}} // pfs::net

#endif /* __PFS_NET_URI_QUERY_HPP__ */

