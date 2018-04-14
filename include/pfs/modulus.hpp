#pragma once

#include <pfs/cxxlang.hpp>
#include <pfs/operationsystem.hpp>
#include <pfs/atomic.hpp>
#include <pfs/traits/associative_container.hpp>
#include <pfs/traits/sequence_container.hpp>
#include <pfs/traits/stdcxx/deque.hpp>
#include <pfs/traits/stdcxx/map.hpp>
#include <pfs/traits/stdcxx/list.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/dynamic_library.hpp>
#include <pfs/active_queue.hpp>
#include <pfs/logger.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/datetime.hpp>
#include <pfs/thread.hpp>
#include <pfs/io/file.hpp>
#include <pfs/io/iterator.hpp>

namespace pfs {

#define PFS_MODULE_CTOR_NAME "__module_ctor__"
#define PFS_MODULE_DTOR_NAME "__module_dtor__"

struct basic_dispatcher
{
    virtual void quit () = 0;

#if PFS_OS_POSIX
    bool activate_posix_signal_handling ();
    void deactivate_posix_signal_handling ();

    basic_dispatcher ()
    {
        (void)activate_posix_signal_handling();
    }

    virtual ~basic_dispatcher ()
    {
        this->deactivate_posix_signal_handling();
    }
#else

    basic_dispatcher ()
    {}

    virtual ~basic_dispatcher ()
    {}
#endif // PFS_OS_POSIX
};

#define PFS_MODULUS_TEMPLETE_SIGNATURE typename StringType                     \
    , template <typename> class AssociativeContainer                           \
    , template <typename> class SequenceContainer                              \
    , template <typename> class ActiveQueueContainer                           \
    , typename BasicLockable                                                   \
    , int GcThreshold

#define PFS_MODULUS_TEMPLETE_ARGS StringType                                   \
    , AssociativeContainer                                                     \
    , SequenceContainer                                                        \
    , ActiveQueueContainer                                                     \
    , BasicLockable                                                            \
    , GcThreshold

template <typename StringType
    // For storing API map and module specs
    , template <typename> class AssociativeContainer = pfs::stdcxx::map

    // For storing runnable modules and threads
    , template <typename> class SequenceContainer = pfs::stdcxx::list

    // Active queue configuration
    , template <typename> class ActiveQueueContainer = pfs::stdcxx::deque
    , typename BasicLockable = pfs::mutex // see [C++ concepts: BasicLockable](http://en.cppreference.com/w/cpp/concept/BasicLockable)>
    , int GcThreshold = 256>
struct modulus
{
    class basic_module;
    class module;
    class async_module;
    class dispatcher;

    typedef pfs::active_queue<ActiveQueueContainer
        , BasicLockable
        , GcThreshold>                      callback_queue_type;

    typedef sigslot<callback_queue_type, BasicLockable> sigslot_ns;

    typedef StringType      string_type;
    typedef log<StringType, sigslot_ns, SequenceContainer> log_ns;
    typedef typename log_ns::logger logger_type;
    typedef typename sigslot_ns::template signal1<void *> emitter_type;

    typedef void (basic_module::* detector_handler)(void *);
    typedef struct { int id; void * emitter; }            emitter_mapper_pair;
    typedef struct { int id; detector_handler detector; } detector_mapper_pair;

    typedef basic_module * (* module_ctor_t)(dispatcher * pdisp, char const * name, void *);
    typedef void  (* module_dtor_t)(basic_module *);

    struct detector_pair
    {
        basic_module *   mod;
        detector_handler detector;

        detector_pair () : mod(0), detector(0) {}
        detector_pair (basic_module * p, detector_handler d) : mod(p), detector(d) {}
    };

    struct module_spec
    {
        shared_ptr<basic_module>    pmodule;
        shared_ptr<dynamic_library> pdl;
    };

    struct module_deleter
    {
        module_dtor_t _deleter;

        module_deleter (module_dtor_t deleter)
            : _deleter(deleter)
        {}

        void operator () (basic_module * p) const
        {
            _deleter(p);
        }
    };

    struct basic_sigslot_mapper
    {
        virtual void connect_all () = 0;
        virtual void disconnect_all () = 0;
        virtual void append_emitter (emitter_type *e) = 0;
        virtual void append_detector (basic_module * m, detector_handler d) = 0;
    };

    struct api_item_type
    {
        int                    id;
        basic_sigslot_mapper * mapper;
        string_type            desc;
    };

    template <typename EmitterType, typename DetectorType>
    struct sigslot_mapper : basic_sigslot_mapper
    {
        typedef traits::sequence_container<EmitterType *
                , SequenceContainer>                  emitter_sequence;

        typedef traits::sequence_container<detector_pair
                , SequenceContainer>                  detector_sequence;

        emitter_sequence  emitters;
        detector_sequence detectors;

        virtual void connect_all ()
        {
            if (emitters.size() == 0 || detectors.size() == 0)
                return;

            typename emitter_sequence::const_iterator itEnd = emitters.cend();
            typename detector_sequence::const_iterator itdEnd = detectors.cend();

            for (typename emitter_sequence::const_iterator it = emitters.cbegin(); it != itEnd; it++) {
                for (typename detector_sequence::const_iterator itd = detectors.cbegin(); itd != itdEnd; itd++) {
                    EmitterType * em = *it;
                    em->connect(itd->mod, reinterpret_cast<DetectorType> (itd->detector));
                }
            }
        }

        virtual void disconnect_all ()
        {
            typename emitter_sequence::const_iterator itEnd = emitters.cend();

            for (typename emitter_sequence::const_iterator it = emitters.cbegin(); it != itEnd; it++) {
                EmitterType * em = *it;
                em->disconnect_all();
            }
        }

        virtual void append_emitter (emitter_type * e)
        {
            emitters.push_back(reinterpret_cast<EmitterType*>(e));
        }

        virtual void append_detector (basic_module * m, detector_handler d)
        {
            detectors.push_back(detector_pair(m, d));
        }
    };

    typedef traits::associative_container<
               traits::kv<int, api_item_type *>
            ,  AssociativeContainer>            api_map_type;

    typedef traits::associative_container<
              traits::kv<string_type, module_spec>
            , AssociativeContainer>             module_spec_map_type;

    typedef int (basic_module::*thread_function)();

    typedef traits::sequence_container<pfs::pair<basic_module *, thread_function>
            , SequenceContainer>                runnable_sequence_type;

    typedef traits::sequence_container<shared_ptr<pfs::thread>
            , SequenceContainer>                thread_sequence_type;

    class basic_module : public sigslot_ns::basic_has_slots
    {
        friend class dispatcher;

        //typedef typename sigslot_ns::basic_has_slots base_class;

    public:
        typedef StringType string_type;
        typedef modulus::emitter_mapper_pair  emitter_mapper_pair;
        typedef modulus::detector_mapper_pair detector_mapper_pair;
        typedef modulus::detector_handler     detector_handler;
        typedef modulus::thread_function      thread_function;

    public: // signals
        typename sigslot_ns::signal0  emit_quit;
        typename sigslot_ns::template signal2<string_type const &, bool &> emit_module_registered;

    public:
        void print_info (basic_module const * m, string_type const & s)
        {
            _pdispatcher->print_info(m, s);
        }

    	void print_debug (basic_module const * m, string_type const & s)
        {
            _pdispatcher->print_debug(m, s);
        }

    	void print_warn (basic_module const * m, string_type const & s)
        {
            _pdispatcher->print_warn(m, s);
        }

    	void print_error (basic_module const * m, string_type const & s)
        {
            _pdispatcher->print_error(m, s);
        }

    protected:
        string_type  _name;
        dispatcher * _pdispatcher;
        atomic_int   _quitfl; // quit flag

    protected:
        basic_module (dispatcher * pdisp)
            //: base_class()
            : _pdispatcher(pdisp)
            , _quitfl(0)
        {}

        void set_name (string_type const & name)
        {
            _name = name;
        }

    public:
        virtual ~basic_module () {}

        string_type const & name() const
        {
            return _name;
        }

        bool is_registered () const
        {
            return _pdispatcher != 0 ? true : false;
        }

        dispatcher * get_dispatcher ()
        {
            return _pdispatcher;
        }

        dispatcher const * get_dispatcher () const
        {
            return _pdispatcher;
        }

        void register_thread_function (thread_function tfunc)
        {
            _pdispatcher->_runnable_modules.push_back(pfs::make_pair(this, tfunc));
        }

        virtual emitter_mapper_pair const * get_emitters (int & count)
        {
            count = 0;
            return 0;
        }

        virtual detector_mapper_pair const * get_detectors (int & count)
        {
            count = 0;
            return 0;
        }

        bool is_quit () const
        {
            return _quitfl == 0 ? false : true;
        }

        /**
         * @brief Module's on_loaded() method called after loaded and before registration.
         * @return
         */
        virtual bool on_loaded ()
        {
            return true;
        }

        /**
         * @brief Module's on_start() method called after loaded and connection completed.
         */
        virtual bool on_start ()
        {
            return true;
        }

        virtual bool on_finish ()
        {
            return true;
        }

    public: // slots
        void on_quit ()
        {
            _quitfl = 1;
        }
    };// basic_module

    //
    // Body must be identical to sigslot's has_slots
    // TODO Reimeplement to avoid code duplication
    //
    class module : public basic_module//, public sigslot_ns::has_slots
    {
    public:
        module (dispatcher * pdisp) : basic_module(pdisp) {}
        virtual bool use_async_slots () const pfs_override { return false; }
    };

    //
    // Body must be identical to sigslot's has_async_slots
    // TODO Reimeplement to avoid code duplication
    //
    class async_module : public basic_module//, public sigslot_ns::has_async_slots
    {
    public:
        async_module (dispatcher * pdisp) : basic_module(pdisp)
        {
            this->_queue_ptr = make_unique<callback_queue_type>();
        }

        virtual bool use_async_slots () const pfs_override { return true; }

        void call_all ()
        {
            this->callback_queue().call_all();
        }
    };

    struct sigslot_mapping0
        : sigslot_mapper<typename sigslot_ns::signal0
                , void (basic_module::*)()>
    {};

    template <typename A1>
    struct sigslot_mapping1
        : sigslot_mapper<typename sigslot_ns::template signal1<A1>
                , void (basic_module::*)(A1)>
    {};

    template <typename A1, typename A2>
    struct sigslot_mapping2
        : sigslot_mapper<typename sigslot_ns::template signal2<A1, A2>
                , void (basic_module::*)(A1, A2)>
    {};

    template <typename A1, typename A2, typename A3>
    struct sigslot_mapping3
        : sigslot_mapper<typename sigslot_ns::template signal3<A1, A2, A3>
                , void (basic_module::*)(A1, A2, A3)>
    {};

    template <typename A1, typename A2, typename A3, typename A4>
    struct sigslot_mapping4
        : sigslot_mapper<typename sigslot_ns::template signal4<A1, A2, A3, A4>
                , void (basic_module::*)(A1, A2, A3, A4)>
    {};

    template <typename A1, typename A2, typename A3, typename A4, typename A5>
    struct sigslot_mapping5
        : sigslot_mapper<typename sigslot_ns::template signal5<A1, A2, A3, A4, A5>
                , void (basic_module::*)(A1, A2, A3, A4, A5)>
    {};

    template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    struct sigslot_mapping6
        : sigslot_mapper<typename sigslot_ns::template signal6<A1, A2, A3, A4, A5, A6>
                , void (basic_module::*)(A1, A2, A3, A4, A5, A6)>
    {};

    template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    struct sigslot_mapping7
        : sigslot_mapper<typename sigslot_ns::template signal7<A1, A2, A3, A4, A5, A6, A7>
                , void (basic_module::*)(A1, A2, A3, A4, A5, A6, A7)>
    {};

    template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    struct sigslot_mapping8
        : sigslot_mapper<typename sigslot_ns::template signal8<A1, A2, A3, A4, A5, A6, A7, A8>
                , void (basic_module::*)(A1, A2, A3, A4, A5, A6, A7, A8)>
    {};

    class dispatcher : basic_dispatcher, public sigslot_ns::has_async_slots
    {
        friend class basic_module;
        friend class module;
        friend class async_module;

        typedef safeformat<StringType> fmt;

    public:
        typedef modulus::string_type string_type;
        typedef modulus::logger_type logger_type;

        struct exit_status
        {
            enum {
                  success = 0
                , failure = -1
            };
        };

    private:
    #if __cplusplus >= 201103L
        dispatcher (dispatcher const &) = delete;
        dispatcher & operator = (dispatcher const &) = delete;
    #else
        dispatcher (dispatcher const &);
        dispatcher & operator = (dispatcher const &);
    #endif

    private:
        void connect_all ();
        void disconnect_all ();
        void unregister_all ();
        bool start ();

        void finalize ()
        {
            if (_module_spec_map.size() > 0) {
                disconnect_all();
                unregister_all();
            }
        }

        /**
         * @brief Internal dispatcher loop.
         */
        void run ();

        int  exec_main ();

    public:
        dispatcher (api_item_type * mapper, int n);

        virtual ~dispatcher ()
        {
            finalize();
        }

        virtual void quit () pfs_override
        {
            _emit_quit();
        }

        int exec ();

        void register_api (api_item_type * mapper, int n);

        void add_search_path (filesystem::path const & dir)
        {
            if (!dir.empty())
                _searchdirs.push_back(dir);
        }

    //    /**
    //     * @brief Output summary of emitters/detectors utilization.
    //     * TODO Implement
    //     */
    //    void print_api_connections () {}
    //
    //    /**
    //     * @brief Output summary of incomplete emitters/detectors utilization.
    //     * TODO Implement
    //     */
    //    void print_api_incomplete_connections () {}

        /**
         * @brief Register modules enumerated in configuration file (i.e. JSON) specified by @a path.
         *
         * @param path Configuration file path.
         * @return @c true if modules registered successfully. @c false otherwise.
         */
        template <typename PropertyTree>
        bool register_modules (filesystem::path const & path);

        /**
         * @brief Register modules enumerated in JSON instance specified by @a conf.
         *
         * @param conf JSON instance contained modules for registration.
         * @return @c true if modules registered successfully. @c false otherwise.
         */
        template <typename Json>
        bool register_modules (Json const & conf);

    	bool register_local_module (basic_module * pmodule, string_type const & name);

    	bool register_module_for_path (filesystem::path const & path
    			, string_type const & name
    			, const char * class_name = 0
    			, void * mod_data = 0);

        bool register_module_for_name (string_type const & name
                , char const * class_name = 0
                , void * mod_data = 0);

    	void set_master_module (string_type const & name);

    	size_t count () const
    	{
    		return _module_spec_map.size();
    	}

    	bool is_module_registered (string_type const & modname) const
    	{
    		return _module_spec_map.find(modname) != _module_spec_map.end();
    	}

        logger_type & get_logger () { return _logger; }
        logger_type const & get_logger () const { return _logger; }

    protected: // slots
        void on_quit ()
        {
            _quitfl = 1;
        }

    public: // slots
        void module_registered ( string_type const & pname, bool & result )
        {
            result = is_module_registered ( pname );
        }

        void print_info (basic_module const * m, string_type const & s)
        {
            this->_queue_ptr->template push_method<logger_type, string_type const &>(
                    & logger_type::info, & _logger, (m != 0 ? m->name() + ": " + s : s));
        }

        void print_debug (basic_module const * m, string_type const & s)
        {
            this->_queue_ptr->template push_method<logger_type, string_type const &>(
                    & logger_type::debug, & _logger, (m != 0 ? m->name() + ": " + s : s));
        }

        void print_warn (basic_module const * m, string_type const & s)
        {
            this->_queue_ptr->template push_method<logger_type, string_type const &>(
                    & logger_type::warn, & _logger, (m != 0 ? m->name() + ": " + s : s));
        }

        void print_error (basic_module const * m, string_type const & s)
        {
            this->_queue_ptr->template push_method<logger_type, string_type const &>(
                    & logger_type::error, & _logger, (m != 0 ? m->name() + ": " + s : s));
        }

      	void print_info (string_type const & s)
        {
            print_info(0, s);
        }

    	void print_debug (string_type const & s)
        {
            print_debug(0, s);
        }

    	void print_warn (string_type const & s)
        {
            print_warn(0, s);
        }

        void print_error (string_type const & s)
        {
            print_error(0, s);
        }

    protected:
        module_spec module_for_path ( filesystem::path const & path
                , char const * class_name = 0
                , void * mod_data = 0 );

        module_spec module_for_name ( string_type const & name
                , char const * class_name = 0
                , void * mod_data = 0 )
        {
            filesystem::path modpath = build_so_filename ( name.native() );
            return module_for_path ( modpath, class_name, mod_data );
        }

        bool register_module ( module_spec const & modspec );

    private:
        typename sigslot_ns::signal0    _emit_quit;
        atomic_int _quitfl; // quit flag
        filesystem::pathlist   _searchdirs;
        api_map_type           _api;
        module_spec_map_type   _module_spec_map;
        runnable_sequence_type _runnable_modules; // modules run in a separate threads
        basic_module *         _master_module_ptr;
        logger_type            _logger;
    }; // class dispatcher
}; // struct modulus

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::dispatcher (
        api_item_type * mapper
        , int n)
    : basic_dispatcher()
    , _master_module_ptr(0)
    , _quitfl(0)
{
    // Initialize default logger
    typename log_ns::appender & cout_appender
            = _logger.template add_appender<typename log_ns::stdout_appender>();
    typename log_ns::appender & cerr_appender
            = _logger.template add_appender<typename log_ns::stderr_appender>();
    cout_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
    cerr_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");

    _logger.connect(log_ns::priority::trace   , cout_appender);
    _logger.connect(log_ns::priority::debug   , cout_appender);
    _logger.connect(log_ns::priority::info    , cout_appender);
    _logger.connect(log_ns::priority::warn    , cerr_appender);
    _logger.connect(log_ns::priority::error   , cerr_appender);
    _logger.connect(log_ns::priority::critical, cerr_appender);

    this->_emit_quit.connect(this, & dispatcher::on_quit);

    register_api(mapper, n);
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
void modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_api (
          api_item_type * mapper
        , int n)
{
    for (int i = 0; i < n; ++i) {
        _api.insert(mapper[i].id, & mapper[i]);
    }
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
void modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::connect_all ()
{
    typename api_map_type::iterator first = _api.begin();
    typename api_map_type::iterator last  = _api.end();

    for (; first != last; ++first) {
        first->second->mapper->connect_all();
    }
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
void modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::run ()
{
    while (! _quitfl) {
        // FIXME Use condition_variable to wait until _callback_queue will not be empty.
        if (this->_queue_ptr->empty()) {
            pfs::this_thread::sleep_for(pfs::chrono::milliseconds(100));
            continue;
        }
        this->_queue_ptr->call_all();
    }
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::start ()
{
    bool r = true;

    typename module_spec_map_type::iterator first = _module_spec_map.begin();
    typename module_spec_map_type::iterator last  = _module_spec_map.end();

    for (; first != last; ++first) {
        module_spec modspec = first->second;
        shared_ptr<basic_module> pmodule = modspec.pmodule;

        if (! pmodule->on_start()) {
            _logger.error(fmt("failed to start module: %s") % pmodule->name());
            r = false;
        }
    }

    return r;
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
void modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::disconnect_all ()
{
    typename api_map_type::iterator first = _api.begin();
    typename api_map_type::iterator last  = _api.end();

    for (; first != last; ++first) {
        first->second->mapper->disconnect_all();
    }
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
void modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::unregister_all ()
{
    _runnable_modules.clear();

    typename module_spec_map_type::iterator first = _module_spec_map.begin();
    typename module_spec_map_type::iterator last = _module_spec_map.end();

    for (; first != last; ++first) {
        module_spec & modspec = first->second;
        shared_ptr<basic_module> & pmodule = modspec.pmodule;
        pmodule->emit_module_registered.disconnect(this);
        _logger.debug(fmt("%s: unregistered") % (pmodule->name()));

        // Need to destroy pmodule before dynamic library will be destroyed automatically
        pmodule.reset();
    }

    _module_spec_map.clear();
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_local_module (
          basic_module * pmodule
        , string_type const & name)
{
    module_spec modspec;
    modspec.pmodule = shared_ptr<basic_module>(pmodule);
    modspec.pmodule->set_name(name);
    return register_module(modspec);
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_module_for_name (
          string_type const & name
        , char const * class_name
        , void * mod_data)
{
    module_spec modspec = module_for_name(name, class_name, mod_data);

    if (modspec.pmodule) {
        modspec.pmodule->set_name(name);
        return register_module(modspec);
    }
    return false;
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
typename modulus<PFS_MODULUS_TEMPLETE_ARGS>::module_spec
modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::module_for_path (
          filesystem::path const & path
        , char const * class_name
        , void * mod_data)
{
    shared_ptr<dynamic_library> pdl = make_shared<dynamic_library>();
    error_code ec;

    filesystem::path dlpath(path);

    if (path.is_relative()) {
        dlpath = filesystem::path(".") / path;
    }

    if (!pdl->open(dlpath, _searchdirs, ec)) {
        _logger.error(fmt("%s: %s")
                % (to_string<string_type>(dlpath))
                % (to_string<string_type>(ec)));
        return module_spec();
    }

    dynamic_library::symbol_type ctor = pdl->resolve(PFS_MODULE_CTOR_NAME, ec);

    if (!ctor) {
        _logger.error(fmt("%s: failed to resolve `ctor' for module: %s")
                % (to_string<string_type>(dlpath))
                % (to_string<string_type>(ec)));

        return module_spec();
    }

    dynamic_library::symbol_type dtor = pdl->resolve(PFS_MODULE_DTOR_NAME, ec);

    if (!dtor) {
        _logger.error(fmt("%s: failed to resolve `dtor' for module: %s")
                % (to_string<string_type>(dlpath))
                % (to_string<string_type>(ec)));

        return module_spec();
    }

    module_ctor_t module_ctor = pfs::void_func_ptr_cast<module_ctor_t>(ctor);
    module_dtor_t module_dtor = pfs::void_func_ptr_cast<module_dtor_t>(dtor);

    //module * ptr = reinterpret_cast<module *>(module_ctor(class_name, mod_data));
    basic_module * ptr = module_ctor(this, class_name, mod_data);

    if (!ptr)
        return module_spec();

    module_spec result;
    result.pdl = pdl;
    result.pmodule = shared_ptr<basic_module>(ptr, module_deleter(module_dtor));

    return result;
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_module (
        module_spec const & modspec)
{
    int nemitters, ndetectors;

    if (!modspec.pmodule)
        return false;

    shared_ptr<basic_module> pmodule = modspec.pmodule;

    if (_module_spec_map.find(pmodule->name()) != _module_spec_map.end()) {
        _logger.error(fmt("%s: module already registered") % (pmodule->name()));
        return false;
    }

    pmodule->emit_quit.connect(this, & dispatcher::quit);
    this->_emit_quit.connect(pmodule.get(), & basic_module::on_quit);

    if (!pmodule->on_loaded()) {
        _logger.error(fmt("%s: on_loaded stage failed") % pmodule->name());
        return false;
    }

    emitter_mapper_pair const * emitters = pmodule->get_emitters(nemitters);
    detector_mapper_pair const * detectors = pmodule->get_detectors(ndetectors);

    typename api_map_type::iterator it_end = _api.end();

    if (emitters) {
        for (int i = 0; i < nemitters; ++i) {
            int emitter_id(emitters[i].id);
            typename api_map_type::iterator it = _api.find(emitter_id);

            if (it != it_end) {
                it->second->mapper->append_emitter(reinterpret_cast<emitter_type *>(emitters[i].emitter));
            } else {
                _logger.warn(fmt("%s: emitter '%s' not found while registering module, "
                        "may be signal/slot mapping is not supported for this application")
                        % (pmodule->name())
                        % (to_string<string_type>(emitters[i].id)));
            }
        }
    }

    if (detectors) {
        for (int i = 0; i < ndetectors; ++i) {
            int detector_id(detectors[i].id);
            typename api_map_type::iterator it = _api.find(detector_id);

            if (it != it_end) {
                it->second->mapper->append_detector(pmodule.get(), detectors[i].detector);
            } else {
                _logger.warn(fmt("%s: detector '%s' not found while registering module, "
                        "may be signal/slot mapping is not supported for this application")
                        % (pmodule->name())
                        % (to_string<string_type>(detectors[i].id)));
            }
        }
    }

    pmodule->emit_module_registered.connect(this, & dispatcher::module_registered);
    _module_spec_map.insert(pmodule->name(), modspec);

    // Module must be run in a separate thread.
    //
//    if (pmodule->run) {
//        _runnable_modules.push_back(pmodule);
//        print_debug(0, current_datetime(), fmt("%s: registered as threaded") % (pmodule->name()));
//    } else {
        _logger.debug(fmt("%s: registered") % (pmodule->name()));
//    }

    return true;
}

/**
 * @brief Load module descriptions in JSON format from UTF-8-encoded file
 *        and register specified modules.
 * @param path Path to file.
 * @return @c true on successful loading and registration, @c false otherwise.
 */
template <PFS_MODULUS_TEMPLETE_SIGNATURE>
template <typename PropertyTree>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_modules (
        filesystem::path const & path)
{
    pfs::error_code ec;

    if (!pfs::filesystem::exists(path, ec)) {
        if (ec) {
            _logger.error(fmt("`%s': %s")
                    % pfs::to_string<string_type>(path)
                    % pfs::to_string<string_type>(ec));
        } else {
            _logger.error(fmt("`%s': file not found")
                    % pfs::to_string<string_type>(path));
        }
        return false;
    }

    pfs::io::device file = pfs::io::open_device<pfs::io::file>(
            pfs::io::open_params<pfs::io::file>(path, pfs::io::read_only), ec);

    if (ec) {
        _logger.error(fmt("`%s`: file open failure: %s")
                % pfs::to_string<string_type>(path)
                % pfs::to_string<string_type>(ec));
        return false;
    }

    io::input_iterator<char> first(file);
    io::input_iterator<char> last;
    string_type content = read_all_u8<string_type>(first, last);

    PropertyTree conf;
    ec = conf.parse(content);

    if (ec) {
        _logger.error(fmt("%s: invalid configuration: %s")
                (to_string<string_type>(path))
                (to_string<string_type>(ec)).str());
        return false;
    }

    return register_modules<PropertyTree>(conf);
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
template <typename PropertyTree>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_modules (
        PropertyTree const & conf)
{
    PropertyTree disp = conf["dispatcher"];

    if (! disp.is_null()) {
        if (not disp.is_object()) {
            _logger.error("dispatcher configuration error");
            return false;
        }

        PropertyTree dlog = disp["log"];

        if (dlog.is_object()) {
            logger_type logger;
            typename PropertyTree::const_iterator it = dlog.cbegin();
            typename PropertyTree::const_iterator last = dlog.cend();

            for (; it != last; ++it) {
                string_type name = it.key();
                PropertyTree priority = *it;
                stringlist<string_type> priorities;
                typename log_ns::appender * pappender = 0;

                if (name == "stdout") {
                    pappender = & logger.template add_appender<typename log_ns::stdout_appender>();
                } else if (name == "stderr") {
                    pappender = & logger.template add_appender<typename log_ns::stderr_appender>();
                } else {
                    // Construct path from pattern
                    filesystem::path path(to_string<string_type>(current_datetime(), name).native()); // `name` is a pattern here
                    pappender = & logger.template add_appender<typename log_ns::file_appender>(path);

                    if (! pappender->is_open()) {
                        pfs::error_code ec = pfs::get_last_system_error();
                        _logger.error(fmt("Failed to create/open log file: %s: %s")
                                     (to_string<string_type>(path))
                                     (to_string<string_type>(ec)).str());
                        return false;
                    }
                }

                PFS_ASSERT(pappender);

                // FIXME must be configurable {
                pappender->set_pattern("%d{ABSOLUTE} [%p]: %m");
                pappender->set_priority_text(log_ns::priority::trace, "T");
                pappender->set_priority_text(log_ns::priority::debug, "D");
                pappender->set_priority_text(log_ns::priority::info , "I");
                pappender->set_priority_text(log_ns::priority::warn , "W");
                pappender->set_priority_text(log_ns::priority::error, "E");
                pappender->set_priority_text(log_ns::priority::fatal, "F");
                // }

                if (priority.is_string()) {
                    priorities.push_back(priority.template get<string_type>());
                } else if (priority.is_array()) {
                    for (typename PropertyTree::const_iterator pri = priority.cbegin()
                            ; pri != priority.cend(); ++pri) {
                        priorities.push_back(pri->template get<string_type>());
                    }
                }

                for (typename stringlist<string_type>::const_iterator pri = priorities.cbegin()
                        ; pri != priorities.cend(); ++pri) {
                    if (*pri == "all") {
                        logger.connect(*pappender);
                    } else if (*pri == "trace") {
                        logger.connect(log_ns::priority::trace, *pappender);
                    } else if (*pri == "debug") {
                        logger.connect(log_ns::priority::debug, *pappender);
                    } else if (*pri == "info") {
                        logger.connect(log_ns::priority::info, *pappender);
                    } else if (*pri == "warn") {
                        logger.connect(log_ns::priority::warn, *pappender);
                    } else if (*pri == "error") {
                        logger.connect(log_ns::priority::error, *pappender);
                    } else if (*pri == "fatal") {
                        logger.connect(log_ns::priority::fatal, *pappender);
                    } else {
                        _logger.error(fmt("Invalid log level name "
                                         "(must be 'all', 'trace', 'debug', 'info', 'warn', 'error' or 'fatal'): '%s'")
                                     % *pri);
                        return false;
                    }
                }
            }

            _logger.swap(logger);
        }
    }

    PropertyTree modules = conf["modules"];
    typename PropertyTree::iterator it = modules.begin();
    typename PropertyTree::iterator it_end = modules.end();

    bool result = true;

    for (; it != it_end; ++it) {
        if (it->is_object()) {
            string_type name_str = (*it)["name"].template get<string_type>();
            string_type path_str = (*it)["path"].template get<string_type>();
            bool is_active  = (*it)["active"].template get<bool>();
            bool is_master  = (*it)["master-module"].template get<bool>();

            if (name_str.empty()) {
                _logger.error("found anonymous module");
                return false;
            }

            if (is_active) {
                bool rc = false;

                if (path_str.empty())
                    rc = register_module_for_name(name_str, 0, & *it);
                else
                    rc = register_module_for_path(
                             filesystem::path(path_str.native())
                             , name_str, 0, & *it);

                if (rc) {
                    if (is_master) {
                        set_master_module(name_str);
                    }
                } else {
                    result = false;
                }
            } else {
                _logger.debug(fmt("%s: module is inactive")(name_str).str());
            }
        }
    }

    return result;
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_module_for_path (
          filesystem::path const & path
        , string_type const & name
        , const char * class_name
        , void * mod_data)
{
    module_spec modspec = module_for_path(path, class_name, mod_data);

    if (modspec.pmodule) {
        modspec.pmodule->set_name(name);
        return register_module(modspec);
    }
    return false;
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
void modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::set_master_module (
        string_type const & name)
{
    typename module_spec_map_type::iterator first = _module_spec_map.begin();
    typename module_spec_map_type::iterator last  = _module_spec_map.end();

    for (; first != last; ++first) {
        module_spec modspec = first->second;
        shared_ptr<basic_module> pmodule = modspec.pmodule;

        if (pmodule->name() == name)
            _master_module_ptr = pmodule.get();
    }
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
int modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::exec_main ()
{
    int r = exit_status::success;

    thread_sequence_type thread_pool;

    typename runnable_sequence_type::iterator irunnable      = _runnable_modules.begin();
    typename runnable_sequence_type::iterator irunnable_last = _runnable_modules.end();

    thread_function master_thread_function = 0;

    for (; irunnable != irunnable_last; ++irunnable) {
        basic_module * m = irunnable->first;
        thread_function tfunc = irunnable->second;

        // Run module if it is not a master
        if (m != _master_module_ptr)
            thread_pool.push_back(pfs::make_shared<thread>(tfunc, m));
        else
            master_thread_function = tfunc;
    }

    // Run module if it is a master
    if (master_thread_function) {
        // Run dispatcher loop in separate thread
        thread dthread(& dispatcher::run, this);

        // And call master function
        r = (_master_module_ptr->*master_thread_function)();
    } else {
        this->run();
    }

    typename thread_sequence_type::iterator ithread      = thread_pool.begin();
    typename thread_sequence_type::iterator ithread_last = thread_pool.end();

    for (; ithread != ithread_last; ++ithread) {
        (*ithread)->join();
    }

    typename module_spec_map_type::iterator imodule      = _module_spec_map.begin();
    typename module_spec_map_type::iterator imodule_last = _module_spec_map.end();

    for (; imodule != imodule_last; ++imodule) {
        module_spec & modspec = imodule->second;
        modspec.pmodule->on_finish();
    }

    return r;
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
int modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::exec ()
{
    int r = exit_status::failure;

    connect_all();

    if (start()) {
        r = exec_main();
    }

    finalize();

    return r;
}

} // pfs

#define PFS_V2_DETECTOR_CAST(slot) reinterpret_cast<detector_handler>(& slot)
#define PFS_V2_EMITTER_CAST(e)     reinterpret_cast<void *>(& e)
#define PFS_V2_MODULE_API extern "C" PFS_DLL_API

#define PFS_V2_MODULE_EMITTER(id, em) { id , PFS_V2_EMITTER_CAST(em) }
#define PFS_V2_MODULE_DETECTOR(id, dt) { id , PFS_V2_DETECTOR_CAST(dt) }

#define PFS_V2_MODULE_EMITTERS_EXTERN                                          \
    emitter_mapper_pair const *                                                \
    get_emitters (int & count) pfs_override;

#define PFS_V2_MODULE_EMITTERS_BEGIN(XMOD)                                     \
XMOD::emitter_mapper_pair const *                                              \
XMOD::get_emitters (int & count)                                               \
{                                                                              \
	static emitter_mapper_pair __emitter_mapper[] = {

#define PFS_V2_MODULE_EMITTERS_INLINE_BEGIN                                    \
emitter_mapper_pair const *                                                    \
get_emitters (int & count) pfs_override                                        \
{                                                                              \
	static emitter_mapper_pair __emitter_mapper[] = {

#define PFS_V2_MODULE_EMITTERS_END                                             \
	};                                                                         \
	count = sizeof(__emitter_mapper)/sizeof(__emitter_mapper[0]);              \
	return & __emitter_mapper[0];                                              \
}

#define PFS_V2_MODULE_DETECTORS_EXTERN                                         \
detector_mapper_pair const *                                                   \
get_detectors (int & count) pfs_override;

#define PFS_V2_MODULE_DETECTORS_BEGIN(XMOD)                                    \
XMOD::detector_mapper_pair const *                                             \
XMOD::get_detectors (int & count)                                              \
{                                                                              \
	static detector_mapper_pair __detector_mapper[] = {

#define PFS_V2_MODULE_DETECTORS_INLINE_BEGIN                                   \
detector_mapper_pair const *                                                   \
get_detectors (int & count) pfs_override                                       \
{                                                                              \
	static detector_mapper_pair __detector_mapper[] = {

#define PFS_V2_MODULE_DETECTORS_END		                                       \
	};                                                                         \
	count = sizeof(__detector_mapper)/sizeof(__detector_mapper[0]);            \
	return & __detector_mapper[0];                                             \
}