#include <iostream>
#include "pfs/test.hpp"
#include <pfs/string.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/sql/sqlite3/id.hpp>
#include <pfs/sql/sqlite3/database.hpp>
#include <pfs/sql/sqlite3/statement.hpp>
#include <pfs/sql/sqlite3/result.hpp>
#include <pfs/sql/debby.hpp>

using std::cout;
using std::cerr;
using std::endl;

typedef pfs::sql::debby<pfs::sql::sqlite3::id
        , pfs::sql::sqlite3::database
        , pfs::sql::sqlite3::statement
        , pfs::sql::sqlite3::result> debby_ns;

void sqlite3_test ()
{
////////////////////////////////////////////////////////////////////////////////
// Open non-exists database                                                   //
////////////////////////////////////////////////////////////////////////////////

    {
        ADD_TESTS(1);

        bool ok = false;
        // Attempt to open non-exists database for read only (by default)
        // Throws exception (test.db no such database file)
        //
        try {
            debby_ns::database db;
            db.open("sqlite3:/nonexists.db");
        } catch (pfs::exception const & ex) {
            cerr << "Exception: " << ex.message() << endl;
            ok = true;
        }

        TEST_OK2(ok, "Attempt to open non-existence database for read only (by default)");
    }


////////////////////////////////////////////////////////////////////////////////
// Open non-exists database                                                   //
////////////////////////////////////////////////////////////////////////////////
    {
        ADD_TESTS(7);

        pfs::error_code ec;
        pfs::filesystem::path path = pfs::filesystem::temp_directory_path();
        TEST_FAIL2(!path.empty(), "Get temporary directory path");
        path /= "test-db.sqlite3";

        if (pfs::filesystem::exists(path, ec))
            pfs::filesystem::remove(path, ec);

        cout << "DB path: " << path << endl;

        bool ok = true;

        if (ok) {
            try {
                TEST_FAIL2(!pfs::filesystem::exists(path, ec), "File not found");

                pfs::string dburi("sqlite3:");
                dburi.append(pfs::to_string(path));
                dburi.append("?mode=rwc");

                cout << "DB URI: " << dburi << endl;
                debby_ns::database db;
                db.open(dburi);
            } catch (pfs::exception const & ex) {
                cerr << "Exception: " << ex.message() << endl;
                ok = false;
            }
        }

        TEST_OK2(ok, "Attempt to create non-existence database for read-write");

        if (ok) {
            try {
                TEST_FAIL2(pfs::filesystem::exists(path, ec), "File found");

                pfs::string dburi("sqlite3:");
                dburi.append(pfs::to_string(path));
                cout << "DB URI: " << dburi << endl;
                debby_ns::database db;
                db.open(dburi);
            } catch (pfs::exception const & ex) {
                cerr << "Exception: " << ex.message() << endl;
                ok = false;
            }
        }

        TEST_OK2(ok, "Attempt to open existence database for read only");
        TEST_FAIL2(pfs::filesystem::remove(path, ec), "Remove DB file");
    }

////////////////////////////////////////////////////////////////////////////////
// Database operations                                                        //
////////////////////////////////////////////////////////////////////////////////
    {
        ADD_TESTS(20);

        pfs::error_code ec;
        pfs::filesystem::path path = pfs::filesystem::temp_directory_path();
        TEST_FAIL2(!path.empty(), "Get temporary directory path");
        path /= "test-db.sqlite3";

        bool ok = true;

        if (ok) {
            try {
                TEST_FAIL2(!pfs::filesystem::exists(path, ec), "File not found");

                pfs::string dburi("sqlite3:");
                dburi.append(pfs::to_string(path));
                dburi.append("?mode=rwc");
                cout << "DB URI: " << dburi << endl;
                debby_ns::database db;
                db.open(dburi);

                //
                // Examples of sql statements are borrowed from
                // https://www.thegeekstuff.com/2012/09/sqlite-command-examples
                //

                debby_ns::statement stmt;
                debby_ns::result res;

                res = db.exec("create table department(deptid integer,name varchar(20),location varchar(10));");
                TEST_OK2(res.done(), "Create table 'department'");

                res = db.exec("create table employee(empid integer,name varchar(20),title varchar(10));");
                TEST_OK2(res.done(), "Create table 'employee'");

                debby_ns::stringlist_type tables = db.tables();

                TEST_OK2(tables.size() == 2, "Two tables created");

                // Ordered by name
                TEST_OK(tables[0] == "department");
                TEST_OK(tables[1] == "employee");

                TEST_OK(db.exec("insert into department values(1,'Sales','Los Angeles')").done());
                TEST_OK(db.exec("insert into department values(2,'Technology','San Jose')").done());
                TEST_OK(db.exec("insert into department values(3,'Marketing','Los Angeles')").done());

                TEST_OK(db.exec("insert into employee values(101,'John Smith','CEO')").done());
                TEST_OK(db.exec("insert into employee values(102,'Raj Reddy','Sysadmin')").done());
                TEST_OK(db.exec("insert into employee values(103,'Jason Bourne','Developer')").done());
                TEST_OK(db.exec("insert into employee values(104,'Jane Smith','Sale Manager')").done());
                TEST_OK(db.exec("insert into employee values(105,'Rita Patel','DBA')").done());

                int count = 0;
                res = db.exec("select * from department");

                while (res.has_more()) {
                    cout << res.get<int>(0)
                            << " | " << res.get<debby_ns::string_type>(1)
                            << " | " << res.get<debby_ns::string_type>(2) << endl;
                    ++res;
                    ++count;
                }

                TEST_OK2(count == 3, "3 records in 'department'");
                TEST_OK2(res.done(), "Select from 'department'");

                count = 0;
                res = db.exec("select * from employee");

                while (res.has_more()) {
                    cout << res.get<int>(0)
                            << " | " << res.get<debby_ns::string_type>(1)
                            << " | " << res.get<debby_ns::string_type>(2) << endl;
                    ++res;
                    ++count;
                }

                TEST_OK2(count == 5, "5 records in 'employee'");
                TEST_OK2(res.done(), "Select from 'employee'");

            } catch (pfs::exception const & ex) {
                cerr << "Exception: " << ex.message() << endl;
                ok = false;
            }
        }

        TEST_OK2(ok, "Process statements for database");
        TEST_FAIL2(pfs::filesystem::remove(path, ec), "Remove DB file");
    }
}
