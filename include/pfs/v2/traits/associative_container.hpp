#pragma once
#include <map>

namespace pfs {

namespace stdcxx {

template <typename Key, typename T>
struct map
{
    typedef std::map<Key, T> type;

    static inline void insert (type & c, Key const & key, T const & value)
    {
        c.insert(std::make_pair(key, value));
    }
};

} // stdcxx

template <typename Key
    , typename T
    , template <typename, typename> class AssociativeContainer>
inline void insert (AssociativeContainer<Key, T> & c, Key const & key, T const & value)
{
    AssociativeContainer<Key, T>::template insert(c, key, value);
}

} // pfs
