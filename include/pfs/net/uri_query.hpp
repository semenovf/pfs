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
    
private:
    void parse (string_type const & querystr);

public:
    uri_query ()
    {}

    uri_query (uri_type const & uri)
    {
        parse(string_type(uri.query()));
    }

    uri_query (StringT const & querystr)
    {
        parse(string_type(querystr));
    }
             
    uri_query (char const * querystr)
    {
        parse(string_type(querystr));
    }

    uri_query (wchar_t const * querystr)
    {
        parse(string_type(querystr));
    }

    uri_query (uri_query const & rhs)
        : _map(rhs._map)
    {}
};

}} // pfs::net

#endif /* __PFS_NET_URI_QUERY_HPP__ */

