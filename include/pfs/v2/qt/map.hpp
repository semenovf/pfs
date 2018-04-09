#pragma once
#include <QMap>

namespace pfs {

namespace qt { struct map {}; }

namespace associative_container {

template <typename Key, typename T>
struct type_traits<Key, T, qt::map>
{
    typedef QMap<Key, T> type;
    typedef Key key_type;
    typedef T value_type;
};

}} // pfs::associative_container
