#pragma once
#include <pfs/utility.hpp>
#include <pfs/stdcxx/map.hpp>
#include <pfs/associative_container.hpp>


namespace pfs {

template <typename Key, typename T>
class map : public stdcxx::map<Key, T, map<Key, T> >
{
    typedef stdcxx::map<Key, T, map<Key, T> > base_class;

public:
    typedef typename base_class::key_type         key_type;
    typedef typename base_class::mapped_type      mapped_type;
    typedef typename base_class::value_type       value_type;
    typedef typename base_class::size_type        size_type;
    typedef typename base_class::difference_type  difference_type;
    typedef typename base_class::reference        reference;
    typedef typename base_class::const_reference  const_reference;
    typedef typename base_class::pointer          pointer;
    typedef typename base_class::const_pointer    const_pointer;
    typedef typename base_class::iterator         iterator;
    typedef typename base_class::const_iterator   const_iterator;
    typedef typename base_class::reverse_iterator reverse_iterator;
    typedef typename base_class::const_reverse_iterator const_reverse_iterator;

public:
    map () : base_class() {}

    template <typename InputIt>
    map (InputIt first, InputIt last)
        : base_class(first, last)
    {}

    map (map const & rhs)
        : base_class(rhs)
    {}

#if __cplusplus >= 201103L
    map (map && rhs)
        : base_class(std::forward<map>(rhs))
    {}

    map (std::initializer_list<std::pair<Key const, T> > ilist)
        : base_class(ilist)
    {}
#endif
};

namespace associative_container {

template <typename Key, typename T>
struct type_traits<Key, T, pfs::map<Key, T> >
{
    typedef pfs::map<Key, T> type;
    typedef Key key_type;
    typedef T value_type;
};

template <typename Key
        , typename T>
struct container_ref<Key, T, pfs::map<Key, T> > : details::container_ref<Key, T, pfs::map<Key, T> >
{
    typedef details::container_ref<Key, T, std::map<Key, T> > base_class;
    typedef typename base_class::container_type container_type;
    typedef typename container_type::mapped_type value_type;
    container_ref (container_type & ac) : base_class(ac) {}
};

template <typename Key, typename T>
struct iterators<Key, T, pfs::map<Key, T> > : details::iterators<Key, T, pfs::map<Key, T> >
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
struct inserter<Key, T, pfs::map<Key, T> > : details::inserter<Key, T, pfs::map<Key, T> >
{
    typedef details::inserter<Key, T, pfs::map<Key, T> > base_class;

    inserter (typename base_class::container_type & ac) : base_class(ac) {}

    void insert (Key const & key, T const & value)
    {
        this->p->insert(pfs::make_pair(key, value));
    }
};

} // associative_container

} // pfs

