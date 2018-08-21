#pragma once
#include <libpq-fe.h>
#include <pfs/memory.hpp>

namespace pfs {
namespace sql {
namespace psql {

struct db_handle_deleter
{
    void operator () (PGconn * dbh) const
    {
        if (dbh) PQfinish(dbh);
    }
};

struct stmt_handle_deleter
{
    void operator () (PGresult * sth) const
    {
        // Frees the storage associated with a PGresult. Every command result
        // should be freed via PQclear when it is no longer needed.
        if (sth) PQclear(sth);
    }
};

typedef stmt_handle_deleter result_handle_deleter;

typedef PGconn *   db_native_handle_type;
typedef PGresult * stmt_native_handle_type;
typedef PGresult * result_native_handle_type;

typedef pfs::shared_ptr<PGconn>   db_handle_shared;
typedef pfs::shared_ptr<PGresult> stmt_handle_shared;
typedef pfs::shared_ptr<PGresult> result_handle_shared;

}}} // pfs::sql::psql
