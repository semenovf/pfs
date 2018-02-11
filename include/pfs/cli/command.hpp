#ifndef __PFS_CLI_COMMAND_HPP__
#define __PFS_CLI_COMMAND_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/command.hpp>

namespace pfs {
namespace cli {

#if __cplusplus >= 201103L
enum class result_code
{
#else
struct result_code
{
    enum value_enum {
#endif
      OK    = 200
    , USAGE = 290

    , ERRORS_BEGIN  = 400
    , BAD_COMMAND   = 400
    , NOT_FOUND     = 404
    , BAD_SCRIPT    = 470
    , UNKNOWN_ERROR = 520
    , SYSTEM_ERROR  = 600
#if __cplusplus < 201103L
    };

    value_enum v;

    result_code (value_enum x)
        : v(x)
    {}

    result_code & operator = (value_enum x)
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

inline char const * RESULT_CODE_KEY ()    { return "result-code"; }
inline char const * RESULT_MESSAGE_KEY () { return "result-message"; }
inline char const * RESULT_USAGE_KEY ()   { return "result-usage"; }
inline char const * RESULT_TEXT_KEY ()    { return "result-text"; }

template <typename PropertyTree>
struct params : public PropertyTree
{
    typedef typename PropertyTree              base_class;
    typedef typename PropertyTree::string_type string_type;

    params () : base_class() {}
};

template <typename PropertyTree>
struct result : public PropertyTree
{
    typedef typename PropertyTree              base_class;
    typedef typename PropertyTree::string_type string_type;

    result () : base_class() {}

    static result make (result_code rc, string_type const & message)
    {
        result_type r;
        r[RESULT_CODE_KEY()] = static_cast<int>(rc);
        r[RESULT_MESSAGE_KEY()] = message;
        return r;
    }

    static result make (result_code rc)
    {
        return make(rc, string_type());
    }

    result_code code () const
    {
        return static_cast<result_code>((*this)[RESULT_CODE_KEY()].template get<int>());
    }

    string_type message () const
    {
        return (*this)[RESULT_MESSAGE_KEY()].get_string();
    }
};

template <typename PropertyTree, typename StringIter>
class command : pfs::command::command<
          params<PropertyTree>
        , result<PropertyTree> >
{
public:
    typedef pfs::command::command<
            params<PropertyTree>
          , result<PropertyTree> >               base_class;
    typedef StringIter                           iterator;
    typedef typename base_class::params_type     params_type;
    typedef typename base_class::result_type     result_type;
    typedef typename PropertyTree::string_type   string_type;
    typedef safeformat<string_type>              fmt_type;

    command (string_type const & name)
        : base_class()
        , _name(name)
    {}

    virtual result_type exec (params_type const & parms) const pfs_override = 0;

    virtual bool undo () const pfs_override
    {}

    virtual bool parse (params_type & params, iterator first, iterator last) = 0;

    virtual result_type usage () const = 0;

    result_type exec (iterator first, iterator last) const
    {
        params_type params;
        return this->parse(params, first, last)
                ? exec(params)
                : result_type::make(result_code::BAD_COMMAND
                        , fmt_type("%s: invalid or incomplete command") % _name);
    }

protected:
    string_type _name;
};

}} // pfs::cli

namespace pfs {

template <typename StringType>
StringType to_string (cli::result_code code)
{
    switch (code) {
    case cli::result_code::OK:    return "OK";
    case cli::result_code::USAGE: return "USAGE";

    case cli::result_code::BAD_COMMAND:   return "BAD_COMMAND";
    case cli::result_code::NOT_FOUND:     return "NOT_FOUND";
    case cli::result_code::BAD_SCRIPT:    return "BAD_SCRIPT";
    case cli::result_code::UNKNOWN_ERROR: return "UNKNOWN_ERROR";
    case cli::result_code::SYSTEM_ERROR:  return "SYSTEM_ERROR";
    default:
        break;
    }
    return "UNKNOWN-RESULT-CODE";
}

} // pfs

#endif /* __PFS_CLI_COMMAND_HPP__ */

