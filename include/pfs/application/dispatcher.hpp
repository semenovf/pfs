#ifndef __PFS_APPLICATION_DISPATCHER_HPP__
#define __PFS_APPLICATION_DISPATCHER_HPP__

#include <pfs/traits/associative_container.hpp>
#include <pfs/traits/sequence_container.hpp>
#include <pfs/traits/stdcxx/map.hpp>
#include <pfs/traits/stdcxx/list.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/system_string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/dynamic_library.hpp>
#include <pfs/logger.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/datetime.hpp>
#include <pfs/thread.hpp>
#include <pfs/io/file.hpp>
#include <pfs/application/module.hpp>
#include <pfs/application/sigslot_mapping.hpp>
#include <pfs/unicode/u8_iterator.hpp>
#include <pfs/io/iterator.hpp>

namespace pfs {
namespace application {

struct module_spec
{
    shared_ptr<module>          pmodule;
    shared_ptr<dynamic_library> pdl;
};

class dispatcher : public has_slots<>
{
public:
    typedef system_string string_type;

    typedef struct api_item_type
    {
        int                 id;
        sigslot_mapping_t * mapping;
        string_type         desc;
    } api_item_type_t;

    typedef traits::associative_container<traits::kv<int, api_item_type *>
            ,  stdcxx::map> api_map;

    typedef traits::associative_container<traits::kv<string_type, module_spec>
            , stdcxx::map>  module_spec_map;

    typedef traits::sequence_container<shared_ptr<module>
            , stdcxx::list>  runnable_sequence;

    typedef traits::sequence_container<shared_ptr<pfs::thread>
            , stdcxx::list>  thread_sequence;

    typedef logger::logger<string_type> logger_type;
    typedef safeformat<string_type>     fmt;

private:
    filesystem::pathlist _searchdirs;
    api_map              _api;
    module_spec_map      _module_spec_map;
    runnable_sequence    _runnable_modules; // modules run in a separate threads
    module *             _master_module_ptr;
    logger_type          _logger;

private:
#if __cplusplus >= 201103L
    dispatcher (dispatcher const &) = delete;
    dispatcher & operator = (dispatcher const &) = delete;
#else
    dispatcher (dispatcher const &);
    dispatcher & operator = (dispatcher const &);
#endif

protected:
//    dispatcher ()
//        : _master_module_ptr(0)
//    {}

    void init_default_logger ()
    {
        logger_type::appender_type & cout_appender = _logger.add_appender<logger::stdout_appender<string_type> >();
        logger_type::appender_type & cerr_appender = _logger.add_appender<logger::stderr_appender<string_type> >();
        cout_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
        cerr_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");

        _logger.connect(pfs::logger::priority::trace   , cout_appender);
        _logger.connect(pfs::logger::priority::debug   , cout_appender);
        _logger.connect(pfs::logger::priority::info    , cout_appender);
        _logger.connect(pfs::logger::priority::warn    , cerr_appender);
        _logger.connect(pfs::logger::priority::error   , cerr_appender);
        _logger.connect(pfs::logger::priority::critical, cerr_appender);
    }

    bool activate_posix_signal_handling ();
    void deactivate_posix_signal_handling ();

public:
    //dispatcher ();
    dispatcher (api_item_type * mapping, int n);

    virtual ~dispatcher ()
    {
        deactivate_posix_signal_handling();
        finalize();
    }

    void finalize ();
    void register_api (api_item_type * mapping, int n);

    void add_search_path (filesystem::path const & dir)
    {
        if (!dir.empty())
            _searchdirs.push_back(dir);
    }

    /**
     * @brief Output summary of emitters/detectors utilization.
     * TODO Implement
     */
    void print_api_connections () {}

    /**
     * @brief Output summary of incomplete emitters/detectors utilization.
     * TODO Implement
     */
    void print_api_incomplete_connections () {}

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

	bool register_local_module (module * pmodule, string_type const & name)
	{
		module_spec modspec;
		modspec.pmodule = shared_ptr<module>(pmodule);
		modspec.pmodule->set_name(name);
		return register_module(modspec);
	}

	bool register_module_for_path (filesystem::path const & path
			, string_type const & name
			, const char * class_name = 0
			, void * mod_data = 0)
	{
		module_spec modspec = module_for_path(path, class_name, mod_data);

        if (modspec.pmodule) {
            modspec.pmodule->set_name(name);
            return register_module(modspec);
        }
        return false;
	}

	bool register_module_for_name (string_type const & name
			, char const * class_name = 0
			, void * mod_data = 0)
	{
		module_spec modspec = module_for_name(name, class_name, mod_data);

		if (modspec.pmodule) {
			modspec.pmodule->set_name(name);
			return register_module(modspec);
		}
		return false;
	}

	void set_master_module (string_type const & name);

	size_t count () const
	{
		return _module_spec_map.size();
	}

	void connect_all ();
	void disconnect_all ();
	void unregister_all ();
	bool start ();
	int  exec ();

	bool is_module_registered (string_type const & modname) const
	{
		return _module_spec_map.find(modname) != _module_spec_map.end();
	}

public: // signals
	signal0<> emit_quit;
	signal2<module const *, string_type const &> emit_info;
	signal2<module const *, string_type const &> emit_debug;
	signal2<module const *, string_type const &> emit_warn;
	signal2<module const *, string_type const &> emit_error;

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

	void print_info  (module const * m, string_type const & s);
	void print_debug (module const * m, string_type const & s);
	void print_warn  (module const * m, string_type const & s);
	void print_error (module const * m, string_type const & s);

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
};

/**
 * @brief Load module descriptions in JSON format from UTF-8-encoded file
 *        and register specified modules.
 * @param path Path to file.
 * @return @c true on successful loading and registration, @c false otherwise.
 */
template <typename JsonType>
bool dispatcher::register_modules (filesystem::path const & path)
{
    pfs::error_code ec;

    if (!pfs::filesystem::exists(path, ec)) {
        if (ec) {
            print_error(0, fmt("`%s': %s")
                    % pfs::to_string<string_type>(path)
                    % pfs::to_string<string_type>(ec));
        } else {
            print_error(0, fmt("`%s': file not found")
                    % pfs::to_string<string_type>(path));
        }
        return false;
    }

    pfs::io::device file = pfs::io::open_device<pfs::io::file>(
            pfs::io::open_params<pfs::io::file>(path, pfs::io::read_only), ec);

    if (ec) {
        print_error(0, fmt("`%s`: file open failure: %s")
                % pfs::to_string<string_type>(path)
                % pfs::to_string<string_type>(ec));
        return false;
    }

    io::input_iterator<char> first(file);
    io::input_iterator<char> last;
    string_type content = read_all_u8<string_type>(first, last);

    JsonType conf;
    ec = conf.parse(content);

    if (ec) {
    	print_error(0, fmt("%s: Invalid JSON: %s")
                (to_string<string_type>(path))
                (to_string<string_type>(ec)).str());
    	return false;
    }

    return register_modules<JsonType>(conf);
}


template <typename JsonType>
bool dispatcher::register_modules (JsonType const & conf)
{
    JsonType disp = conf["dispatcher"];

    if (! disp.is_null()) {
    	if (not disp.is_object()) {
        	print_error(0, "Dispatcher configuration error");
    		return false;
    	}

    	JsonType dlog = disp["log"];

    	if (dlog.is_object()) {
    		logger::logger<string_type> logger;
    		typename JsonType::const_iterator it = dlog.cbegin();
    		typename JsonType::const_iterator last = dlog.cend();

    		for (; it != last; ++it) {
    			string_type name = it.key();
    			JsonType priority = *it;
    			stringlist<string_type> priorities;
    			logger::appender<typename JsonType::string_type> * pappender = 0;

    			if (name == "stdout") {
    				pappender = & logger.add_appender<logger::stdout_appender<string_type> >();
    			} else if (name == "stderr") {
    				pappender = & logger.add_appender<logger::stderr_appender<string_type> >();
    			} else {
                    // Construct path from pattern
    				filesystem::path path(to_string<string_type>(current_datetime(), name).native()); // `name` is a pattern here
    				pappender = & logger.add_appender<logger::file_appender<string_type> >(path);

                    if (! pappender->is_open()) {
                        pfs::error_code ec = pfs::get_last_system_error();
    					print_error(0, fmt("Failed to create/open log file: %s: %s")
    							(to_string<string_type>(path))
    							(to_string<string_type>(ec)).str());
    					return false;
                    }
    			}

    			PFS_ASSERT(pappender);

                // FIXME must be configurable {
                pappender->set_pattern("%d{ABSOLUTE} [%p]: %m");
                pappender->set_priority_text(logger::priority::trace, "T");
                pappender->set_priority_text(logger::priority::debug, "D");
                pappender->set_priority_text(logger::priority::info , "I");
                pappender->set_priority_text(logger::priority::warn , "W");
                pappender->set_priority_text(logger::priority::error, "E");
                pappender->set_priority_text(logger::priority::fatal, "F");
                // }

    			if (priority.is_string()) {
    				priorities.push_back(priority.template get<string_type>());
    			} else if (priority.is_array()) {
    				for (typename JsonType::const_iterator pri = priority.cbegin()
                            ; pri != priority.cend(); ++pri) {
    					priorities.push_back(pri->template get<string_type>());
    				}
    			}

        		for (stringlist<string_type>::const_iterator pri = priorities.cbegin()
                        ; pri != priorities.cend(); ++pri) {
        			if (*pri == "all") {
        				logger.connect(*pappender);
        			} else if (*pri == "trace") {
        				logger.connect(logger::priority::trace, *pappender);
        			} else if (*pri == "debug") {
        				logger.connect(logger::priority::debug, *pappender);
        			} else if (*pri == "info") {
        				logger.connect(logger::priority::info, *pappender);
        			} else if (*pri == "warn") {
        				logger.connect(logger::priority::warn, *pappender);
        			} else if (*pri == "error") {
        				logger.connect(logger::priority::error, *pappender);
        			} else if (*pri == "fatal") {
        				logger.connect(logger::priority::fatal, *pappender);
        			} else {
    					print_error(0, fmt("Invalid log level name (must be 'all', 'trace', 'debug', 'info', 'warn', 'error' or 'fatal'): '%s'")
    							(*pri).str());
    					return false;
        			}
        		}
    		}

    		_logger.swap(logger);
    	}
    }

    JsonType modules = conf["modules"];
    typename JsonType::iterator it = modules.begin();
    typename JsonType::iterator it_end = modules.end();

    bool result = true;

    for (; it != it_end; ++it) {
    	if (it->is_object()) {
    		string_type name_str = (*it)["name"].template get<string_type>();
    		string_type path_str = (*it)["path"].template get<string_type>();
    		bool is_active  = (*it)["active"].template get<bool>();
    		bool is_master  = (*it)["master-module"].template get<bool>();

    		if (name_str.empty()) {
    	    	print_error(0, "Found anonymous module");
    	    	return false;
    		}

    		if (is_active) {
    			bool rc = false;

    			if (path_str.empty())
    				rc = register_module_for_name(name_str, 0, & *it);
    			else
    				rc = register_module_for_path(pfs::filesystem::path(path_str.native()), name_str, 0, & *it);

    			if (rc) {
    				if (is_master) {
    					set_master_module(name_str);
    				}
    			} else {
    				result = false;
    			}
    		} else {
    			print_debug(0, fmt("%s: Module is inactive")(name_str).str());
    		}
    	}
    }

    return result;
}

}} // pfs::application

#endif /* __PFS_APPLICATION_DISPATCHER_HPP__ */
