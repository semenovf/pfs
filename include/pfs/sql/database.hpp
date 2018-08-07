#pragma once
#include <pfs/string.hpp>
#include <pfs/sql/exception.hpp>

namespace pfs {
namespace sql {

template <template <typename> class DatabaseRep>
class database : public DatabaseRep<database<DatabaseRep> >
{
    typedef DatabaseRep<database<DatabaseRep> > base_class;

    using base_class::open;

public:
    typedef typename base_class::string_type string_type;

#if __cplusplus >= 201103L
    // Avoid CopyConstructable and CopyAssignable
    database (database const &) = delete;
    database & operator = (database const &) = delete;
#else
private:
    database (database const &);
    database & operator = (database const &);
#endif

public:
    database () : base_class() {}

    /**
     * @fn bool open (string_type const & uristr, error_code & ec, string_type * errstr)
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

        if (!open(uri, ec, & errstr))
            PFS_THROW(sql_exception(ec, uri + ": " + errstr));

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
     * @fn bool query (string_type const & sql, pfs::error_code & ec, string_type * errstr = 0)
     * @param sql
     * @return
     *
     * @note Cannot be used for statements that contain binary data (sqlite3)
     */

//
// //  statement prepare (string_type const & sql); // TODO move to statement (as static)
// //  stringlist tables () const;
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

// template <typename Traits>
// bool database<Traits>::query (string_type const & sql)
// {
//     pfs::error_code ec;
//     string_type errstr;
//
//     if (!_d.query(sql, ec, & errstr))
//         PFS_THROW(sql_exception(ec, errstr));
//
//     return true;
// }

}} // namespace pfs::sql
