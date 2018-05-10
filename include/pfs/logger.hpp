#pragma once
#include <iostream>
#include <fstream>
#include <pfs/exception.hpp>
#include <pfs/datetime.hpp>
#include <pfs/memory.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/fsm/fsm.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/string.hpp>
#include <pfs/list.hpp>

namespace pfs {

#define PFS_LOG_TEMPLETE_SIGNATURE typename SigslotNS                          \
            , template <typename> class SequenceContainer
#define PFS_LOG_TEMPLETE_ARGS SigslotNS, SequenceContainer

template <typename SigslotNS = sigslot<>
    , template <typename> class SequenceContainer = list>
struct log
{
    typedef pfs::string string_type;
    typedef SigslotNS sigslot_ns;

    struct priority
    {
        enum type {
            trace
            , debug
            , info
            , warn
            , error
            , fatal
            , critical = fatal
            , no_priority
            , count
        } value;

        priority ()
            : value(trace)
        {}

        priority (type v)
            : value(v)
        {}

        priority & operator = (type v)
        {
            value = v;
            return *this;
        }

        operator int () const
        {
            return value;
        }
    };

class appender;

class logger
{
private:
    typedef SequenceContainer<appender *> appender_sequence;
    typedef typename sigslot_ns::template signal3<priority, datetime const &, string_type const &> emitter_type;

private:
    struct data_t {
        priority          _priority;
        appender_sequence _appenders;
        emitter_type      _emitters[static_cast<size_t> ( priority::count )];
    };

    unique_ptr<data_t> _d;

public:
    logger ()
#if __cplusplus >= 201103L
        : _d(make_unique<data_t>())
#else
        : _d(new data_t)
#endif
    {
        _d->_priority = priority::trace;
    }

    ~logger ()
    {
        typename appender_sequence::iterator it   = _d->_appenders.begin();
        typename appender_sequence::iterator last = _d->_appenders.end();

        for (; it != last; ++it)
            delete *it;

        _d->_appenders.clear();
    }

    void swap (logger & other)
    {
        _d.swap(other._d);
    }

    template <typename Appender>
    appender & add_appender ()
    {
        //_d->_appenders.emplace_back(make_shared<Appender>());
        _d->_appenders.push_back ( new Appender );
        return *_d->_appenders.back();
    }

    // TODO Implement add_appender using C++ variadic templates
//#if __cplusplus < 201103L
    template <typename Appender, typename Arg1>
    appender & add_appender ( Arg1 a1 )
    {
        //_d->_appenders.emplace_back(make_shared<Appender>(a1)); // Valid for C++11
        _d->_appenders.push_back ( new Appender ( a1 ) );
        return *_d->_appenders.back();
    }

    template <typename Appender, typename Arg1, typename Arg2>
    appender & add_appender ( Arg1 a1, Arg2 a2 )
    {
        //_d->_appenders.emplace_back(make_shared<Appender>(a1, a2)); // Valid for C++11
        _d->_appenders.push_back ( new Appender ( a1, a2 ) );
        return *_d->_appenders.back();
    }

    template <typename Appender, typename Arg1, typename Arg2, typename Arg3>
    appender & add_appender ( Arg1 a1, Arg2 a2, Arg3 a3 )
    {
        //_d->_appenders.emplace_back(make_shared<Appender>(a1, a2, a3)); // Valid for C++11
        _d->_appenders.push_back ( new Appender ( a1, a2, a3 ) );
        return *_d->_appenders.back();
    }

    template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    appender & add_appender ( Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4 )
    {
        //_d->_appenders.emplace_back(make_shared<Appender>(a1, a2, a3, a4)); // Valid for C++11
        _d->_appenders.push_back ( new Appender ( a1, a2, a3, a4 ) );
        return *_d->_appenders.back();
    }

    template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    appender & add_appender ( Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5 )
    {
        //_d->_appenders.emplace_back(make_shared<Appender>(a1, a2, a3, a4, a5)); // Valid for C++11
        _d->_appenders.push_back ( new Appender ( a1, a2, a3, a4, a5 ) );
        return *_d->_appenders.back();
    }

    template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
    appender & add_appender ( Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6 )
    {
        //_d->_appenders.emplace_back(make_shared<Appender>(a1, a2, a3, a4, a5, a6)); // Valid for C++11
        _d->_appenders.push_back ( new Appender ( a1, a2, a3, a4, a5, a6 ) );
        return *_d->_appenders.back();
    }

    template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
    appender & add_appender ( Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7 )
    {
        //_d->_appenders.emplace_back(make_shared<Appender>(a1, a2, a3, a4, a5, a6, a7)); // Valid for C++11
        _d->_appenders.push_back ( new Appender ( a1, a2, a3, a4, a5, a6, a7 ) );
        return *_d->_appenders.back();
    }

    template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
    appender & add_appender ( Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8 )
    {
        //_d->_appenders.emplace_back(make_shared<Appender>(a1, a2, a3, a4, a5, a6, a7, a8)); // Valid for C++11
        _d->_appenders.push_back ( new Appender ( a1, a2, a3, a4, a5, a6, a7, a8 ) );
        return *_d->_appenders.back();
    }
//#else
//#	error "Need to implement `add_appender` using variadic templates"
//#endif

    void print ( priority level, datetime const & dt, string_type const & msg )
    {
        if ( level.value >= _d->_priority && level.value != priority::no_priority )
            _d->_emitters[level] ( level, dt, msg );
    }

    void connect ( priority level, appender & ap )
    {
        PFS_ASSERT ( level >= 0 && level < priority::count );
        _d->_emitters[level].connect ( & ap, & appender::print_helper );
    }

    void connect ( appender & ap )
    {
        _d->_emitters[priority::trace].connect ( & ap, & appender::print_helper );
        _d->_emitters[priority::debug].connect ( & ap, & appender::print_helper );
        _d->_emitters[priority::info].connect ( & ap, & appender::print_helper );
        _d->_emitters[priority::warn].connect ( & ap, & appender::print_helper );
        _d->_emitters[priority::error].connect ( & ap, & appender::print_helper );
        _d->_emitters[priority::fatal].connect ( & ap, & appender::print_helper );
    }

    void disconnect ( appender & ap )
    {
        _d->_emitters[priority::trace].disconnect ( & ap );
        _d->_emitters[priority::debug].disconnect ( & ap );
        _d->_emitters[priority::info].disconnect ( & ap );
        _d->_emitters[priority::warn].disconnect ( & ap );
        _d->_emitters[priority::error].disconnect ( & ap );
        _d->_emitters[priority::fatal].disconnect ( & ap );
    }

    void disconnect ( int level )
    {
        PFS_ASSERT ( level >= 0 && level < priority::count );
        _d->_emitters[level].disconnect_all();
    }

    void disconnect_all ()
    {
        for ( int i = 0; i < priority::count; ++i )
            _d->_emitters[i].disconnect_all();
    }

    void remove_disconnected ()
    {
        typename appender_sequence::iterator it   = _d->_appenders.begin();

        for ( ; it != _d->_appenders.end(); ) {
            if ( ( *it )->count() == 0 ) { // No signal connected
                delete *it;
                it = _d->_appenders.erase ( it );
            } else {
                ++it;
            }
        }
    }

    void set_priority ( priority pri )
    {
        _d->_priority = pri;
    }

    int get_priority ()
    {
        return _d->_priority;
    }

    void trace ( string_type const & text )
    {
        print ( priority::trace, current_datetime(), text );
    }

    void debug ( string_type const & text )
    {
        print ( priority::debug, current_datetime(), text );
    }

    void info ( string_type const & text )
    {
        print ( priority::info, current_datetime(), text );
    }

    void warn ( string_type const & text )
    {
        print ( priority::warn, current_datetime(), text );
    }

    void error ( string_type const & text )
    {
        print ( priority::error, current_datetime(), text );
    }

    void critical ( string_type const & text )
    {
        print ( priority::critical, current_datetime(), text );
        abort();
    }

    void fatal ( string_type const & text )
    {
        critical ( text );
    }
};

class appender : public sigslot_ns::has_slots
{
    friend class logger;

public:
    typedef pfs::string string_type;

protected:
    string_type _pattern;
    string_type _priority_text[static_cast<size_t>(priority::count) - 1]; // excluding no_priority

protected:
    virtual void print (priority level, datetime const & dt, string_type const & msg) = 0;

    void print_helper (priority level, datetime const & dt, string_type const & msg)
    {
        print(level, dt, _pattern.empty()
                ? msg
                : patternify(this, level, dt, _pattern, msg));
    }

    void init ()
    {
        set_priority_text(priority::trace, "T");
        set_priority_text(priority::debug, "D");
        set_priority_text(priority::info , "I");
        set_priority_text(priority::warn , "W");
        set_priority_text(priority::error, "E");
        set_priority_text(priority::fatal, "F");
    }

    /**
     * Each conversion specifier starts with a percent sign (%)
     * and is followed by optional format modifiers and a conversion character.
     *
     * Format modifier  left justify    minimum width   maximum width   comment
     * %20m             false           20              none            Left pad with spaces if the message is less than 20 characters long.
     * %-20m            true            20              none            Right pad with spaces if the message is less than 20 characters long.
     * %.30m            NA              none            30              Truncate from the beginning if the message is longer than 30 characters.
     * %20.30m          false           20              30              Left pad with spaces if the message is shorter than 20 characters.
     *                                                                  However, if message is longer than 30 characters, then truncate from the beginning.
     * %-20.30m         true            20              30              Right pad with spaces if the message is shorter than 20 characters.
     *                                                                  However, if message is longer than 30 characters, then truncate from the beginning.
     *
     * Conversation characters
     * m    message associated with the logging event.
     * d    date of the logging event. The date conversion specifier may be followed
     *      by a date format specifier enclosed between braces.
     *      For example, %d{HH:mm:ss,SSS} or %d{dd MMM yyyy HH:mm:ss,SSS}.
     *      If no date format specifier is given then ISO8601 format is assumed.
     * p    Priority (Trace, Debug, Info etc.).
     * n    new line
     * t    horizontal tab
     */
    struct pattern_grammar
    {
        typedef typename pfs::unicode::unicode_iterator_traits<
                typename string_type::const_iterator>::iterator iterator;
        typedef typename pfs::unicode::unicode_iterator_traits<
                typename string_type::iterator>::output_iterator output_iterator;
        typedef fsm::fsm<iterator>                 fsm_type;
        typedef typename fsm_type::transition_type transition_type;
        typedef typename fsm_type::char_type       value_type;

        struct specifier
        {
            //typename string_type::value_type spec_char;
            value_type              spec_char;
            bool                    left_justify;
            size_t                  min_width;
            size_t                  max_width;
            string_type             fspec; /* format specifier */
        };

        struct parse_context
        {
            appender *          appender_ptr;
            priority            level;
            datetime            dt;
            string_type         result;
            string_type const * msg_ptr;
            specifier           spec;
        };

        pattern_grammar ();

        static bool begin_spec (iterator /*first*/, iterator /*last*/, void * context, void * /*action_args*/)
        {
            if (context) {
                parse_context * ctx = static_cast<parse_context *>(context);
                ctx->spec.spec_char = '\0';
                ctx->spec.left_justify = false;
                ctx->spec.min_width = 0;
                ctx->spec.max_width = 0;
                ctx->spec.fspec.clear();
            }
            return true;
        }

        static bool end_spec (iterator /*first*/, iterator /*last*/, void * context, void * /*action_args*/)
        {
            if (context) {
                parse_context * ctx = static_cast<parse_context *>(context);
                string_type result;

                switch (to_ascii(ctx->spec.spec_char)) {
                case 'n':
                    result.push_back('\n');
                    break;
                case 't':
                    result.push_back('\t');
                    break;
                case 'p':
                    result = ctx->appender_ptr->priority_text(ctx->level);
                    break;
                case 'm':
                    if (ctx->msg_ptr)
                        result = *ctx->msg_ptr;
                    else
                        result.append("<null>");
                    break;

                case 'd': {
                    //datetime dt = pfs::current_datetime();

                    if (ctx->spec.fspec == "ABSOLUTE") {
                        result = pfs::to_string(ctx->dt, "%H:%M:%S.%Q");
                    } else if (ctx->spec.fspec == "DATE") {
                        result = pfs::to_string(ctx->dt, "%d %b %Y %H:%M:%S.%Q");
                    } else if (ctx->spec.fspec == "ISO8601") {
                        result = pfs::to_string(ctx->dt, "%Y-%m-%d %H:%M:%S.%Q");
                    } else {
                        result = pfs::to_string(ctx->dt, ctx->spec.fspec);
                    }

                    break;
                }

                default:
                    break;
                }

                /* truncate */
                if (ctx->spec.max_width > 0 && result.length() > ctx->spec.max_width) {
                    result = result.substr(0, ctx->spec.max_width);
                }

                /* pad */
                if (ctx->spec.min_width > 0 && result.length() < ctx->spec.min_width) {
                    string_type padding(ctx->spec.min_width - result.length(), ' ');

                    if (ctx->spec.left_justify) {
                        result.append(padding);
                    } else {
                        result = padding + result;
                    }
                }

                ctx->result.append(result);

            }
            return true;
        }

        static bool append_plain_char (iterator first, iterator last, void * context, void * /*action_args*/)
        {
            if (context) {
                parse_context * ctx = static_cast<parse_context *>(context);
                ctx->result.append(string_type(first, last));
            }
            return true;
        }

        static bool set_left_justify (iterator first, iterator last, void * context, void * /*action_args*/)
        {
            if (context) {
                parse_context * ctx = static_cast<parse_context *>(context);
                pfs::advance(first, 1);
                if (first == last) {
                    ctx->spec.left_justify = true;
                }
            }
            return true;
        }

        static bool set_min_width (iterator first, iterator last, void * context, void * /*action_args*/)
        {
            if (context) {
                parse_context * ctx = static_cast<parse_context *>(context);

                if (first != last) {
                    string_type n(first, last);
                    try {
                        ctx->spec.min_width = lexical_cast<size_t>(n);
                    } catch (...) {
                        ;
                    }
                }
            }
            return true;
        }

        static bool set_max_width (iterator first, iterator last, void * context, void * /*action_args*/)
        {
            if (context) {
                parse_context * ctx = static_cast<parse_context *>(context);

                if (first != last) {
                    string_type n(first, last);
                    try {
                        ctx->spec.max_width = lexical_cast<size_t>(n);
                    } catch (...) {
                        ;
                    }
                }
            }
            return true;
        }

        static bool set_spec_char (iterator first, iterator /*last*/, void * context, void * /*action_args*/)
        {
            if (context) {
                parse_context * ctx = static_cast<parse_context *>(context);
                ctx->spec.spec_char = *first;
            }
            return true;
        }

        static bool set_format_spec (iterator first, iterator last, void * context, void * /*action_args*/)
        {
            if (context) {
                parse_context * ctx = static_cast<parse_context *>(context);
                ctx->spec.fspec = string_type(first, last);
            }
            return true;
        }

#if PFS_TEST
#endif
        transition_type const * p_pattern_tr;
    };

    static string_type patternify (appender * a
        , priority level
        , datetime const & dt
        , string_type const & pattern
        , string_type const & text)
    {
        typedef typename pattern_grammar::fsm_type fsm_type;

        static pattern_grammar grammar;
        typename pattern_grammar::parse_context context;
        context.appender_ptr = a;
        context.level        = level;
        context.msg_ptr      = & text;
        context.dt           = dt;

        fsm_type fsm(grammar.p_pattern_tr, & context);
        typename fsm_type::result_type r = fsm.exec(0, pattern.cbegin(), pattern.cend());

        if (r.first)
            return context.result;

        string_type broken_msg("[<!INVALID PATTERN!>]: ");
        broken_msg.append(text);
        return broken_msg;
    }

public:
    appender ()
        : _pattern ( "%m" )
    {
        init();
    }

    appender ( string_type const & pattern )
        : _pattern ( pattern )
    {
        init();
    }

    virtual ~appender ()
    {}

    virtual bool is_open () const = 0;

    string_type pattern () const
    {
        return _pattern;
    }

    void set_pattern ( string_type const & pattern )
    {
        _pattern = pattern;
    }

    string_type priority_text (priority pri) const
    {
        return _priority_text[pri];
    }

    void set_priority_text (priority pri, string_type const & text)
    {
        _priority_text[pri] = text;
    }
};

class stdout_appender : public appender
{
public:
    stdout_appender () : appender() {}

    virtual bool is_open () const
    {
        return true;
    }

protected:
	virtual void print (priority, datetime const &, string_type const & msg) pfs_override
	{
		std::cout << msg << std::endl;
	}
};

class stderr_appender : public appender
{
public:
    stderr_appender () : appender() {}

    virtual bool is_open () const
    {
        return true;
    }

protected:
    virtual void print (priority, datetime const &, string_type const & msg) pfs_override
    {
        std::cerr << msg << std::endl;
    }
};

class file_appender : public appender
{
    typedef std::basic_fstream<typename string_type::value_type> fstream_type;

    fstream_type _d;

public:
    file_appender () : appender() {}

    file_appender (filesystem::path const & path)
#if __cplusplus >= 201103L
        : _d(path.native(), std::ios::out)
#else
        : _d(path.native().c_str(), std::ios::out)
#endif
    {}

    virtual bool is_open () const
    {
        return _d.is_open();
    }

protected:
    virtual void print (priority, datetime const &, string_type const & msg) pfs_override
    {
        _d << msg << "\n";
    }
};

}; // log

template <PFS_LOG_TEMPLETE_SIGNATURE>
log<PFS_LOG_TEMPLETE_ARGS>::appender::pattern_grammar::pattern_grammar ()
{
#   undef FSM_ONE_OF
#   undef FSM_TR
#   undef FSM_OPT_TR
#   undef FSM_RPT_TR
#   undef FSM_OPT_ONE_OF
#   undef FSM_RPT_ONE_OF
#   undef FSM_RANGE
#   define FSM_ONE_OF(x)         fsm_type::one_of(x.begin(), x.end())
#   define FSM_TR(x)             fsm_type::tr(x)
#   define FSM_OPT_TR(x)         fsm_type::opt_tr(x)
#   define FSM_RPT_TR(x,a,b)     fsm_type::rpt_tr(x,a,b)
#   define FSM_OPT_ONE_OF(x)     fsm_type::opt_one_of(x.begin(),x.end())
#   define FSM_RPT_ONE_OF(x,a,b) fsm_type::rpt_one_of(x.begin(),x.end(),a,b)
#   define FSM_RANGE(a,b)        fsm_type::range(a,b)

    static string_type const DIGIT("0123456789");
    static string_type const PERIOD(".");
    static string_type const DASH("-");
    static string_type const LEFT_CURLY_BRACKET("{");
    static string_type const RIGHT_CURLY_BRACKET("}");
    static string_type const PERCENT_SIGN("%");

    /* exclude '%' (0x25) */
    //typename string_type::value_type plain_char[] = {
    value_type plain_char[] = {
          0x20u, 0x24u
        , 0x26u, 0x10FFFFu
    };

    static transition_type const plain_char_tr[] = {
          {-1, 1, FSM_RANGE(plain_char[0], plain_char[1]), fsm_type::accept, 0, 0}
        , {-1,-1, FSM_RANGE(plain_char[2], plain_char[3]), fsm_type::accept, 0, 0}
    };

    /* format-mod = [ "-" ] *2DIGIT [ "." *2DIGIT ] */
    static transition_type const dot_digit_tr[] = {
          { 1, -1, FSM_ONE_OF(PERIOD)         , fsm_type::normal, 0, 0}
        , {-1, -1, FSM_RPT_ONE_OF(DIGIT, 0, 2), fsm_type::accept, set_max_width, 0}
    };

    static transition_type const format_mod_tr[] = {
          { 1, -1, FSM_OPT_ONE_OF(DASH)       , fsm_type::normal, set_left_justify, 0}
        , { 2, -1, FSM_RPT_ONE_OF(DIGIT, 0, 2), fsm_type::normal, set_min_width, 0}
        , {-1, -1, FSM_OPT_TR(dot_digit_tr)   , fsm_type::accept, 0, 0}
    };

    /* format-spec = "{" *( <exclude '{' (0x7B) and '}' (0x7D) > ) "}" */
    //typename string_type::value_type format_spec_char[] = {
    value_type format_spec_char[] = {
          0x20u, 0x7Au
        , 0x7Cu, 0x7Cu
        , 0x7Eu, 0x10FFFFu
    };

    static transition_type const format_spec_char_tr[] = {
          {-1, 1, FSM_RANGE(format_spec_char[0], format_spec_char[1]), fsm_type::accept, 0, 0}
        , {-1, 2, FSM_RANGE(format_spec_char[2], format_spec_char[3]), fsm_type::accept, 0, 0}
        , {-1,-1, FSM_RANGE(format_spec_char[4], format_spec_char[5]), fsm_type::accept, 0, 0}
    };

    static transition_type const format_spec_tr[] = {
          { 1, -1, FSM_ONE_OF(LEFT_CURLY_BRACKET)         , fsm_type::normal, 0, 0}
        , { 2, -1, FSM_RPT_TR(format_spec_char_tr, 0, 256), fsm_type::normal, set_format_spec, 0}
        , {-1, -1, FSM_ONE_OF(RIGHT_CURLY_BRACKET)        , fsm_type::accept, 0, 0}
    };

    static string_type const SPEC_CHARS("mdpnt");

    /* spec = "%" [ format-mod ] ( "m" / "d" / "p" ) [ format-spec ]*/
    static transition_type const spec_tr[] = {
          { 1, -1, FSM_ONE_OF(PERCENT_SIGN)  , fsm_type::normal, begin_spec, 0}
        , { 2, -1, FSM_OPT_TR(format_mod_tr) , fsm_type::normal, 0, 0}
        , { 3, -1, FSM_ONE_OF(SPEC_CHARS)    , fsm_type::normal, set_spec_char, 0}
        , {-1, -1, FSM_OPT_TR(format_spec_tr), fsm_type::accept, end_spec, 0}
    };

    /* pattern = *( spec-fsm / plain-char ) */
    /* spec-fsm / plain-char */
    static transition_type const pattern_unit_tr[] = {
          {-1, 1, FSM_TR(spec_tr)      , fsm_type::accept, 0, 0}
        , {-1,-1, FSM_TR(plain_char_tr), fsm_type::accept, append_plain_char, 0}
    };

    static transition_type const pattern_tr[] = {
        {-1, -1, FSM_RPT_TR(pattern_unit_tr, 0, -1), fsm_type::accept, 0, 0}
    };

    p_pattern_tr = pattern_tr;
}

} // pfs
