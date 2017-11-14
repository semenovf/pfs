#include "pfs/thread.hpp"
#include "pfs/application/dispatcher.hpp"

namespace pfs {
namespace application {

struct module_deleter
{
	module_dtor_t _deleter;

	module_deleter (module_dtor_t deleter)
		: _deleter(deleter)
	{}

	void operator () (module * p) const {
		_deleter(p);
	}
};

//dispatcher::dispatcher ()
//	: _master_module_ptr(0)
//{
//    init_default_logger();
//    activate_posix_signal_handling();
//}

dispatcher::dispatcher (api_item_type * mapping, int n)
    : _master_module_ptr(0)
{
    init_default_logger();
    activate_posix_signal_handling();
    register_api(mapping, n);
}

void dispatcher::register_api (api_item_type * mapping, int n)
{
    for (int i = 0; i < n; ++i) {
        _api.insert(mapping[i].id, & mapping[i]);
    }
}

inline dispatcher::string_type __build_string_for_log (module const * m
    , dispatcher::string_type const & s)
{
    return m != 0 ? m->name() + ": " + s : s;
}

void dispatcher::print_info (module const * m, string_type const & s)
{
	_logger.info(__build_string_for_log(m, s));
	emit_info(m, s);
}

void dispatcher::print_debug (module const * m, string_type const & s)
{
	_logger.debug(__build_string_for_log(m, s));
	emit_debug(m, s);
}

void dispatcher::print_warn  (module const * m, string_type const & s)
{
	_logger.warn(__build_string_for_log(m, s));
	emit_warn(m, s);
}

void dispatcher::print_error (module const * m, string_type const & s)
{
	_logger.error(__build_string_for_log(m, s));
	emit_error(m, s);
}

void dispatcher::finalize ()
{
	if (_module_spec_map.size() > 0) {
		disconnect_all();
		unregister_all();
	}
}

module_spec dispatcher::module_for_path (filesystem::path const & path
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
        print_error(0, fmt("%s: %s")
                % (to_string<string_type>(dlpath))
                % (to_string<string_type>(ec)));
        return module_spec();
    }

    dynamic_library::symbol_type ctor = pdl->resolve(PFS_MODULE_CTOR_NAME, ec);

    if (!ctor) {
        print_error(0, fmt("%s: Failed to resolve `ctor' for module: %s")
                % (to_string<string_type>(dlpath))
                % (to_string<string_type>(ec)));

        return module_spec();
    }

    dynamic_library::symbol_type dtor = pdl->resolve(PFS_MODULE_DTOR_NAME, ec);

    if (!dtor) {
        print_error(0, fmt("%s: Failed to resolve `dtor' for module: %s")
                % (to_string<string_type>(dlpath))
                % (to_string<string_type>(ec)));

        return module_spec();
    }

    module_ctor_t module_ctor = pfs::void_func_ptr_cast<module_ctor_t>(ctor);
    module_dtor_t module_dtor = pfs::void_func_ptr_cast<module_dtor_t>(dtor);

    //module * ptr = reinterpret_cast<module *>(module_ctor(class_name, mod_data));
    module * ptr = module_ctor(class_name, mod_data);

    if (!ptr)
        return module_spec();

    module_spec result;
    result.pdl = pdl;
    result.pmodule = shared_ptr<module>(ptr, module_deleter(module_dtor));

    return result;
}

bool dispatcher::register_module (module_spec const & modspec)
{
    int nemitters, ndetectors;

    if (!modspec.pmodule)
        return false;

    shared_ptr<module> pmodule = modspec.pmodule;

    if (_module_spec_map.find(pmodule->name()) != _module_spec_map.end()) {
        print_error(0, fmt("%s: Module already registered") % (pmodule->name()));
        return false;
    }

    pmodule->set_dispatcher(this);

    pmodule->emit_quit.connect(this, & dispatcher::broadcast_quit);
    this->emit_quit.connect(pmodule.get(), & module::on_quit);

    pmodule->emit_info.connect(this, & dispatcher::print_info);
    pmodule->emit_debug.connect(this, & dispatcher::print_debug);
    pmodule->emit_warn.connect(this, & dispatcher::print_warn);
    pmodule->emit_error.connect(this, & dispatcher::print_error);

    if (!pmodule->on_loaded()) {
        print_error(pmodule.get(), "on_loaded stage failed");
        return false;
    }

    emitter_mapping const * emitters = pmodule->get_emitters(nemitters);
    detector_mapping const * detectors = pmodule->get_detectors(ndetectors);

    api_map::iterator it_end = _api.end();

    if (emitters) {
        for (int i = 0; i < nemitters; ++i) {
            int emitter_id(emitters[i]._id);
            api_map::iterator it = _api.find(emitter_id);

            if (it != it_end) {
                it->second->mapping->append_emitter(reinterpret_cast<emitter_t *> (emitters[i]._emitter));
            } else {
                print_warn(0, fmt("%s: Emitter '%s' not found while registering module, "
                        "may be signal/slot mapping is not supported for this application")
                        % (pmodule->name())
                        % (to_string<string_type>(emitters[i]._id)));
            }
        }
    }

    if (detectors) {
        for (int i = 0; i < ndetectors; ++i) {
            int detector_id(detectors[i]._id);
            api_map::iterator it = _api.find(detector_id);

            if (it != it_end) {
                it->second->mapping->append_detector(pmodule.get(), detectors[i]._detector);
            } else {
                print_warn(0, fmt("%s: Detector '%s' not found while registering module, "
                        "may be signal/slot mapping is not supported for this application")
                        % (pmodule->name())
                        % (to_string<string_type>(detectors[i]._id)));
            }
        }
    }

    pmodule->emit_module_registered.connect(this, & dispatcher::module_registered);
    _module_spec_map.insert(pmodule->name(), modspec);

    // Module must be run in a separate thread.
    //
    if (pmodule->run) {
        _runnable_modules.push_back(pmodule);
        print_debug(0, fmt("%s: registered as threaded") % (pmodule->name()));
    } else {
        print_debug(0, fmt("%s: registered") % (pmodule->name()));
    }

    return true;
}

void dispatcher::set_master_module (string_type const & name)
{
    module_spec_map::iterator it = _module_spec_map.begin();
    module_spec_map::iterator last = _module_spec_map.end();

    for (; it != last; ++it) {
        module_spec modspec = it->second;
        shared_ptr<module> pmodule = modspec.pmodule;

        if (pmodule->name() == name)
            _master_module_ptr = pmodule.get();
    }
}

void dispatcher::connect_all ()
{
    api_map::iterator it = _api.begin();
    api_map::iterator itEnd = _api.end();

    for (; it != itEnd; ++it) {
        it->second->mapping->connect_all();
    }
}

void dispatcher::disconnect_all ()
{
    api_map::iterator it = _api.begin();
    api_map::iterator itEnd = _api.end();

    for (; it != itEnd; ++it) {
        it->second->mapping->disconnect_all();
    }
}

void dispatcher::unregister_all ()
{
    _runnable_modules.clear();

    module_spec_map::iterator it = _module_spec_map.begin();
    module_spec_map::iterator last = _module_spec_map.end();

    for (; it != last; ++it) {
        module_spec modspec = it->second;
        shared_ptr<module> pmodule = modspec.pmodule;
        pmodule->emit_module_registered.disconnect(this);
        print_debug(0, fmt("%s: unregistered") % (pmodule->name()));
    }

    _module_spec_map.clear();
}

bool dispatcher::start ()
{
    bool r = true;

    typename module_spec_map::iterator it   = _module_spec_map.begin();
    typename module_spec_map::iterator last = _module_spec_map.end();

    for (; it != last; ++it) {
        module_spec modspec = it->second;
        shared_ptr<module> pmodule = modspec.pmodule;

        if (! pmodule->on_start()) {
            print_error(pmodule.get(), "Failed to start module");
            r = false;
        }
    }

    return r;
}

int dispatcher::exec ()
{
    int r = 0;

    thread_sequence thread_pool;

    typename runnable_sequence::iterator irunnable = _runnable_modules.begin();
    typename runnable_sequence::iterator irunnable_last = _runnable_modules.end();

    for (; irunnable != irunnable_last; ++irunnable) {
        // run module if it is not a master
        if (!_master_module_ptr
                || (_master_module_ptr && irunnable->get() != _master_module_ptr))
            thread_pool.push_back(pfs::make_shared<thread>((*irunnable)->run, irunnable->get()));
    }

    if (_master_module_ptr && _master_module_ptr->run) {
        r = _master_module_ptr->run(_master_module_ptr);
    }

    typename thread_sequence::iterator ithread = thread_pool.begin();
    typename thread_sequence::iterator ithread_last = thread_pool.end();

    for (; ithread != ithread_last; ++ithread) {
        (*ithread)->join();
    }

    module_spec_map::iterator imodule = _module_spec_map.begin();
    module_spec_map::iterator imodule_last = _module_spec_map.end();

    for (; imodule != imodule_last; ++imodule) {
        module_spec modspec = imodule->second;
        shared_ptr<module> pmodule = modspec.pmodule;
        pmodule->on_finish();

        pmodule->emit_info.disconnect(this);
        pmodule->emit_debug.disconnect(this);
        pmodule->emit_warn.disconnect(this);
        pmodule->emit_error.disconnect(this);
    }

    return r;
}

void module::connect_info (log_consumer * p)
{
    _pdispatcher->emit_info.connect(p, & log_consumer::_on_info);
}

void module::connect_debug (log_consumer * p)
{
    _pdispatcher->emit_debug.connect(p, & log_consumer::_on_debug);
}

void module::connect_warn (log_consumer * p)
{
    _pdispatcher->emit_warn.connect(p, & log_consumer::_on_warn);
}

void module::connect_error (log_consumer * p)
{
    _pdispatcher->emit_error.connect(p, & log_consumer::_on_error);
}

}} // pfs::application
