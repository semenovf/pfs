#pragma once
#include <pfs/sql/sqlite3/sqlite3.h>

namespace pfs {
namespace sql {
namespace sqlite3 {

template <typename StringT>
struct result_code
{
    typedef StringT string_type;

    static string_type to_string (int rc)
    {
        switch (rc) {
        case SQLITE_OK          : return string_type("SQLITE_OK");
        case SQLITE_ERROR       : return string_type("SQLITE_ERROR");
        case SQLITE_INTERNAL    : return string_type("SQLITE_INTERNAL");
        case SQLITE_PERM        : return string_type("SQLITE_PERM");
        case SQLITE_ABORT       : return string_type("SQLITE_ABORT");
        case SQLITE_BUSY        : return string_type("SQLITE_BUSY");
        case SQLITE_LOCKED      : return string_type("SQLITE_LOCKED");
        case SQLITE_NOMEM       : return string_type("SQLITE_NOMEM");
        case SQLITE_READONLY    : return string_type("SQLITE_READONLY");
        case SQLITE_INTERRUPT   : return string_type("SQLITE_INTERRUPT");
        case SQLITE_IOERR       : return string_type("SQLITE_IOERR");
        case SQLITE_CORRUPT     : return string_type("SQLITE_CORRUPT");
        case SQLITE_NOTFOUND    : return string_type("SQLITE_NOTFOUND");
        case SQLITE_FULL        : return string_type("SQLITE_FULL");
        case SQLITE_CANTOPEN    : return string_type("SQLITE_CANTOPEN");
        case SQLITE_PROTOCOL    : return string_type("SQLITE_PROTOCOL");
        case SQLITE_EMPTY       : return string_type("SQLITE_EMPTY");
        case SQLITE_SCHEMA      : return string_type("SQLITE_SCHEMA");
        case SQLITE_TOOBIG      : return string_type("SQLITE_TOOBIG");
        case SQLITE_CONSTRAINT  : return string_type("SQLITE_CONSTRAINT");
        case SQLITE_MISMATCH    : return string_type("SQLITE_MISMATCH");
        case SQLITE_MISUSE      : return string_type("SQLITE_MISUSE");
        case SQLITE_NOLFS       : return string_type("SQLITE_NOLFS");
        case SQLITE_AUTH        : return string_type("SQLITE_AUTH");
        case SQLITE_FORMAT      : return string_type("SQLITE_FORMAT");
        case SQLITE_RANGE       : return string_type("SQLITE_RANGE");
        case SQLITE_NOTADB      : return string_type("SQLITE_NOTADB");
        case SQLITE_NOTICE      : return string_type("SQLITE_NOTICE");
        case SQLITE_WARNING     : return string_type("SQLITE_WARNING");
        case SQLITE_ROW         : return string_type("SQLITE_ROW");
        case SQLITE_DONE        : return string_type("SQLITE_DONE");
        default: return string_type("unknown code (may be extended code) = ") + pfs::to_string(rc).utf8().c_str();
        }
    }

    static string_type errorstr (string_type const & msg, int rc)
    {
        string_type r(msg);
        r += " (";
        r += to_string(rc);
        r += ')';
        return r;
    }
};

}}} // namespace pfs::sql::sqlite3
