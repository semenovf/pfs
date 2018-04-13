#include <iostream>
#include <string>
#include <sstream>
#include <pfs/test.hpp>
#include <pfs/cxxlang.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/traits/stdcxx/map.hpp>
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

struct hello_command : pfs::command
{
    char const *        _name;
    std::stringstream * output;
    
    hello_command (char const * name, std::stringstream * out)
        : _name(name)
        , output(out)
    {}

    void construct (char const * name, std::stringstream * out)
    {
        _name = name;
        output = out;
    }

    virtual void exec () const pfs_override
    {
        (*output) << "Hello, " << _name << "!\n";
        std::cout << "Hello, " << _name << "!\n";
    }

    virtual void undo () const pfs_override
    {
        (*output) << "Bye, " << _name << "!\n";
        std::cout << "Bye, " << _name << "!\n";
    }
};

bool test (size_t limit)
{
    invoker_t invoker(limit);
    std::stringstream output;

    pfs::shared_ptr<pfs::command> hello_john  = pfs::make_command<hello_command>("John", & output);
    pfs::shared_ptr<pfs::command> hello_ann   = pfs::make_command<hello_command>("Ann", & output);
    pfs::shared_ptr<pfs::command> hello_peter = pfs::make_command<hello_command>("Peter", & output);
    pfs::shared_ptr<pfs::command> hello_mary  = pfs::make_command<hello_command>("Mary", & output);
    pfs::shared_ptr<pfs::command> hello_sam   = pfs::make_command<hello_command>("Sam", & output);

    invoker.undo();  // Nothing happened
    invoker.redo();  // Nothing happened
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

    return output.str() == output_sample;
}

//void test_mapper ()
//{
//    typedef char const * key_t;
//    typedef command_mapper<key_t, pfs::stdcxx::map> command_mapper_t;
//
//    command_mapper_t cm;
//    pfs::command_factory<hello_command> hello_command_factory;
//    
//    cm.insert("hello", & hello_command_factory);
//    
//    invoker_t invoker(limit);
//    std::stringstream output;
//
//    pfs::shared_ptr<pfs::command> hello_john = cm.make("hello");
//    hello_john.construct("John", & output);
//    
//    invoker.exec(hello_john);
//}

int main ()
{
    size_t limit = 21;
    BEGIN_TESTS(limit - 1);

//    TEST_OK(test(100));
    
    while (--limit) {
        TEST_OK(test(limit + 5));
    }

    return END_TESTS;
}
