#ifndef __PFS_V2_APP_DISPATCHER_HPP__
#define __PFS_V2_APP_DISPATCHER_HPP__

#include "modulus.hpp"


namespace pfs {
namespace app {

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::dispatcher (
        api_item_type * mapper
        , int n)
    : basic_dispatcher()
    , _master_module_ptr(0)
    , _quitfl(0)
{
    // Initialize default logger
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

    this->emit_quit.connect(this, & dispatcher::on_quit);

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
        if (_callback_queue.empty()) {
            pfs::this_thread::sleep_for(pfs::chrono::milliseconds(100));
            continue;
        }
        _callback_queue.call_all();
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
        shared_ptr<module> pmodule = modspec.pmodule;

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
        shared_ptr<module> & pmodule = modspec.pmodule;
        pmodule->emit_module_registered.disconnect(this);
        _logger.debug(fmt("%s: unregistered") % (pmodule->name()));

        // Need to destroy pmodule before dynamic library will be destroyed automatically
        pmodule.reset();
    }

    _module_spec_map.clear();
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
void modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::finalize ()
{
    if (_module_spec_map.size() > 0) {
        disconnect_all();
        unregister_all();
    }
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_local_module (
          module * pmodule
        , string_type const & name)
{
    module_spec modspec;
    modspec.pmodule = shared_ptr<module>(pmodule);
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
    module * ptr = module_ctor(this, class_name, mod_data);

    if (!ptr)
        return module_spec();

    module_spec result;
    result.pdl = pdl;
    result.pmodule = shared_ptr<module>(ptr, module_deleter(module_dtor));

    return result;
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_module (
        module_spec const & modspec)
{
    int nemitters, ndetectors;

    if (!modspec.pmodule)
        return false;

    shared_ptr<module> pmodule = modspec.pmodule;

    if (_module_spec_map.find(pmodule->name()) != _module_spec_map.end()) {
        _logger.error(fmt("%s: module already registered") % (pmodule->name()));
        return false;
    }

    pmodule->emit_quit.connect(this, & dispatcher::broadcast_quit);
    this->emit_quit.connect(pmodule.get(), & module::on_quit);

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
template <typename JsonType>
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

    JsonType conf;
    ec = conf.parse(content);

    if (ec) {
    	_logger.error(fmt("%s: invalid JSON: %s")
                (to_string<string_type>(path))
                (to_string<string_type>(ec)).str());
    	return false;
    }

    return register_modules<JsonType>(conf);
}

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
template <typename JsonType>
bool modulus<PFS_MODULUS_TEMPLETE_ARGS>::dispatcher::register_modules (
        JsonType const & conf)
{
    JsonType disp = conf["dispatcher"];

    if (! disp.is_null()) {
    	if (not disp.is_object()) {
        	_logger.error("dispatcher configuration error");
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
    				pappender = & logger.template add_appender<logger::stdout_appender<string_type> >();
    			} else if (name == "stderr") {
    				pappender = & logger.template add_appender<logger::stderr_appender<string_type> >();
    			} else {
                    // Construct path from pattern
    				filesystem::path path(to_string<string_type>(current_datetime(), name).native()); // `name` is a pattern here
    				pappender = & logger.template add_appender<logger::file_appender<string_type> >(path);

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

        		for (typename stringlist<string_type>::const_iterator pri = priorities.cbegin()
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
    	    	_logger.error("found anonymous module");
    	    	return false;
    		}

    		if (is_active) {
    			bool rc = false;

    			if (path_str.empty())
    				rc = register_module_for_name(name_str, 0, & *it);
    			else
    				rc = register_module_for_path(
                              pfs::filesystem::path(path_str.native())
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
        shared_ptr<module> pmodule = modspec.pmodule;

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
        module * m = irunnable->first;
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
        module_spec modspec = imodule->second;
        shared_ptr<module> pmodule = modspec.pmodule;
        pmodule->on_finish();
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

}} // pfs::app

#endif /* __PFS_V2_APP_DISPATCHER_HPP__ */

