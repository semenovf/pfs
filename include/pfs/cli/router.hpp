#ifndef __PFS_CLI_ROUTER_HPP__
#define __PFS_CLI_ROUTER_HPP__

#include <pfs/memory.hpp>
#include <pfs/traits/associative_container.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/io/file.hpp>
#include <pfs/io/iterator.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/io/iterator.hpp>
#include <pfs/cli/command.hpp>

namespace pfs {
namespace cli {

template <typename PropertyTree, typename StringIter>
class route
{
public:
    typedef StringIter                           iterator;
    typedef command<PropertyTree, iterator>      command_type;
    typedef typename command_type::request_type  request_type;
    typedef typename command_type::response_type response_type;
    typedef typename response_type::string_type  string_type;

private:
    pfs::shared_ptr<command_type> _pcmd;

public:
#if __cplusplus >= 201103L
    route (route const & rhs) = default;
    route & operator = (route const & rhs) = default;
    ~route () = default;
#else
    route (route const & rhs) : _pcmd(rhs._pcmd) {}
    route & operator = (route const & rhs)
    {
        _pcmd = rhs._pcmd;
        return *this;
    }
    ~route () {}
#endif

    route (command_type * pcmd)
        : _pcmd(pcmd)
    {}

    response_type exec (iterator first, iterator last) const
    {
        request_type rq;
        return _pcmd->parse(& rq, first, last)
                ? exec(rq)
                : response_type::make(response_code::BAD_REQUEST, "invalid or incomplete command");
    }
   
    response_type exec (request_type const & rq) const
    {
        return _pcmd->exec(rq);
    }
    
    response_type usage () const
    {
        return _pcmd->usage();
    }
};

template <typename RouteType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
class router
{
public:
    typedef RouteType                                  route_type;
    typedef Ostream                                    ostream_type;
    typedef typename route_type::iterator              iterator;
    typedef typename route_type::command_type          command_type;
    typedef typename route_type::request_type          request_type;
    typedef typename route_type::response_type         response_type;
    typedef typename route_type::string_type           string_type;
    typedef pfs::traits::associative_container<
              pfs::traits::kv<string_type, route_type>
            , AssociativeContainerTraits>              route_map;
    typedef safeformat<string_type>                    fmt_type;

private:
    route_map _routemap;
    int       _nmaxcommands;
    bool      _echo_command;    // TODO merge into ... 
    bool      _print_response;  // ... integer flag
    ostream_type & _sout;
    ostream_type & _serr;
    
public:
    static string_type build_key (iterator & first, iterator last, int n = -1);

    static string_type build_key (string_type const & cmd
            , string_type const & subcmd)
    {
        string_type r = cmd;
        r += '$';
        r += subcmd;
        return r;
    }

    static string_type build_key (string_type const & cmd
            , string_type const & subcmd1
            , string_type const & subcmd2)
    {
        string_type r = cmd;
        r += '$';
        r += subcmd1;
        r += '$';
        r += subcmd2;
        return r;
    }
    
    static string_type replace_separator (string_type const & str)
    {
        return string_type::replace(str.begin(), str.end(), '$', ' ');
    }

public:
    router (ostream_type & sout, ostream_type & serr) 
        : _nmaxcommands(0)
        , _echo_command(true)
        , _print_response(false)
        , _sout(sout)
        , _serr(serr)
    {}
    
    void set_echo_command (bool value)
    {
        _echo_command = value;
    }

    void set_print_response (bool value)
    {
        _print_response = value;
    }
    
    router & add (string_type const & cmd, command_type * pcmd)
    {
        _routemap.insert(cmd, route_type(pcmd));
        _nmaxcommands = pfs::max(_nmaxcommands, 1);
        return *this;
    }

    router & add (string_type const & cmd
            , string_type const & subcmd, command_type * pcmd)
    {
        _routemap.insert(build_key(cmd, subcmd), route_type(pcmd));
        _nmaxcommands = pfs::max(_nmaxcommands, 2);
        return *this;
    }

    router & add (string_type const & cmd
            , string_type const & subcmd1
            , string_type const & subcmd2
            , command_type * pcmd)
    {
        _routemap.insert(build_key(cmd, subcmd1, subcmd2), route_type(pcmd));
        _nmaxcommands = pfs::max(_nmaxcommands, 3);
        return *this;
    }
    
    route_type const * find (string_type const & cmd) const
    {
        typename route_map::const_iterator it = _routemap.find(cmd);
        return (it != _routemap.cend())
                ? & (*it)
                : static_cast<route_type *>(0);
    }

    route_type const * find (iterator & first, iterator last) const;
    
    template <typename OutputIt>
    void command_list (OutputIt out) const
    {
        for (typename route_map::const_iterator i = _routemap.cbegin()
                ; i != _routemap.cend()
                ; ++i) {
            *out++ = route_map::key(i);
        }
    }
    
    void echo (string_type const & text)
    {
        _sout << text << "\n";
    }

    void echo_command (string_type const & cmd)
    {
        if (_echo_command)
            _sout << cmd << "\n";
    }
    
    void print_response (response_type const & rp) const;

    void print_usage (response_type const & rp) const
    {
        print_response(rp);
    }
    
    bool exec (string_type const & command, bool is_print_response = true);
    response_type exec (filesystem::path const & script);
};

template <typename RouteType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
typename router<RouteType, Ostream, AssociativeContainerTraits>::string_type
router<RouteType, Ostream, AssociativeContainerTraits>::build_key (iterator & first, iterator last, int n)
{
    if (first == last || n == 0)
        return "";
    
    if (n < 0)
        n = pfs::numeric_limits<int>::max();
    
    string_type r = *first++;
    --n;

    while (first != last && n > 0) {
        r += '$';
        r += *first++;
        --n;
    }

    return r;
}

template <typename RouteType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
typename router<RouteType, Ostream, AssociativeContainerTraits>::route_type const *
router<RouteType, Ostream, AssociativeContainerTraits>::find (iterator & first, iterator last) const
{
    route_type const * r = 0;
    int i = _nmaxcommands;
    iterator it = first;

    while (i > 0 && r == 0) {
        it = first;
        r = this->find(build_key(it, last, i--));
    }

    if (r)
        first = it;
    return r;
}

template <typename RouteType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
void router<RouteType, Ostream, AssociativeContainerTraits>::print_response (response_type const & rp) const
{
    ostream_type * out = & _sout;
            
    if (rp.code() < response_code::ERRORS_BEGIN) {
        _sout << fmt_type("[%d]: %s") 
                % static_cast<int>(rp.code()) 
                % pfs::to_string<string_type>(rp.code());
        _sout << "\n";
    } else if (rp.code() >= response_code::ERRORS_BEGIN) {
        _serr << fmt_type("[%d]: %s: %s") 
                % static_cast<int>(rp.code()) 
                % pfs::to_string<string_type>(rp.code())
                % rp.message();
        _serr << "\n";
        out = & _serr;
    }
    
    typename response_type::const_iterator it = rp.find(response_type::TEXT_KEY());
    
    if (it != rp.cend() && it->is_array()) {
        for (typename response_type::size_type i = 0; i < it->size(); ++i) {
            *out << (*it)[i].get_string();
            *out << "\n";
        }
    }
}

template <typename RouteType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
bool router<RouteType, Ostream, AssociativeContainerTraits>::exec (
          string_type const & command
        , bool is_print_response)
{
    typedef stringlist<string_type> stringlist_type;

    stringlist_type command_line;
    command_line.split(command, string_type(1, ' '), pfs::dont_keep_empty);

    typename stringlist_type::const_iterator first = command_line.cbegin();
    typename stringlist_type::const_iterator last = command_line.cend();
    route_type const * r = this->find(first, last);

    response_type rp = (r != 0)
            ? r->exec(first, last)
            : response_type::make(response_code::NOT_FOUND
                    , fmt_type("`%s': bad command") % *first);

    if (is_print_response) {
        if (rp.code() >= response_code::ERRORS_BEGIN) {
            print_response(rp);

            if (r && rp.code() == response_code::BAD_REQUEST)
                print_usage(r->usage());
        } else {
            print_response(rp);
        }
    }
    
    return ok(rp);
}

template <typename RouteType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
typename router<RouteType, Ostream, AssociativeContainerTraits>::response_type
router<RouteType, Ostream, AssociativeContainerTraits>::exec (filesystem::path const & script)
{
    pfs::error_code ec;
            
    if (!filesystem::exists(script, ec)) {
        if (ec)
            return response_type::make(ec);
        return response_type::make(response_code::NOT_FOUND
                , fmt_type("`%s`: file not found") % pfs::to_string<string_type>(script));
    }
    
    io::device file = io::open_device<io::file>(
            io::open_params<io::file>(script, io::read_only), ec);
    
    if (ec) {
        return response_type::make(response_code::SYSTEM_ERROR
                , fmt_type("`%s`: file open failure: %s")
                        % pfs::to_string<string_type>(script)
                        % pfs::to_string<string_type>(ec));
    }

    io::input_iterator<char> first(file);
    io::input_iterator<char> last;
    
    bool bad_script = false;
    int line = 0;

    while (first != last) {
        string_type cmd = pfs::read_line_u8<string_type>(first, last);
        ++line;
        
        if (!cmd.empty()) {
            echo_command(cmd);
            
            if (!exec(cmd, _print_response)) {
                bad_script = true;
                break;
            }
        }
    }

    if (file.is_error()) {
        return response_type::make(response_code::SYSTEM_ERROR
                , fmt_type("`%s`: file read failure: %s")
                        % pfs::to_string<string_type>(script)
                        % pfs::to_string<string_type>(file.errorcode()));
    }

    return bad_script 
            ? response_type::make(response_code::BAD_SCRIPT
                    , fmt_type("`%s`: bad command at line %d")
                            % pfs::to_string<string_type>(script)
                            % line)
            : response_type::make();
}

template <typename ResponseType>
ResponseType read_all (filesystem::path const & path)
{
    typedef ResponseType response_type;
    typedef typename response_type::string_type string_type;

    pfs::error_code ec;
            
    if (!filesystem::exists(path, ec)) {
        if (ec)
            return response_type::make(ec);
        
        string_type msg;
        msg += '`';
        msg += pfs::to_string<string_type>(path);
        msg += "': ";
        msg += "file not found";
        return response_type::make(response_code::NOT_FOUND, msg);
    }
    
    io::device file = io::open_device<io::file>(
            io::open_params<io::file>(path, io::read_only), ec);

    if (ec) {
        string_type msg;
        msg += '`';
        msg += pfs::to_string<string_type>(path);
        msg += "': ";
        msg += "file open failure";
        return response_type::make(response_code::SYSTEM_ERROR, msg);
    }
    
    response_type rp = response_type::make(response_code::OK);
    rp[response_type::VALUE_KEY()] = pfs::read_all_u8<string_type>(file);

    return rp;
}

}} // pfs::cli

#endif // __PFS_CLI_ROUTER_HPP__