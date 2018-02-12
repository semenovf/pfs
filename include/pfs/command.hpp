#ifndef __PFS_COMMAND_HPP__
#define __PFS_COMMAND_HPP__

#include <pfs/memory.hpp>
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
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>(a1));
}

template <typename ConcreteCommand, typename A1, typename A2>
shared_ptr<command> make_command (A1 a1, A2 a2)
{
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3)
{
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4)
{
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4, a5));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6, a7));
}

template <typename ConcreteCommand, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
shared_ptr<command> make_command (A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    return dynamic_pointer_cast<command>(make_shared<ConcreteCommand>(a1, a2, a3, a4, a5, a6, a7, a8));
}

template <template <typename> class SequenceContainerImpl, size_t UndoLimit>
struct invoker
{
    invoker ()
        : _pos(_undo_queue.end())
    {}

    void exec (shared_ptr<command> cmd)
    {
        cmd->exec();

        if (_undo_queue.size() < UndoLimit) {
           _undo_queue.push_back(cmd);
           _pos = _undo_queue.end();
        } else {

        }

        *_pos++ = cmd;

//        if (_pos != _undo_queue.cend())
//            _undo_queue.erase(_pos, _undo_queue.cend());
//
//        _undo_queue.push_back(cmd);
//        _pos = _undo_queue.cend();
    }

    void undo (int count = 1)
    {
        while (count-- > 0 && _pos != _undo_queue.begin())
            (*--_pos)->undo();
    }

    void redo (int count = 1)
    {
        while (count-- > 0 && _pos != _undo_queue.end())
            (*_pos++)->exec();
    }

protected:
    typedef traits::sequence_container<shared_ptr<command>
            , SequenceContainerImpl>            queue_type;
    typedef typename queue_type::iterator       iterator;
    typedef typename queue_type::const_iterator const_iterator;

    queue_type _undo_queue;
    iterator   _pos;
};

} // pfs::command

#endif /* __PFS_COMMAND_HPP__ */
