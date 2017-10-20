#include <iostream>
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/string.hpp>
#include <pfs/db/sqlite3/database.hpp>
#include <pfs/db/database.hpp>

using std::cout;
using std::cerr;
using std::endl;

typedef pfs::string<pfs::stdcxx::string> string_t;
typedef pfs::db::database_traits<string_t, pfs::db::sqlite3::database> database_traits_t;
typedef pfs::db::database<database_traits_t> db_t;


// Prerequisites:
//      * PostgreSQL must be installed
//      * role `postgres`

int main ()
{
    // Attempt open non-exists database for read write (by default)
    // Throws exception (test.db no such database file)
    //
    try {
        db_t db;
        db.open("sqlite3://nonexists.db");
        db.close();
    } catch (pfs::db_exception ex) {
        cerr << ex.what() << endl;
        return 1;
    }
    
    return 0;
}
