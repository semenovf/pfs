#pragma once
#include <pfs/memory.hpp>
#include <pfs/ring_iterator.hpp>
#include <pfs/list.hpp>

//
//                                         -----------------
//  --------------                         |    Command    |
//  |  Invoker   |<>---------------------->|----------------
//  --------------                         |   Execute()   |
//                                         -----------------
//  --------------                                ^
//  |  Client    |------------------------------- |
//  --------------                              | |
//        |                                     V |
//        |            -------------       -----------------
//        |            | Receiver  |       |ConcreteCommand|
//        |----------->|-----------|<------|---------------|
//                     |  Action   |       |   Execute()   |
//                     -------------       -----------------
//
// Command is an interface for executing an action.
//
// ConcreteCommand specifies the binding between a Receiver/action taker and an
//      action invoker. This object is responsible for executing the
//      corresponding operation on the Receiver.
//
// Client creates a ConcreteCommand object and sets its receiver.
//
// Invoker use the Command object to carry out the request.
//
// Receiver contains the real operational logic that needs to be performed
//      on the data.
//
namespace pfs {

struct command
{
    command () {}

    virtual void exec () const = 0;
    virtual void undo () const = 0;
};

// TODO For C++11: Variadic templates

template <typename ConcreteCommand>
shared_ptr<command> make_command ()
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>());
}

template <typename ConcreteCommand, typename A1>
shared_ptr<command> make_command (A1 a1)
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>(a1));
}

template <typename ConcreteCommand, typename A1, typename A2>
shared_ptr<command> make_command (A1 a1, A2 a2)
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>(a1, a2));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3)
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>(a1, a2, a3));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4)
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>(a1, a2, a3, a4));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>(a1, a2, a3, a4, a5));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6, a7));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    return pfs::static_pointer_cast<command>(pfs::make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6, a7, a8));
}

template <template <typename> class SequenceContainer = pfs::list>
struct invoker
{
    typedef SequenceContainer<shared_ptr<command> >            queue_type;
    typedef ring_iterator<typename queue_type::iterator>       iterator;
    typedef ring_iterator<typename queue_type::const_iterator> const_iterator;

    invoker (size_t undo_limit)
    {
        _undo_queue.resize(undo_limit);
        _pos = iterator(_undo_queue.begin(), _undo_queue.end());
        _last = _pos;
        _first = _pos;
        --_first;
    }

    void clear ()
    {
        _undo_queue.clear();
        _pos = iterator(_undo_queue.begin(), _undo_queue.end());
        _last = _pos;
        _first = _pos;
        --_first;
    }

    void exec (shared_ptr<command> cmd)
    {
        cmd->exec();
        *_pos++ = cmd;
        _last = _pos;

        if (_pos == _first)
            ++_first;
    }

    void undo (int count = 1)
    {
        while (count-- > 0 && --_pos != _first)
            (*_pos)->undo();

        if (_pos == _first)
            ++_pos;
    }

    void redo (int count = 1)
    {
        while (count-- > 0 && _pos != _last)
            (*_pos++)->exec();
    }

protected:
    queue_type _undo_queue;
    iterator   _pos;
    iterator   _first;
    iterator   _last;
};

//template <template <typename> class SequenceContainerImpl>
//struct command_history
//{
//    typedef traits::sequence_container<shared_ptr<command>
//            , SequenceContainerImpl>                           queue_type;
//    typedef ring_iterator<typename queue_type::iterator>       iterator;
//    typedef ring_iterator<typename queue_type::const_iterator> const_iterator;
//
//    command_history (size_t limit)
//    {
//        _queue.resize(limit);
//        _pos = iterator(_queue.begin(), _queue.end());
//        _first = _pos;
//        _last = _pos;
//    }
//
//    void clear ()
//    {
//        _queue.clear();
//        _pos = iterator(_queue.begin(), _queue.end());
//        _last = _pos;
//    }
//
//    void push_back (shared_ptr<command> cmd)
//    {
//        *_last++ = cmd;
//        _pos = _last;
//    }
//
//    shared_ptr<command> back () const
//    {
//        return (_queue.size() > 0)
//                ? _queue.back()
//                : shared_ptr<command>();
//    }
//
//    shared_ptr<command> last () const
//    {
//        return back();
//    }
//
//    shared_ptr<command> front () const
//    {
//        return (_queue.size() > 0)
//                ? _queue.front()
//                : shared_ptr<command>();
//    }
//
//    shared_ptr<command> first () const
//    {
//        return front();
//    }
//
//    shared_ptr<command> pop (int count = 1) const
//    {
//        while (count-- > 0)
//        return front();
//    }
//
//protected:
//    queue_type _queue;
//    iterator   _pos;
//    iterator   _first;
//    iterator   _last;
//};

//struct basic_command_factory
//{
//    virtual shared_ptr<command> make () = 0;
//};
//
//template <typename ConcreteCommand>
//struct command_factory : basic_command_factory
//{
//    virtual shared_ptr<command> make () pfs_override
//    {
//        return make_command<ConcreteCommand>();
//    }
//};
//
//template <typename KeyType, template <typename> class AssociativeContainerImplType>
//struct command_mapper
//{
//    typedef KeyType key_type;
//    typedef pfs::traits::associative_container<
//              pfs::traits::kv<key_type, basic_command_factory const *>
//            , AssociativeContainerImplType> map_type;
//
//    void insert (key_type const & key, basic_command_factory const * pcf)
//    {
//        _mapping.insert(key, pcf);
//    }
//
//private:
//    map_type _mapping;
//};

} // pfs
