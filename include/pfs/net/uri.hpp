/* 
 * File:   uri.hpp
 * Author: wladt
 *
 * Created on March 10, 2017, 1:49 PM
 */

#ifndef __PFS_NET_URI_HPP__
#define __PFS_NET_URI_HPP__

#include <pfs/traits/string.hpp>
#include <pfs/traits/map.hpp>
#include <pfs/traits/vector.hpp>
#include <pfs/algo/split.hpp>

namespace pfs {
namespace net {

/* C#   System.Uri
 * Java java.net.URI
 * Qt   QUrl, QUrlQuery (for query string parsing)
 */

template <typename Foundation, typename CharT>
class uri
{
public:
    typedef traits::string<Foundation, CharT> string_type;
    typedef traits::map<Foundation, string_type, string_type> items_map_type;
    
    struct data_rep
    {
        data_rep ()
            : port(0)
            , is_raw_host(false)
        {}
        
        void clear ();

        string_type scheme;
        /*string_type m_schemeSpecificPart;*/
        string_type authority;
        string_type userinfo;
        string_type host;
        uint16_t    port;
        string_type path;
        string_type query;
        string_type fragment;
        bool        is_raw_host;
    };

private:
	data_rep _d;

public:
	uri ()
	{}

	/**
	 * @return URI scheme as string.
	 */
	string_type const & scheme () const
	{
		return _d.scheme;
	}

	/**
	 * @return URI authority as string.
	 */
	string_type const & authority () const
	{
		return _d.authority;
	}

	/**
	 * @return URI user info as string.
	 */
	string_type const & userinfo () const
	{
		return _d.userinfo;
	}

	/**
	 * @return URI host as string.
	 */
	string_type const & host () const
	{
		return _d.host;
	}

	/**
	 * @return URI port.
	 */
	uint16_t port () const 
    {
        return _d.port;
    }


	/**
	 * @return URI path as string.
	 */
	string_type const & path () const
	{
		return _d.path;
	}

	/**
	 * @return URI query as string.
	 */
	string_type const & query () const
	{
		return _d.query;
	}

	/**
	 * @return URI fragment as string.
	 */
	string_type const & fragment () const
	{
		return _d.fragment;
	}

	/**
	 * @brief Checks if host represented as IP address (as opposite to DNS name).
	 */
	bool host_is_ip () const
	{
		return _d.is_raw_host;
	}

	items_map_type query_items (
		  string_type const & value_delim = string_type(1, '=')
		, string_type const & pair_delim = string_type(1, '&'));

	bool parse (string_type const & uri);

	string_type to_string () const;
};

template <typename Foundation, typename CharT>
void uri<Foundation, CharT>::data_rep::clear()
{
	scheme.clear();
	authority.clear();
	userinfo.clear();
	host.clear();
	port = 0;
	path.clear();
	query.clear();
	fragment.clear();
	is_raw_host = false;
}

template <typename Foundation, typename CharT>
typename uri<Foundation, CharT>::items_map_type
uri<Foundation, CharT>::query_items (
		  string_type const & value_delim
		, string_type const & pair_delim)
{
    typedef traits::vector<Foundation, string_type> stringlist_type;
	items_map_type r;

	stringlist_type pairs;
	split(this->query().cbegin(), this->query().cend()
            , pair_delim.cbegin(), pair_delim.cend()
            , pfs::keep_empty
            , & pairs);

	typename stringlist_type::const_iterator it = pairs.cbegin();
	typename stringlist_type::const_iterator it_end = pairs.cend();

	while (it != it_end) {
		stringlist_type pair;
        
        split(it->cbegin(), it->cend()
                , value_delim.cbegin(), value_delim.cend()
                , pfs::keep_empty
                , & pair);

		if (!pair.empty()) {
			r.insert(items_map_type::value_type(pair[0], pair.size() > 1 
                    ? pair[1]
                    : string_type()));
		}
		++it;
	}

	return r;
}

template <typename Foundation, typename CharT>
typename uri<Foundation, CharT>::string_type 
uri<Foundation, CharT>::to_string () const
{
	string_type r;

	// userinfo without host is an error
	//
	if (!_d.userinfo.empty() && _d.host.empty()) {
		return string_type(); // null string
	}

	if (!_d.scheme.empty()) {
		r.append(_d.scheme);
		r.append(string_type(":"));
	}

	if (!_d.userinfo.empty() || !_d.host.empty()) {
		r.append(string_type("//"));

		if (!_d.userinfo.empty()) {
			r.append(_d.userinfo);
			r.append(string_type("@"));
		}

		if (!_d.host.empty()) {
			r.append(_d.host);
		}

		if (_d.port > 0) {
			r.append(string_type(":"));
			r.append(to_string(_d.port));
		}

		if (!_d.path.empty()) {
			r.append(_d.path);
		}

		if (!_d.query.empty()) {
			r.append(string_type("?"));
			r.append(_d.query);
		}

		if (!_d.fragment.empty()) {
			r.append(string_type("#"));
			r.append(_d.fragment);
		}
	}

	return r;
}

}} // pfs::net

#endif /* __PFS_NET_URI_HPP__ */

