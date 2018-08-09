#pragma once
#include <pfs/sql/sqlite3/sqlite3.h>

namespace pfs {
namespace db {
namespace sqlite3 {

class id
{
public:
    typedef int native_type;

private:
    native_type _value;

public:
    id (native_type value) : _value(value) {}

    native_type native () const { return _value; }
};

}}} // pfs::db::sqlite3

