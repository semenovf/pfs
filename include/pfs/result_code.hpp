#pragma once
#include <pfs/string.hpp>

namespace pfs {

template <typename T = int, typename StringT = string>
class result_code
{
public:
    typedef T       value_type;
    typedef StringT string_type;

public:
    result_code ()
        : _code(0)
    {}

    result_code (value_type const & code, string_type const & errstr)
        : _code(code)
        , _errstr(errstr)
    {}


#if __cplusplus >= 201103L
    result_code (value_type const & code, string_type && errstr)
        : _code(code)
        , _errstr(std::forward<string_type>(errstr))
    {}

    result_code (result_code const &) = default;
    result_code (result_code &&) = default;
    result_code & operator = (result_code const &) = default;
    result_code & operator = (result_code &&) = default;
    ~result_code () = default;
#else
    result_code (result_code const & rhs)
        : _code(rhs._code)
        , _errstr(rhs._errstr)
    {}

    result_code & operator = (result_code const & rhs)
    {
        _code   = rhs._code;
        _errstr = rhs._errstr;
        return *this;
    }

    ~result_code () {}
#endif

    /**
     * Implicit conversion to T type value
     * @return Status value.
     */
    operator T () const
    {
        return _code;
    }

    T value () const
    {
        return _code;
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
    void swap (result_code & rhs)
    {
        swap(_code, rhs._code);
        swap(_errstr, rhs._errstr);
    }

private:
    value_type  _code;
    string_type _errstr;
};

} // namespace pfs

