/* 
 * File:   database.hpp
 * Author: wladt
 *
 * Created on February 8, 2017, 4:02 PM
 */

#ifndef __PFS_DB_DATABASE_HPP__
#define __PFS_DB_DATABASE_HPP__

#include <pfs/string.hpp>
#include <pfs/db/exception.hpp>

namespace pfs {
namespace db {

template <typename StringImplType, template <typename> class DatabaseRep>
struct database_traits
{
    typedef string<StringImplType>   string_type;
    typedef db_exception             exception_type;
    typedef DatabaseRep<string_type> rep_type;
};

template <typename Traits>
class database
{
public:
    typedef Traits                               traits_type;
    typedef typename traits_type::string_type    string_type;
    typedef typename traits_type::exception_type exception_type;
    typedef typename traits_type::rep_type       rep_type;

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
    
    bool open (string_type const & uri, pfs::error_code & ec)
    {
        return _d.open(uri, ec);
    }
    
    bool open (string_type const & uri)
    {
        pfs::error_code ec;

        if (!open(uri, ec))
            throw db_exception(ec, "bad database URI");

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
	 *
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

//	statement prepare (string_type const & sql); // TODO move to statement (as static)
//	stringlist tables () const;
//	bool tableExists (const string & name) const;

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
        errstr = "query failed: " + errstr;
        throw db_exception(ec, u8string<std::string>(errstr).c_str());
    }

    return true;
}

}} // pfs::db

#endif /* __PFS_DB_DATABASE_HPP__ */

