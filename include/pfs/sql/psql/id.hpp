#pragma once
#include "pfs/sql/psql/private_data.hpp"

namespace pfs {
namespace sql {
namespace psql {

class id
{
public:
    typedef Oid native_type;

private:
    native_type _value;

public:
    id (native_type value) : _value(value) {}

    native_type native () const { return _value; }
};

}}} // pfs::sql::psql

