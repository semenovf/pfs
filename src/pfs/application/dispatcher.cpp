/*
 * dispacther.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

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


//class module_threaded
//{
//    shared_ptr<module> _pmodule;
//    
//public:
//	module_threaded (shared_ptr<module> pmodule)
//		: _pmodule(pmodule)
//	{
//		PFS_ASSERT(_pmodule);
//	}
//
//	virtual ~module_threaded ()
//	{}
//
//	module const * module_ptr () const
//	{
//		return _pmodule.get();
//	}
//
//protected:
//	virtual void run ()
//	{
//		_pmodule->run(_pmodule.get());
//	}
//};

dispatcher::dispatcher (api_item_type * mapping, int n)
	: _master_module(0)
{
    activate_posix_signal_handling();
            
	for (int i = 0; i < n; ++i) {
		_api.insert(api_map::value_type(mapping[i].id, & mapping[i]));
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

	if (! pdl->open(dlpath, _searchdirs, ec)) {
		print_error(0, fmt("%s: %s")
                (to_string<string_type>(dlpath))(to_string<string_type>(ec)).str());
		return module_spec();
	}

	dynamic_library::symbol_type ctor = pdl->resolve(PFS_MODULE_CTOR_NAME, ec);

	if (!ctor) {
		print_error(0, fmt("%s: Failed to resolve `ctor' for module: %s")
                (to_string<string_type>(dlpath))
                (to_string<string_type>(ec)).str());

		return module_spec();
	}

	dynamic_library::symbol_type dtor = pdl->resolve(PFS_MODULE_DTOR_NAME, ec);

	if (!dtor) {
		print_error(0, fmt("%s: Failed to resolve `dtor' for module: %s")
                (to_string<string_type>(dlpath))
                (to_string<string_type>(ec)).str());

		return module_spec();
	}

	module_ctor_t module_ctor = reinterpret_cast<module_ctor_t>(ctor);
	module_dtor_t module_dtor = reinterpret_cast<module_dtor_t>(dtor);

	module * ptr = reinterpret_cast<module *>(module_ctor(class_name, mod_data));

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
		print_error(0, fmt("%s: Module already registered")(pmodule->name()).str());
		return false;
	}

	pmodule->emit_quit.connect (this, & dispatcher::broadcast_quit);
	this->emit_quit.connect(pmodule.get(), & module::on_quit);

	pmodule->emit_info.connect (this, & dispatcher::print_info);
	pmodule->emit_debug.connect(this, & dispatcher::print_debug);
	pmodule->emit_warn.connect (this, & dispatcher::print_warn);
	pmodule->emit_error.connect(this, & dispatcher::print_error);

	emitter_mapping const * emitters = pmodule->get_emitters(nemitters);
	detector_mapping const * detectors = pmodule->get_detectors(ndetectors);

	api_map::iterator it_end = _api.end();

	if (emitters) {
		for (int i = 0; i < nemitters; ++i) {
			int emitter_id(emitters[i]._id);
			api_map::iterator it = _api.find(emitter_id);

			if (it != it_end) {
				it->second->map->append_emitter(reinterpret_cast<emitter *>(emitters[i]._emitter));
			} else {
				print_warn(0, fmt("%s: Emitter '%s' not found while registering module, "
						"may be signal/slot mapping is not supported for this application")
						(pmodule->name())
						(to_string<string_type>(emitters[i]._id)).str());
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; ++i) {
			int detector_id(detectors[i]._id);
			api_map::iterator it = _api.find(detector_id);

			if (it != it_end) {
				it->second->map->append_detector(pmodule.get(), detectors[i]._detector);
			} else {
				print_warn(0, fmt("%s: Detector '%s' not found while registering module, "
						"may be signal/slot mapping is not supported for this application")
						(pmodule->name())
						(to_string<string_type>(detectors[i]._id)).str());
			}
		}
	}

	pmodule->emit_module_registered.connect(this, & dispatcher::module_registered);
	pmodule->set_dispatcher(this);

	_module_spec_map.insert(module_spec_map::value_type(pmodule->name(), modspec));

	// Module must be run in a separate thread.
	//
	if (pmodule->run) {
		_runnable_modules.push_back(pmodule);
		print_debug(0, fmt("%s: Module registered as threaded")(pmodule->name()).str());
	} else {
		print_debug(0, fmt("%s: Module registered")(pmodule->name()).str());
	}

	return true;
}

void dispatcher::set_master_module (string_type const  & name)
{
	module_spec_map::iterator it = _module_spec_map.begin();
	module_spec_map::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; ++it) {
		module_spec modspec = it->second;
		shared_ptr<module> pmodule = modspec.pmodule;

		if (pmodule->name() == name)
			_master_module = pmodule;
	}
}

void dispatcher::connect_all ()
{
	api_map::iterator it = _api.begin();
	api_map::iterator itEnd = _api.end();

	for (; it != itEnd; ++it) {
		it->second->map->connect_all();
	}
}

void dispatcher::disconnect_all ()
{
	api_map::iterator it = _api.begin();
	api_map::iterator itEnd = _api.end();

	for (; it != itEnd; ++it) {
		it->second->map->disconnect_all();
	}
}

void dispatcher::unregister_all ()
{
	runnable_sequence::iterator itThread    = _runnable_modules.begin();
	runnable_sequence::iterator itThreadEnd = _runnable_modules.end();

	for (; itThread != itThreadEnd; ++itThread) {
		delete *itThread;
	}
	_runnable_modules.clear();

	module_spec_map::iterator it = _module_spec_map.begin();
	module_spec_map::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; ++it) {
		module_spec modspec = it->second;
		shared_ptr<module> pmodule = modspec.pmodule;
		pmodule->emit_module_registered.disconnect(this);

//		if (pspec.dtor) {
//			pspec.dtor(mod);
//		}
//		if (pspec.dl) {
//			pfs::dynamic_library & dl = pfs::dynamic_library::getDL();
//			dl.close(pspec.dl);
//		}

		print_debug(0, fmt("%s: Module unregistered")(pmodule->name()).str());
	}

	_module_spec_map.clear();
}

bool dispatcher::start ()
{
	bool r = true;

	module_spec_map_type::iterator it = _module_spec_map.begin();
	module_spec_map_type::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; ++it) {
		module_spec modspec = it->second;
		shared_ptr<module> pmodule = modspec.pmodule;

		if (not pmodule->on_start()) {
			print_error(pmodule.get(), "Failed to start module");
			r = false;
		}
	}

	return r;
}

int dispatcher::exec ()
{
	int r = 0;

	// Exclude master module from dispatcher's threads pool
	//
	if (_master_module) {
		size_t size = _runnable_modules.size();

		for (size_t i = 0; i < size; ++i) {
			const thread * th = _runnable_modules[i];
			const module_threaded * pt = dynamic_cast<const module_threaded*>(th);

			if (pt->module_ptr() == _master_module.get()) {
				_runnable_modules.erase(_runnable_modules.begin() + i);
				delete pt;
				break;
			}
		}
	}

	vector<thread *>::iterator itThread = _runnable_modules.begin();
	vector<thread *>::iterator itThreadEnd = _runnable_modules.end();

	for (; itThread != itThreadEnd; ++itThread) {
		thread * t = *itThread;
		t->start();
	}

	if (_master_module && _master_module->run) {
		r = _master_module->run(_master_module.get());
	}

	for (itThread = _runnable_modules.begin(); itThread != itThreadEnd; ++itThread) {
		thread * t = *itThread;
		t->wait();
	}

	module_spec_map_type::iterator itModule = _module_spec_map.begin();
	module_spec_map_type::iterator itModuleEnd = _module_spec_map.end();

	for (; itModule != itModuleEnd; ++itModule) {
		module_spec modspec = itModule->second;
		shared_ptr<module> pmodule = modspec.pmodule;
		pmodule->on_finish();

		pmodule->emit_info.disconnect(this);
		pmodule->emit_debug.disconnect(this);
		pmodule->emit_warn.disconnect(this);
		pmodule->emit_error.disconnect(this);
	}

	return r;
}

static string __build_log_filename (string const & pattern)
{
	string r(pattern);

	pfs::time ct = current_time();
	pfs::date cd = current_date();

	r = pfs::to_string(cd, r);
	r = pfs::to_string(ct, r);

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
