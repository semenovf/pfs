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
//      For C++98: Add make_command() up to 8 arguments

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

// TODO Command limit support 

template <template <typename> class SequenceContainerImpl>
struct invoker
{
    invoker ()
        : _pos(_command_queue.cend())
    {}

    void exec (shared_ptr<command> cmd)
    {
        cmd->exec();

        if (_pos != _command_queue.cend())
            _command_queue.erase(_pos, _command_queue.cend());

        _command_queue.push_back(cmd);
        _pos = _command_queue.cend();
    }

    void undo (int count = 1)
    {
        while (count-- > 0 && _pos != _command_queue.begin())
            (*--_pos)->undo();
    }

    void redo (int count = 1)
    {
        while (count-- > 0 && _pos != _command_queue.end())
            (*_pos++)->exec();
    }

protected:
    typedef traits::sequence_container<shared_ptr<command>
            , SequenceContainerImpl>            queue_type;
    typedef typename queue_type::iterator       iterator;
    typedef typename queue_type::const_iterator const_iterator;

    queue_type     _command_queue;
    const_iterator _pos;
};

} // pfs::command

#endif /* __PFS_COMMAND_HPP__ */

