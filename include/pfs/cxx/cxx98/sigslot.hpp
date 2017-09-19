// sigslot.h: Signal/Slot classes
//
// Written by Sarah Thompson (sarah@telergy.com) 2002.
//
// License: Public domain. You are free to use this code however you like, with the proviso that
//          the author takes on no responsibility or liability for any use.
//
// QUICK DOCUMENTATION
//
//				(see also the full documentation at http://sigslot.sourceforge.net/)
//
//		#define switches
//			SIGSLOT_PURE_ISO			- Define this to force ISO C++ compliance. This also disables
//										  all of the thread safety support on platforms where it is
//										  available.
//
//			SIGSLOT_USE_POSIX_THREADS	- Force use of Posix threads when using a C++ compiler other than
//										  gcc on a platform that supports Posix threads. (When using gcc,
//										  this is the default - use SIGSLOT_PURE_ISO to disable this if
//										  necessary)
//
//			PFS_DEFAULT_MT_POLICY	    - Where thread support is enabled, this defaults to multi_threaded_global.
//										  Otherwise, the default is single_threaded. #define this yourself to
//										  override the default. In pure ISO mode, anything other than
//										  single_threaded will cause a compiler error.
//
//		PLATFORM NOTES
//
//			Win32						- On Win32, the WIN32 symbol must be #defined. Most mainstream
//										  compilers do this by default, but you may need to define it
//										  yourself if your build environment is less standard. This causes
//										  the Win32 thread support to be compiled in and used automatically.
//
//			Unix/Linux/BSD, etc.		- If you're using gcc, it is assumed that you have Posix threads
//										  available, so they are used automatically. You can override this
//										  (as under Windows) with the SIGSLOT_PURE_ISO switch. If you're using
//										  something other than gcc but still want to use Posix threads, you
//										  need to #define SIGSLOT_USE_POSIX_THREADS.
//
//			ISO C++						- If none of the supported platforms are detected, or if
//										  SIGSLOT_PURE_ISO is defined, all multithreading support is turned off,
//										  along with any code that might cause a pure ISO C++ environment to
//										  complain. Before you ask, gcc -ansi -pedantic won't compile this
//										  library, but gcc -ansi is fine. Pedantic mode seems to throw a lot of
//										  errors that aren't really there. If you feel like investigating this,
//										  please contact the author.
//
//
//		THREADING MODES
//
//			single_threaded				- Your program is assumed to be single threaded from the point of view
//										  of signal/slot usage (i.e. all objects using signals and slots are
//										  created and destroyed from a single thread). Behaviour if objects are
//										  destroyed concurrently is undefined (i.e. you'll get the occasional
//										  segmentation fault/memory exception).
//
//			multi_threaded_global		- Your program is assumed to be multi threaded. Objects using signals and
//										  slots can be safely created and destroyed from any thread, even when
//										  connections exist. In multi_threaded_global mode, this is achieved by a
//										  single global mutex (actually a critical section on Windows because they
//										  are faster). This option uses less OS resources, but results in more
//										  opportunities for contention, possibly resulting in more context switches
//										  than are strictly necessary.
//
//			multi_threaded_local		- Behaviour in this mode is essentially the same as multi_threaded_global,
//										  except that each signal, and each object that inherits has_slots, all
//										  have their own mutex/critical section. In practice, this means that
//										  mutex collisions (and hence context switches) only happen if they are
//										  absolutely essential. However, on some platforms, creating a lot of
//										  mutexes can slow down the whole OS, so use this option with care.
//
//		USING THE LIBRARY
//
//			See the full documentation at http://sigslot.sourceforge.net/
//
//

//
// !!! Do not name methods with same name as for signals and regular methods
//

#ifndef __PFS_CXX98_SIGSLOT_HPP__
#define __PFS_CXX98_SIGSLOT_HPP__

#include <set>
#include <list>
#include <pfs/mutex.hpp>

#ifndef PFS_DEFAULT_MT_POLICY
#	define PFS_DEFAULT_MT_POLICY pfs::mutex
#endif

namespace pfs {

/*
	template <class Mutex>
	class lock_guard
	{
	public:
		Mutex *_mutex;

		lock_guard(Mutex *mtx)
			: _mutex(mtx)
		{
			_mutex->lock();
		}

		~lock_guard()
		{
			_mutex->unlock();
		}
	};
*/

template <class Mutex>
class has_slots;

template <class Mutex>
class _connection_base0
{
public:
    virtual ~_connection_base0() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_() = 0;
    virtual _connection_base0* clone() = 0;
    virtual _connection_base0* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class arg1_type
        , class Mutex>
class _connection_base1
{
public:
    virtual ~_connection_base1() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_(arg1_type) = 0;
    virtual _connection_base1<arg1_type, Mutex>* clone() = 0;
    virtual _connection_base1<arg1_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class arg1_type
        , class arg2_type
        , class Mutex>
class _connection_base2
{
public:
    virtual ~_connection_base2() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_(arg1_type, arg2_type) = 0;
    virtual _connection_base2<arg1_type, arg2_type, Mutex>* clone() = 0;
    virtual _connection_base2<arg1_type, arg2_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class Mutex>
class _connection_base3
{
public:
    virtual ~_connection_base3() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_(arg1_type, arg2_type, arg3_type) = 0;
    virtual _connection_base3<arg1_type, arg2_type, arg3_type, Mutex>* clone() = 0;
    virtual _connection_base3<arg1_type, arg2_type, arg3_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class Mutex>
class _connection_base4
{
public:
    virtual ~_connection_base4() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_(arg1_type, arg2_type, arg3_type, arg4_type) = 0;
    virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, Mutex>* clone() = 0;
    virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class arg5_type
        , class Mutex>
class _connection_base5
{
public:
    virtual ~_connection_base5() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_(arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type) = 0;
    virtual _connection_base5<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, Mutex>* clone() = 0;
    virtual _connection_base5<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class arg5_type
        , class arg6_type
        , class Mutex>
class _connection_base6
{
public:
    virtual ~_connection_base6() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
        arg6_type) = 0;
    virtual _connection_base6<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, Mutex>* clone() = 0;
    virtual _connection_base6<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class arg5_type
        , class arg6_type
        , class arg7_type
        , class Mutex>
class _connection_base7
{
public:
    virtual ~_connection_base7() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
        arg6_type, arg7_type) = 0;
    virtual _connection_base7<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, Mutex>* clone() = 0;
    virtual _connection_base7<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class arg5_type
        , class arg6_type
        , class arg7_type
        , class arg8_type
        , class Mutex>
class _connection_base8
{
public:
    virtual ~_connection_base8() {}
    virtual has_slots<Mutex>* getdest() const = 0;
    virtual void emit_(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
        arg6_type, arg7_type, arg8_type) = 0;
    virtual _connection_base8<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, arg8_type, Mutex>* clone() = 0;
    virtual _connection_base8<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, arg8_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest) = 0;
};

template <class Mutex>
class _signal_base : public Mutex
{
public:
    virtual ~_signal_base() {}
    virtual void slot_disconnect(has_slots<Mutex>* pslot) = 0;
    virtual void slot_duplicate(const has_slots<Mutex>* poldslot, has_slots<Mutex>* pnewslot) = 0;
};

template <class Mutex = PFS_DEFAULT_MT_POLICY>
class has_slots : public Mutex
{
private:
    typedef std::set<_signal_base<Mutex> *> sender_set;
    typedef typename sender_set::const_iterator const_iterator;

public:
    has_slots()
    {}

    has_slots (const has_slots & hs)
        : Mutex(hs)
    {
        lock_guard<Mutex> lock(*this);
        const_iterator it = hs._senders.begin();
        const_iterator itEnd = hs._senders.end();

        while(it != itEnd) {
            (*it)->slot_duplicate(&hs, this);
            _senders.insert(*it);
            ++it;
        }
    }

    void signal_connect(_signal_base<Mutex>* sender)
    {
        lock_guard<Mutex> lock(*this);
        _senders.insert(sender);
    }

    void signal_disconnect(_signal_base<Mutex>* sender)
    {
        lock_guard<Mutex> lock(*this);
        _senders.erase(sender);
    }

    virtual ~has_slots()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        const_iterator it = _senders.begin();
        const_iterator itEnd = _senders.end();

        while(it != itEnd) {
            (*it)->slot_disconnect(this);
            ++it;
        }

        _senders.erase(_senders.begin(), _senders.end());
    }

private:

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
//#ifdef PFS_CC_MSVC
//#	pragma warning(push)
//#	pragma warning(disable:4251)
//#endif

    sender_set _senders;

//#ifdef PFS_CC_MSVC
//#	pragma warning(pop)
//#endif
};

template <class Mutex>
class _signal_base0 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base0<Mutex> *>  connections_list;

    _signal_base0()
    {}

    _signal_base0(const _signal_base0& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while(it != itEnd)
        {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    ~_signal_base0()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect(has_slots<Mutex>* pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot) {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    void slot_duplicate(const has_slots<Mutex>* oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == oldtarget) {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};

template <class arg1_type, class Mutex>
class _signal_base1 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base1<arg1_type, Mutex> *>  connections_list;

    _signal_base1()
    {}

    _signal_base1(const _signal_base1<arg1_type, Mutex>& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    void slot_duplicate(const has_slots<Mutex>* oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == oldtarget) {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    ~_signal_base1()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect(has_slots<Mutex>* pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot) {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};

template <class arg1_type
        , class arg2_type
        , class Mutex>
class _signal_base2 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base2<arg1_type, arg2_type, Mutex> *>
        connections_list;

    _signal_base2()
    {}

    _signal_base2(const _signal_base2<arg1_type, arg2_type, Mutex>& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    void slot_duplicate (const has_slots<Mutex> * oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == oldtarget) {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    ~_signal_base2()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect (has_slots<Mutex> * pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot) {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class Mutex>
class _signal_base3 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base3<arg1_type, arg2_type, arg3_type, Mutex> *>
        connections_list;

    _signal_base3()
    {}

    _signal_base3(const _signal_base3<arg1_type, arg2_type, arg3_type, Mutex>& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    void slot_duplicate(const has_slots<Mutex>* oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == oldtarget) {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    ~_signal_base3()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect(has_slots<Mutex>* pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot) {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class Mutex>
class _signal_base4 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base4<arg1_type, arg2_type, arg3_type,
        arg4_type, Mutex> *>  connections_list;

    _signal_base4()
    {}

    _signal_base4 (const _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, Mutex>& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    void slot_duplicate(const has_slots<Mutex>* oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == oldtarget) {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    ~_signal_base4()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect(has_slots<Mutex>* pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot) {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class arg5_type
        , class Mutex>
class _signal_base5 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base5<arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, Mutex> *>  connections_list;

    _signal_base5()
    {}

    _signal_base5(const _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, Mutex>& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    void slot_duplicate(const has_slots<Mutex>* oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == oldtarget) {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    ~_signal_base5()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect(has_slots<Mutex>* pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot) {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type, class arg6_type, class Mutex>
class _signal_base6 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base6<arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type, Mutex> *>  connections_list;

    _signal_base6()
    {}

    _signal_base6(const _signal_base6<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, Mutex>& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while (it != itEnd) {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    void slot_duplicate(const has_slots<Mutex>* oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while (it != itEnd) {
            if((*it)->getdest() == oldtarget) {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    ~_signal_base6()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect(has_slots<Mutex>* pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot) {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type, class arg6_type, class arg7_type, class Mutex>
class _signal_base7 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base7<arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type, arg7_type, Mutex> *>  connections_list;

    _signal_base7()
    {}

    _signal_base7(const _signal_base7<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, Mutex>& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    void slot_duplicate(const has_slots<Mutex>* oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == oldtarget) {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    ~_signal_base7()
    {
        disconnect_all();
    }

    void disconnect_all ()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect(has_slots<Mutex>* pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot) {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type, class arg6_type, class arg7_type, class arg8_type, class Mutex>
class _signal_base8 : public _signal_base<Mutex>
{
public:
    typedef std::list<_connection_base8<arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, Mutex> *>
        connections_list;

    _signal_base8()
    {}

    _signal_base8(const _signal_base8<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, arg8_type, Mutex>& s)
        : _signal_base<Mutex>(s)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = s._connected_slots.begin();
        typename connections_list::const_iterator itEnd = s._connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_connect(this);
            _connected_slots.push_back((*it)->clone());

            ++it;
        }
    }

    void slot_duplicate(const has_slots<Mutex>* oldtarget, has_slots<Mutex>* newtarget)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd)
        {
            if((*it)->getdest() == oldtarget)
            {
                _connected_slots.push_back((*it)->duplicate(newtarget));
            }

            ++it;
        }
    }

    ~_signal_base8()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::const_iterator it = _connected_slots.begin();
        typename connections_list::const_iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            (*it)->getdest()->signal_disconnect(this);
            delete *it;

            ++it;
        }

        _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
    }

    void disconnect(has_slots<Mutex>* pclass)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd) {
            if((*it)->getdest() == pclass) {
                delete *it;
                _connected_slots.erase(it);
                pclass->signal_disconnect(this);
                return;
            }

            ++it;
        }
    }

    void slot_disconnect(has_slots<Mutex>* pslot)
    {
        lock_guard<Mutex> lock(*this);
        typename connections_list::iterator it = _connected_slots.begin();
        typename connections_list::iterator itEnd = _connected_slots.end();

        while(it != itEnd)
        {
            typename connections_list::iterator itNext = it;
            ++itNext;

            if((*it)->getdest() == pslot)
            {
                _connected_slots.erase(it);
                //			delete *it;
            }

            it = itNext;
        }
    }

    bool isConnected() const { return _connected_slots.size() > 0; }

protected:
    connections_list _connected_slots;
};


template <class dest_type, class Mutex>
class _connection0 : public _connection_base0<Mutex>
{
public:
    _connection0()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection0(dest_type* pobject, void (dest_type::*pmemfun)())
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base0<Mutex>* clone()
    {
        return new _connection0<dest_type, Mutex>(*this);
    }

    virtual _connection_base0<Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection0<dest_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_()
    {
        (_pobject->*_pmemfun)();
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)();
};

template <class dest_type, class arg1_type, class Mutex>
class _connection1 : public _connection_base1<arg1_type, Mutex>
{
public:
    _connection1()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection1(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base1<arg1_type, Mutex>* clone()
    {
        return new _connection1<dest_type, arg1_type, Mutex>(*this);
    }

    virtual _connection_base1<arg1_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection1<dest_type, arg1_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(arg1_type a1)
    {
        (_pobject->*_pmemfun)(a1);
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(arg1_type);
};

template <class dest_type, class arg1_type, class arg2_type, class Mutex>
class _connection2 : public _connection_base2<arg1_type, arg2_type, Mutex>
{
public:
    _connection2()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection2(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
        arg2_type))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    //~_connection2() {}

    virtual _connection_base2<arg1_type, arg2_type, Mutex>* clone()
    {
        return new _connection2<dest_type, arg1_type, arg2_type, Mutex>(*this);
    }

    virtual _connection_base2<arg1_type, arg2_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection2<dest_type, arg1_type, arg2_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(arg1_type a1, arg2_type a2)
    {
        (_pobject->*_pmemfun)(a1, a2);
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(arg1_type, arg2_type);
};

template <class dest_type, class arg1_type, class arg2_type, class arg3_type, class Mutex>
class _connection3 : public _connection_base3<arg1_type, arg2_type, arg3_type, Mutex>
{
public:
    _connection3()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection3(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
        arg2_type, arg3_type))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base3<arg1_type, arg2_type, arg3_type, Mutex>* clone()
    {
        return new _connection3<dest_type, arg1_type, arg2_type, arg3_type, Mutex>(*this);
    }

    virtual _connection_base3<arg1_type, arg2_type, arg3_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection3<dest_type, arg1_type, arg2_type, arg3_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(arg1_type a1, arg2_type a2, arg3_type a3)
    {
        (_pobject->*_pmemfun)(a1, a2, a3);
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(arg1_type, arg2_type, arg3_type);
};

template <class dest_type, class arg1_type, class arg2_type, class arg3_type,
class arg4_type, class Mutex>
class _connection4 : public _connection_base4<arg1_type, arg2_type,
    arg3_type, arg4_type, Mutex>
{
public:
    _connection4()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection4(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, Mutex>* clone()
    {
        return new _connection4<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, Mutex>(*this);
    }

    virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection4<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(arg1_type a1, arg2_type a2, arg3_type a3,
        arg4_type a4)
    {
        (_pobject->*_pmemfun)(a1, a2, a3, a4);
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(arg1_type, arg2_type, arg3_type,
        arg4_type);
};

template <class dest_type, class arg1_type, class arg2_type, class arg3_type,
class arg4_type, class arg5_type, class Mutex>
class _connection5 : public _connection_base5<arg1_type, arg2_type,
    arg3_type, arg4_type, arg5_type, Mutex>
{
public:
    _connection5()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection5(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type, arg5_type))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base5<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, Mutex>* clone()
    {
        return new _connection5<dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, Mutex>(*this);
    }

    virtual _connection_base5<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection5<dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5)
    {
        (_pobject->*_pmemfun)(a1, a2, a3, a4, a5);
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type);
};

template <class dest_type, class arg1_type, class arg2_type, class arg3_type,
class arg4_type, class arg5_type, class arg6_type, class Mutex>
class _connection6 : public _connection_base6<arg1_type, arg2_type,
    arg3_type, arg4_type, arg5_type, arg6_type, Mutex>
{
public:
    _connection6()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection6(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type, arg5_type, arg6_type))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base6<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, Mutex>* clone()
    {
        return new _connection6<dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, Mutex>(*this);
    }

    virtual _connection_base6<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection6<dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6)
    {
        (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6);
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type);
};

template <class dest_type, class arg1_type, class arg2_type, class arg3_type,
class arg4_type, class arg5_type, class arg6_type, class arg7_type, class Mutex>
class _connection7 : public _connection_base7<arg1_type, arg2_type,
    arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, Mutex>
{
public:
    _connection7()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection7(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base7<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, Mutex>* clone()
    {
        return new _connection7<dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, arg7_type, Mutex>(*this);
    }

    virtual _connection_base7<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection7<dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, arg7_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6, arg7_type a7)
    {
        (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7);
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type);
};

template <class dest_type, class arg1_type, class arg2_type, class arg3_type,
class arg4_type, class arg5_type, class arg6_type, class arg7_type,
class arg8_type, class Mutex>
class _connection8 : public _connection_base8<arg1_type, arg2_type,
    arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, Mutex>
{
public:
    _connection8()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection8(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
        arg7_type, arg8_type))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base8<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, arg8_type, Mutex>* clone()
    {
        return new _connection8<dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, arg7_type, arg8_type, Mutex>(*this);
    }

    virtual _connection_base8<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, arg8_type, Mutex>* duplicate(has_slots<Mutex>* pnewdest)
    {
        return new _connection8<dest_type, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, arg7_type, arg8_type, Mutex>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8)
    {
        (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7, a8);
    }

    virtual has_slots<Mutex>* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, arg8_type);
};

template <class Mutex = PFS_DEFAULT_MT_POLICY>
class signal0 : public _signal_base0<Mutex>
{
    typedef _signal_base0<Mutex> __base_class;

public:
    signal0()
    {}

    signal0(const signal0<Mutex>& s)
        : _signal_base0<Mutex>(s)
    {}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)())
    {
        lock_guard<Mutex> lock(*this);
        _connection0<desttype, Mutex>* conn =
            new _connection0<desttype, Mutex>(pclass, pmemfun);
        this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_()
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_();

            it = itNext;
        }
    }

    void operator()()
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_();

            it = itNext;
        }
    }
};

template <class arg1_type, class Mutex = PFS_DEFAULT_MT_POLICY>
class signal1 : public _signal_base1<arg1_type, Mutex>
{
    typedef _signal_base1<arg1_type, Mutex> __base_class;

public:
    signal1()
    {}

    signal1(const signal1<arg1_type, Mutex>& s)
        : _signal_base1<arg1_type, Mutex>(s)
    {}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type))
    {
        lock_guard<Mutex> lock(*this);
        _connection1<desttype, arg1_type, Mutex>* conn =
            new _connection1<desttype, arg1_type, Mutex>(pclass, pmemfun);
        this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(arg1_type a1)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1);

            it = itNext;
        }
    }

    void operator()(arg1_type a1)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1);

            it = itNext;
        }
    }
};

template <class arg1_type, class arg2_type, class Mutex = PFS_DEFAULT_MT_POLICY>
class signal2 : public _signal_base2<arg1_type, arg2_type, Mutex>
{
    typedef _signal_base2<arg1_type, arg2_type, Mutex> __base_class;
public:
    signal2()
    {}

    signal2(const signal2<arg1_type, arg2_type, Mutex>& s)
        : _signal_base2<arg1_type, arg2_type, Mutex>(s)
    {}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
        arg2_type))
    {
        lock_guard<Mutex> lock(*this);
        _connection2<desttype, arg1_type, arg2_type, Mutex>* conn = new
            _connection2<desttype, arg1_type, arg2_type, Mutex>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(arg1_type a1, arg2_type a2)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd)
        {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2);

            it = itNext;
        }
    }

    void operator()(arg1_type a1, arg2_type a2)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd)
        {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2);

            it = itNext;
        }
    }
};

template <class arg1_type, class arg2_type, class arg3_type, class Mutex = PFS_DEFAULT_MT_POLICY>
class signal3 : public _signal_base3<arg1_type, arg2_type, arg3_type, Mutex>
{
    typedef  _signal_base3<arg1_type, arg2_type, arg3_type, Mutex> __base_class;
public:
    signal3()
    {}

    signal3(const signal3<arg1_type, arg2_type, arg3_type, Mutex>& s)
        : _signal_base3<arg1_type, arg2_type, arg3_type, Mutex>(s)
    {}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
        arg2_type, arg3_type))
    {
        lock_guard<Mutex> lock(*this);
        _connection3<desttype, arg1_type, arg2_type, arg3_type, Mutex>* conn =
            new _connection3<desttype, arg1_type, arg2_type, arg3_type, Mutex>(pclass,
            pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(arg1_type a1, arg2_type a2, arg3_type a3)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd)
        {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3);

            it = itNext;
        }
    }

    void operator () (arg1_type a1, arg2_type a2, arg3_type a3)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while (it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3);

            it = itNext;
        }
    }
};

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type, class Mutex = PFS_DEFAULT_MT_POLICY>
class signal4 : public _signal_base4<arg1_type, arg2_type, arg3_type,
    arg4_type, Mutex>
{
    typedef _signal_base4<arg1_type, arg2_type, arg3_type,
    arg4_type, Mutex> __base_class;
public:
    signal4()
    {;}

    signal4(const signal4<arg1_type, arg2_type, arg3_type, arg4_type, Mutex>& s)
        : _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, Mutex>(s)
    {;}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type))
    {
        lock_guard<Mutex> lock(*this);
        _connection4<desttype, arg1_type, arg2_type, arg3_type, arg4_type, Mutex>*
            conn = new _connection4<desttype, arg1_type, arg2_type, arg3_type,
            arg4_type, Mutex>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4);

            it = itNext;
        }
    }

    void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4);

            it = itNext;
        }
    }
};

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type, class Mutex = PFS_DEFAULT_MT_POLICY>
class signal5 : public _signal_base5<arg1_type, arg2_type, arg3_type,
    arg4_type, arg5_type, Mutex>
{
    typedef _signal_base5<arg1_type, arg2_type, arg3_type,
    arg4_type, arg5_type, Mutex> __base_class;

public:
    signal5()
    {;}

    signal5(const signal5<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, Mutex>& s)
        : _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, Mutex>(s)
    {;}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type, arg5_type))
    {
        lock_guard<Mutex> lock(*this);
        _connection5<desttype, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, Mutex>* conn = new _connection5<desttype, arg1_type, arg2_type,
            arg3_type, arg4_type, arg5_type, Mutex>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4, arg5_type a5)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4, a5);

            it = itNext;
        }
    }

    void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while (it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4, a5);

            it = itNext;
        }
    }
};


template <class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type, class arg6_type, class Mutex = PFS_DEFAULT_MT_POLICY>
class signal6 : public _signal_base6<arg1_type, arg2_type, arg3_type,
    arg4_type, arg5_type, arg6_type, Mutex>
{
    typedef _signal_base6<arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, Mutex> __base_class;
public:
    signal6 ()
    {
        ;
    }

    signal6(const signal6<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, Mutex>& s)
        : _signal_base6<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, Mutex>(s)
    {
        ;
    }

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type, arg5_type, arg6_type))
    {
        lock_guard<Mutex> lock(*this);
        _connection6<desttype, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, Mutex>* conn =
            new _connection6<desttype, arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, Mutex>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4, a5, a6);

            it = itNext;
        }
    }

    void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4, a5, a6);

            it = itNext;
        }
    }
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class arg5_type
        , class arg6_type
        , class arg7_type
        , class Mutex = PFS_DEFAULT_MT_POLICY>
class signal7 : public _signal_base7<arg1_type, arg2_type, arg3_type,
    arg4_type, arg5_type, arg6_type, arg7_type, Mutex>
{
    typedef _signal_base7<arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, arg7_type, Mutex> __base_class;
public:
    signal7()
    {
        ;
    }

    signal7(const signal7<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, Mutex>& s)
        : _signal_base7<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, Mutex>(s)
    {
        ;
    }

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
        arg7_type))
    {
        lock_guard<Mutex> lock(*this);
        _connection7<desttype, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, arg7_type, Mutex>* conn =
            new _connection7<desttype, arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, arg7_type, Mutex>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6, arg7_type a7)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd)
        {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4, a5, a6, a7);

            it = itNext;
        }
    }

    void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6, arg7_type a7)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd)
        {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4, a5, a6, a7);

            it = itNext;
        }
    }
};

template <class arg1_type
        , class arg2_type
        , class arg3_type
        , class arg4_type
        , class arg5_type
        , class arg6_type
        , class arg7_type
        , class arg8_type
        , class Mutex = PFS_DEFAULT_MT_POLICY>
class signal8 : public _signal_base8<arg1_type, arg2_type, arg3_type,
    arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, Mutex>
{
    typedef _signal_base8<arg1_type, arg2_type, arg3_type,
    arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, Mutex> __base_class;

public:
    signal8()
    {
        ;
    }

    signal8 (const signal8<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, arg8_type, Mutex>& s)
        : _signal_base8<arg1_type, arg2_type, arg3_type, arg4_type,
        arg5_type, arg6_type, arg7_type, arg8_type, Mutex>(s)
    {
        ;
    }

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
        arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
        arg7_type, arg8_type))
    {
        lock_guard<Mutex> lock(*this);
        _connection8<desttype, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, arg7_type, arg8_type, Mutex>* conn =
            new _connection8<desttype, arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, arg7_type,
            arg8_type, Mutex>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4, a5, a6, a7, a8);

            it = itNext;
        }
    }

    void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
        arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8)
    {
        lock_guard<Mutex> lock(*this);
        typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
        typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

        while(it != itEnd) {
            itNext = it;
            ++itNext;

            (*it)->emit_(a1, a2, a3, a4, a5, a6, a7, a8);

            it = itNext;
        }
    }
};

} // pfs

#endif // __PFS_CXX98_SIGSLOT_HPP__

