#pragma once
#include <set>
#include <list>
#include <pfs/mutex.hpp>
#include <pfs/memory.hpp>
//#include <pfs/deque.hpp>

namespace pfs {

class fake_active_queue
{
public:
    fake_active_queue () {}

    template <typename C, typename ...Args>
    void push_method (void (C::*) (Args...), C *,  Args...)
    {}
};

template <typename ActiveQueue = fake_active_queue, typename BasicLockable = pfs::mutex>
struct sigslot
{
    typedef ActiveQueue   callback_queue_type;
    typedef BasicLockable mutex_type;

    class basic_has_slots;

    class _connection_base0
    {
    public:
        virtual ~_connection_base0() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_() = 0;
        virtual void sync_emit() = 0;
        virtual void async_emit() = 0;
        virtual _connection_base0* clone() = 0;
        virtual _connection_base0* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class A1>
    class _connection_base1
    {
    public:
        virtual ~_connection_base1() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(A1) = 0;
        virtual void sync_emit(A1) = 0;
        virtual void async_emit(A1) = 0;
        virtual _connection_base1<A1>* clone() = 0;
        virtual _connection_base1<A1>* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class A1, class A2>
    class _connection_base2
    {
    public:
        virtual ~_connection_base2() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(A1, A2) = 0;
        virtual void sync_emit(A1, A2) = 0;
        virtual void async_emit(A1, A2) = 0;
        virtual _connection_base2<A1, A2>* clone() = 0;
        virtual _connection_base2<A1, A2>* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class A1, class A2, class A3>
    class _connection_base3
    {
    public:
        virtual ~_connection_base3() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(A1, A2, A3) = 0;
        virtual void sync_emit(A1, A2, A3) = 0;
        virtual void async_emit(A1, A2, A3) = 0;
        virtual _connection_base3<A1, A2, A3>* clone() = 0;
        virtual _connection_base3<A1, A2, A3>* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class A1, class A2, class A3, class A4>
    class _connection_base4
    {
    public:
        virtual ~_connection_base4() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(A1, A2, A3, A4) = 0;
        virtual void sync_emit(A1, A2, A3, A4) = 0;
        virtual void async_emit(A1, A2, A3, A4) = 0;
        virtual _connection_base4<A1, A2, A3, A4>* clone() = 0;
        virtual _connection_base4<A1, A2, A3, A4>* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class A1, class A2, class A3, class A4, class A5>
    class _connection_base5
    {
    public:
        virtual ~_connection_base5() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(A1, A2, A3, A4, A5) = 0;
        virtual void sync_emit(A1, A2, A3, A4, A5) = 0;
        virtual void async_emit(A1, A2, A3, A4, A5) = 0;

        virtual _connection_base5<A1, A2, A3, A4, A5>* clone() = 0;
        virtual _connection_base5<A1, A2, A3, A4, A5>* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class A1, class A2, class A3, class A4, class A5, class A6>
    class _connection_base6
    {
    public:
        virtual ~_connection_base6() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(A1, A2, A3, A4, A5, A6) = 0;
        virtual void sync_emit(A1, A2, A3, A4, A5, A6) = 0;
        virtual void async_emit(A1, A2, A3, A4, A5, A6) = 0;

        virtual _connection_base6<A1, A2, A3, A4, A5, A6> * clone() = 0;
        virtual _connection_base6<A1, A2, A3, A4, A5, A6> * duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    class _connection_base7
    {
    public:
        virtual ~_connection_base7() {}
        virtual basic_has_slots * getdest() const = 0;
        virtual void emit_(A1, A2, A3, A4, A5, A6, A7) = 0;
        virtual void sync_emit(A1, A2, A3, A4, A5, A6, A7) = 0;
        virtual void async_emit(A1, A2, A3, A4, A5, A6, A7) = 0;

        virtual _connection_base7<A1, A2, A3, A4, A5, A6, A7> * clone() = 0;
        virtual _connection_base7<A1, A2, A3, A4, A5, A6, A7> * duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    class _connection_base8
    {
    public:
        virtual ~_connection_base8() {}
        virtual basic_has_slots * getdest() const = 0;
        virtual void emit_(A1, A2, A3, A4, A5, A6, A7, A8) = 0;
        virtual void sync_emit(A1, A2, A3, A4, A5, A6, A7, A8) = 0;
        virtual void async_emit(A1, A2, A3, A4, A5, A6, A7, A8) = 0;

        virtual _connection_base8<A1, A2, A3, A4, A5, A6, A7, A8> * clone() = 0;
        virtual _connection_base8<A1, A2, A3, A4, A5, A6, A7, A8> * duplicate(basic_has_slots* pnewdest) = 0;
    };

    class _signal_base : public mutex_type
    {
    public:
        virtual ~_signal_base() {}
        virtual void slot_disconnect(basic_has_slots * pslot) = 0;

        // Used inside has_slots copy constructor
        virtual void slot_duplicate(const basic_has_slots * poldslot, basic_has_slots * pnewslot) = 0;
    };

    class basic_has_slots : public mutex_type
    {
    private:
        typedef std::set<_signal_base *> sender_set;
        typedef typename sender_set::const_iterator const_iterator;

    public:
        basic_has_slots()
        {}

        virtual bool use_async_slots () const = 0;

        void signal_connect(_signal_base* sender)
        {
            lock_guard<mutex_type> lock(*this);
            _senders.insert(sender);
        }

        void signal_disconnect(_signal_base* sender)
        {
            lock_guard<mutex_type> lock(*this);
            _senders.erase(sender);
        }

        virtual ~basic_has_slots()
        {
            disconnect_all();
        }

        void disconnect_all()
        {
            lock_guard<mutex_type> lock(*this);
            const_iterator it = _senders.begin();
            const_iterator itEnd = _senders.end();

            while(it != itEnd) {
                (*it)->slot_disconnect(this);
                ++it;
            }

            _senders.erase(_senders.begin(), _senders.end());
        }

        size_t count () const
        {
            return _senders.size();
        }

        callback_queue_type & callback_queue () { return *_queue_ptr; }

    protected:
        sender_set _senders;
        unique_ptr<callback_queue_type> _queue_ptr;
    };

    class has_slots : public basic_has_slots
    {
    public:
        has_slots () : basic_has_slots () {}
        virtual bool use_async_slots () const pfs_override { return false; }
    };

    class has_async_slots : public basic_has_slots
    {
    public:
        has_async_slots () : basic_has_slots ()
        {
            this->_queue_ptr = make_unique<callback_queue_type>();
        }

        virtual bool use_async_slots () const pfs_override { return true; }
    };

    class _signal_base0 : public _signal_base
    {
    public:
        typedef std::list<_connection_base0 *>  connections_list;

        _signal_base0()
        {}

        _signal_base0(const _signal_base0& s)
            : _signal_base(s)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::iterator it = _connected_slots.begin();
            typename connections_list::iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                if((*it)->getdest() == oldtarget) {
                    _connected_slots.push_back((*it)->duplicate(newtarget));
                }

                ++it;
            }
        }

        void disconnect_all()
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect(basic_has_slots* pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1>
    class _signal_base1 : public _signal_base
    {
    public:
        typedef std::list<_connection_base1<A1> *>  connections_list;

        _signal_base1()
        {}

        ~_signal_base1()
        {
            disconnect_all();
        }

        void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::iterator it = _connected_slots.begin();
            typename connections_list::iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                if((*it)->getdest() == oldtarget) {
                    _connected_slots.push_back((*it)->duplicate(newtarget));
                }

                ++it;
            }
        }

        void disconnect_all()
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect(basic_has_slots* pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2>
    class _signal_base2 : public _signal_base
    {
    public:
        typedef std::list<_connection_base2<A1, A2> *>
            connections_list;

        _signal_base2()
        {}

        _signal_base2(const _signal_base2<A1, A2>& s)
            : _signal_base(s)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = s._connected_slots.begin();
            typename connections_list::const_iterator itEnd = s._connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_connect(this);
                _connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        void slot_duplicate (const basic_has_slots * oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
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
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect (basic_has_slots * pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3>
    class _signal_base3 : public _signal_base
    {
    public:
        typedef std::list<_connection_base3<A1, A2, A3> *>
            connections_list;

        _signal_base3()
        {}

        _signal_base3(const _signal_base3<A1, A2, A3>& s)
            : _signal_base(s)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = s._connected_slots.begin();
            typename connections_list::const_iterator itEnd = s._connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_connect(this);
                _connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
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
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect(basic_has_slots* pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3, class A4>
    class _signal_base4 : public _signal_base
    {
    public:
        typedef std::list<_connection_base4<A1, A2, A3,
            A4> *>  connections_list;

        _signal_base4()
        {}

        _signal_base4 (const _signal_base4<A1, A2, A3, A4>& s)
            : _signal_base(s)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = s._connected_slots.begin();
            typename connections_list::const_iterator itEnd = s._connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_connect(this);
                _connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
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
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect(basic_has_slots* pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3, class A4, class A5>
    class _signal_base5 : public _signal_base
    {
    public:
        typedef std::list<_connection_base5<A1, A2, A3, A4, A5> *>  connections_list;

        _signal_base5()
        {}

        _signal_base5(const _signal_base5<A1, A2, A3, A4,A5> & s)
                : _signal_base(s)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = s._connected_slots.begin();
            typename connections_list::const_iterator itEnd = s._connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_connect(this);
                _connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
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
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect(basic_has_slots* pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::iterator it = _connected_slots.begin();
            typename connections_list::iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                typename connections_list::iterator itNext = it;
                ++itNext;

                if((*it)->getdest() == pslot) {
                    _connected_slots.erase(it);
                    //delete *it;
                }

                it = itNext;
            }
        }

        bool isConnected() const { return _connected_slots.size() > 0; }

    protected:
        connections_list _connected_slots;
    };

    template <class A1, class A2, class A3, class A4, class A5, class A6>
    class _signal_base6 : public _signal_base
    {
    public:
        typedef std::list<_connection_base6<A1, A2, A3, A4, A5, A6> *>  connections_list;

        _signal_base6()
        {}

        _signal_base6(const _signal_base6<A1, A2, A3, A4, A5, A6> & s)
                : _signal_base(s)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = s._connected_slots.begin();
            typename connections_list::const_iterator itEnd = s._connected_slots.end();

            while (it != itEnd) {
                (*it)->getdest()->signal_connect(this);
                _connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
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
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect(basic_has_slots* pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    class _signal_base7 : public _signal_base
    {
    public:
        typedef std::list<_connection_base7<A1, A2, A3, A4, A5, A6, A7> *>  connections_list;

        _signal_base7()
        {}

        _signal_base7(const _signal_base7<A1, A2, A3, A4, A5, A6, A7> & s)
                : _signal_base(s)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = s._connected_slots.begin();
            typename connections_list::const_iterator itEnd = s._connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_connect(this);
                _connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
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
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect(basic_has_slots* pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::iterator it = _connected_slots.begin();
            typename connections_list::iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                typename connections_list::iterator itNext = it;
                ++itNext;

                if((*it)->getdest() == pslot) {
                    _connected_slots.erase(it);
                    // delete *it;
                }

                it = itNext;
            }
        }

        bool isConnected() const { return _connected_slots.size() > 0; }

    protected:
        connections_list _connected_slots;
    };

    template <class A1, class A2, class A3, class A4 , class A5, class A6, class A7, class A8>
    class _signal_base8 : public _signal_base
    {
    public:
        typedef std::list<_connection_base8<A1, A2, A3, A4, A5, A6, A7, A8> *>
            connections_list;

        _signal_base8()
        {}

        _signal_base8(const _signal_base8<A1, A2, A3, A4, A5, A6, A7, A8> & s)
                : _signal_base(s)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = s._connected_slots.begin();
            typename connections_list::const_iterator itEnd = s._connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_connect(this);
                _connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
        {
            lock_guard<mutex_type> lock(*this);
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
            lock_guard<mutex_type> lock(*this);
            typename connections_list::const_iterator it = _connected_slots.begin();
            typename connections_list::const_iterator itEnd = _connected_slots.end();

            while(it != itEnd) {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
        }

        void disconnect(basic_has_slots* pclass)
        {
            lock_guard<mutex_type> lock(*this);
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

        void slot_disconnect(basic_has_slots* pslot)
        {
            lock_guard<mutex_type> lock(*this);
            typename connections_list::iterator it = _connected_slots.begin();
            typename connections_list::iterator itEnd = _connected_slots.end();

            while(it != itEnd)
            {
                typename connections_list::iterator itNext = it;
                ++itNext;

                if((*it)->getdest() == pslot)
                {
                    _connected_slots.erase(it);
                    // delete *it;
                }

                it = itNext;
            }
        }

        bool isConnected() const { return _connected_slots.size() > 0; }

    protected:
        connections_list _connected_slots;
    };

    template <class dest_type>
    class _connection0 : public _connection_base0
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

        virtual _connection_base0 * clone()
        {
            return new _connection0<dest_type>(*this);
        }

        virtual _connection_base0* duplicate(basic_has_slots* pnewdest)
        {
            return new _connection0<dest_type>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_()
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type>(_pmemfun, _pobject);
            else
                (_pobject->*_pmemfun)();
        }

        virtual void sync_emit () pfs_override
        {
            (_pobject->*_pmemfun)();
        }

        virtual void async_emit () pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type>(_pmemfun, _pobject);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)();
    };

    template <class dest_type, class A1>
    class _connection1 : public _connection_base1<A1>
    {
    public:
        _connection1()
            : _pobject(0)
            , _pmemfun(0)
        {}

        _connection1 (dest_type* pobject, void (dest_type::*pmemfun)(A1))
            : _pobject(pobject)
            , _pmemfun(pmemfun)
        {}

        virtual _connection_base1<A1>* clone()
        {
            return new _connection1<dest_type, A1>(*this);
        }

        virtual _connection_base1<A1>* duplicate(basic_has_slots* pnewdest)
        {
            return new _connection1<dest_type, A1>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_(A1 a1)
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type
                        , A1>(_pmemfun, _pobject, a1);
            else
                (_pobject->*_pmemfun)(a1);
        }

        virtual void sync_emit (A1 a1) pfs_override
        {
            (_pobject->*_pmemfun)(a1);
        }

        virtual void async_emit (A1 a1) pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type
                    , A1>(_pmemfun, _pobject, a1);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)(A1);
    };

    template <class dest_type, class A1, class A2>
    class _connection2 : public _connection_base2<A1, A2>
    {
    public:
        _connection2()
        {
            _pobject = NULL;
            _pmemfun = NULL;
        }

        _connection2(dest_type* pobject, void (dest_type::*pmemfun)(A1,
            A2))
        {
            _pobject = pobject;
            _pmemfun = pmemfun;
        }

        //~_connection2() {}

        virtual _connection_base2<A1, A2>* clone()
        {
            return new _connection2<dest_type, A1, A2>(*this);
        }

        virtual _connection_base2<A1, A2>* duplicate(basic_has_slots* pnewdest)
        {
            return new _connection2<dest_type, A1, A2>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_(A1 a1, A2 a2)
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type
                        , A1
                        , A2>(_pmemfun, _pobject, a1, a2);
            else
                (_pobject->*_pmemfun)(a1, a2);
        }

        virtual void sync_emit (A1 a1, A2 a2) pfs_override
        {
            (_pobject->*_pmemfun)(a1, a2);
        }

        virtual void async_emit (A1 a1, A2 a2) pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type
                    , A1
                    , A2>(_pmemfun, _pobject, a1, a2);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)(A1, A2);
    };

    template <class dest_type, class A1, class A2, class A3>
    class _connection3 : public _connection_base3<A1, A2, A3>
    {
    public:
        _connection3()
        {
            _pobject = NULL;
            _pmemfun = NULL;
        }

        _connection3(dest_type* pobject, void (dest_type::*pmemfun)(A1,
            A2, A3))
        {
            _pobject = pobject;
            _pmemfun = pmemfun;
        }

        virtual _connection_base3<A1, A2, A3>* clone()
        {
            return new _connection3<dest_type, A1, A2, A3>(*this);
        }

        virtual _connection_base3<A1, A2, A3>* duplicate(basic_has_slots* pnewdest)
        {
            return new _connection3<dest_type, A1, A2, A3>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_(A1 a1, A2 a2, A3 a3)
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type
                    , A1
                    , A2
                    , A3>(_pmemfun, _pobject, a1, a2, a3);
            else
                (_pobject->*_pmemfun)(a1, a2, a3);
        }

        virtual void sync_emit (A1 a1, A2 a2, A3 a3) pfs_override
        {
            (_pobject->*_pmemfun)(a1, a2, a3);
        }

        virtual void async_emit (A1 a1, A2 a2, A3 a3) pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type
                , A1
                , A2
                , A3>(_pmemfun, _pobject, a1, a2, a3);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)(A1, A2, A3);
    };

    template <class dest_type, class A1, class A2, class A3, class A4>
    class _connection4 : public _connection_base4<A1, A2, A3, A4>
    {
    public:
        _connection4()
        {
            _pobject = NULL;
            _pmemfun = NULL;
        }

        _connection4(dest_type* pobject, void (dest_type::*pmemfun)(A1,
            A2, A3, A4))
        {
            _pobject = pobject;
            _pmemfun = pmemfun;
        }

        virtual _connection_base4<A1, A2, A3, A4>* clone()
        {
            return new _connection4<dest_type, A1, A2, A3, A4>(*this);
        }

        virtual _connection_base4<A1, A2, A3, A4>* duplicate(basic_has_slots* pnewdest)
        {
            return new _connection4<dest_type, A1, A2, A3, A4>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_(A1 a1, A2 a2, A3 a3, A4 a4)
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type
                    , A1
                    , A2
                    , A3
                    , A4>(_pmemfun, _pobject, a1, a2, a3, a4);
            else
                (_pobject->*_pmemfun)(a1, a2, a3, a4);
        }

        virtual void sync_emit (A1 a1, A2 a2, A3 a3, A4 a4) pfs_override
        {
            (_pobject->*_pmemfun)(a1, a2, a3, a4);
        }

        virtual void async_emit (A1 a1, A2 a2, A3 a3, A4 a4) pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type
                , A1
                , A2
                , A3
                , A4>(_pmemfun, _pobject, a1, a2, a3, a4);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)(A1, A2, A3, A4);
    };

    template <class dest_type, class A1, class A2, class A3, class A4, class A5>
    class _connection5 : public _connection_base5<A1, A2, A3, A4, A5>
    {
    public:
        _connection5()
        {
            _pobject = NULL;
            _pmemfun = NULL;
        }

        _connection5(dest_type* pobject, void (dest_type::*pmemfun)(A1,
            A2, A3, A4, A5))
        {
            _pobject = pobject;
            _pmemfun = pmemfun;
        }

        virtual _connection_base5<A1, A2, A3, A4, A5> * clone()
        {
            return new _connection5<dest_type, A1, A2, A3, A4, A5>(*this);
        }

        virtual _connection_base5<A1, A2, A3, A4, A5> * duplicate(basic_has_slots* pnewdest)
        {
            return new _connection5<dest_type
                    , A1
                    , A2
                    , A3
                    , A4
                    , A5>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type
                    , A1
                    , A2
                    , A3
                    , A4
                    , A5>(_pmemfun, _pobject, a1, a2, a3, a4, a5);
            else
                (_pobject->*_pmemfun)(a1, a2, a3, a4, a5);
        }

        virtual void sync_emit (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) pfs_override
        {
            (_pobject->*_pmemfun)(a1, a2, a3, a4, a5);
        }

        virtual void async_emit (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type
                , A1
                , A2
                , A3
                , A4
                , A5>(_pmemfun, _pobject, a1, a2, a3, a4, a5);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)(A1, A2, A3, A4,
            A5);
    };

    template <class dest_type, class A1, class A2, class A3, class A4, class A5, class A6>
    class _connection6 : public _connection_base6<A1, A2, A3, A4, A5, A6>
    {
    public:
        _connection6()
        {
            _pobject = NULL;
            _pmemfun = NULL;
        }

        _connection6(dest_type* pobject, void (dest_type::*pmemfun)(A1,
            A2, A3, A4, A5, A6))
        {
            _pobject = pobject;
            _pmemfun = pmemfun;
        }

        virtual _connection_base6<A1, A2, A3, A4,
            A5, A6>* clone()
        {
            return new _connection6<dest_type, A1, A2, A3, A4,
                A5, A6>(*this);
        }

        virtual _connection_base6<A1, A2, A3, A4,
            A5, A6>* duplicate(basic_has_slots* pnewdest)
        {
            return new _connection6<dest_type, A1, A2, A3, A4,
                A5, A6>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type
                    , A1
                    , A2
                    , A3
                    , A4
                    , A5
                    , A6>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6);
            else
                (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6);
        }

        virtual void sync_emit (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) pfs_override
        {
            (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6);
        }

        virtual void async_emit (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type
                , A1
                , A2
                , A3
                , A4
                , A5
                , A6>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)(A1, A2, A3, A4,
            A5, A6);
    };

    template <class dest_type, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    class _connection7 : public _connection_base7<A1, A2, A3, A4, A5, A6, A7>
    {
    public:
        _connection7()
        {
            _pobject = NULL;
            _pmemfun = NULL;
        }

        _connection7(dest_type* pobject, void (dest_type::*pmemfun)(A1,
            A2, A3, A4, A5, A6, A7))
        {
            _pobject = pobject;
            _pmemfun = pmemfun;
        }

        virtual _connection_base7<A1, A2, A3, A4, A5, A6, A7>* clone()
        {
            return new _connection7<dest_type, A1, A2, A3, A4, A5, A6, A7>(*this);
        }

        virtual _connection_base7<A1, A2, A3, A4, A5, A6, A7>* duplicate(basic_has_slots* pnewdest)
        {
            return new _connection7<dest_type, A1, A2, A3, A4, A5, A6, A7>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_(A1 a1, A2 a2, A3 a3, A4 a4 , A5 a5, A6 a6, A7 a7)
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type
                    , A1
                    , A2
                    , A3
                    , A4
                    , A5
                    , A6
                    , A7>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6, a7);
            else
                (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7);
        }

        virtual void sync_emit (A1 a1, A2 a2, A3 a3, A4 a4
                , A5 a5, A6 a6, A7 a7) pfs_override
        {
            (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7);
        }

        virtual void async_emit (A1 a1, A2 a2, A3 a3, A4 a4
                , A5 a5, A6 a6, A7 a7) pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type
                , A1
                , A2
                , A3
                , A4
                , A5
                , A6
                , A7>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6, a7);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)(A1, A2, A3, A4,
            A5, A6, A7);
    };

    template <class dest_type, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    class _connection8 : public _connection_base8<A1, A2, A3, A4, A5, A6, A7, A8>
    {
    public:
        _connection8()
        {
            _pobject = NULL;
            _pmemfun = NULL;
        }

        _connection8(dest_type* pobject, void (dest_type::*pmemfun)(A1,
            A2, A3, A4, A5, A6,
            A7, A8))
        {
            _pobject = pobject;
            _pmemfun = pmemfun;
        }

        virtual _connection_base8<A1, A2, A3, A4, A5, A6, A7, A8>* clone()
        {
            return new _connection8<dest_type, A1, A2, A3, A4, A5, A6, A7, A8>(*this);
        }

        virtual _connection_base8<A1, A2, A3, A4,
            A5, A6, A7, A8>* duplicate(basic_has_slots* pnewdest)
        {
            return new _connection8<dest_type, A1, A2, A3, A4,
                A5, A6, A7, A8>(static_cast<dest_type *>(pnewdest), _pmemfun);
        }

        virtual void emit_(A1 a1, A2 a2, A3 a3, A4 a4
                , A5 a5, A6 a6, A7 a7, A8 a8)
        {
            if (_pobject->use_async_slots())
                _pobject->callback_queue().template push_method<dest_type
                    , A1
                    , A2
                    , A3
                    , A4
                    , A5
                    , A6
                    , A7
                    , A8>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6, a7, a8);
            else
                (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7, a8);
        }

        virtual void sync_emit (A1 a1, A2 a2, A3 a3, A4 a4
                , A5 a5, A6 a6, A7 a7, A8 a8) pfs_override
        {
            (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7, a8);
        }

        virtual void async_emit (A1 a1, A2 a2, A3 a3, A4 a4
                , A5 a5, A6 a6, A7 a7, A8 a8) pfs_override
        {
            _pobject->callback_queue().template push_method<dest_type
                , A1
                , A2
                , A3
                , A4
                , A5
                , A6
                , A7
                , A8>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6, a7, a8);
        }

        virtual basic_has_slots* getdest() const
        {
            return _pobject;
        }

    private:
        dest_type* _pobject;
        void (dest_type::* _pmemfun)(A1, A2, A3, A4,
            A5, A6, A7, A8);
    };

    class signal0 : public _signal_base0
    {
        typedef _signal_base0 __base_class;

    public:
        signal0()
        {}

        signal0(const signal0 & s)
            : _signal_base0(s)
        {}

        template <class desttype>
            void connect(desttype* pclass, void (desttype::*pmemfun)())
        {
            lock_guard<mutex_type> lock(*this);
            _connection0<desttype>* conn
                    = new _connection0<desttype>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_()
        {
            lock_guard<mutex_type> lock(*this);
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
            lock_guard<mutex_type> lock(*this);
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

    template <class A1>
    class signal1 : public _signal_base1<A1>
    {
        typedef _signal_base1<A1> __base_class;

    public:
        signal1()
        {}

        signal1(const signal1<A1>& s)
            : _signal_base1<A1>(s)
        {}

        template <class desttype>
            void connect(desttype* pclass, void (desttype::*pmemfun)(A1))
        {
            lock_guard<mutex_type> lock(*this);
            _connection1<desttype, A1>* conn =
                new _connection1<desttype, A1>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_(A1 a1)
        {
            lock_guard<mutex_type> lock(*this);
            typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
            typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

            while(it != itEnd) {
                itNext = it;
                ++itNext;

                (*it)->emit_(a1);

                it = itNext;
            }
        }

        void operator()(A1 a1)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2>
    class signal2 : public _signal_base2<A1, A2>
    {
        typedef _signal_base2<A1, A2> __base_class;
    public:
        signal2()
        {}

        signal2(const signal2<A1, A2>& s)
            : _signal_base2<A1, A2>(s)
        {}

        template <class desttype>
            void connect(desttype* pclass, void (desttype::*pmemfun)(A1,
            A2))
        {
            lock_guard<mutex_type> lock(*this);
            _connection2<desttype, A1, A2>* conn = new
                _connection2<desttype, A1, A2>(pclass, pmemfun);
                this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_(A1 a1, A2 a2)
        {
            lock_guard<mutex_type> lock(*this);
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

        void operator()(A1 a1, A2 a2)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3>
    class signal3 : public _signal_base3<A1, A2, A3>
    {
        typedef  _signal_base3<A1, A2, A3> __base_class;
    public:
        signal3()
        {}

        signal3(const signal3<A1, A2, A3>& s)
            : _signal_base3<A1, A2, A3>(s)
        {}

        template <class desttype>
            void connect(desttype* pclass, void (desttype::*pmemfun)(A1,
            A2, A3))
        {
            lock_guard<mutex_type> lock(*this);
            _connection3<desttype, A1, A2, A3>* conn =
                new _connection3<desttype, A1, A2, A3>(pclass,
                pmemfun);
                this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_(A1 a1, A2 a2, A3 a3)
        {
            lock_guard<mutex_type> lock(*this);
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

        void operator () (A1 a1, A2 a2, A3 a3)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3, class A4>
    class signal4 : public _signal_base4<A1, A2, A3,
        A4>
    {
        typedef _signal_base4<A1, A2, A3, A4> __base_class;
    public:
        signal4()
        {;}

        signal4(const signal4<A1, A2, A3, A4>& s)
            : __base_class(s)
        {;}

        template <class desttype>
            void connect(desttype* pclass, void (desttype::*pmemfun)(A1,
            A2, A3, A4))
        {
            lock_guard<mutex_type> lock(*this);
            _connection4<desttype, A1, A2, A3, A4>*
                conn = new _connection4<desttype, A1, A2, A3, A4>(pclass, pmemfun);
                this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_(A1 a1, A2 a2, A3 a3, A4 a4)
        {
            lock_guard<mutex_type> lock(*this);
            typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
            typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

            while(it != itEnd) {
                itNext = it;
                ++itNext;

                (*it)->emit_(a1, a2, a3, a4);

                it = itNext;
            }
        }

        void operator()(A1 a1, A2 a2, A3 a3, A4 a4)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3, class A4, class A5>
    class signal5 : public _signal_base5<A1, A2, A3, A4, A5>
    {
        typedef _signal_base5<A1, A2, A3, A4, A5> __base_class;

    public:
        signal5()
        {;}

        signal5(const signal5<A1, A2, A3, A4, A5> & s)
            : _signal_base5<A1, A2, A3, A4, A5>(s)
        {;}

        template <class desttype>
            void connect(desttype* pclass, void (desttype::*pmemfun)(A1,
            A2, A3, A4, A5))
        {
            lock_guard<mutex_type> lock(*this);
            _connection5<desttype, A1, A2, A3, A4, A5>* conn = new _connection5<desttype, A1, A2,
                A3, A4, A5>(pclass, pmemfun);
                this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
        {
            lock_guard<mutex_type> lock(*this);
            typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
            typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

            while(it != itEnd) {
                itNext = it;
                ++itNext;

                (*it)->emit_(a1, a2, a3, a4, a5);

                it = itNext;
            }
        }

        void operator()(A1 a1, A2 a2, A3 a3, A4 a4,
            A5 a5)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3, class A4, class A5, class A6>
    class signal6 : public _signal_base6<A1, A2, A3, A4, A5, A6>
    {
        typedef _signal_base6<A1, A2, A3, A4, A5, A6> __base_class;
    public:
        signal6 ()
        {
            ;
        }

        signal6(const signal6<A1, A2, A3, A4, A5, A6>& s)
            : _signal_base6<A1, A2, A3, A4, A5, A6>(s)
        {
            ;
        }

        template <class desttype>
            void connect(desttype* pclass, void (desttype::*pmemfun)(A1,
            A2, A3, A4, A5, A6))
        {
            lock_guard<mutex_type> lock(*this);
            _connection6<desttype, A1, A2, A3, A4, A5, A6> * conn =
                new _connection6<desttype, A1, A2, A3, A4, A5, A6>(pclass, pmemfun);
                this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_(A1 a1, A2 a2, A3 a3, A4 a4,
            A5 a5, A6 a6)
        {
            lock_guard<mutex_type> lock(*this);
            typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
            typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

            while(it != itEnd) {
                itNext = it;
                ++itNext;

                (*it)->emit_(a1, a2, a3, a4, a5, a6);

                it = itNext;
            }
        }

        void operator()(A1 a1, A2 a2, A3 a3, A4 a4,
            A5 a5, A6 a6)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    class signal7 : public _signal_base7<A1, A2, A3, A4, A5, A6, A7>
    {
        typedef _signal_base7<A1, A2, A3, A4, A5, A6, A7> __base_class;
    public:
        signal7()
        {}

        signal7(const signal7<A1, A2, A3, A4, A5, A6, A7>& s)
            : _signal_base7<A1, A2, A3, A4, A5, A6, A7>(s)
        {}

        template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(A1, A2, A3, A4, A5, A6, A7))
        {
            lock_guard<mutex_type> lock(*this);
            _connection7<desttype, A1, A2, A3, A4, A5, A6, A7>* conn =
                new _connection7<desttype, A1, A2, A3, A4, A5, A6, A7>(pclass, pmemfun);
                this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
        {
            lock_guard<mutex_type> lock(*this);
            typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
            typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

            while(it != itEnd) {
                itNext = it;
                ++itNext;

                (*it)->emit_(a1, a2, a3, a4, a5, a6, a7);

                it = itNext;
            }
        }

        void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
        {
            lock_guard<mutex_type> lock(*this);
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

    template <class A1, class A2, class A3, class A4, class A5 , class A6 , class A7 , class A8>
    class signal8 : public _signal_base8<A1, A2, A3, A4, A5, A6, A7, A8>
    {
        typedef _signal_base8<A1, A2, A3, A4, A5, A6, A7, A8> __base_class;

    public:
        signal8()
        {}

        signal8 (const signal8<A1, A2, A3, A4, A5, A6, A7, A8> & s)
            : _signal_base8<A1, A2, A3, A4, A5, A6, A7, A8>(s)
        {}

        template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(A1, A2, A3, A4, A5, A6, A7, A8))
        {
            lock_guard<mutex_type> lock(*this);
            _connection8<desttype, A1, A2, A3, A4, A5, A6, A7, A8>* conn
                    = new _connection8<desttype, A1, A2, A3, A4, A5, A6, A7, A8>(pclass, pmemfun);
                this->_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit_(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
        {
            lock_guard<mutex_type> lock(*this);
            typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
            typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();

            while(it != itEnd) {
                itNext = it;
                ++itNext;

                (*it)->emit_(a1, a2, a3, a4, a5, a6, a7, a8);

                it = itNext;
            }
        }

        void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
        {
            lock_guard<mutex_type> lock(*this);
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

}; // struct sigslot

} // pfs
