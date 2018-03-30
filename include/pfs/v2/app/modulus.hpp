#ifndef __PFS_V2_APP_MODULUS_HPP__
#define __PFS_V2_APP_MODULUS_HPP__

#include <pfs/operationsystem.hpp>
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
#include <pfs/unicode/u8_iterator.hpp>
#include <pfs/io/iterator.hpp>

namespace pfs {
namespace app {

#define PFS_MODULE_CTOR_NAME "__module_ctor__"
#define PFS_MODULE_DTOR_NAME "__module_dtor__"

struct basic_dispatcher
{
    signal0<> emit_quit;

#if PFS_OS_POSIX
    bool activate_posix_signal_handling ();
    void deactivate_posix_signal_handling ();

    basic_dispatcher ()
    {
        activate_posix_signal_handling();
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
    class module;
    class dispatcher;
    
    typedef StringType      string_type;
    typedef signal1<void *> emitter_type;

    typedef void (module::* detector_handler)(void *);
    typedef struct { int id; void * emitter; }            emitter_mapper_pair;
    typedef struct { int id; detector_handler detector; } detector_mapper_pair;

    typedef module * (* module_ctor_t)(char const * name, void *);
    typedef void  (* module_dtor_t)(module *);

    struct detector_pair
    {
        module *         mod;
        detector_handler detector;

        detector_pair () : module(0), detector(0) {}
        detector_pair (module * p, detector_handler d) : mod(p), detector(d) {}
    };

    struct module_spec
    {
        shared_ptr<module>          pmodule;
        shared_ptr<dynamic_library> pdl;
    };

    struct module_deleter
    {
        module_dtor_t _deleter;

        module_deleter (module_dtor_t deleter)
            : _deleter(deleter)
        {}

        void operator () (module * p) const
        {
            _deleter(p);
        }
    };

    struct basic_sigslot_mapper
    {
        virtual void connect_all () = 0;
        virtual void disconnect_all () = 0;
        virtual void append_emitter (emitter_type *e) = 0;
        virtual void append_detector (module * m, detector_handler d) = 0;
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

        virtual void append_detector (module * m, detector_handler d)
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

    typedef traits::sequence_container<shared_ptr<module>
            , SequenceContainer>                runnable_sequence_type;

    typedef traits::sequence_container<shared_ptr<pfs::thread>
            , SequenceContainer>                thread_sequence_type;

    typedef pfs::active_queue<ActiveQueueContainer
            , BasicLockable
            , GcThreshold>                      active_queue_type;

    class module : public has_slots<>
    {
        friend class dispatcher;

    public:
        typedef StringType string_type;
        typedef modulus::emitter_mapper_pair  emitter_mapper_pair;
        typedef modulus::detector_mapper_pair detector_mapper_pair;
        typedef modulus::detector_handler     detector_handler;
    //
    //	struct log_consumer : public has_slots<>
    //	{
    //		friend class module;
    //		virtual void on_info  (module const *, datetime const &, string_type const &) = 0; // {}
    //		virtual void on_debug (module const *, datetime const &, string_type const &) = 0; // {}
    //		virtual void on_warn  (module const *, datetime const &, string_type const &) = 0; // {}
    //		virtual void on_error (module const *, datetime const &, string_type const &) = 0; // {}
    //
    //	private:
    //		void _on_info  (module const * m, datetime const & dt, string_type const & s)
    //		{
    //			this->on_info(m, dt, s);
    //		}
    //
    //		void _on_debug (module const * m, datetime const & dt, string_type const & s)
    //		{
    //			this->on_debug(m, dt, s);
    //		}
    //
    //		void _on_warn  (module const * m, datetime const & dt, string_type const & s)
    //		{
    //			this->on_warn(m, dt, s);
    //		}
    //
    //		void _on_error (module const * m, datetime const & dt, string_type const & s)
    //		{
    //			this->on_error(m, dt, s);
    //		}
    //	};

    protected:
    //	signal3<module const *, datetime const &, string_type const &> _emit_info;
    //	signal3<module const *, datetime const &, string_type const &> _emit_debug;
    //	signal3<module const *, datetime const &, string_type const &> _emit_warn;
    //	signal3<module const *, datetime const &, string_type const &> _emit_error;

    public: // signals
        signal0<>                            emit_quit;
        signal2<string_type const &, bool &> emit_module_registered;

    public:
        void print_info (module const * m, string_type const & s)
        {
            _pdispatcher->print_info(m, current_datetime(), s);
        }
    
    	void print_debug (module const * m, string_type const & s)
        {
            _pdispatcher->print_debug(m, current_datetime(), s);
        }
    
    	void print_warn (module const * m, string_type const & s)
        {
            _pdispatcher->print_warn(m, current_datetime(), s);
        }
    
    	void print_error (module const * m, string_type const & s)
        {
            _pdispatcher->print_error(m, current_datetime(), s);
        }

    private:
        string_type  _name;
        dispatcher * _pdispatcher;
        atomic_int   _quitfl; // quit flag

    public:
        int (* run) (module *);

    protected:
        module ()
            : _pdispatcher(0)
            , _quitfl(0)
            , run(0)
        {}

        void set_name (string_type const & name)
        {
            _name = name;
        }

    public:
        virtual ~module () {}

        string_type const & name() const
        {
            return _name;
        }

        bool is_registered () const
        {
            return _pdispatcher != 0 ? true : false;
        }

        void set_dispatcher (dispatcher * pdispatcher)
        {
            _pdispatcher = pdispatcher;
        }

        dispatcher * get_dispatcher ()
        {
            return _pdispatcher;
        }

        dispatcher const * get_dispatcher () const
        {
            return _pdispatcher;
        }

    //	void connect_info  (log_consumer * p);
    //	void connect_debug (log_consumer * p);
    //	void connect_warn  (log_consumer * p);
    //	void connect_error (log_consumer * p);
    //
    //   	void disconnect_info  (log_consumer * p);
    //	void disconnect_debug (log_consumer * p);
    //	void disconnect_warn  (log_consumer * p);
    //	void disconnect_error (log_consumer * p);
    //    void disconnect_all_loggers (log_consumer * p);

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
    };// module

    struct sigslot_mapping0 : sigslot_mapper<signal0<>, void (module::*)()> {};

    template <typename a1>
    struct sigslot_mapping1 : sigslot_mapper<signal1<a1>, void (module::*)(a1)> {};

    template <typename a1, typename a2>
    struct sigslot_mapping2 : sigslot_mapper<signal2<a1, a2>, void (module::*)(a1, a2)> {};

    template <typename a1, typename a2, typename a3>
    struct sigslot_mapping3 : sigslot_mapper<signal3<a1, a2, a3>, void (module::*)(a1, a2, a3)> {};

    template <typename a1, typename a2, typename a3, typename a4>
    struct sigslot_mapping4 : sigslot_mapper<signal4<a1, a2, a3, a4>, void (module::*)(a1, a2, a3, a4)> {};

    template <typename a1, typename a2, typename a3, typename a4, typename a5>
    struct sigslot_mapping5 : sigslot_mapper<signal5<a1, a2, a3, a4, a5>, void (module::*)(a1, a2, a3, a4, a5)> {};

    template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6>
    struct sigslot_mapping6 : sigslot_mapper<signal6<a1, a2, a3, a4, a5, a6>, void (module::*)(a1, a2, a3, a4, a5, a6)> {};

    template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7>
    struct sigslot_mapping7 : sigslot_mapper<signal7<a1, a2, a3, a4, a5, a6, a7>, void (module::*)(a1, a2, a3, a4, a5, a6, a7)> {};

    template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8>
    struct sigslot_mapping8 : sigslot_mapper<signal8<a1, a2, a3, a4, a5, a6, a7, a8>, void (module::*)(a1, a2, a3, a4, a5, a6, a7, a8)> {};

    class dispatcher : basic_dispatcher, public has_slots<>
    {
    //    typedef typename Traits::active_queue_type      active_queue_type;
    //    typedef typename Traits::api_map_type           api_map_type
    //    typedef typename Traits::module_spec_map_type   module_spec_map_type;
    //    typedef typename Traits::api_item_type          api_item_type;
    //    typedef typename Traits::runnable_sequence_type runnable_sequence_type;
    //    typedef typename Traits::thread_sequence_type   thread_sequence_type;
        typedef logger::logger<StringType> logger_type;
        typedef safeformat<StringType>     fmt;

    public:
        typedef StringType string_type;

        struct exit_status
        {
            enum {
                  success = 0
                , failure = -1
            };
        };

    private:
        filesystem::pathlist   _searchdirs;
        api_map_type           _api;
        module_spec_map_type   _module_spec_map;
        runnable_sequence_type _runnable_modules; // modules run in a separate threads
        module *               _master_module_ptr;
        logger_type            _logger;
        active_queue_type      _callback_queue;

    private:
    #if __cplusplus >= 201103L
        dispatcher (dispatcher const &) = delete;
        dispatcher & operator = (dispatcher const &) = delete;
    #else
        dispatcher (dispatcher const &);
        dispatcher & operator = (dispatcher const &);
    #endif

    private:
        void init_default_logger ()
        {
            typename logger_type::appender_type & cout_appender 
                    = _logger.template add_appender<logger::stdout_appender<string_type> >();
            typename logger_type::appender_type & cerr_appender 
                    = _logger.template add_appender<logger::stderr_appender<string_type> >();
            cout_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
            cerr_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
    
            _logger.connect(pfs::logger::priority::trace   , cout_appender);
            _logger.connect(pfs::logger::priority::debug   , cout_appender);
            _logger.connect(pfs::logger::priority::info    , cout_appender);
            _logger.connect(pfs::logger::priority::warn    , cerr_appender);
            _logger.connect(pfs::logger::priority::error   , cerr_appender);
            _logger.connect(pfs::logger::priority::critical, cerr_appender);
        }

        void connect_all ();
        void disconnect_all ();
        void unregister_all ();
        bool start ();
        void finalize ();
        int exec_main ();

    public:
        dispatcher (api_item_type * mapper, int n);

        virtual ~dispatcher ()
        {
            finalize();
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
         * @brief Register modules enumerated in configuration file (JSON) specified by @a path.
         *
         * @param path Configuration file path (JSON format).
         * @return @c true if modules registered successfully. @c false otherwise.
         */
        template <typename JsonType>
        bool register_modules (filesystem::path const & path);
    
        /**
         * @brief Register modules enumerated in JSON instance specified by @a conf.
         *
         * @param conf JSON instance contained modules for registration.
         * @return @c true if modules registered successfully. @c false otherwise.
         */
        template <typename Json>
        bool register_modules (Json const & conf);
    
    	bool register_local_module (module * pmodule, string_type const & name);
    
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

    protected: // signals
    	signal3<module const *, datetime const &, string_type const &> _emit_info;
    	signal3<module const *, datetime const &, string_type const &> _emit_debug;
    	signal3<module const *, datetime const &, string_type const &> _emit_warn;
    	signal3<module const *, datetime const &, string_type const &> _emit_error;
    
    //    void emit_info (module const * m, string_type const & s)
    //    {
    //        _emit_info(m, current_datetime(), s);
    //    }
    //
    //	void emit_debug (module const * m, string_type const & s)
    //    {
    //        _emit_debug(m, current_datetime(), s);
    //    }
    //
    //	void emit_warn (module const * m, string_type const & s)
    //    {
    //        _emit_warn(m, current_datetime(), s);
    //    }
    //
    //	void emit_error (module const * m, string_type const & s)
    //    {
    //        _emit_error(m, current_datetime(), s);
    //    }

    public: // slots
    	void module_registered (string_type const & pname, bool & result)
    	{
    		result = is_module_registered(pname);
    	}
    
    	void broadcast_quit ()
    	{
    		emit_quit();
    	}
    
        logger_type const & logger () const
        {
            return _logger;
        }
    
        logger_type & logger ()
        {
            return _logger;
        }
    
    	void print_info  (module const * m, datetime const & dt, string_type const & s);
    	void print_debug (module const * m, datetime const & dt, string_type const & s);
    	void print_warn  (module const * m, datetime const & dt, string_type const & s);
    	void print_error (module const * m, datetime const & dt, string_type const & s);
    
      	void print_info (module const * m, string_type const & s)
        {
            print_info(m, current_datetime(), s);
        }
    
      	void print_info (string_type const & s)
        {
            print_info(0, current_datetime(), s);
        }

    	void print_debug (module const * m, string_type const & s)
        {
            print_debug(m, current_datetime(), s);
        }
    
    	void print_debug (string_type const & s)
        {
            print_debug(0, current_datetime(), s);
        }
        
    	void print_warn (module const * m, string_type const & s)
        {
            print_warn(m, current_datetime(), s);
        }

    	void print_warn (string_type const & s)
        {
            print_warn(0, current_datetime(), s);
        }

        void print_error (module const * m, string_type const & s)
        {
            print_error(m, current_datetime(), s);
        }

        void print_error (string_type const & s)
        {
            print_error(0, current_datetime(), s);
        }

    protected:
    	module_spec module_for_path (filesystem::path const & path
    			, char const * class_name = 0
    			, void * mod_data = 0);
    
    	module_spec module_for_name (string_type const & name
    			, char const * class_name = 0
    			, void * mod_data = 0)
    	{
    		filesystem::path modpath = build_so_filename(name.native());
    		return module_for_path(modpath, class_name, mod_data);
    	}
    
    	bool register_module (module_spec const & modspec);
    }; // class dispatcher
}; // struct modulus


/**
 * @brief Load module descriptions in JSON format from UTF-8-encoded file
 *        and register specified modules.
 * @param path Path to file.
 * @return @c true on successful loading and registration, @c false otherwise.
 */
//template <typename JsonType>
//bool dispatcher::register_modules (filesystem::path const & path)
//{
//    pfs::error_code ec;
//
//    if (!pfs::filesystem::exists(path, ec)) {
//        if (ec) {
//            print_error(0, current_datetime(), fmt("`%s': %s")
//                    % pfs::to_string<string_type>(path)
//                    % pfs::to_string<string_type>(ec));
//        } else {
//            print_error(0, current_datetime(), fmt("`%s': file not found")
//                    % pfs::to_string<string_type>(path));
//        }
//        return false;
//    }
//
//    pfs::io::device file = pfs::io::open_device<pfs::io::file>(
//            pfs::io::open_params<pfs::io::file>(path, pfs::io::read_only), ec);
//
//    if (ec) {
//        print_error(0, current_datetime(), fmt("`%s`: file open failure: %s")
//                % pfs::to_string<string_type>(path)
//                % pfs::to_string<string_type>(ec));
//        return false;
//    }
//
//    io::input_iterator<char> first(file);
//    io::input_iterator<char> last;
//    string_type content = read_all_u8<string_type>(first, last);
//
//    JsonType conf;
//    ec = conf.parse(content);
//
//    if (ec) {
//    	print_error(0, current_datetime(), fmt("%s: Invalid JSON: %s")
//                (to_string<string_type>(path))
//                (to_string<string_type>(ec)).str());
//    	return false;
//    }
//
//    return register_modules<JsonType>(conf);
//}


//template <typename JsonType>
//bool dispatcher::register_modules (JsonType const & conf)
//{
//    JsonType disp = conf["dispatcher"];
//
//    if (! disp.is_null()) {
//    	if (not disp.is_object()) {
//        	print_error(0, current_datetime(), "Dispatcher configuration error");
//    		return false;
//    	}
//
//    	JsonType dlog = disp["log"];
//
//    	if (dlog.is_object()) {
//    		logger::logger<string_type> logger;
//    		typename JsonType::const_iterator it = dlog.cbegin();
//    		typename JsonType::const_iterator last = dlog.cend();
//
//    		for (; it != last; ++it) {
//    			string_type name = it.key();
//    			JsonType priority = *it;
//    			stringlist<string_type> priorities;
//    			logger::appender<typename JsonType::string_type> * pappender = 0;
//
//    			if (name == "stdout") {
//    				pappender = & logger.add_appender<logger::stdout_appender<string_type> >();
//    			} else if (name == "stderr") {
//    				pappender = & logger.add_appender<logger::stderr_appender<string_type> >();
//    			} else {
//                    // Construct path from pattern
//    				filesystem::path path(to_string<string_type>(current_datetime(), name).native()); // `name` is a pattern here
//    				pappender = & logger.add_appender<logger::file_appender<string_type> >(path);
//
//                    if (! pappender->is_open()) {
//                        pfs::error_code ec = pfs::get_last_system_error();
//    					print_error(0, current_datetime(), fmt("Failed to create/open log file: %s: %s")
//    							(to_string<string_type>(path))
//    							(to_string<string_type>(ec)).str());
//    					return false;
//                    }
//    			}
//
//    			PFS_ASSERT(pappender);
//
//                // FIXME must be configurable {
//                pappender->set_pattern("%d{ABSOLUTE} [%p]: %m");
//                pappender->set_priority_text(logger::priority::trace, "T");
//                pappender->set_priority_text(logger::priority::debug, "D");
//                pappender->set_priority_text(logger::priority::info , "I");
//                pappender->set_priority_text(logger::priority::warn , "W");
//                pappender->set_priority_text(logger::priority::error, "E");
//                pappender->set_priority_text(logger::priority::fatal, "F");
//                // }
//
//    			if (priority.is_string()) {
//    				priorities.push_back(priority.template get<string_type>());
//    			} else if (priority.is_array()) {
//    				for (typename JsonType::const_iterator pri = priority.cbegin()
//                            ; pri != priority.cend(); ++pri) {
//    					priorities.push_back(pri->template get<string_type>());
//    				}
//    			}
//
//        		for (stringlist<string_type>::const_iterator pri = priorities.cbegin()
//                        ; pri != priorities.cend(); ++pri) {
//        			if (*pri == "all") {
//        				logger.connect(*pappender);
//        			} else if (*pri == "trace") {
//        				logger.connect(logger::priority::trace, *pappender);
//        			} else if (*pri == "debug") {
//        				logger.connect(logger::priority::debug, *pappender);
//        			} else if (*pri == "info") {
//        				logger.connect(logger::priority::info, *pappender);
//        			} else if (*pri == "warn") {
//        				logger.connect(logger::priority::warn, *pappender);
//        			} else if (*pri == "error") {
//        				logger.connect(logger::priority::error, *pappender);
//        			} else if (*pri == "fatal") {
//        				logger.connect(logger::priority::fatal, *pappender);
//        			} else {
//    					print_error(0, current_datetime()
//                                , fmt("Invalid log level name (must be 'all', 'trace', 'debug', 'info', 'warn', 'error' or 'fatal'): '%s'")
//    							% *pri);
//    					return false;
//        			}
//        		}
//    		}
//
//    		_logger.swap(logger);
//    	}
//    }
//
//    JsonType modules = conf["modules"];
//    typename JsonType::iterator it = modules.begin();
//    typename JsonType::iterator it_end = modules.end();
//
//    bool result = true;
//
//    for (; it != it_end; ++it) {
//    	if (it->is_object()) {
//    		string_type name_str = (*it)["name"].template get<string_type>();
//    		string_type path_str = (*it)["path"].template get<string_type>();
//    		bool is_active  = (*it)["active"].template get<bool>();
//    		bool is_master  = (*it)["master-module"].template get<bool>();
//
//    		if (name_str.empty()) {
//    	    	print_error(0, current_datetime(), "Found anonymous module");
//    	    	return false;
//    		}
//
//    		if (is_active) {
//    			bool rc = false;
//
//    			if (path_str.empty())
//    				rc = register_module_for_name(name_str, 0, & *it);
//    			else
//    				rc = register_module_for_path(pfs::filesystem::path(path_str.native()), name_str, 0, & *it);
//
//    			if (rc) {
//    				if (is_master) {
//    					set_master_module(name_str);
//    				}
//    			} else {
//    				result = false;
//    			}
//    		} else {
//    			print_debug(0, current_datetime(), fmt("%s: Module is inactive")(name_str).str());
//    		}
//    	}
//    }
//
//    return result;
//}

}} // pfs::app

#include "dispatcher.hpp"
#include "module.hpp"

#endif /* __PFS_V2_APP_MODULUS_HPP__ */
