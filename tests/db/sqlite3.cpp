#include <iostream>
#include "pfs/test.hpp"
#include <pfs/string.hpp>
#include <pfs/db/sqlite3/database.hpp>
#include <pfs/db/database.hpp>
#include <pfs/filesystem.hpp>

using std::cout;
using std::cerr;
using std::endl;

typedef pfs::string string_t;
typedef pfs::db::database_traits<string_t, pfs::db::sqlite3::database> database_traits_t;
typedef pfs::db::database<database_traits_t> db_t;

void sqlite3_test0 ()
{
    ADD_TESTS(1);

    bool ok = false;
    // Attempt to open non-exists database for read only (by default)
    // Throws exception (test.db no such database file)
    //
    try {
        db_t db;
        db.open("sqlite3:/nonexists.db");
        db.close();
    } catch (pfs::db_exception ex) {
        cerr << "Exception:" << ex.what() << endl;
        ok = true;
    }

    TEST_OK2(ok, "Attempt to open non-existence database for read only (by default)");
}

void sqlite3_test1 ()
{
    ADD_TESTS(6);

    pfs::error_code ec;
    pfs::filesystem::path path = pfs::filesystem::temp_directory_path();
    TEST_FAIL2(!path.empty(), "Get temporary directory path");
    path /= "test-db.sqlite3";

    cout << "DB path: " << path << endl;

    bool ok = true;

    if (ok) {
        try {
            TEST_FAIL2(!pfs::filesystem::exists(path, ec), "File not found");

            string_t dburi("sqlite3:");
            dburi.append(pfs::to_string<string_t>(path));
            dburi.append("?mode=rwc");

            cout << "DB URI: " << dburi << endl;
            db_t db;
            db.open(dburi);
            db.close();
        } catch (pfs::db_exception ex) {
            cerr << "Exception:" << ex.what() << endl;
            ok = false;
        }
    }

    TEST_OK2(ok, "Attempt to create non-existence database for read-write");

    if (ok) {
        try {
            TEST_FAIL2(pfs::filesystem::exists(path, ec), "File found");

            string_t dburi("sqlite3:");
            dburi.append(pfs::to_string<string_t>(path));
            cout << "DB URI: " << dburi << endl;
            db_t db;
            db.open(dburi);
            db.close();
        } catch (pfs::db_exception ex) {
            cerr << "Exception:" << ex.what() << endl;
            ok = false;
        }
    }

    TEST_OK2(ok, "Attempt to open existence database for read only");
    TEST_FAIL2(pfs::filesystem::remove(path, ec), "Remove DB file");
}
