/* 
 * File:   statement.hpp
 * Author: wladt
 *
 * Created on April 27, 2017, 3:57 PM
 */

#ifndef __PFS_FOUNDATION_DB_SQLITE3_STATEMENT_HPP__
#define __PFS_FOUNDATION_DB_SQLITE3_STATEMENT_HPP__

#include <sqlite3.h>
#include <pfs/foundation/db/sqlite3/tag.hpp>
#include <pfs/traits/db/database.hpp>

namespace pfs {
namespace db {

template <typename StringT, typename DatabaseTag>
struct statement_rep<StringT, sqlite3_tag>
{};

}} // pfs::db

#endif /* __PFS_FOUNDATION_DB_SQLITE3_STATEMENT_HPP__ */

