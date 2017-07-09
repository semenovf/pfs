/* 
 * File:   uri.hpp
 * Author: wladt
 *
 * Created on March 10, 2017, 1:49 PM
 */

#ifndef __PFS_NET_URI_HPP__
#define __PFS_NET_URI_HPP__

#include <pfs/string.hpp>
#include <pfs/traits/map.hpp>
#include <pfs/traits/vector.hpp>
#include <pfs/algo/split.hpp>

namespace pfs {
namespace net {

/* C#   System.Uri
 * Java java.net.URI
 * Qt   QUrl, QUrlQuery (for query string parsing)
 */

template <typename StringT>
class uri
{
public:
    typedef StringT string_type;
    typedef traits::string<StringT> string_traits;

    struct data_rep
    {
        data_rep ()
            : port(0)
            , is_raw_host(false)
        {}
        
        void clear ();

        string_traits scheme;
        string_traits authority;
        string_traits userinfo;
        string_traits host;
        uint16_t      port;
        string_traits path;
        string_traits query;
        string_traits fragment;
        bool          is_raw_host;
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
	bool raw_host () const
	{
		return _d.is_raw_host;
	}

//	items_map_type query_items (
//		  string_type const & value_delim = string_type(1, '=')
//		, string_type const & pair_delim = string_type(1, '&'));

	bool parse (string_type const & str);

	void set_scheme (string_type const & scheme)
	{
		_d.scheme = scheme;
	}

	void set_authority (string_type const & authority)
	{
		_d.authority = authority;
	}

	void set_userinfo (string_type const & userinfo)
	{
		_d.userinfo = userinfo;
	}

	void set_host (string_type const & host)
	{
		_d.host = host;
	}

	void set_port (uint16_t port)
    {
        _d.port = port;
    }

	void set_path (string_type const & path)
	{
		_d.path = path;
	}

	void set_query (string_type const & query)
	{
		_d.query = query;
	}

	void set_fragment (string_type const & fragment)
	{
		_d.fragment = fragment;
	}
    
	string_type to_string () const;
};

template <typename StringT>
void uri<StringT>::data_rep::clear()
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

template <typename StringT>
typename uri<StringT>::string_type 
uri<StringT>::to_string () const
{
	string_traits r;

	// userinfo without host is an error
	//
	if (!_d.userinfo.empty() && _d.host.empty()) {
		return string_type(); // null string
	}

	if (!_d.scheme.empty()) {
		r.append(_d.scheme);
		r.append(string_traits(":"));
	}

	if (!_d.userinfo.empty() || !_d.host.empty()) {
		r.append(string_traits("//"));

		if (!_d.userinfo.empty()) {
			r.append(_d.userinfo);
			r.append(string_traits("@"));
		}

		if (!_d.host.empty()) {
			r.append(_d.host);
		}

		if (_d.port > 0) {
			r.append(string_traits(":"));
			r.append(to_string(_d.port));
		}

		if (!_d.path.empty()) {
			r.append(_d.path);
		}

		if (!_d.query.empty()) {
			r.append(string_traits("?"));
			r.append(_d.query);
		}

		if (!_d.fragment.empty()) {
			r.append(string_traits("#"));
			r.append(_d.fragment);
		}
	}

	return r;
}

}} // pfs::net

#endif /* __PFS_NET_URI_HPP__ */

