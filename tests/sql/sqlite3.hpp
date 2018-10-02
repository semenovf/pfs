void sqlite3_test ()
{
    typedef sqlite3_debby_ns debby_ns;
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
            std::cerr << "Exception: " << ex.message() << std::endl;
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

        std::cout << "DB path: " << path << std::endl;

        bool ok = true;

        if (ok) {
            try {
                TEST_FAIL2(!pfs::filesystem::exists(path, ec), "File not found");

                pfs::string dburi("sqlite3:");
                dburi.append(pfs::to_string(path));
                dburi.append("?mode=rwc");

                std::cout << "DB URI: " << dburi << std::endl;
                debby_ns::database db;
                db.open(dburi);
            } catch (pfs::exception const & ex) {
                std::cerr << "Exception: " << ex.message() << std::endl;
                ok = false;
            }
        }

        TEST_OK2(ok, "Attempt to create non-existence database for read-write");

        if (ok) {
            try {
                TEST_FAIL2(pfs::filesystem::exists(path, ec), "File found");

                pfs::string dburi("sqlite3:");
                dburi.append(pfs::to_string(path));
                std::cout << "DB URI: " << dburi << std::endl;
                debby_ns::database db;
                db.open(dburi);
            } catch (pfs::exception const & ex) {
                std::cerr << "Exception: " << ex.message() << std::endl;
                ok = false;
            }
        }

        TEST_OK2(ok, "Attempt to open existence database for read only");
        TEST_FAIL2(pfs::filesystem::remove(path, ec), "Remove DB file");
    }

    {
        ADD_TESTS(31);

        pfs::error_code ec;
        pfs::filesystem::path path = pfs::filesystem::temp_directory_path();
        TEST_FAIL2(!path.empty(), "Get temporary directory path");
        path /= "test-db.sqlite3";

        bool ok = true;

        if (ok) {
            try {
////////////////////////////////////////////////////////////////////////////////
// Database operations                                                        //
////////////////////////////////////////////////////////////////////////////////

                TEST_FAIL2(!pfs::filesystem::exists(path, ec), "File not found");

                pfs::string dburi("sqlite3:");
                dburi.append(pfs::to_string(path));
                dburi.append("?mode=rwc");
                std::cout << "DB URI: " << dburi << std::endl;
                debby_ns::database db;
                db.open(dburi);

                //
                // Examples of sql statements are borrowed from
                // https://www.thegeekstuff.com/2012/09/sqlite-command-examples
                //

                debby_ns::result res;

                res = db.exec("create table department(deptid integer,name varchar(30),location varchar(20));");
                TEST_OK2(res.done(), "Create table 'department'");

                res = db.exec("create table employee(empid integer,name varchar(30),title varchar(20));");
                TEST_OK2(res.done(), "Create table 'employee'");

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

                TEST_OK2(res.column_count() == 3, "There are 3 columns by row in the select 'department' result");

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

                TEST_OK2(res.column_count() == 3, "There are 3 columns by row in the select 'employee' result");

                while (res.has_more()) {
                    std::cout << res.get<int>(0)
                            << " | " << res.get<debby_ns::string_type>(1)
                            << " | " << res.get<debby_ns::string_type>(2) << std::endl;
                    ++res;
                    ++count;
                }

                TEST_OK2(count == 6, "6 records in 'employee'");
                TEST_OK2(res.done(), "Select from 'employee'");

                db.clear();

            } catch (pfs::exception const & ex) {
                std::cerr << "Exception: " << ex.message() << std::endl;
                ok = false;
            }
        }

        TEST_OK2(ok, "Process statements for database");
        TEST_FAIL2(pfs::filesystem::remove(path, ec), "Remove DB file");
    }
}
