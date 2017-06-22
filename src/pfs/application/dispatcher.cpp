/*
 * dispacther.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */
#include <pfs/logger.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/io/device.hpp>
#include <pfs/io/file.hpp>
#include <pfs/time.hpp>
#include <pfs/date.hpp>
#include "pfs/dispatcher.hpp"

namespace pfs {

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


class module_threaded : public thread
{
public:
	module_threaded (shared_ptr<module> pmodule)
		: thread()
		, _pmodule(pmodule)
	{
		PFS_ASSERT(_pmodule);
	}

	virtual ~module_threaded ()
	{}

	const module * module_ptr () const
	{
		return _pmodule.get();
	}

protected:
	virtual void run ()
	{
		_pmodule->run(_pmodule.get());
	}

private:
	shared_ptr<module> _pmodule;
};

dispatcher::dispatcher (api_item_type * mapping, int n)
	: _master_module(0)
{
    activate_posix_signal_handling();
            
	for (int i = 0; i < n; ++i) {
		_api.insert(api_type::value_type(mapping[i].id, & mapping[i]));
	}
}

inline pfs::string __build_string_for_log (const module * m, const string & s)
{
	if (!m)
		return s;

	pfs::string r(m->name());
	r.append(": ");
	r.append(s);

	return r;
}

void dispatcher::print_info (const module * m, const string & s)
{
	pfs::log_info(__build_string_for_log(m, s));
	emit_info(m, s);
}

void dispatcher::print_debug (const module * m, const string & s)
{
	pfs::log_debug(__build_string_for_log(m, s));
	emit_debug(m, s);
}

void dispatcher::print_warn  (const module * m, const string & s)
{
	pfs::log_warn(__build_string_for_log(m, s));
	emit_warn(m, s);
}

void dispatcher::print_error (const module * m, const string & s)
{
	pfs::log_error(__build_string_for_log(m, s));
	emit_error(m, s);
}

void dispatcher::finalize ()
{
	if (_module_spec_map.size() > 0) {
		disconnect_all();
		unregister_all();
	}
}

module_spec dispatcher::module_for_path (const fs::path & path
			, const char * class_name
			, void * mod_data)
{
	dynamic_library dl;
	error_code ex;
	string extended_errstr;

	fs::path dlpath(path);

	if (path.is_relative()) {
		dlpath = fs::join(fs::path("."), path);
	}

	if (not dl.open(dlpath, _searchdirs, & ex, & extended_errstr)) {
		print_error(0, _Sf("%s: %s")(to_string(dlpath))(to_string(ex)).str());

		if (not extended_errstr.empty()) {
			print_error(0, _Sf("%s: %s")(to_string(dlpath))(extended_errstr).str());
		}

		return module_spec();
	}

	dynamic_library::symbol ctor = dl.resolve(PFS_MODULE_CTOR_NAME
			, & ex
			, & extended_errstr);

	if (!ctor) {
		print_error(0, _Sf("%s: Failed to resolve `ctor' for module")(to_string(dlpath)).str());
		print_error(0, _Sf("%s: %s")(to_string(dlpath))(to_string(ex)).str());

		if (not extended_errstr.empty()) {
			print_error(0, _Sf("%s: %s")(to_string(dlpath))(extended_errstr).str());
		}

		return module_spec();
	}

	dynamic_library::symbol dtor = dl.resolve(PFS_MODULE_DTOR_NAME
			, & ex
			, & extended_errstr);

	if (!dtor) {
		print_error(0, _Sf("%s: Failed to resolve `dtor' for module")(to_string(dlpath)).str());
		print_error(0, _Sf("%s: %s")(to_string(dlpath))(to_string(ex)).str());

		if (not extended_errstr.empty()) {
			print_error(0, _Sf("%s: %s")(to_string(dlpath))(extended_errstr).str());
		}

		return module_spec();
	}

	module_ctor_t module_ctor = reinterpret_cast<module_ctor_t>(ctor);
	module_dtor_t module_dtor = reinterpret_cast<module_dtor_t>(dtor);

	module * ptr = reinterpret_cast<module *>(module_ctor(class_name, mod_data));

	if (!ptr)
		return module_spec();

	module_spec result;
	result.dl = dl;
	result.pmodule = shared_ptr<module>(ptr, module_deleter(module_dtor));

	return result;
}

bool dispatcher::register_module (const module_spec & modspec)
{
	int nemitters, ndetectors;

	if (modspec.pmodule.is_null())
		return false;

	shared_ptr<module> pmodule = modspec.pmodule;

	if (_module_spec_map.find(pmodule->name()) != _module_spec_map.end()) {
		print_error(0, _Sf("%s: Module already registered")(pmodule->name()).str());
		return false;
	}

	pmodule->emit_quit.connect (this, & dispatcher::broadcast_quit);
	this->emit_quit.connect(pmodule.get(), & module::on_quit);

	pmodule->emit_info.connect (this, & dispatcher::print_info);
	pmodule->emit_debug.connect(this, & dispatcher::print_debug);
	pmodule->emit_warn.connect (this, & dispatcher::print_warn);
	pmodule->emit_error.connect(this, & dispatcher::print_error);

	const emitter_mapping * emitters = pmodule->get_emitters(& nemitters);
	const detector_mapping * detectors = pmodule->get_detectors(& ndetectors);

	api_type::iterator it_end = _api.end();

	if (emitters) {
		for (int i = 0; i < nemitters; ++i) {
			int emitter_id(emitters[i]._id);
			api_type::iterator it = _api.find(emitter_id);

			if (it != it_end) {
				it->second->map->append_emitter(reinterpret_cast<emitter *>(emitters[i]._emitter));
			} else {
				print_warn(0, _Sf("%s: Emitter '%s' not found while registering module, "
						"may be signal/slot mapping is not supported for this application")
						(pmodule->name())
						(to_string(emitters[i]._id)).str());
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; ++i) {
			int detector_id(detectors[i]._id);
			api_type::iterator it = _api.find(detector_id);

			if (it != it_end) {
				it->second->map->append_detector(pmodule.get(), detectors[i]._detector);
			} else {
				print_warn(0, _Sf(_u8("%s: Detector '%s' not found while registering module, "
						"may be signal/slot mapping is not supported for this application"))
						(pmodule->name())
						(to_string(detectors[i]._id)).str());
			}
		}
	}

	pmodule->emit_module_registered.connect(this, & dispatcher::module_registered);
	pmodule->set_dispatcher(this);

	_module_spec_map.insert(module_spec_map_type::value_type(pmodule->name(), modspec));

	// Module must be run in a separate thread.
	//
	if (pmodule->run) {
		_threads.push_back(new module_threaded(pmodule));
		print_debug(0, _Sf("%s: Module registered as threaded")(pmodule->name()).str());
	} else {
		print_debug(0, _Sf("%s: Module registered")(pmodule->name()).str());
	}

	return true;
}

void dispatcher::set_master_module (const string & name)
{
	module_spec_map_type::iterator it = _module_spec_map.begin();
	module_spec_map_type::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; ++it) {
		module_spec modspec = it->second;
		shared_ptr<module> pmodule = modspec.pmodule;

		if (pmodule->name() == name)
			_master_module = pmodule;
	}
}

void dispatcher::connect_all ()
{
	api_type::iterator it = _api.begin();
	api_type::iterator itEnd = _api.end();

	for (; it != itEnd; ++it) {
		it->second->map->connect_all();
	}
}

void dispatcher::disconnect_all ()
{
	api_type::iterator it = _api.begin();
	api_type::iterator itEnd = _api.end();

	for (; it != itEnd; ++it) {
		it->second->map->disconnect_all();
	}
}

void dispatcher::unregister_all ()
{
	vector<thread *>::iterator itThread = _threads.begin();
	vector<thread *>::iterator itThreadEnd = _threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		delete *itThread;
	}
	_threads.clear();

	module_spec_map_type::iterator it = _module_spec_map.begin();
	module_spec_map_type::iterator itEnd = _module_spec_map.end();

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

		print_debug(0, _Sf("%s: Module unregistered")(pmodule->name()).str());
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
			print_error(pmodule.get(), _u8("Failed to start module"));
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
		size_t size = _threads.size();

		for (size_t i = 0; i < size; ++i) {
			const thread * th = _threads[i];
			const module_threaded * pt = dynamic_cast<const module_threaded*>(th);

			if (pt->module_ptr() == _master_module.get()) {
				_threads.erase(_threads.begin() + i);
				delete pt;
				break;
			}
		}
	}

	vector<thread *>::iterator itThread = _threads.begin();
	vector<thread *>::iterator itThreadEnd = _threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		thread * t = *itThread;
		t->start();
	}

	if (_master_module && _master_module->run) {
		r = _master_module->run(_master_module.get());
	}

	for (itThread = _threads.begin(); itThread != itThreadEnd; ++itThread) {
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

bool dispatcher::register_modules (fs::path const & path)
{
    byte_string content;
    string errstr;

    if (!pfs::fs::exists(path)) {
    	print_error(0, _Sf("%s: File not found")(to_string(path)).str());
    	return false;
    }

    error_code ex;
    io::device dev = io::open_device(io::open_params<io::file>(path, io::read_only), ex);

    if (ex) {
    	errstr = _Sf("%s: Open file error: %s")
                (to_string(path))
                (to_string(ex)).str();
    	return false;
    }

    ex = dev.read(content, dev.available());

    if (ex) {
    	errstr = _Sf("%s: Read file error: %s")
                (to_string(path))
                (to_string(ex)).str();
    	return false;
    }

    pfs::json::json conf;
    bool rc = pfs::json::parse(conf, string(reinterpret_cast<const char *>(content.data())));
    
    if (not rc) {
    	print_error(0, _Sf("%s: File is not JSON")(to_string(path)).str());
    	return false;
    }

    return register_modules(conf);
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

bool dispatcher::register_modules (json::json const & conf)
{
    pfs::json::value disp = conf["dispatcher"];

    if (not disp.is_null()) {
    	if (not disp.is_object()) {
        	print_error(0, _u8("Dispatcher configuration error"));
    		return false;
    	}

    	pfs::json::value dlog = disp["log"];

    	if (dlog.is_object()) {

    		pfs::logger logger;
    		pfs::json::value::const_iterator it = dlog.cbegin();
    		pfs::json::value::const_iterator itEnd = dlog.cend();

    		for (; it != itEnd; ++it) {
    			pfs::string name = it.key();
    			pfs::json::value priority = *it;
    			pfs::stringlist priorities;
    			pfs::logger_appender * pappender = 0;

    			if (name == _u8("stdout")) {
    				pappender = & logger.add_appender<pfs::stdout_appender>();
    			} else if (name == _u8("stderr")) {
    				pappender = & logger.add_appender<pfs::stderr_appender>();
    			} else {
    				string pathStr = __build_log_filename(name);
    				fs::path path(pathStr);

    				error_code ex;
    				io::device d = io::open_device(io::open_params<io::file>(path, io::write_only), ex);

    				if (ex) {
    					print_error(0, _Sf("Failed to create/open log file: %s: %s")
    							(to_string(path))
    							(to_string(ex)).str());
    					return false;
    				}

    				pappender = & logger.add_appender<file_appender, io::device>(d);
    			}

    			PFS_ASSERT(pappender);

    			if (priority.is_string()) {
    				priorities.push_back(priority.get<pfs::string>());
    			} else if (priority.is_array()) {
    				for (size_t i = 0; i < priority.size(); ++i) {
    					priorities.push_back(priority[i].get<pfs::string>());
    				}
    			}

        		for (size_t i = 0; i < priorities.size(); ++i) {
        			if (priorities[i] == _u8("all")) {
        				logger.connect(*pappender);
        			} else if (priorities[i] == _u8("trace")) {
        				logger.connect(pfs::logger::trace_priority, *pappender);
        			} else if (priorities[i] == _u8("debug")) {
        				logger.connect(pfs::logger::debug_priority, *pappender);
        			} else if (priorities[i] == _u8("info")) {
        				logger.connect(pfs::logger::info_priority, *pappender);
        			} else if (priorities[i] == _u8("warn")) {
        				logger.connect(pfs::logger::warn_priority, *pappender);
        			} else if (priorities[i] == _u8("error")) {
        				logger.connect(pfs::logger::error_priority, *pappender);
        			} else if (priorities[i] == _u8("fatal")) {
        				logger.connect(pfs::logger::fatal_priority, *pappender);
        			} else {
    					print_error(0, _Sf("Invalid log level name (must be 'all', 'trace', 'debug', 'info', 'warn', 'error' or 'fatal'): '%s'")
    							(priorities[i]).str());
    					return false;
        			}
        		}
    		}

    		logger::default_logger().move(logger);
    	}
    }

    json::value modules = conf["modules"];
    json::value::iterator it = modules.begin();
    json::value::iterator it_end = modules.end();

    bool result = true;

    for (; it != it_end; ++it) {
    	if (it->is_object()) {
    		string name_str = (*it)["name"].get<string>();
    		string path_str = (*it)["path"].get<string>();
    		bool is_active       = (*it)["active"].get<bool>();
    		bool is_master       = (*it)["master-module"].get<bool>();

    		if (name_str.empty()) {
    	    	print_error(0, _u8("Found anonymous module"));
    	    	return false;
    		}

    		if (is_active) {
    			bool rc = false;

    			if (path_str.empty())
    				rc = register_module_for_name(name_str, 0, & *it);
    			else
    				rc = register_module_for_path(fs::path(path_str), name_str, 0, & *it);

    			if (rc) {
    				if (is_master) {
    					set_master_module(name_str);
    				}
    			} else {
    				result = false;
    			}
    		} else {
    			print_debug(0, _Sf("%s: Module is inactive")(name_str).str());
    		}
    	}
    }

    return result;    
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

} // pfs
