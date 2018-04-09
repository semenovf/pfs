#pragma once
#include <map>
#include <pfs/cxxlang.hpp>
#include <pfs/v2/associative_container.hpp>

namespace pfs {

namespace stdcxx { struct map {}; }

namespace associative_container {

template <typename Key, typename T>
struct type_traits<Key, T, stdcxx::map>
{
    typedef std::map<Key, T> type;
    typedef Key key_type;
    typedef T value_type;
};

template <typename Key
        , typename T>
struct container_ref<Key, T, std::map<Key, T> > : details::container_ref<Key, T, std::map<Key, T> >
{
    typedef details::container_ref<Key, T, std::map<Key, T> > base_class;
    typedef typename base_class::container_type container_type;
    typedef typename container_type::mapped_type value_type;
    container_ref (container_type & ac) : base_class(ac) {}
};

template <typename Key, typename T>
struct iterators<Key, T, std::map<Key, T> > : details::iterators<Key, T, std::map<Key, T> >
{
    typedef details::iterators<Key, T, std::map<Key, T> > base_class;
    typedef typename base_class::container_type container_type;
    typedef typename base_class::iterator iterator;
    typedef typename base_class::const_iterator const_iterator;
    typedef typename base_class::value_type value_type;

    iterators (container_type & ac) : base_class(ac) {}

    value_type & value (iterator it)
    {
        return it->second;
    }

    value_type const & value (iterator it) const
    {
        return it->second;
    }

    value_type const & value (const_iterator it) const
    {
        return it->second;
    }
};

template <typename Key, typename T>
struct inserter<Key, T, std::map<Key, T> > : details::inserter<Key, T, std::map<Key, T> >
{
    typedef details::inserter<Key, T, std::map<Key, T> > base_class;

    inserter (typename base_class::container_type & ac) : base_class(ac) {}

    void insert (Key const & key, T const & value)
    {
        this->p->insert(std::make_pair(key, value));
    }
};

}} // pfs::associative_container

