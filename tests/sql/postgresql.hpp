#include "pfs/safeformat.hpp"
#include "pfs/getusername.hpp"

static char const * postgresql_help =
    "********************************************************************************\n"
    "To test on PostgreSQL work properly need:\n"
    "1. Allow current user to login to PostgreSQL instance.\n"
    "   Need to configure 'pg_hba.conf'\n"
    "2. Add role for current user\n"
    "3. Create database with name 'pfs-test-db'\n"
    "********************************************************************************\n";

void postgresql_test ()
{
    typedef psql_debby_ns debby_ns;
    pfs::string user = pfs::getusername();

    std::cout << "postgresql_test (): user: " << user << std::endl;

////////////////////////////////////////////////////////////////////////////////
// Open non-exists database                                                   //
////////////////////////////////////////////////////////////////////////////////

    {
        ADD_TESTS(1);

        pfs::string db_uri = pfs::safeformat("postgresql://%s@localhost:5432/nonexists-db?connect_timeout=10")
                % user;

        std::cout << "Database URI: " << db_uri << std::endl;

        bool ok = false;
        // Attempt to open non-exists database for read only (by default)
        // Throws exception (test.db no such database file)
        //
        try {
            debby_ns::database db;
            db.open(db_uri);
        } catch (pfs::exception const & ex) {
            std::cout << "Exception: " << ex.message() << std::endl;
            ok = true;
        }

        TEST_OK2(ok, "Attempt to open non-existence database");
    }

    {
        ADD_TESTS(29);

        pfs::string db_uri = pfs::safeformat("postgresql://%s@localhost:5432/pfs-test-db?connect_timeout=10")
                % user;

        std::cout << "Database URI: " << db_uri << std::endl;

        bool ok = true;

        if (ok) {
            try {
// ////////////////////////////////////////////////////////////////////////////////
// // Database operations                                                        //
// ////////////////////////////////////////////////////////////////////////////////
                debby_ns::database db;
                db.open(db_uri);

                //
                // Examples of sql statements are borrowed from
                // https://www.thegeekstuff.com/2012/09/sqlite-command-examples
                //

                debby_ns::result res;

                TEST_FAIL(db.exec("DROP TABLE IF EXISTS department;"));
                TEST_FAIL(db.exec("DROP TABLE IF EXISTS employee;"));

                TEST_OK2(db.exec("create table department(deptid integer,name varchar(30),location varchar(20));")
                        , "Create table 'department'");

                TEST_OK2(db.exec("create table employee(empid integer,name varchar(30),title varchar(20));")
                        , "Create table 'employee'");

                debby_ns::stringlist_type tables = db.tables();

                TEST_OK2(tables.size() == 2, "Two tables created");

                TEST_OK(db.table_exists("department"));
                TEST_OK(db.table_exists("employee"));
                TEST_OK(!db.table_exists("non-exists-table"));

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

////////////////////////////////////////////////////////////////////////////////
// Statement operations                                                       //
////////////////////////////////////////////////////////////////////////////////

                debby_ns::statement stmt;
                stmt = db.prepare("insert into department values($1, 'Research & Development','Yekaterinburg')");
                TEST_OK(stmt.bind(0, 4));
                res = stmt.exec();
                TEST_OK2(res.done(), "Inserted department 'Research & Development'");

                stmt = db.prepare("insert into employee values($1,$2,$3)");
                TEST_OK(stmt.bind(0, 401));
                TEST_OK(stmt.bind(1, "Fedor Semenov"));
                TEST_OK(stmt.bind(2, std::string("Software Developer")));
                res = stmt.exec();
                TEST_OK2(res.done(), "Inserted employee 'Software Developer'");

////////////////////////////////////////////////////////////////////////////////
// Check operations' results                                                  //
////////////////////////////////////////////////////////////////////////////////

                int count = 0;
                res = db.exec("select * from department");

                while (res.has_more()) {
                    std::cout << res.get<int>(0)
                            << " | " << res.get<debby_ns::string_type>(1)
                            << " | " << res.get<debby_ns::string_type>(2) << std::endl;
                    ++res;
                    ++count;
                }

                TEST_OK2(count == 4, "4 records in 'department'");
                TEST_OK2(res.done(), "Select from 'department'");

                count = 0;
                res = db.exec("select * from employee");

                while (res.has_more()) {
                    std::cout << res.get<int>(0)
                            << " | " << res.get<debby_ns::string_type>(1)
                            << " | " << res.get<debby_ns::string_type>(2) << std::endl;
                    ++res;
                    ++count;
                }

                TEST_OK2(count == 6, "6 records in 'employee'");
                TEST_OK2(res.done(), "Select from 'employee'");

            } catch (pfs::exception const & ex) {
                std::cerr << "Exception: " << ex.message() << std::endl;
                std::cout << postgresql_help;
                ok = false;
            }
        }

        TEST_OK2(ok, "Process statements for database");
   }
}
