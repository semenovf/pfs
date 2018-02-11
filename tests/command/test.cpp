#include <iostream>
#include <string>
#include <sstream>
#include <pfs/test/test.hpp>
#include <pfs/cxxlang.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/command.hpp>

static char const * output_sample =
    "Hello, John!\n"
    "Hello, Ann!\n"
    "Hello, Peter!\n"
    "Bye, Peter!\n"
    "Hello, Peter!\n"
    "Bye, Peter!\n"
    "Bye, Ann!\n"
    "Hello, Ann!\n"
    "Hello, Mary!\n"
    "Bye, Mary!\n"
    "Bye, Ann!\n"
    "Bye, John!\n"
    "Hello, John!\n"
    "Hello, Ann!\n"
    "Bye, Ann!\n"
    "Bye, John!\n"
    "Hello, Sam!\n"
    "Bye, Sam!\n"
    "Hello, Sam!\n";

typedef pfs::invoker<pfs::stdcxx::vector> invoker_t;
static std::stringstream output;

struct hello_command : pfs::command
{
    hello_command (char const * name)
        : _name(name)
    {}

    virtual void exec () const pfs_override
    {
        output << "Hello, " << _name << "!\n";
        std::cout << "Hello, " << _name << "!\n";
    }

    virtual void undo () const pfs_override
    {
        output << "Bye, " << _name << "!\n";
        std::cout << "Bye, " << _name << "!\n";
    }

    char const * _name;
};

int main ()
{
    BEGIN_TESTS(1);

    invoker_t invoker;

    pfs::shared_ptr<pfs::command> hello_john  = pfs::make_command<hello_command>("John");
    pfs::shared_ptr<pfs::command> hello_ann   = pfs::make_command<hello_command>("Ann");
    pfs::shared_ptr<pfs::command> hello_peter = pfs::make_command<hello_command>("Peter");
    pfs::shared_ptr<pfs::command> hello_mary  = pfs::make_command<hello_command>("Mary");
    pfs::shared_ptr<pfs::command> hello_sam  = pfs::make_command<hello_command>("Sam");

    invoker.undo();
    invoker.redo();
    invoker.exec(hello_john);
    invoker.exec(hello_ann);
    invoker.exec(hello_peter);
    invoker.undo();
    invoker.redo();
    invoker.undo(2);
    invoker.redo();
    invoker.exec(hello_mary);
    invoker.undo(3);
    invoker.redo(2);
    invoker.undo(10);
    invoker.exec(hello_sam);
    invoker.undo(10);
    invoker.redo(10);

    TEST_OK(output.str() == output_sample);

    return END_TESTS;
}
