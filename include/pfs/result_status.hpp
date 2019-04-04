#pragma once
#include <pfs/string.hpp>

namespace pfs {

template <typename StringT = string>
class result_status
{
public:
    typedef string string_type;

public:
    result_status ()
        : _status(true)
    {}

    result_status (string_type const & errstr)
        : _status(false)
        , _errstr(errstr)
    {}


#if __cplusplus >= 201103L
    result_status (string_type && errstr)
        : _status(false)
        , _errstr(std::forward<string_type>(errstr))
    {}

    result_status (result_status const &) = default;
    result_status (result_status &&) = default;
    result_status & operator = (result_status const &) = default;
    result_status & operator = (result_status &&) = default;
    ~result_status () = default;
#else
    result_status (result_status const & rhs)
        : _status(rhs._status)
        , _errstr(rhs._errstr)
    {}

    result_status & operator = (result_status const & rhs)
    {
        _status = rhs._status;
        _errstr = rhs._errstr;
        return *this;
    }

    ~result_status () {}
#endif

    /**
     * Implicit conversion to bool value
     * @return Status value.
     */
    operator bool () const
    {
        return _status;
    }

    /**
     * @return Error string.
     */
    string_type const & what () const
    {
        return _errstr;
    }

    /**
     * Swap values.
     */
    void swap (result_status & rhs)
    {
        swap(_status, rhs._status);
        swap(_errstr, rhs._errstr);
    }

private:
    bool        _status;
    string_type _errstr;
};

} // namespace pfs
