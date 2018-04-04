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
    signal0<>  emit_quit;

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

    typedef StringType      string_type;
    typedef logger::logger<StringType> logger_type;
    typedef signal1<void *> emitter_type;

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

    typedef pfs::active_queue<ActiveQueueContainer
            , BasicLockable
            , GcThreshold>                      active_queue_type;

    class basic_module// : public basic_has_slots<>
    {
        friend class dispatcher;

    public:
        typedef StringType string_type;
        typedef modulus::emitter_mapper_pair  emitter_mapper_pair;
        typedef modulus::detector_mapper_pair detector_mapper_pair;
        typedef modulus::detector_handler     detector_handler;
        typedef modulus::thread_function      thread_function;

    public: // signals
        signal0<>                            emit_quit;
        signal2<string_type const &, bool &> emit_module_registered;

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

        void register_thread_function (thread_function tfunc);

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

    class module : public basic_module, public has_slots<>
    {
    public:
        module (dispatcher * pdisp) : basic_module(pdisp), has_slots<>() {}
    };

    class async_module : public basic_module, public has_async_slots<active_queue_type>
    {
    public:
        async_module (dispatcher * pdisp) : basic_module(pdisp), has_async_slots<active_queue_type>() {}
    };

    struct sigslot_mapping0 : sigslot_mapper<signal0<>, void (basic_module::*)()> {};

    template <typename a1>
    struct sigslot_mapping1 : sigslot_mapper<signal1<a1>, void (basic_module::*)(a1)> {};

    template <typename a1, typename a2>
    struct sigslot_mapping2 : sigslot_mapper<signal2<a1, a2>, void (basic_module::*)(a1, a2)> {};

    template <typename a1, typename a2, typename a3>
    struct sigslot_mapping3 : sigslot_mapper<signal3<a1, a2, a3>, void (basic_module::*)(a1, a2, a3)> {};

    template <typename a1, typename a2, typename a3, typename a4>
    struct sigslot_mapping4 : sigslot_mapper<signal4<a1, a2, a3, a4>, void (basic_module::*)(a1, a2, a3, a4)> {};

    template <typename a1, typename a2, typename a3, typename a4, typename a5>
    struct sigslot_mapping5 : sigslot_mapper<signal5<a1, a2, a3, a4, a5>, void (basic_module::*)(a1, a2, a3, a4, a5)> {};

    template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6>
    struct sigslot_mapping6 : sigslot_mapper<signal6<a1, a2, a3, a4, a5, a6>, void (basic_module::*)(a1, a2, a3, a4, a5, a6)> {};

    template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7>
    struct sigslot_mapping7 : sigslot_mapper<signal7<a1, a2, a3, a4, a5, a6, a7>, void (basic_module::*)(a1, a2, a3, a4, a5, a6, a7)> {};

    template <typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7, typename a8>
    struct sigslot_mapping8 : sigslot_mapper<signal8<a1, a2, a3, a4, a5, a6, a7, a8>, void (basic_module::*)(a1, a2, a3, a4, a5, a6, a7, a8)> {};

    class dispatcher : basic_dispatcher, public has_slots<>
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
        void finalize ();

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
    	void module_registered (string_type const & pname, bool & result)
    	{
    		result = is_module_registered(pname);
    	}

    	void broadcast_quit ()
    	{
    		emit_quit();
    	}

        void print_info (basic_module const * m, string_type const & s)
        {
            _callback_queue.template push_method<logger_type, string_type const &>(
                    & logger_type::info, & _logger, (m != 0 ? m->name() + ": " + s : s));
        }

        void print_debug (basic_module const * m, string_type const & s)
        {
            _callback_queue.template push_method<logger_type, string_type const &>(
                    & logger_type::debug, & _logger, (m != 0 ? m->name() + ": " + s : s));
        }

        void print_warn (basic_module const * m, string_type const & s)
        {
            _callback_queue.template push_method<logger_type, string_type const &>(
                    & logger_type::warn, & _logger, (m != 0 ? m->name() + ": " + s : s));
        }

        void print_error (basic_module const * m, string_type const & s)
        {
            _callback_queue.template push_method<logger_type, string_type const &>(
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

    private:
        atomic_int _quitfl; // quit flag
        filesystem::pathlist   _searchdirs;
        api_map_type           _api;
        module_spec_map_type   _module_spec_map;
        runnable_sequence_type _runnable_modules; // modules run in a separate threads
        basic_module *         _master_module_ptr;
        logger_type            _logger;
        active_queue_type      _callback_queue;
    }; // class dispatcher
}; // struct modulus

}} // pfs::app

#include "dispatcher.hpp"
#include "module.hpp"

#endif /* __PFS_V2_APP_MODULUS_HPP__ */
