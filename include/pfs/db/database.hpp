#pragma once
#include <pfs/string.hpp>
#include <pfs/db/exception.hpp>

namespace pfs {
namespace db {

template <typename DatabaseRep>
class database
{
public:
    typedef string      string_type;
    typedef DatabaseRep rep_type;

private:
    rep_type _d;

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
    database () : _d() {}

    bool open (string_type const & uri, error_code & ec, string_type * errstr = 0)
    {
        return _d.open(uri, ec, errstr);
    }

    bool open (string_type const & uri)
    {
        pfs::error_code ec;
        string_type errstr;

        if (!open(uri, ec, & errstr))
            throw db_exception(ec, errstr);

        return true;
    }

    bool close ()
    {
        return _d.close();
    }

    bool opened () const
    {
        return _d.opened();
    }

    /**
     * @param sql
     * @return
     *
     * @note Cannot be used for statements that contain binary data
     */
    bool query (string_type const & sql, pfs::error_code & ec, string_type * errstr = 0)
    {
        return _d.query(sql, ec, errstr);
    }

    bool query (string_type const & sql);

//  statement prepare (string_type const & sql); // TODO move to statement (as static)
//  stringlist tables () const;
//  bool tableExists (const string & name) const;

    bool begin ()
    {
        return _d.begin();
    }

    bool commit ()
    {
        return _d.commit();
    }

    bool rollback ()
    {
        return _d.rallback();
    }

    bool end (bool success)
    {
        return success ? commit() : rollback();
    }
};

template <typename Traits>
bool database<Traits>::query (string_type const & sql)
{
    pfs::error_code ec;
    string_type errstr;

    if (!_d.query(sql, ec, & errstr)) {
        //errstr = "query failed: " + errstr;
        throw db_exception(ec, errstr);
    }

    return true;
}

}} // pfs::db
