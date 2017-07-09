/* 
 * File:   uri_query.hpp
 * Author: wladt
 *
 * Created on April 28, 2017, 5:04 PM
 */

#ifndef __PFS_NET_URI_QUERY_HPP__
#define __PFS_NET_URI_QUERY_HPP__

#include <pfs/string.hpp>
#include <pfs/traits/stdcxx/multimap.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/net/uri.hpp>

namespace pfs {
namespace net {

template <typename StringT
    , template <typename, typename> class MultiMapT = stdcxx::multimap
    , template <typename> class VectorT = stdcxx::vector>
class uri_query
{
public:
    typedef StringT string_type;
    
private:    
    typedef traits::string<StringT>                       string_traits;
    typedef traits::multimap<StringT, StringT, MultiMapT> multimap_traits;
    typedef traits::vector<StringT, VectorT>              stringlist_traits;
    typedef uri<StringT>                                  uri_type;

private:
    multimap_traits _map;
    
private:
    void parse (string_traits const & querystr
            , string_traits const & value_delim
            , string_traits const & pair_delim);

public:
    uri_query ()
    {}

    uri_query (uri_type const & uri
            , string_type const & value_delim = string_type("=")
            , string_type const & pair_delim = string_type("&"))
    {
        parse(string_traits(uri.query())
                , string_traits(value_delim)
                , string_traits(value_delim));
    }

    uri_query (string_type const & querystr
            , string_type const & value_delim = string_type("=")
            , string_type const & pair_delim = string_type("&"))
    {
        parse(string_traits(querystr)
                , string_traits(value_delim)
                , string_traits(value_delim));
    }
             
    uri_query (char const * querystr
            , char const * value_delim = "="
            , char const * pair_delim = "&")
    {
        parse(string_traits(querystr)
                , string_traits(value_delim)
                , string_traits(value_delim));
    }

    uri_query (wchar_t const * querystr
            , wchar_t const * value_delim = L"="
            , wchar_t const * pair_delim = L"&")
    {
        parse(string_traits(querystr)
                , string_traits(value_delim)
                , string_traits(value_delim));
    }

    uri_query (uri_query const & rhs)
        : _map(rhs._map)
    {}
};

template <typename StringT>
void uri_query<StringT>::parse (string_traits const & querystr
        , string_traits const & value_delim
        , string_traits const & pair_delim)
{
//template <typename Foundation, typename CharT>
//typename uri<Foundation, CharT>::items_map_type
//uri<Foundation, CharT>::query_items (
//		  string_type const & value_delim
//		, string_type const & pair_delim)
//{
//	items_map_type r;
//
	stringlist_type pairs;
	split(querystr.cbegin(), querystr.cend()
            , pair_delim.cbegin(), pair_delim.cend()
            , pfs::keep_empty
            , & pairs);

//	typename stringlist_type::const_iterator it = pairs.cbegin();
//	typename stringlist_type::const_iterator it_end = pairs.cend();
//
//	while (it != it_end) {
//		stringlist_type pair;
//        
//        split(it->cbegin(), it->cend()
//                , value_delim.cbegin(), value_delim.cend()
//                , pfs::keep_empty
//                , & pair);
//
//		if (!pair.empty()) {
//			r.insert(items_map_type::value_type(pair[0], pair.size() > 1 
//                    ? pair[1]
//                    : string_type()));
//		}
//		++it;
//	}
//
//	return r;
//}
    
}

}} // pfs::net

#endif /* __PFS_NET_URI_QUERY_HPP__ */

