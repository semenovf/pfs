#pragma once
#include <pfs/noncopyable.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/system_error.hpp>
#include <pfs/sql/exception.hpp>

namespace pfs {
namespace sql {

template <typename IdRep
        , template <typename> class DatabaseRep
        , template <typename> class StatementRep
        , typename StringListT = pfs::stringlist<>
        , typename StringT = pfs::string>
struct debyy
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
// Statement                                                                  //
////////////////////////////////////////////////////////////////////////////////

    class statement : public StatementRep<string_type>, noncopyable
    {
        typedef StatementRep<string_type> base_class;

    public:
        statement () : base_class() {}
        // statement (statement const & rhs) = default;
        // statement & operator = (statement const & rhs) = default;

        id last_insert_id () const
        {
            return id(base_class::last_insert_id());
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Database                                                                   //
////////////////////////////////////////////////////////////////////////////////

    class database : public DatabaseRep<stringlist_type, string_type>, noncopyable
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
            statement result;

            return result.prepare(native_handle(), sql, ec, errstr)
                    ? result
                    : statement();
        }

        /**
         * @fn stringlist_type tables () const
         */

    // //  bool tableExists (const string & name) const;

        /**
        * @fn bool begin ()
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


