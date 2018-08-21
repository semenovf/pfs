#pragma once
#include <pfs/sql/psql/private_data.hpp>

namespace pfs {
namespace sql {
namespace psql {

template <typename StringT>
struct result_code
{
    typedef StringT string_type;

    static string_type to_string (PGconn * dbh)
    {
        // Returns the error message most recently generated by an operation
        // on the connection.
        // Note that by 'libpq' convention, a nonempty 'PQerrorMessage'
        // result can consist of multiple lines, and will include a trailing
        // newline. The caller should not free the result directly.
        // It will be freed when the associated PGconn handle is passed to
        // PQfinish . The result string should not be expected to remain
        // the same across operations on the PGconn structure.
        string_type r(PQerrorMessage(dbh));

        return r;
    }

    static string_type to_string (PGresult * res)
    {
        ExecStatusType status = PQresultStatus(res);

        string_type r = PQresultErrorMessage(res);
        r += "(status = ";
        r += PQresStatus(status);
        r += ")";
        return r;
    }

//     static string_type errorstr (string_type const & msg, int rc)
//     {
//         string_type r(msg);
//         r += " (";
//         r += to_string(rc);
//         r += ')';
//         return r;
//     }
};

}}} // namespace pfs::sql::psql
