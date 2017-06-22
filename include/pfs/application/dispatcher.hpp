/*
 * dispatcher.hpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on Feb 12, 2013
 *  Renamed to dispatcher Aug 25, 2015
 */

#ifndef __PFS_APPLICATION_DISPATCHER_HPP__
#define __PFS_APPLICATION_DISPATCHER_HPP__

#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/mutex.hpp>
#include <pfs/thread.hpp>
#include <pfs/dl.hpp>
#include <pfs/memory.hpp>
#include <pfs/sigslotmapping.hpp>
#include <pfs/utility.hpp>
#include <pfs/json/json.hpp>

namespace pfs {

struct module_spec
{
	shared_ptr<module> pmodule;
	dynamic_library dl;

	~module_spec ()
	{
		// Need to destroy pmodule before dynamic library will be destroyed automatically
		//
		pmodule.reset();
	}
};

class dispatcher : public has_slots<>
{
public:
	typedef struct api_item_type_t
	{
		int id;
		sigslot_mapping_t * map;
		string desc;
	} api_item_type;

	typedef map<int, api_item_type *> api_type;
	typedef map<string, module_spec> module_spec_map_type;

private:
	filesystem::pathlist _searchdirs;
	api_type             _api;
	module_spec_map_type _module_spec_map;
	vector<thread *>     _threads;
	shared_ptr<module>   _master_module;

private:
#if __cplusplus >= 201103L
	dispatcher (dispatcher const &) = delete;
	dispatcher & operator = (dispatcher const &) = delete;
#else    
	dispatcher (dispatcher const &);
	dispatcher & operator = (dispatcher const &);
#endif

protected:
    dispatcher ()
		: _master_module(0)
	{}
        
    bool activate_posix_signal_handling ();
    void deactivate_posix_signal_handling ();

public:
	dispatcher (api_item_type * mapping, int n);

	virtual ~dispatcher ()
	{
        deactivate_posix_signal_handling();
		finalize();
	}

	void finalize ();

	void add_search_path (filesystem::path const & dir)
	{
		if (!dir.empty())
			_searchdirs.push_back(dir);
	}

    /**
     * @brief Register modules enumerated in configuration file (JSON) specified by @a path.
     * 
     * @param path Configuration file path (JSON format).
     * @return @c true if modules registered successfully. @c false otherwise.
     */
    bool register_modules (filesystem::path const & path);
    
    /**
     * @brief Register modules enumerated in JSON instance specified by @a conf.
     * 
     * @param conf JSON instance contained modules for registration.
     * @return @c true if modules registered successfully. @c false otherwise.
     */
    bool register_modules (json::json const & conf);

	bool register_local_module (module * pmodule, string const & name)
	{
		module_spec modspec;
		modspec.pmodule = shared_ptr<module>(pmodule);
		modspec.dl = dynamic_library();
		modspec.pmodule->set_name(name);
		return register_module(modspec);
	}

	bool register_module_for_path (filesystem::path const & path
			, string const & name
			, const char * class_name = 0
			, void * mod_data = 0)
	{
		module_spec modspec = module_for_path(path, class_name, mod_data);
		modspec.pmodule->set_name(name);
		return register_module(modspec);
	}

	bool register_module_for_name (const string & name
			, const char * class_name = 0
			, void * mod_data = 0)
	{
		module_spec modspec = module_for_name(name, class_name, mod_data);
		if (modspec.pmodule) {
			modspec.pmodule->set_name(name);
			return register_module(modspec);
		}
		return false;
	}

	void set_master_module (const string & name);

	size_t count () const
	{
		return _module_spec_map.size();
	}

	void connect_all ();
	void disconnect_all ();
	void unregister_all ();
	bool start ();
	int  exec ();

	bool is_module_registered (string const & modname) const
	{
		return _module_spec_map.find(modname) != _module_spec_map.end();
	}

public: // signals
	signal0<> emit_quit;
	signal2<module const *, string const &> emit_info;
	signal2<module const *, string const &> emit_debug;
	signal2<module const *, string const &> emit_warn;
	signal2<module const *, string const &> emit_error;

public: // slots
	void module_registered (string const & pname, bool & result)
	{
		result = is_module_registered(pname);
	}

	void broadcast_quit ()
	{
		emit_quit();
	}

	void print_info  (module const * m, string const & s);
	void print_debug (module const * m, string const & s);
	void print_warn  (module const * m, string const & s);
	void print_error (module const * m, string const & s);

protected:
	module_spec module_for_path (filesystem::path const & path
			, const char * class_name = 0
			, void * mod_data = 0);

	module_spec module_for_name (string const & name
			, char const * class_name = 0
			, void * mod_data = 0)
	{
		fs::path modpath = dynamic_library::build_filename(name);
		return module_for_path(modpath, class_name, mod_data);
	}

	bool register_module (module_spec const & modspec);
};

} // pfs

#endif /* __PFS_APPLICATION_DISPATCHER_HPP__ */
