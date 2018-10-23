#include <iostream>
#include "pfs/config.h"
#include "pfs/test.hpp"
#include "pfs/string.hpp"
#include "pfs/safeformat.hpp"
#include "pfs/filesystem.hpp"
#include "pfs/sql/sqlite3/id.hpp"
#include "pfs/sql/sqlite3/database.hpp"
#include "pfs/sql/sqlite3/statement.hpp"
#include "pfs/sql/sqlite3/result.hpp"
#include "pfs/sql/debby.hpp"

typedef pfs::sql::debby<pfs::sql::sqlite3::id
        , pfs::sql::sqlite3::database
        , pfs::sql::sqlite3::statement
        , pfs::sql::sqlite3::result> sqlite3_debby_ns;

#include "sqlite3.hpp"

#if HAVE_POSTGRESQL

#include "pfs/sql/psql/id.hpp"
#include "pfs/sql/psql/database.hpp"
#include "pfs/sql/psql/statement.hpp"
#include "pfs/sql/psql/result.hpp"

typedef pfs::sql::debby<pfs::sql::psql::id
        , pfs::sql::psql::database
        , pfs::sql::psql::statement
        , pfs::sql::psql::result> psql_debby_ns;

#include "postgresql.hpp"

#endif // HAVE_POSTGRESQL

int main ()
{
    BEGIN_TESTS(0);

    sqlite3_test();

#if HAVE_POSTGRESQL
    postgresql_test();
#endif

    return END_TESTS;
}
