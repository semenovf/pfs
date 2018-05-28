#pragma once
#include <string>
#include <stdexcept>
#include <cstdio>
#include <pfs/cxxlang.hpp>
#include <pfs/assert.hpp>

namespace pfs {

class exception : public std::exception
{
    typedef std::exception base_class;

    std::string _msg;
    std::string _enhanced_msg;

public:
    exception () pfs_noexcept
        : base_class()
    {}

    explicit exception (char const * s)
        : base_class()
        , _msg(s)
        , _enhanced_msg(s)
    {}

    explicit exception (std::string const & s)
        : base_class()
        , _msg(s)
        , _enhanced_msg(s)
    {}

    virtual ~exception () throw() { };

    virtual std::string message () const
    {
        return _msg;
    }

    virtual char const * what () const throw()
    {
        return _enhanced_msg.c_str();
    }

    exception & at (char const * filename, int lineno)
    {
        char linestr[12];
        sprintf(linestr, "%d", lineno);
        _enhanced_msg += std::string() + " at " + filename + ":" + linestr;
        return *this;
    }

    exception & backtrace ()
    {
        append_backtrace(_enhanced_msg);
        return *this;
    }

private:
    void append_backtrace (std::string & buffer);
};

// typedef std::bad_alloc        bad_alloc;
// typedef std::length_error     length_error;
// typedef std::overflow_error   overflow_error;
// typedef std::underflow_error  underflow_error;

class runtime_error : public exception
{
public:
    runtime_error () : exception() {}
    explicit runtime_error (char const * s) : exception(s) {}
    explicit runtime_error (std::string const & s) : exception(s) {}
};

class logic_error : public exception
{
public:
    logic_error () : exception() {}
    explicit logic_error (char const * s) : exception(s) {}
    explicit logic_error (std::string const & s) : exception(s) {}
};

class invalid_argument : public logic_error
{
public:
    invalid_argument () : logic_error() {}
    explicit invalid_argument (char const * s) : logic_error(s) {}
    explicit invalid_argument (std::string const & s) : logic_error(s) {}
};

class domain_error : public logic_error
{
public:
    domain_error () : logic_error() {}
    explicit domain_error (char const * s) : logic_error(s) {}
    explicit domain_error (std::string const & s) : logic_error(s) {}
};

class out_of_range : public logic_error
{
public:
    out_of_range () : logic_error() {}
    explicit out_of_range (char const * s) : logic_error(s) {}
    explicit out_of_range (std::string const & s) : logic_error(s) {}
};

class range_error : public runtime_error
{
public:
    range_error () : runtime_error() {}
    explicit range_error (char const * s) : runtime_error(s) {}
    explicit range_error (std::string const & s) : runtime_error(s) {}
};

} // pfs
