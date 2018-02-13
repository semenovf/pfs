#ifndef __PFS_COMMAND_HPP__
#define __PFS_COMMAND_HPP__

#include <pfs/memory.hpp>
#include <pfs/ring_iterator.hpp>
#include <pfs/traits/sequence_container.hpp>

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
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>());
}

template <typename ConcreteCommand, typename A1>
shared_ptr<command> make_command (A1 a1)
{
    return static_pointer_cast<command>(make_shared<ConcreteCommand>(a1));
}

template <typename ConcreteCommand, typename A1, typename A2>
shared_ptr<command> make_command (A1 a1, A2 a2)
{
    return static_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3)
{
    return static_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4)
{
    return static_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return static_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4, a5));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    return static_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    return static_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6, a7));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    return static_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6, a7, a8));
}

template <template <typename> class SequenceContainerImpl>
struct invoker
{
    typedef traits::sequence_container<shared_ptr<command>
            , SequenceContainerImpl>                           queue_type;
    typedef ring_iterator<typename queue_type::iterator>       iterator;
    typedef ring_iterator<typename queue_type::const_iterator> const_iterator;

    invoker (size_t undo_limit)
    {
        _undo_queue.resize(undo_limit);
        _pos = iterator(_undo_queue.begin(), _undo_queue.end());
        _first = _pos;
        _last = _pos;
    }

    void clear () 
    {
        _undo_queue.clear();
        _pos = iterator(_undo_queue.begin(), _undo_queue.end());
        _last = _pos;
    }
    
    void exec (shared_ptr<command> cmd)
    {
        cmd->exec();
        *_pos++ = cmd;
        _last = _pos;
    }

    void undo (int count = 1)
    {
        while (count-- > 0 && _pos != _first)
            (*--_pos)->undo();
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

} // pfs::command

#endif /* __PFS_COMMAND_HPP__ */
