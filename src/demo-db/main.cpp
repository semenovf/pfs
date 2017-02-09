#include <iostream>
#include <pfs/foundation/cxx/stdcxx/string.hpp>
#include <pfs/foundation/cxx/stdcxx/string_builder.hpp>
#include <pfs/foundation/db/psql/database.hpp>

using std::cout;
using std::cerr;
using std::endl;


// Prerequisites:
//      * PostgreSQL must be installed
//      * role `postgres`

typedef pfs::db::database<char, pfs::db::postgresql_tag> database;

int main (int argc, char * argv[])
{
    try {
        database db;
        db.open("postgresql://");
        db.close();
    } catch (database::exception ex) {
        cerr << ex.what() << endl;
        return 1;
    }
    
    return 0;
}
