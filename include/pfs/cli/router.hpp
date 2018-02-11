#ifndef __PFS_CLI_ROUTER_HPP__
#define __PFS_CLI_ROUTER_HPP__

#include <pfs/memory.hpp>
#include <pfs/traits/associative_container.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/io/file.hpp>
#include <pfs/io/iterator.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/cli/command.hpp>

namespace pfs {
namespace cli {

template <typename CommandType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
class router
{
    typedef CommandType                                command_type;
    typedef Ostream                                    ostream_type;
    typedef typename command_type::params_type         params_type;
    typedef typename command_type::result_type         result_type;
    typedef typename command_type::string_type         string_type;
    typedef typename command_type::iterator            iterator;

    static string_type replace_separator (string_type const & str)
    {
        return string_type::replace(str.begin(), str.end(), '$', ' ');
    }

    router (ostream_type & sout, ostream_type & serr)
        : _nmaxcommands(0)
        , _echo_command(true)
        , _print_result(false)
        , _sout(sout)
        , _serr(serr)
    {}

    void set_echo_command (bool value)
    {
        _echo_command = value;
    }

    void set_print_result (bool value)
    {
        _print_result = value;
    }

    router & insert (string_type const & name
            , shared_ptr<command_type> pcmd)
    {
        _routemap.insert(name, pcmd);
        _nmaxcommands = pfs::max(_nmaxcommands, 1);
        return *this;
    }

    router & insert (string_type const & name
            , string_type const & subcmd
            , shared_ptr<command_type> pcmd)
    {
        _routemap.insert(build_key(name, subcmd), pcmd);
        _nmaxcommands = pfs::max(_nmaxcommands, 2);
        return *this;
    }

    router & insert (string_type const & name
            , string_type const & subcmd1
            , string_type const & subcmd2
            , shared_ptr<command_type> pcmd)
    {
        _routemap.insert(build_key(name, subcmd1, subcmd2), pcmd);
        _nmaxcommands = pfs::max(_nmaxcommands, 3);
        return *this;
    }

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

    void echo_command (string_type const & name)
    {
        if (_echo_command)
            _sout << name << "\n";
    }

    void print_result (result_type const & r) const;

    void print_usage (result_type const & r) const
    {
        print_result(r);
    }

    bool exec (string_type const & name, bool is_print_result = true);

    result_type exec (filesystem::path const & script);

private:
    typedef traits::associative_container<
              traits::kv<string_type, shared_ptr<command_type> >
            , AssociativeContainerTraits>   route_map;
    typedef safeformat<string_type>         fmt_type;
    typedef stringlist<string_type>         stringlist_type;

    static string_type build_key (typename stringlist_type::const_iterator & first
            , typename stringlist_type::const_iterator last
            , int n = -1);

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

    shared_ptr<command_type> find (string_type const & name) const
    {
        typename route_map::const_iterator it = _routemap.find(name);
        return (it != _routemap.cend())
                ? *it
                : shared_ptr();
    }

    shared_ptr<command_type> find (typename stringlist_type::const_iterator & first
            , typename stringlist_type::const_iterator last) const;

private:
    route_map _routemap;
    int       _nmaxcommands;
    bool      _echo_command;    // TODO merge into ...
    bool      _print_result;  // ... integer flag
    ostream_type & _sout;
    ostream_type & _serr;
};

template <typename CommandType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
typename router<CommandType, Ostream, AssociativeContainerTraits>::string_type
router<CommandType, Ostream, AssociativeContainerTraits>::build_key (
          typename stringlist_type::const_iterator & first
        , typename stringlist_type::const_iterator last
        , int n)
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

template <typename CommandType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
shared_ptr<CommandType>
router<CommandType, Ostream, AssociativeContainerTraits>::find (
          typename stringlist_type::const_iterator & first
        , typename stringlist_type::const_iterator last) const
{
    shared_ptr<CommandType> r;
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

template <typename CommandType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
void router<CommandType, Ostream, AssociativeContainerTraits>::print_result (
        result_type const & res) const
{
    ostream_type * out = & _sout;
    result_code rc = res.code();

    if (rc < result_code::ERRORS_BEGIN) {
        _sout << fmt_type("[%d]: %s")
                % static_cast<int>(rc)
                % pfs::to_string<string_type>(rc);
        _sout << "\n";
    } else if (rc >= result_code::ERRORS_BEGIN) {
        _serr << fmt_type("[%d]: %s: %s")
                % static_cast<int>(rc)
                % pfs::to_string<string_type>(rc)
                % res.message();
        _serr << "\n";
        out = & _serr;
    }

    typename result_type::const_iterator it = res.find(RESULT_TEXT_KEY());

    if (it != res.cend()) {
        if (it->is_array()) {
            for (typename result_type::size_type i = 0; i < it->size(); ++i) {
                *out << (*it)[i].get_string();
                *out << "\n";
            }
        } else if (it->is_scalar()) {
            *out << it->get_string();
            *out << "\n";
        }
    }
}

template <typename CommandType
        , typename Ostream
        , template <typename> class AssociativeContainerTraits>
bool router<CommandType, Ostream, AssociativeContainerTraits>::exec (
          string_type const & command_line
        , bool is_print_result)
{
    stringlist_type arglist;
    arglist.split(command_line, string_type(1, ' '), pfs::dont_keep_empty);

    typename stringlist_type::const_iterator first = arglist.cbegin();
    typename stringlist_type::const_iterator last = arglist.cend();

    shared_ptr<command_type> pcmd = this->find(first, last);

    result_type result = pcmd.is_null()
            ? result_type::make(result_code::NOT_FOUND
                    , fmt_type("`%s': command not found") % *first)
            : pcmd->exec(first, last);

    if (is_print_result) {
        result_code rc = result.code();

        if (rc >= result_code::ERRORS_BEGIN) {
            print_result(result);

            if (rc == result_code::BAD_COMMAND)
                print_usage(pcmd->usage());
        } else {
            print_result(result);
        }
    }

    return result == result_code::OK;
}

//template <typename RouteType
//        , typename Ostream
//        , template <typename> class AssociativeContainerTraits>
//typename router<RouteType, Ostream, AssociativeContainerTraits>::result_type
//router<RouteType, Ostream, AssociativeContainerTraits>::exec (
//        filesystem::path const & script)
//{
//    pfs::error_code ec;
//
//    if (!filesystem::exists(script, ec)) {
//        if (ec)
//            return response_type::make(ec);
//        return response_type::make(response_code::NOT_FOUND
//                , fmt_type("`%s`: file not found") % pfs::to_string<string_type>(script));
//    }
//
//    io::device file = io::open_device<io::file>(
//            io::open_params<io::file>(script, io::read_only), ec);
//
//    if (ec) {
//        return response_type::make(response_code::SYSTEM_ERROR
//                , fmt_type("`%s`: file open failure: %s")
//                        % pfs::to_string<string_type>(script)
//                        % pfs::to_string<string_type>(ec));
//    }
//
//    io::input_iterator<char> first(file);
//    io::input_iterator<char> last;
//
//    bool bad_script = false;
//    int line = 0;
//
//    while (first != last) {
//        string_type cmd = pfs::read_line_u8<string_type>(first, last);
//        ++line;
//
//        if (!cmd.empty()) {
//            echo_command(cmd);
//
//            if (!exec(cmd, _print_response)) {
//                bad_script = true;
//                break;
//            }
//        }
//    }
//
//    if (file.is_error()) {
//        return response_type::make(response_code::SYSTEM_ERROR
//                , fmt_type("`%s`: file read failure: %s")
//                        % pfs::to_string<string_type>(script)
//                        % pfs::to_string<string_type>(file.errorcode()));
//    }
//
//    return bad_script
//            ? response_type::make(response_code::BAD_SCRIPT
//                    , fmt_type("`%s`: bad command at line %d")
//                            % pfs::to_string<string_type>(script)
//                            % line)
//            : response_type::make();
//}
//
//template <typename ResponseType>
//ResponseType read_all (filesystem::path const & path)
//{
//    typedef ResponseType response_type;
//    typedef typename response_type::string_type string_type;
//
//    pfs::error_code ec;
//
//    if (!filesystem::exists(path, ec)) {
//        if (ec)
//            return response_type::make(ec);
//
//        string_type msg;
//        msg += '`';
//        msg += pfs::to_string<string_type>(path);
//        msg += "': ";
//        msg += "file not found";
//        return response_type::make(response_code::NOT_FOUND, msg);
//    }
//
//    io::device file = io::open_device<io::file>(
//            io::open_params<io::file>(path, io::read_only), ec);
//
//    if (ec) {
//        string_type msg;
//        msg += '`';
//        msg += pfs::to_string<string_type>(path);
//        msg += "': ";
//        msg += "file open failure";
//        return response_type::make(response_code::SYSTEM_ERROR, msg);
//    }
//
//    response_type rp = response_type::make(response_code::OK);
//    rp[response_type::VALUE_KEY()] = pfs::read_all_u8<string_type>(file);
//
//    return rp;
//}

}} // pfs::cli

#endif // __PFS_CLI_ROUTER_HPP__