#include <iostream>
#include "pfs/test/test.hpp"
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/string.hpp>
#include <pfs/db/sqlite3/database.hpp>
#include <pfs/db/database.hpp>
#include <pfs/filesystem.hpp>

using std::cout;
using std::cerr;
using std::endl;

typedef pfs::string<pfs::stdcxx::string> string_t;
typedef pfs::db::database_traits<string_t, pfs::db::sqlite3::database> database_traits_t;
typedef pfs::db::database<database_traits_t> db_t;

void test0 ()
{
    ADD_TESTS(1);

    // Attempt to open non-exists database for read only (by default)
    // Throws exception (test.db no such database file)
    //
    try {
        db_t db;
        db.open("sqlite3:/nonexists.db");
        db.close();
    } catch (pfs::db_exception ex) {
        cerr << ex.what() << endl;
        TEST_OK2(true, "Attempt to open non-exists database for read only (by default)");
    }    
}

void test1 ()
{
    ADD_TESTS(3);

    string_t dburi("sqlite3:");
    
    pfs::filesystem::path path = pfs::filesystem::temp_directory_path();
    TEST_FAIL2(!path.empty(), "Get temporary directory path");
    path /= "test-db.sqlite3";
    
    cout << "DB path: " << path << endl;

    TEST_FAIL2(!pfs::filesystem::exists(path), "File not found");
    
    dburi.append(pfs::to_string<string_t>(path));
    dburi.append("?mode=rwc");
    
    cout << "DB URI: " << dburi << endl;
    
    try {
        db_t db;
        db.open(dburi);
        db.close();
    } catch (pfs::db_exception ex) {
        ADD_TESTS(1);
        cerr << ex.what() << endl;
        TEST_OK2(false, "Attempt to create non-exists database for read-write");
    }
    
    TEST_FAIL2(pfs::filesystem::remove(path), "Remove DB file");
}

int main ()
{
   	BEGIN_TESTS(0);

    test0();
    test1();

    return END_TESTS;
}
