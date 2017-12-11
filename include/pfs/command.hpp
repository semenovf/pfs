#ifndef __PFS_COMMAND_HPP__
#define __PFS_COMMAND_HPP__

#include <pfs/system_error.hpp>
#include <pfs/safeformat.hpp>

namespace pfs {

template <typename PropertyTree>
struct request : public PropertyTree
{
    static request make ()
    {
        return request();
    }
};

#if __cplusplus >= 201103L
enum class response_code
{
#else
struct response_code
{
    enum value_enum {
#endif
      OK    = 200
    , USAGE = 290

    , ERRORS_BEGIN  = 400
    , BAD_REQUEST   = 400
    , NOT_FOUND     = 404
    , BAD_SCRIPT    = 470
    , UNKNOWN_ERROR = 520
    , SYSTEM_ERROR  = 600
#if __cplusplus < 201103L
    };

    value_enum v;

    response_code (value_enum x)
        : v(x)
    {}

    response_code & operator = (value_enum x)
    {
        v = x;
        return *this;
    }

    operator int () const
    {
        return static_cast<int>(v);
    }
#endif
};

template <typename PropertyTree>
struct response : public PropertyTree
{
    typedef typename PropertyTree::string_type string_type;
    typedef safeformat<string_type> fmt_type;

    static string_type CODE_KEY ()    { return "$code$"; };
    static string_type MESSAGE_KEY () { return "$message$"; }
    static string_type TEXT_KEY ()    { return "$text$"; }
    static string_type VALUE_KEY ()   { return "$value$"; }

    typedef response_code code_type;

    response () : PropertyTree() {}

    code_type code () const
    {
        return this->is_null()
                ? response_code::OK
                : static_cast<response_code>((*this)[CODE_KEY()].template get<int>());
    }

    string_type message () const
    {
        return (*this)[MESSAGE_KEY()].get_string();
    }

    response & push_text (string_type const & text)
    {
        (*this)[TEXT_KEY()].push_back(text);
        return *this;
    }

    operator bool () const
    {
        return code() == response_code::OK;
    }

    template <typename T>
    T value (T const & default_value = T()) const
    {
        return (*this)[VALUE_KEY()].template get<T>(default_value);
    }

    // Light OK response
    static response make ()
    {
        return response();
    }

    static response make (code_type code)
    {
        response r;
        r[CODE_KEY()] = static_cast<int>(code);
        return r;
    }

    static response make (code_type code, string_type const & message)
    {
        response r;
        r[CODE_KEY()] = static_cast<int>(code);
        r[MESSAGE_KEY()] = message;
        return r;
    }

    // SYSTEM_ERROR
    static response make (pfs::error_code const & ec, string_type const & message = string_type());
};

template <typename PropertyTree>
struct command
{
    typedef request<PropertyTree> request_type;
    typedef response<PropertyTree> response_type;

    virtual response_type exec (request_type const & rq) const = 0;

    // TODO For future implementation of do/undo functionality
    virtual response_type undo (request_type const & /*rq*/) const { return response_type::make(); }
};

template <typename PropertyTree>
response<PropertyTree> response<PropertyTree>::make (pfs::error_code const & ec
        , string_type const & message)
{
    response r;
    r[CODE_KEY()] = static_cast<int>(response_code::SYSTEM_ERROR);

    if (!message.empty())
        r[MESSAGE_KEY()] = fmt_type("%s: %s") 
                % message 
                % pfs::to_string<string_type>(ec);
    else
        r[MESSAGE_KEY()] = pfs::to_string<string_type>(ec);
    return r;
}

template <typename PropertyTree>
inline bool ok (response<PropertyTree> const & rp)
{
    return rp.is_null() || rp.code() == response_code::OK;
}


template <typename StringType>
StringType to_string (response_code code)
{
    switch (code) {
    case response_code::OK:    return "OK";
    case response_code::USAGE: return "USAGE";

    case response_code::BAD_REQUEST:   return "BAD_REQUEST";
    case response_code::NOT_FOUND:     return "NOT_FOUND";
    case response_code::BAD_SCRIPT:    return "BAD_SCRIPT";
    case response_code::UNKNOWN_ERROR: return "UNKNOWN_ERROR";
    case response_code::SYSTEM_ERROR:  return "SYSTEM_ERROR";
    default:
        break;
    }
    return "UNKNOWN-RESPONSE-CODE";
}

} // pfs

#endif /* __PFS_COMMAND_HPP__ */

