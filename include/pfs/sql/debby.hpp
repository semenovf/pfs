#pragma once
#include <pfs/noncopyable.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/system_error.hpp>
#include <pfs/sql/exception.hpp>

namespace pfs {
namespace sql {

template <typename IdRep
        , template <typename, typename> class DatabaseRep // must inherits noncopyable
        , template <typename> class StatementRep
        , template <typename> class ResultRep
        , typename StringListT = pfs::stringlist<>
        , typename StringT = pfs::string>
struct debby
{
    typedef StringT     string_type;
    typedef StringListT stringlist_type;

////////////////////////////////////////////////////////////////////////////////
// Id                                                                         //
////////////////////////////////////////////////////////////////////////////////

    class id : public IdRep
    {
        typedef IdRep base_class;

    public:
        typedef typename base_class::native_type native_type;

    public:
        id (native_type value) : base_class(value) {}

        /**
        * @fn native_type native () const
        */
    };

////////////////////////////////////////////////////////////////////////////////
// Result                                                                     //
////////////////////////////////////////////////////////////////////////////////

    class result : public ResultRep<string_type>
    {
        typedef ResultRep<string_type> base_class;

    public:
        result () : base_class() {}
        result (base_class const & rhs) : base_class(rhs) {}

        /**
         * @fn string_type column_name (int column)
         * @brief Return the name assigned to a particular column in the result
         *        set of a @c SELECT statement.
         */

    };

////////////////////////////////////////////////////////////////////////////////
// Statement                                                                  //
////////////////////////////////////////////////////////////////////////////////

    class statement : public StatementRep<string_type>
    {
        typedef StatementRep<string_type> base_class;

    public:
        statement () : base_class() {}
        statement (base_class const & rhs) : base_class(rhs) {}

        id last_insert_id () const
        {
            return id(base_class::last_insert_id());
        }

        result exec (pfs::error_code & ec, string_type & errstr)
        {
            result res(base_class::exec(ec, errstr));
            return res;
        }

        /**
         *
         */
        result exec ()
        {
            pfs::error_code ec;
            string_type errstr;
            result res = base_class::exec(ec, errstr);

            if (ec) PFS_THROW(sql_exception(ec, errstr));

            return res;
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Database                                                                   //
////////////////////////////////////////////////////////////////////////////////

    // Non-copyable
    class database : public DatabaseRep<stringlist_type, string_type>
    {
        typedef DatabaseRep<stringlist_type, string_type> base_class;
        using base_class::open;

    public:
        database () : base_class() {}

        /**
        * @fn bool open (string_type const & uristr, error_code & ec, string_type & errstr)
        * @brief Open connection to database.
        */

        /**
        * @brief Open connection to database.
        * @throw sql_exception.
        */
        bool open (string_type const & uri)
        {
            pfs::error_code ec;
            string_type errstr;

            if (!open(uri, ec, errstr)) {
                if (errstr.empty())
                    PFS_THROW(sql_exception(ec));
                else
                    PFS_THROW(sql_exception(ec, uri + ": " + errstr));
            }

            return true;
        }

        /**
        * @fn bool close ()
        * @brief Close connection to database
        */

        /**
        * bool opened () const
        * @brief Checks if database already connected.
        */

        /**
        * @param sql
        * @param ec
        * @param errstr
        * @return
        *
        * @note Cannot be used for statements that contain binary data (sqlite3)
        */

        statement prepare (string_type const & sql, pfs::error_code & ec, string_type & errstr)
        {
            statement stmt(base_class::prepare(sql, ec, errstr));
            return stmt;
        }

        statement prepare (string_type const & sql)
        {
            pfs::error_code ec;
            string_type errstr;
            statement stmt = this->prepare(sql, ec, errstr);

            if (ec) PFS_THROW(sql_exception(ec, errstr));

            return stmt;
        }

        result exec (string_type const & sql, pfs::error_code & ec, string_type & errstr)
        {
            statement stmt = prepare(sql, ec, errstr);

            if (ec)
                return result();

            result res = stmt.exec(ec, errstr);

            if (ec)
                return result();

            return res;
        }

        result exec (string_type const & sql)
        {
            pfs::error_code ec;
            string_type errstr;
            result res = exec(sql, ec, errstr);

            if (ec) PFS_THROW(sql_exception(ec, errstr));

            return res;
        }

        /**
         * @fn stringlist_type tables (pfs::error_code & ec, string_type & errstr) const
         */

        /**
         * @brief
         */
        stringlist_type tables ()
        {
            pfs::error_code ec;
            string_type errstr;

            stringlist_type r = base_class::tables(ec, errstr);

            if (ec) PFS_THROW(sql_exception(ec, errstr));

            return r;
        }

        /**
        * @fn bool begin_transaction ()
        * @brief Begin transaction
        */

        /**
        * @fn bool commit ()
        * @brief Commit transaction
        */

        /**
        * @fn bool rollback ()
        * @brief Rollback transaction
        */

        /**
        * @brief Complete transaction according the @a success status.
        */
        bool end (bool success)
        {
            return success ? this->commit() : this->rollback();
        }
    };
}; // debby

}} // namespace pfs::sql


