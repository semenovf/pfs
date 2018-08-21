#pragma once
#include <pfs/string.hpp>
#include <pfs/vector.hpp>
#include <pfs/sql/psql/private_data.hpp>
#include <pfs/sql/psql/result.hpp>

#include <pfs/debug.hpp>

namespace pfs {
namespace sql {
namespace psql {

template <typename StringT = pfs::string>
class statement
{
    typedef StringT                 string_type;
    typedef result<string_type>     result_type;
    typedef stmt_native_handle_type native_handle_type;

    stmt_handle_shared        _pd;
    db_handle_shared          _pdbh;
    string_type               _name;
    pfs::vector<std::string>  _param_string_values;
    pfs::vector<char const *> _param_values;
    pfs::vector<int>          _param_lengths;
    //pfs::vector<int>          _param_formats;
    //int                       _result_format;

private:
    void ensure_capacity (int index)
    {
        if (index >= _param_string_values.size()) {
            _param_string_values.resize(index + 1);
            _param_values.resize(index + 1);
            _param_lengths.resize(index + 1);
        }
    }

public:
    statement () {}
    statement (native_handle_type sth
            , db_handle_shared const & pdbh
            , string_type const & name)
        : _pd(sth, stmt_handle_deleter())
        , _pdbh(pdbh)
        , _name(name)
    {}

    native_handle_type native_handle () const
    {
        return _pd.get();
    }

    operator bool () const
    {
        return _pd.get() != 0;
    }

    id::native_type last_insert_id () const
    {
        return PQoidValue(_pd.get());
    }

    result_type exec (pfs::error_code & ec, string_type & errstr)
    {
        if (!_pd) {
            ec = pfs::make_error_code(sql_errc::bad_handle);
            errstr = "statement is not initialized";
            return result_type();
        }

        PFS_DEBUG(std::cout << "***Statement name: " << _name << std::endl);

        native_handle_type res = PQexecPrepared(_pdbh.get()
                , _name.utf8().c_str()
                , _param_values.size()
                , _param_values.data()
                , _param_lengths.data()
                , 0 /* _param_formats.data() */
                , 0 /*_result_format */);

        return result_type(res);
    }

    template <typename T>
    bool bind (int index, T const & value, pfs::error_code & ec, string_type & errstr)
    {
        ensure_capacity(index);
        _param_string_values[index] = to_string(value).utf8();
        _param_values[index]        = _param_string_values[index].c_str();
        _param_lengths[index]       = _param_string_values[index].size();

        return true;
    }

//     template <typename T>
//     bool bind (int index, string_type const & value, pfs::error_code & ec, string_type & errstr)
//     {
//         _param_string_values[index] = value.utf8();
//         _param_values[index]        = _param_string_values[index].c_str();
//         _param_lengths[index]       = _param_string_values[index].size();
//
//         return true;
//     }

//     template <typename T>
//     bool bind (int index, std::string const & value, pfs::error_code & ec, string_type & errstr)
//     {
//         _param_string_values[index] = value;
//         _param_values[index]        = _param_string_values[index].c_str();
//         _param_lengths[index]       = _param_string_values[index].size();
//
//         return true;
//     }

    bool bind (int index, char const * value, pfs::error_code & ec, string_type & errstr)
    {
        ensure_capacity(index);
        _param_string_values[index] = std::string(value);
        _param_values[index]        = _param_string_values[index].c_str();
        _param_lengths[index]       = _param_string_values[index].size();

        return true;
    }
};

}}} // namespace pfs::sql::psql
