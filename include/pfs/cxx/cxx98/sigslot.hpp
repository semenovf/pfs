#pragma once

#include <set>
#include <list>
#include <pfs/mutex.hpp>
#include <pfs/traits/stdcxx/deque.hpp>

namespace pfs {

class fake_active_queue
{
public:
    fake_active_queue () {}

//#if __cplusplus >= 201103L
//#   error Implement using variadic templates
//#else



    template <typename C>
    void push_method (void (C::*) (), C *)
    {}

    template <typename C, typename Arg1>
    void push_method (void (C::*) (Arg1), C *,  Arg1)
    {}

    template <typename C, typename Arg1, typename Arg2>
    void push_method (void (C::*) (Arg1, Arg2), C *, Arg1, Arg2)
    {}

    template <typename C, typename Arg1, typename Arg2, typename Arg3>
    void push_method (void (C::* f) (Arg1, Arg2, Arg3), C * c, Arg1 a1, Arg2 a2, Arg3 a3)
    {}

    template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
    {}

    template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
    {}

    template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
    void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
    {}

    template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
    void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
    {}

    template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
    void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
    {}
//#endif
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

    template <class Arg1>
    class _connection_base1
    {
    public:
        virtual ~_connection_base1() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(Arg1) = 0;
        virtual void sync_emit(Arg1) = 0;
        virtual void async_emit(Arg1) = 0;
        virtual _connection_base1<Arg1>* clone() = 0;
        virtual _connection_base1<Arg1>* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class Arg1
            , class Arg2>
    class _connection_base2
    {
    public:
        virtual ~_connection_base2() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(Arg1, Arg2) = 0;
        virtual void sync_emit(Arg1, Arg2) = 0;
        virtual void async_emit(Arg1, Arg2) = 0;
        virtual _connection_base2<Arg1, Arg2>* clone() = 0;
        virtual _connection_base2<Arg1, Arg2>* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class Arg1
            , class Arg2
            , class Arg3>
    class _connection_base3
    {
    public:
        virtual ~_connection_base3() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(Arg1, Arg2, Arg3) = 0;
        virtual void sync_emit(Arg1, Arg2, Arg3) = 0;
        virtual void async_emit(Arg1, Arg2, Arg3) = 0;
        virtual _connection_base3<Arg1, Arg2, Arg3>* clone() = 0;
        virtual _connection_base3<Arg1, Arg2, Arg3>* duplicate(basic_has_slots* pnewdest) = 0;
    };

    template <class Arg1
            , class Arg2
            , class Arg3
            , class Arg4>
    class _connection_base4
    {
    public:
        virtual ~_connection_base4() {}
        virtual basic_has_slots* getdest() const = 0;
        virtual void emit_(Arg1, Arg2, Arg3, Arg4) = 0;
        virtual void sync_emit(Arg1, Arg2, Arg3, Arg4) = 0;
        virtual void async_emit(Arg1, Arg2, Arg3, Arg4) = 0;
        virtual _connection_base4<Arg1, Arg2, Arg3, Arg4>* clone() = 0;
        virtual _connection_base4<Arg1, Arg2, Arg3, Arg4>* duplicate(basic_has_slots* pnewdest) = 0;
    };

//     template <class Arg1
//             , class Arg2
//             , class Arg3
//             , class Arg4
//             , class Arg5>
//     class _connection_base5
//     {
//     public:
//         virtual ~_connection_base5() {}
//         virtual basic_has_slots* getdest() const = 0;
//         virtual void emit_(Arg1, Arg2, Arg3, Arg4,
//             Arg5) = 0;
//         virtual void sync_emit(Arg1, Arg2, Arg3, Arg4,
//             Arg5) = 0;
//         virtual void async_emit(Arg1, Arg2, Arg3, Arg4,
//             Arg5) = 0;
//
//         virtual _connection_base5<Arg1, Arg2, Arg3, Arg4,
//             Arg5, mutex_type>* clone() = 0;
//         virtual _connection_base5<Arg1, Arg2, Arg3, Arg4,
//             Arg5, mutex_type>* duplicate(basic_has_slots* pnewdest) = 0;
//     };

// template <class Arg1
//         , class Arg2
//         , class Arg3
//         , class Arg4
//         , class Arg5
//         , class Arg6
//         , class mutex_type>
// class _connection_base6
// {
// public:
//     virtual ~_connection_base6() {}
//     virtual basic_has_slots* getdest() const = 0;
//     virtual void emit_(Arg1, Arg2, Arg3, Arg4, Arg5,
//         Arg6) = 0;
//     virtual void sync_emit(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) = 0;
//     virtual void async_emit(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) = 0;
//
//     virtual _connection_base6<Arg1, Arg2, Arg3, Arg4, Arg5
//             , Arg6, mutex_type>* clone() = 0;
//     virtual _connection_base6<Arg1, Arg2, Arg3, Arg4, Arg5
//             , Arg6, mutex_type>* duplicate(basic_has_slots* pnewdest) = 0;
// };
//
// template <class Arg1
//         , class Arg2
//         , class Arg3
//         , class Arg4
//         , class Arg5
//         , class Arg6
//         , class Arg7
//         , class mutex_type>
// class _connection_base7
// {
// public:
//     virtual ~_connection_base7() {}
//     virtual basic_has_slots* getdest() const = 0;
//     virtual void emit_(Arg1, Arg2, Arg3, Arg4, Arg5
//             , Arg6, Arg7) = 0;
//     virtual void sync_emit(Arg1, Arg2, Arg3, Arg4, Arg5
//             , Arg6, Arg7) = 0;
//     virtual void async_emit(Arg1, Arg2, Arg3, Arg4, Arg5
//             , Arg6, Arg7) = 0;
//
//     virtual _connection_base7<Arg1, Arg2, Arg3, Arg4
//             , Arg5, Arg6, Arg7, mutex_type>* clone() = 0;
//     virtual _connection_base7<Arg1, Arg2, Arg3, Arg4
//             , Arg5, Arg6, Arg7, mutex_type>* duplicate(basic_has_slots* pnewdest) = 0;
// };
//
// template <class Arg1
//         , class Arg2
//         , class Arg3
//         , class Arg4
//         , class Arg5
//         , class Arg6
//         , class Arg7
//         , class Arg8
//         , class mutex_type>
// class _connection_base8
// {
// public:
//     virtual ~_connection_base8() {}
//     virtual basic_has_slots* getdest() const = 0;
//     virtual void emit_(Arg1, Arg2, Arg3, Arg4, Arg5
//             , Arg6, Arg7, Arg8) = 0;
//     virtual void sync_emit(Arg1, Arg2, Arg3, Arg4, Arg5
//             , Arg6, Arg7, Arg8) = 0;
//     virtual void async_emit(Arg1, Arg2, Arg3, Arg4, Arg5
//             , Arg6, Arg7, Arg8) = 0;
//
//     virtual _connection_base8<Arg1, Arg2, Arg3, Arg4
//             , Arg5, Arg6, Arg7, Arg8, mutex_type>* clone() = 0;
//     virtual _connection_base8<Arg1, Arg2, Arg3, Arg4
//             , Arg5, Arg6, Arg7, Arg8, mutex_type>* duplicate(basic_has_slots* pnewdest) = 0;
// };

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

template <class Arg1>
class _signal_base1 : public _signal_base
{
public:
    typedef std::list<_connection_base1<Arg1> *>  connections_list;

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

template <class Arg1
        , class Arg2>
class _signal_base2 : public _signal_base
{
public:
    typedef std::list<_connection_base2<Arg1, Arg2> *>
        connections_list;

    _signal_base2()
    {}

    _signal_base2(const _signal_base2<Arg1, Arg2>& s)
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

template <class Arg1
        , class Arg2
        , class Arg3>
class _signal_base3 : public _signal_base
{
public:
    typedef std::list<_connection_base3<Arg1, Arg2, Arg3> *>
        connections_list;

    _signal_base3()
    {}

    _signal_base3(const _signal_base3<Arg1, Arg2, Arg3>& s)
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

template <class Arg1
        , class Arg2
        , class Arg3
        , class Arg4>
class _signal_base4 : public _signal_base
{
public:
    typedef std::list<_connection_base4<Arg1, Arg2, Arg3,
        Arg4> *>  connections_list;

    _signal_base4()
    {}

    _signal_base4 (const _signal_base4<Arg1, Arg2, Arg3, Arg4>& s)
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

// template <class Arg1
//         , class Arg2
//         , class Arg3
//         , class Arg4
//         , class Arg5
//         , class mutex_type>
// class _signal_base5 : public _signal_base<mutex_type>
// {
// public:
//     typedef std::list<_connection_base5<Arg1, Arg2, Arg3,
//         Arg4, Arg5, mutex_type> *>  connections_list;
//
//     _signal_base5()
//     {}
//
//     _signal_base5(const _signal_base5<Arg1, Arg2, Arg3, Arg4,
//         Arg5, mutex_type>& s)
//         : _signal_base<mutex_type>(s)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::const_iterator it = s._connected_slots.begin();
//         typename connections_list::const_iterator itEnd = s._connected_slots.end();
//
//         while(it != itEnd) {
//             (*it)->getdest()->signal_connect(this);
//             _connected_slots.push_back((*it)->clone());
//
//             ++it;
//         }
//     }
//
//     void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             if((*it)->getdest() == oldtarget) {
//                 _connected_slots.push_back((*it)->duplicate(newtarget));
//             }
//
//             ++it;
//         }
//     }
//
//     ~_signal_base5()
//     {
//         disconnect_all();
//     }
//
//     void disconnect_all()
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::const_iterator it = _connected_slots.begin();
//         typename connections_list::const_iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             (*it)->getdest()->signal_disconnect(this);
//             delete *it;
//
//             ++it;
//         }
//
//         _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
//     }
//
//     void disconnect(basic_has_slots* pclass)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             if((*it)->getdest() == pclass) {
//                 delete *it;
//                 _connected_slots.erase(it);
//                 pclass->signal_disconnect(this);
//                 return;
//             }
//
//             ++it;
//         }
//     }
//
//     void slot_disconnect(basic_has_slots* pslot)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             typename connections_list::iterator itNext = it;
//             ++itNext;
//
//             if((*it)->getdest() == pslot) {
//                 _connected_slots.erase(it);
//                 //			delete *it;
//             }
//
//             it = itNext;
//         }
//     }
//
//     bool isConnected() const { return _connected_slots.size() > 0; }
//
// protected:
//     connections_list _connected_slots;
// };
//
// template <class Arg1, class Arg2, class Arg3, class Arg4,
// class Arg5, class Arg6, class mutex_type>
// class _signal_base6 : public _signal_base<mutex_type>
// {
// public:
//     typedef std::list<_connection_base6<Arg1, Arg2, Arg3,
//         Arg4, Arg5, Arg6, mutex_type> *>  connections_list;
//
//     _signal_base6()
//     {}
//
//     _signal_base6(const _signal_base6<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, mutex_type>& s)
//         : _signal_base<mutex_type>(s)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::const_iterator it = s._connected_slots.begin();
//         typename connections_list::const_iterator itEnd = s._connected_slots.end();
//
//         while (it != itEnd) {
//             (*it)->getdest()->signal_connect(this);
//             _connected_slots.push_back((*it)->clone());
//
//             ++it;
//         }
//     }
//
//     void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while (it != itEnd) {
//             if((*it)->getdest() == oldtarget) {
//                 _connected_slots.push_back((*it)->duplicate(newtarget));
//             }
//
//             ++it;
//         }
//     }
//
//     ~_signal_base6()
//     {
//         disconnect_all();
//     }
//
//     void disconnect_all()
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::const_iterator it = _connected_slots.begin();
//         typename connections_list::const_iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             (*it)->getdest()->signal_disconnect(this);
//             delete *it;
//
//             ++it;
//         }
//
//         _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
//     }
//
//     void disconnect(basic_has_slots* pclass)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             if((*it)->getdest() == pclass) {
//                 delete *it;
//                 _connected_slots.erase(it);
//                 pclass->signal_disconnect(this);
//                 return;
//             }
//
//             ++it;
//         }
//     }
//
//     void slot_disconnect(basic_has_slots* pslot)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             typename connections_list::iterator itNext = it;
//             ++itNext;
//
//             if((*it)->getdest() == pslot) {
//                 _connected_slots.erase(it);
//                 //			delete *it;
//             }
//
//             it = itNext;
//         }
//     }
//
//     bool isConnected() const { return _connected_slots.size() > 0; }
//
// protected:
//     connections_list _connected_slots;
// };
//
// template <class Arg1, class Arg2, class Arg3, class Arg4,
// class Arg5, class Arg6, class Arg7, class mutex_type>
// class _signal_base7 : public _signal_base<mutex_type>
// {
// public:
//     typedef std::list<_connection_base7<Arg1, Arg2, Arg3,
//         Arg4, Arg5, Arg6, Arg7, mutex_type> *>  connections_list;
//
//     _signal_base7()
//     {}
//
//     _signal_base7(const _signal_base7<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, mutex_type>& s)
//         : _signal_base<mutex_type>(s)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::const_iterator it = s._connected_slots.begin();
//         typename connections_list::const_iterator itEnd = s._connected_slots.end();
//
//         while(it != itEnd) {
//             (*it)->getdest()->signal_connect(this);
//             _connected_slots.push_back((*it)->clone());
//
//             ++it;
//         }
//     }
//
//     void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             if((*it)->getdest() == oldtarget) {
//                 _connected_slots.push_back((*it)->duplicate(newtarget));
//             }
//
//             ++it;
//         }
//     }
//
//     ~_signal_base7()
//     {
//         disconnect_all();
//     }
//
//     void disconnect_all ()
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::const_iterator it = _connected_slots.begin();
//         typename connections_list::const_iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             (*it)->getdest()->signal_disconnect(this);
//             delete *it;
//
//             ++it;
//         }
//
//         _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
//     }
//
//     void disconnect(basic_has_slots* pclass)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             if((*it)->getdest() == pclass) {
//                 delete *it;
//                 _connected_slots.erase(it);
//                 pclass->signal_disconnect(this);
//                 return;
//             }
//
//             ++it;
//         }
//     }
//
//     void slot_disconnect(basic_has_slots* pslot)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             typename connections_list::iterator itNext = it;
//             ++itNext;
//
//             if((*it)->getdest() == pslot) {
//                 _connected_slots.erase(it);
//                 //			delete *it;
//             }
//
//             it = itNext;
//         }
//     }
//
//     bool isConnected() const { return _connected_slots.size() > 0; }
//
// protected:
//     connections_list _connected_slots;
// };
//
// template <class Arg1, class Arg2, class Arg3, class Arg4
//         , class Arg5, class Arg6, class Arg7, class Arg8, class mutex_type>
// class _signal_base8 : public _signal_base<mutex_type>
// {
// public:
//     typedef std::list<_connection_base8<Arg1, Arg2, Arg3,
//         Arg4, Arg5, Arg6, Arg7, Arg8, mutex_type> *>
//         connections_list;
//
//     _signal_base8()
//     {}
//
//     _signal_base8(const _signal_base8<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, Arg8, mutex_type>& s)
//         : _signal_base<mutex_type>(s)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::const_iterator it = s._connected_slots.begin();
//         typename connections_list::const_iterator itEnd = s._connected_slots.end();
//
//         while(it != itEnd) {
//             (*it)->getdest()->signal_connect(this);
//             _connected_slots.push_back((*it)->clone());
//
//             ++it;
//         }
//     }
//
//     void slot_duplicate(const basic_has_slots* oldtarget, basic_has_slots* newtarget)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd)
//         {
//             if((*it)->getdest() == oldtarget)
//             {
//                 _connected_slots.push_back((*it)->duplicate(newtarget));
//             }
//
//             ++it;
//         }
//     }
//
//     ~_signal_base8()
//     {
//         disconnect_all();
//     }
//
//     void disconnect_all()
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::const_iterator it = _connected_slots.begin();
//         typename connections_list::const_iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             (*it)->getdest()->signal_disconnect(this);
//             delete *it;
//
//             ++it;
//         }
//
//         _connected_slots.erase(_connected_slots.begin(), _connected_slots.end());
//     }
//
//     void disconnect(basic_has_slots* pclass)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd) {
//             if((*it)->getdest() == pclass) {
//                 delete *it;
//                 _connected_slots.erase(it);
//                 pclass->signal_disconnect(this);
//                 return;
//             }
//
//             ++it;
//         }
//     }
//
//     void slot_disconnect(basic_has_slots* pslot)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename connections_list::iterator it = _connected_slots.begin();
//         typename connections_list::iterator itEnd = _connected_slots.end();
//
//         while(it != itEnd)
//         {
//             typename connections_list::iterator itNext = it;
//             ++itNext;
//
//             if((*it)->getdest() == pslot)
//             {
//                 _connected_slots.erase(it);
//                 //			delete *it;
//             }
//
//             it = itNext;
//         }
//     }
//
//     bool isConnected() const { return _connected_slots.size() > 0; }
//
// protected:
//     connections_list _connected_slots;
// };

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

template <class dest_type, class Arg1>
class _connection1 : public _connection_base1<Arg1>
{
public:
    _connection1()
        : _pobject(0)
        , _pmemfun(0)
    {}

    _connection1 (dest_type* pobject, void (dest_type::*pmemfun)(Arg1))
        : _pobject(pobject)
        , _pmemfun(pmemfun)
    {}

    virtual _connection_base1<Arg1>* clone()
    {
        return new _connection1<dest_type, Arg1>(*this);
    }

    virtual _connection_base1<Arg1>* duplicate(basic_has_slots* pnewdest)
    {
        return new _connection1<dest_type, Arg1>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(Arg1 a1)
    {
        if (_pobject->use_async_slots())
            _pobject->callback_queue().template push_method<dest_type
                    , Arg1>(_pmemfun, _pobject, a1);
        else
            (_pobject->*_pmemfun)(a1);
    }

    virtual void sync_emit (Arg1 a1) pfs_override
    {
        (_pobject->*_pmemfun)(a1);
    }

    virtual void async_emit (Arg1 a1) pfs_override
    {
        _pobject->callback_queue().template push_method<dest_type
                , Arg1>(_pmemfun, _pobject, a1);
    }

    virtual basic_has_slots* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(Arg1);
};

template <class dest_type, class Arg1, class Arg2>
class _connection2 : public _connection_base2<Arg1, Arg2>
{
public:
    _connection2()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection2(dest_type* pobject, void (dest_type::*pmemfun)(Arg1,
        Arg2))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    //~_connection2() {}

    virtual _connection_base2<Arg1, Arg2>* clone()
    {
        return new _connection2<dest_type, Arg1, Arg2>(*this);
    }

    virtual _connection_base2<Arg1, Arg2>* duplicate(basic_has_slots* pnewdest)
    {
        return new _connection2<dest_type, Arg1, Arg2>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(Arg1 a1, Arg2 a2)
    {
        if (_pobject->use_async_slots())
            _pobject->callback_queue().template push_method<dest_type
                    , Arg1
                    , Arg2>(_pmemfun, _pobject, a1, a2);
        else
            (_pobject->*_pmemfun)(a1, a2);
    }

    virtual void sync_emit (Arg1 a1, Arg2 a2) pfs_override
    {
        (_pobject->*_pmemfun)(a1, a2);
    }

    virtual void async_emit (Arg1 a1, Arg2 a2) pfs_override
    {
        _pobject->callback_queue().template push_method<dest_type
                , Arg1
                , Arg2>(_pmemfun, _pobject, a1, a2);
    }

    virtual basic_has_slots* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(Arg1, Arg2);
};

template <class dest_type, class Arg1, class Arg2, class Arg3>
class _connection3 : public _connection_base3<Arg1, Arg2, Arg3>
{
public:
    _connection3()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection3(dest_type* pobject, void (dest_type::*pmemfun)(Arg1,
        Arg2, Arg3))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base3<Arg1, Arg2, Arg3>* clone()
    {
        return new _connection3<dest_type, Arg1, Arg2, Arg3>(*this);
    }

    virtual _connection_base3<Arg1, Arg2, Arg3>* duplicate(basic_has_slots* pnewdest)
    {
        return new _connection3<dest_type, Arg1, Arg2, Arg3>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(Arg1 a1, Arg2 a2, Arg3 a3)
    {
        if (_pobject->use_async_slots())
            _pobject->callback_queue().template push_method<dest_type
                , Arg1
                , Arg2
                , Arg3>(_pmemfun, _pobject, a1, a2, a3);
        else
            (_pobject->*_pmemfun)(a1, a2, a3);
    }

    virtual void sync_emit (Arg1 a1, Arg2 a2, Arg3 a3) pfs_override
    {
        (_pobject->*_pmemfun)(a1, a2, a3);
    }

    virtual void async_emit (Arg1 a1, Arg2 a2, Arg3 a3) pfs_override
    {
        _pobject->callback_queue().template push_method<dest_type
            , Arg1
            , Arg2
            , Arg3>(_pmemfun, _pobject, a1, a2, a3);
    }

    virtual basic_has_slots* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(Arg1, Arg2, Arg3);
};

template <class dest_type, class Arg1, class Arg2, class Arg3, class Arg4>
class _connection4 : public _connection_base4<Arg1, Arg2, Arg3, Arg4>
{
public:
    _connection4()
    {
        _pobject = NULL;
        _pmemfun = NULL;
    }

    _connection4(dest_type* pobject, void (dest_type::*pmemfun)(Arg1,
        Arg2, Arg3, Arg4))
    {
        _pobject = pobject;
        _pmemfun = pmemfun;
    }

    virtual _connection_base4<Arg1, Arg2, Arg3, Arg4>* clone()
    {
        return new _connection4<dest_type, Arg1, Arg2, Arg3, Arg4>(*this);
    }

    virtual _connection_base4<Arg1, Arg2, Arg3, Arg4>* duplicate(basic_has_slots* pnewdest)
    {
        return new _connection4<dest_type, Arg1, Arg2, Arg3, Arg4>(static_cast<dest_type *>(pnewdest), _pmemfun);
    }

    virtual void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
    {
        if (_pobject->use_async_slots())
            _pobject->callback_queue().template push_method<dest_type
                , Arg1
                , Arg2
                , Arg3
                , Arg4>(_pmemfun, _pobject, a1, a2, a3, a4);
        else
            (_pobject->*_pmemfun)(a1, a2, a3, a4);
    }

    virtual void sync_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) pfs_override
    {
        (_pobject->*_pmemfun)(a1, a2, a3, a4);
    }

    virtual void async_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) pfs_override
    {
        _pobject->callback_queue().template push_method<dest_type
            , Arg1
            , Arg2
            , Arg3
            , Arg4>(_pmemfun, _pobject, a1, a2, a3, a4);
    }

    virtual basic_has_slots* getdest() const
    {
        return _pobject;
    }

private:
    dest_type* _pobject;
    void (dest_type::* _pmemfun)(Arg1, Arg2, Arg3,
        Arg4);
};

// template <class dest_type, class Arg1, class Arg2, class Arg3,
// class Arg4, class Arg5>
// class _connection5 : public _connection_base5<Arg1, Arg2,
//     Arg3, Arg4, Arg5, mutex_type>
// {
// public:
//     _connection5()
//     {
//         _pobject = NULL;
//         _pmemfun = NULL;
//     }
//
//     _connection5(dest_type* pobject, void (dest_type::*pmemfun)(Arg1,
//         Arg2, Arg3, Arg4, Arg5))
//     {
//         _pobject = pobject;
//         _pmemfun = pmemfun;
//     }
//
//     virtual _connection_base5<Arg1, Arg2, Arg3, Arg4,
//         Arg5, mutex_type>* clone()
//     {
//         return new _connection5<dest_type, Arg1, Arg2, Arg3, Arg4,
//             Arg5, mutex_type>(*this);
//     }
//
//     virtual _connection_base5<Arg1, Arg2, Arg3, Arg4,
//         Arg5, mutex_type>* duplicate(basic_has_slots* pnewdest)
//     {
//         return new _connection5<dest_type, Arg1, Arg2, Arg3, Arg4,
//             Arg5, mutex_type>(static_cast<dest_type *>(pnewdest), _pmemfun);
//     }
//
//     virtual void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
//     {
//         if (_pobject->use_async_slots())
//             _pobject->callback_queue().template push_method<dest_type
//                 , Arg1
//                 , Arg2
//                 , Arg3
//                 , Arg4
//                 , Arg5>(_pmemfun, _pobject, a1, a2, a3, a4, a5);
//         else
//             (_pobject->*_pmemfun)(a1, a2, a3, a4, a5);
//     }
//
//     virtual void sync_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) pfs_override
//     {
//         (_pobject->*_pmemfun)(a1, a2, a3, a4, a5);
//     }
//
//     virtual void async_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) pfs_override
//     {
//         _pobject->callback_queue().template push_method<dest_type
//             , Arg1
//             , Arg2
//             , Arg3
//             , Arg4
//             , Arg5>(_pmemfun, _pobject, a1, a2, a3, a4, a5);
//     }
//
//     virtual basic_has_slots* getdest() const
//     {
//         return _pobject;
//     }
//
// private:
//     dest_type* _pobject;
//     void (dest_type::* _pmemfun)(Arg1, Arg2, Arg3, Arg4,
//         Arg5);
// };
//
// template <class dest_type, class Arg1, class Arg2, class Arg3,
// class Arg4, class Arg5, class Arg6, class mutex_type>
// class _connection6 : public _connection_base6<Arg1, Arg2,
//     Arg3, Arg4, Arg5, Arg6, mutex_type>
// {
// public:
//     _connection6()
//     {
//         _pobject = NULL;
//         _pmemfun = NULL;
//     }
//
//     _connection6(dest_type* pobject, void (dest_type::*pmemfun)(Arg1,
//         Arg2, Arg3, Arg4, Arg5, Arg6))
//     {
//         _pobject = pobject;
//         _pmemfun = pmemfun;
//     }
//
//     virtual _connection_base6<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, mutex_type>* clone()
//     {
//         return new _connection6<dest_type, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, mutex_type>(*this);
//     }
//
//     virtual _connection_base6<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, mutex_type>* duplicate(basic_has_slots* pnewdest)
//     {
//         return new _connection6<dest_type, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, mutex_type>(static_cast<dest_type *>(pnewdest), _pmemfun);
//     }
//
//     virtual void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
//     {
//         if (_pobject->use_async_slots())
//             _pobject->callback_queue().template push_method<dest_type
//                 , Arg1
//                 , Arg2
//                 , Arg3
//                 , Arg4
//                 , Arg5
//                 , Arg6>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6);
//         else
//             (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6);
//     }
//
//     virtual void sync_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) pfs_override
//     {
//         (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6);
//     }
//
//     virtual void async_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) pfs_override
//     {
//         _pobject->callback_queue().template push_method<dest_type
//             , Arg1
//             , Arg2
//             , Arg3
//             , Arg4
//             , Arg5
//             , Arg6>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6);
//     }
//
//     virtual basic_has_slots* getdest() const
//     {
//         return _pobject;
//     }
//
// private:
//     dest_type* _pobject;
//     void (dest_type::* _pmemfun)(Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6);
// };
//
// template <class dest_type, class Arg1, class Arg2, class Arg3,
// class Arg4, class Arg5, class Arg6, class Arg7, class mutex_type>
// class _connection7 : public _connection_base7<Arg1, Arg2,
//     Arg3, Arg4, Arg5, Arg6, Arg7, mutex_type>
// {
// public:
//     _connection7()
//     {
//         _pobject = NULL;
//         _pmemfun = NULL;
//     }
//
//     _connection7(dest_type* pobject, void (dest_type::*pmemfun)(Arg1,
//         Arg2, Arg3, Arg4, Arg5, Arg6, Arg7))
//     {
//         _pobject = pobject;
//         _pmemfun = pmemfun;
//     }
//
//     virtual _connection_base7<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, mutex_type>* clone()
//     {
//         return new _connection7<dest_type, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, Arg7, mutex_type>(*this);
//     }
//
//     virtual _connection_base7<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, mutex_type>* duplicate(basic_has_slots* pnewdest)
//     {
//         return new _connection7<dest_type, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, Arg7, mutex_type>(static_cast<dest_type *>(pnewdest), _pmemfun);
//     }
//
//     virtual void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4
//             , Arg5 a5, Arg6 a6, Arg7 a7)
//     {
//         if (_pobject->use_async_slots())
//             _pobject->callback_queue().template push_method<dest_type
//                 , Arg1
//                 , Arg2
//                 , Arg3
//                 , Arg4
//                 , Arg5
//                 , Arg6
//                 , Arg7>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6, a7);
//         else
//             (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7);
//     }
//
//     virtual void sync_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4
//             , Arg5 a5, Arg6 a6, Arg7 a7) pfs_override
//     {
//         (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7);
//     }
//
//     virtual void async_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4
//             , Arg5 a5, Arg6 a6, Arg7 a7) pfs_override
//     {
//         _pobject->callback_queue().template push_method<dest_type
//             , Arg1
//             , Arg2
//             , Arg3
//             , Arg4
//             , Arg5
//             , Arg6
//             , Arg7>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6, a7);
//     }
//
//     virtual basic_has_slots* getdest() const
//     {
//         return _pobject;
//     }
//
// private:
//     dest_type* _pobject;
//     void (dest_type::* _pmemfun)(Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7);
// };
//
// template <class dest_type, class Arg1, class Arg2, class Arg3,
// class Arg4, class Arg5, class Arg6, class Arg7,
// class Arg8, class mutex_type>
// class _connection8 : public _connection_base8<Arg1, Arg2,
//     Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, mutex_type>
// {
// public:
//     _connection8()
//     {
//         _pobject = NULL;
//         _pmemfun = NULL;
//     }
//
//     _connection8(dest_type* pobject, void (dest_type::*pmemfun)(Arg1,
//         Arg2, Arg3, Arg4, Arg5, Arg6,
//         Arg7, Arg8))
//     {
//         _pobject = pobject;
//         _pmemfun = pmemfun;
//     }
//
//     virtual _connection_base8<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, Arg8, mutex_type>* clone()
//     {
//         return new _connection8<dest_type, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, Arg7, Arg8, mutex_type>(*this);
//     }
//
//     virtual _connection_base8<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, Arg8, mutex_type>* duplicate(basic_has_slots* pnewdest)
//     {
//         return new _connection8<dest_type, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, Arg7, Arg8, mutex_type>(static_cast<dest_type *>(pnewdest), _pmemfun);
//     }
//
//     virtual void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4
//             , Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
//     {
//         if (_pobject->use_async_slots())
//             _pobject->callback_queue().template push_method<dest_type
//                 , Arg1
//                 , Arg2
//                 , Arg3
//                 , Arg4
//                 , Arg5
//                 , Arg6
//                 , Arg7
//                 , Arg8>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6, a7, a8);
//         else
//             (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7, a8);
//     }
//
//     virtual void sync_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4
//             , Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) pfs_override
//     {
//         (_pobject->*_pmemfun)(a1, a2, a3, a4, a5, a6, a7, a8);
//     }
//
//     virtual void async_emit (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4
//             , Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) pfs_override
//     {
//         _pobject->callback_queue().template push_method<dest_type
//             , Arg1
//             , Arg2
//             , Arg3
//             , Arg4
//             , Arg5
//             , Arg6
//             , Arg7
//             , Arg8>(_pmemfun, _pobject, a1, a2, a3, a4, a5, a6, a7, a8);
//     }
//
//     virtual basic_has_slots* getdest() const
//     {
//         return _pobject;
//     }
//
// private:
//     dest_type* _pobject;
//     void (dest_type::* _pmemfun)(Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, Arg8);
// };

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

template <class Arg1>
class signal1 : public _signal_base1<Arg1>
{
    typedef _signal_base1<Arg1> __base_class;

public:
    signal1()
    {}

    signal1(const signal1<Arg1>& s)
        : _signal_base1<Arg1>(s)
    {}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(Arg1))
    {
        lock_guard<mutex_type> lock(*this);
        _connection1<desttype, Arg1>* conn =
            new _connection1<desttype, Arg1>(pclass, pmemfun);
        this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(Arg1 a1)
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

    void operator()(Arg1 a1)
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

template <class Arg1, class Arg2>
class signal2 : public _signal_base2<Arg1, Arg2>
{
    typedef _signal_base2<Arg1, Arg2> __base_class;
public:
    signal2()
    {}

    signal2(const signal2<Arg1, Arg2>& s)
        : _signal_base2<Arg1, Arg2>(s)
    {}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(Arg1,
        Arg2))
    {
        lock_guard<mutex_type> lock(*this);
        _connection2<desttype, Arg1, Arg2>* conn = new
            _connection2<desttype, Arg1, Arg2>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(Arg1 a1, Arg2 a2)
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

    void operator()(Arg1 a1, Arg2 a2)
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

template <class Arg1, class Arg2, class Arg3>
class signal3 : public _signal_base3<Arg1, Arg2, Arg3>
{
    typedef  _signal_base3<Arg1, Arg2, Arg3> __base_class;
public:
    signal3()
    {}

    signal3(const signal3<Arg1, Arg2, Arg3>& s)
        : _signal_base3<Arg1, Arg2, Arg3>(s)
    {}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(Arg1,
        Arg2, Arg3))
    {
        lock_guard<mutex_type> lock(*this);
        _connection3<desttype, Arg1, Arg2, Arg3>* conn =
            new _connection3<desttype, Arg1, Arg2, Arg3>(pclass,
            pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(Arg1 a1, Arg2 a2, Arg3 a3)
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

    void operator () (Arg1 a1, Arg2 a2, Arg3 a3)
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

template <class Arg1, class Arg2, class Arg3, class Arg4>
class signal4 : public _signal_base4<Arg1, Arg2, Arg3,
    Arg4>
{
    typedef _signal_base4<Arg1, Arg2, Arg3, Arg4> __base_class;
public:
    signal4()
    {;}

    signal4(const signal4<Arg1, Arg2, Arg3, Arg4>& s)
        : __base_class(s)
    {;}

    template <class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(Arg1,
        Arg2, Arg3, Arg4))
    {
        lock_guard<mutex_type> lock(*this);
        _connection4<desttype, Arg1, Arg2, Arg3, Arg4>*
            conn = new _connection4<desttype, Arg1, Arg2, Arg3, Arg4>(pclass, pmemfun);
            this->_connected_slots.push_back(conn);
        pclass->signal_connect(this);
    }

    void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
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

    void operator()(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
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

// template <class Arg1, class Arg2, class Arg3, class Arg4,
// class Arg5, class mutex_type = PFS_DEFAULT_MT_POLICY>
// class signal5 : public _signal_base5<Arg1, Arg2, Arg3,
//     Arg4, Arg5, mutex_type>
// {
//     typedef _signal_base5<Arg1, Arg2, Arg3,
//     Arg4, Arg5, mutex_type> __base_class;
//
// public:
//     signal5()
//     {;}
//
//     signal5(const signal5<Arg1, Arg2, Arg3, Arg4,
//         Arg5, mutex_type>& s)
//         : _signal_base5<Arg1, Arg2, Arg3, Arg4,
//         Arg5, mutex_type>(s)
//     {;}
//
//     template <class desttype>
//         void connect(desttype* pclass, void (desttype::*pmemfun)(Arg1,
//         Arg2, Arg3, Arg4, Arg5))
//     {
//         lock_guard<mutex_type> lock(*this);
//         _connection5<desttype, Arg1, Arg2, Arg3, Arg4,
//             Arg5, mutex_type>* conn = new _connection5<desttype, Arg1, Arg2,
//             Arg3, Arg4, Arg5, mutex_type>(pclass, pmemfun);
//             this->_connected_slots.push_back(conn);
//         pclass->signal_connect(this);
//     }
//
//     void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
//         typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();
//
//         while(it != itEnd) {
//             itNext = it;
//             ++itNext;
//
//             (*it)->emit_(a1, a2, a3, a4, a5);
//
//             it = itNext;
//         }
//     }
//
//     void operator()(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4,
//         Arg5 a5)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
//         typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();
//
//         while (it != itEnd) {
//             itNext = it;
//             ++itNext;
//
//             (*it)->emit_(a1, a2, a3, a4, a5);
//
//             it = itNext;
//         }
//     }
// };
//
//
// template <class Arg1, class Arg2, class Arg3, class Arg4,
// class Arg5, class Arg6, class mutex_type = PFS_DEFAULT_MT_POLICY>
// class signal6 : public _signal_base6<Arg1, Arg2, Arg3,
//     Arg4, Arg5, Arg6, mutex_type>
// {
//     typedef _signal_base6<Arg1, Arg2, Arg3,
//             Arg4, Arg5, Arg6, mutex_type> __base_class;
// public:
//     signal6 ()
//     {
//         ;
//     }
//
//     signal6(const signal6<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, mutex_type>& s)
//         : _signal_base6<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, mutex_type>(s)
//     {
//         ;
//     }
//
//     template <class desttype>
//         void connect(desttype* pclass, void (desttype::*pmemfun)(Arg1,
//         Arg2, Arg3, Arg4, Arg5, Arg6))
//     {
//         lock_guard<mutex_type> lock(*this);
//         _connection6<desttype, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, mutex_type>* conn =
//             new _connection6<desttype, Arg1, Arg2, Arg3,
//             Arg4, Arg5, Arg6, mutex_type>(pclass, pmemfun);
//             this->_connected_slots.push_back(conn);
//         pclass->signal_connect(this);
//     }
//
//     void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4,
//         Arg5 a5, Arg6 a6)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
//         typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();
//
//         while(it != itEnd) {
//             itNext = it;
//             ++itNext;
//
//             (*it)->emit_(a1, a2, a3, a4, a5, a6);
//
//             it = itNext;
//         }
//     }
//
//     void operator()(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4,
//         Arg5 a5, Arg6 a6)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
//         typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();
//
//         while(it != itEnd) {
//             itNext = it;
//             ++itNext;
//
//             (*it)->emit_(a1, a2, a3, a4, a5, a6);
//
//             it = itNext;
//         }
//     }
// };
//
// template <class Arg1
//         , class Arg2
//         , class Arg3
//         , class Arg4
//         , class Arg5
//         , class Arg6
//         , class Arg7
//         , class mutex_type = PFS_DEFAULT_MT_POLICY>
// class signal7 : public _signal_base7<Arg1, Arg2, Arg3,
//     Arg4, Arg5, Arg6, Arg7, mutex_type>
// {
//     typedef _signal_base7<Arg1, Arg2, Arg3,
//             Arg4, Arg5, Arg6, Arg7, mutex_type> __base_class;
// public:
//     signal7()
//     {
//         ;
//     }
//
//     signal7(const signal7<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, mutex_type>& s)
//         : _signal_base7<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, mutex_type>(s)
//     {
//         ;
//     }
//
//     template <class desttype>
//         void connect(desttype* pclass, void (desttype::*pmemfun)(Arg1,
//         Arg2, Arg3, Arg4, Arg5, Arg6,
//         Arg7))
//     {
//         lock_guard<mutex_type> lock(*this);
//         _connection7<desttype, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, Arg7, mutex_type>* conn =
//             new _connection7<desttype, Arg1, Arg2, Arg3,
//             Arg4, Arg5, Arg6, Arg7, mutex_type>(pclass, pmemfun);
//             this->_connected_slots.push_back(conn);
//         pclass->signal_connect(this);
//     }
//
//     void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4,
//         Arg5 a5, Arg6 a6, Arg7 a7)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
//         typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();
//
//         while(it != itEnd)
//         {
//             itNext = it;
//             ++itNext;
//
//             (*it)->emit_(a1, a2, a3, a4, a5, a6, a7);
//
//             it = itNext;
//         }
//     }
//
//     void operator()(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4,
//         Arg5 a5, Arg6 a6, Arg7 a7)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
//         typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();
//
//         while(it != itEnd)
//         {
//             itNext = it;
//             ++itNext;
//
//             (*it)->emit_(a1, a2, a3, a4, a5, a6, a7);
//
//             it = itNext;
//         }
//     }
// };
//
// template <class Arg1
//         , class Arg2
//         , class Arg3
//         , class Arg4
//         , class Arg5
//         , class Arg6
//         , class Arg7
//         , class Arg8
//         , class mutex_type = PFS_DEFAULT_MT_POLICY>
// class signal8 : public _signal_base8<Arg1, Arg2, Arg3,
//     Arg4, Arg5, Arg6, Arg7, Arg8, mutex_type>
// {
//     typedef _signal_base8<Arg1, Arg2, Arg3,
//     Arg4, Arg5, Arg6, Arg7, Arg8, mutex_type> __base_class;
//
// public:
//     signal8()
//     {
//         ;
//     }
//
//     signal8 (const signal8<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, Arg8, mutex_type>& s)
//         : _signal_base8<Arg1, Arg2, Arg3, Arg4,
//         Arg5, Arg6, Arg7, Arg8, mutex_type>(s)
//     {
//         ;
//     }
//
//     template <class desttype>
//         void connect(desttype* pclass, void (desttype::*pmemfun)(Arg1,
//         Arg2, Arg3, Arg4, Arg5, Arg6,
//         Arg7, Arg8))
//     {
//         lock_guard<mutex_type> lock(*this);
//         _connection8<desttype, Arg1, Arg2, Arg3, Arg4,
//             Arg5, Arg6, Arg7, Arg8, mutex_type>* conn =
//             new _connection8<desttype, Arg1, Arg2, Arg3,
//             Arg4, Arg5, Arg6, Arg7,
//             Arg8, mutex_type>(pclass, pmemfun);
//             this->_connected_slots.push_back(conn);
//         pclass->signal_connect(this);
//     }
//
//     void emit_(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4,
//         Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
//         typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();
//
//         while(it != itEnd) {
//             itNext = it;
//             ++itNext;
//
//             (*it)->emit_(a1, a2, a3, a4, a5, a6, a7, a8);
//
//             it = itNext;
//         }
//     }
//
//     void operator()(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4,
//         Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
//     {
//         lock_guard<mutex_type> lock(*this);
//         typename __base_class::connections_list::const_iterator itNext, it = this->_connected_slots.begin();
//         typename __base_class::connections_list::const_iterator itEnd = this->_connected_slots.end();
//
//         while(it != itEnd) {
//             itNext = it;
//             ++itNext;
//
//             (*it)->emit_(a1, a2, a3, a4, a5, a6, a7, a8);
//
//             it = itNext;
//         }
//     }
// };

}; // struct sigslot

} // pfs
