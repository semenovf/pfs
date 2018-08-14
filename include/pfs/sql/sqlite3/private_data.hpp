#pragma once
#include <pfs/sql/sqlite3/sqlite3.h>
#include <pfs/memory.hpp>

namespace pfs {
namespace sql {
namespace sqlite3 {

struct db_handle_deleter
{
    void operator () (struct sqlite3 * dbh) const
    {
        if (dbh) {
            sqlite3_close_v2(dbh);
        }
    }
};

struct stmt_handle_deleter
{
    void operator () (struct sqlite3_stmt * sth) const
    {
        if (sth) {
            sqlite3_finalize(sth);
        }
    }
};

typedef struct sqlite3 *      db_native_handle_type;
typedef struct sqlite3_stmt * stmt_native_handle_type;

typedef pfs::shared_ptr<struct sqlite3>      db_handle_shared;
typedef pfs::shared_ptr<struct sqlite3_stmt> stmt_handle_shared;

}}} // pfs::sql::sqlite3
