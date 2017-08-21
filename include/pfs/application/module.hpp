/*
 * module.hpp
 *
 *  Created on: Feb 11, 2011
 *      Author: wladt
 */

#ifndef __PFS_MODULE_HPP__
#define __PFS_MODULE_HPP__

#include <pfs/compiler.hpp>
#include <pfs/system_string.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/atomic.hpp>

#define PFS_DETECTOR_CAST(slot) reinterpret_cast<pfs::application::detector>(& slot)
#define PFS_EMITTER_CAST(e)     reinterpret_cast<void *>(& e)
#define PFS_MODULE_API extern "C" PFS_DLL_API
#define PFS_MODULE_CTOR_NAME "__module_ctor__"
#define PFS_MODULE_DTOR_NAME "__module_dtor__"

namespace pfs {
namespace application {

class dispatcher;
class module;
typedef signal1<void *> emitter;
typedef void (module::* detector)(void *);
typedef struct { int _id; void * _emitter; } emitter_mapping;
typedef struct { int _id; detector _detector; } detector_mapping;

typedef module * (* module_ctor_t)(char const * name, void *);
typedef void  (* module_dtor_t)(module *);

class module : public has_slots<>
{
	friend class dispatcher;

public:
    typedef system_string string_type;
    
	struct log_consumer : public has_slots<>
	{
		friend class module;
		virtual void on_info  (module const *, string_type const &) = 0; // {}
		virtual void on_debug (module const *, string_type const &) = 0; // {}
		virtual void on_warn  (module const *, string_type const &) = 0; // {}
		virtual void on_error (module const *, string_type const &) = 0; // {}

	private:
		void _on_info  (module const * m, string_type const & s)
		{
			this->on_info(m, s);
		}

		void _on_debug (module const * m, string_type const & s)
		{
			this->on_debug(m, s);
		}

		void _on_warn  (module const * m, string_type const & s)
		{
			this->on_warn(m, s);
		}

		void _on_error (module const * m, string_type const & s)
		{
			this->on_error(m, s);
		}
	};

public: // signals
	signal2<string_type const &, bool &> emit_module_registered;

	signal0<>                                    emit_quit;
	signal2<module const *, string_type const &> emit_info;
	signal2<module const *, string_type const &> emit_debug;
	signal2<module const *, string_type const &> emit_warn;
	signal2<module const *, string_type const &> emit_error;
    
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

	void connect_info  (log_consumer * p);
	void connect_debug (log_consumer * p);
	void connect_warn  (log_consumer * p);
	void connect_error (log_consumer * p);

	virtual emitter_mapping const * get_emitters (int & count)
	{
		count = 0;
		return 0;
	}

	virtual detector_mapping const * get_detectors (int & count)
	{
		count = 0;
		return 0;
	}

	bool is_quit () const
	{
		return _quitfl == 0 ? false : true;
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
};

struct detector_pair
{
	detector_pair () : _module(NULL), _detector(NULL) {}
	detector_pair (module * p, detector d) : _module(p), _detector(d) {}
	module * _module;
	detector _detector;
};

}} // pfs::application

#define PFS_MODULE_EMITTER(id, em) { id , PFS_EMITTER_CAST(em) }
#define PFS_MODULE_DETECTOR(id, dt) { id , PFS_DETECTOR_CAST(dt) }

#define PFS_MODULE_EMITTERS_BEGIN                                       \
const pfs::application::emitter_mapping * get_emitters (int & count)    \
{                                                                       \
	static pfs::application::emitter_mapping __emitter_mapping[] = {

#define PFS_MODULE_EMITTERS_END                                         \
	};                                                                  \
	count = sizeof(__emitter_mapping)/sizeof(__emitter_mapping[0]) ;    \
	return & __emitter_mapping[0];                                      \
}

#define PFS_MODULE_DETECTORS_BEGIN                                      \
const pfs::application::detector_mapping * get_detectors (int & count)  \
{                                                                       \
	static pfs::application::detector_mapping __detector_mapping[] = {

#define PFS_MODULE_DETECTORS_END		                                \
	};                                                                  \
	count = sizeof(__detector_mapping)/sizeof(__detector_mapping[0]);   \
	return & __detector_mapping[0];                                     \
}

#endif /* __PFS_MODULE_HPP__ */
