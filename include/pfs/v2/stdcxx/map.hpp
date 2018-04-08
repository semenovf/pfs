#pragma once
#include <map>
#include <pfs/cxxlang.hpp>
#include <pfs/v2/container.hpp>
#include <pfs/v2/associative_container.hpp>

namespace pfs {

namespace stdcxx { struct map {}; } //  stdcxx

namespace associative_container {

template <typename Key, typename T>
struct type_trait<Key, T, stdcxx::map>
{
    typedef std::map<Key, T> type;
};

template <typename Key, typename T>
struct iterators<Key, T, stdcxx::map>
{
    typedef typename std::map<Key, T>::iterator iterator;
    typedef typename std::map<Key, T>::const_iterator const_iterator;

    std::map<Key, T> * p;

    iterators (std::map<Key, T> & ac) : p(& ac) {}

    inline iterator begin ()
    {
        p->begin();
    }

    inline const_iterator begin () const
    {
        p->begin();
    }

    inline const_iterator cbegin () const
    {
        p->begin();
    }

    inline iterator end ()
    {
        p->end();
    }

    inline const_iterator end () const
    {
        p->end();
    }

    inline const_iterator cend () const
    {
        p->end();
    }

    T & value (iterator it)
    {
        return it->second;
    }

    T const & value (iterator it) const
    {
        return it->second;
    }

    T const & value (const_iterator it) const
    {
        return it->second;
    }
};

template <typename Key, typename T>
struct inserter<Key, T, stdcxx::map>
{
    std::map<Key, T> * p;

    inserter (std::map<Key, T> & ac) : p(& ac) {}

    void insert (Key const & key, T const & value)
    {
        p->insert(std::make_pair(key, value));
    }
};

template <typename Key, typename T>
struct finder<Key, T, stdcxx::map>
{
    std::map<Key, T> * p;

    finder (std::map<Key, T> & ac) : p(& ac) {}

    typename std::map<Key, T>::iterator find (Key const & key)
    {
        return p->find(key);
    }

    typename std::map<Key, T>::const_iterator find (Key const & key) const
    {
        return p->find(key);
    }
};

}} // pfs::associative_container

