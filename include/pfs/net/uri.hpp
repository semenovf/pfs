#pragma once
#include <pfs/types.hpp>
#include <pfs/string.hpp>

namespace pfs {
namespace net {

/*
 * C#   System.Uri
 * Java java.net.URI
 * Qt   QUrl, QUrlQuery (for query string parsing)
 */
template <typename StringT = pfs::string>
class uri
{
public:
    typedef StringT string_type;

    struct data_rep
    {
       data_rep ()
            : port (0)
            , is_raw_host (false)
        {}

        void clear ()
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

        string_type scheme;
        string_type authority;
        string_type userinfo;
        string_type host;
        uint16_t port;
        string_type path;
        string_type query;
        string_type fragment;
        bool is_raw_host;
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

    string_type to_string () const
    {
        string_type r;

        // userinfo without host is an error
        //
        if (!_d.userinfo.empty() && _d.host.empty()) {
            return string_type(); // null string
        }

        if (!_d.scheme.empty()) {
            r.append(_d.scheme);
            r.push_back(':');
        }

        if (!_d.userinfo.empty() || !_d.host.empty()) {
            r.append("//");

            if (!_d.userinfo.empty()) {
                r.append(_d.userinfo);
                r.push_back('@');
            }

            if (!_d.host.empty()) {
                r.append(_d.host);
            }

            if (_d.port > 0) {
                r.push_back(':');
                r.append(pfs::to_string(_d.port));
            }
        }

        if (!_d.path.empty()) {
            r.append(_d.path);
        }

        if (!_d.query.empty()) {
            r.push_back('?');
            r.append(_d.query);
        }

        if (!_d.fragment.empty()) {
            r.push_back('#');
            r.append(_d.fragment);
        }

        return r;
    }

    bool parse (string_type const & str)
    {
        return parse(str.cbegin(), str.cend());
    }

    bool parse (typename string_type::const_iterator first
            , typename string_type::const_iterator last);

};

}} // pfs::net

#include <pfs/net/uri_parse.hpp>
