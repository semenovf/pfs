#pragma once
#include <pfs/string.hpp>
#include <pfs/sql/cast.hpp>
#include <pfs/sql/psql/private_data.hpp>

namespace pfs {
namespace sql {
namespace psql {

template <typename StringT = pfs::string>
class result
{
    typedef StringT string_type;
    typedef result_native_handle_type native_handle_type;

    result_handle_shared _pd;
    int _nrows;
    int _irow;  // current row

private:
    template <typename T>
    struct cast_traits : public details::cast_traits<T, string_type>
    {};

public:
    result () : _nrows(0), _irow(0) {}
    result (native_handle_type res)
        : _pd(res, result_handle_deleter())
        , _nrows(0)
        , _irow(0)
    {
        _nrows = PQntuples(_pd.get());
    }

    ~result () {}

    operator bool () const
    {
        return _pd.get() != 0;
    }

    bool done () const
    {
        return _nrows == _irow;
    }

    bool fail () const
    {
        return _pd.get() == 0;
    }

    bool has_more () const
    {
        return _irow < _nrows;
    }

    result & operator ++ ()
    {
        ++_irow;
        return *this;
    }

    result operator ++ (int) const
    {
        result r(*this);
        ++r;
        return r;
    }

    int column_count () const
    {
        return PQnfields(_pd.get());
    }

    template <typename T>
    T get (int column) const
    {
        char * value = PQgetvalue(_pd.get(), _irow, column);
        return cast_traits<T>::cast(string_type(value));
    }

    string_type column_name (int column)
    {
        string_type name(PQfname(_pd.get(), column));
        return name;
    }
};

}}} // namespace pfs::sql::psql
