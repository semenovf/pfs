#pragma once
#include "container.hpp"

namespace pfs {
namespace associative_container {
namespace details {

template <typename Key
        , typename T
        , typename AssociativeContainer>
struct container_ref : container::details::container_ref<T, AssociativeContainer>
{
    typedef container::details::container_ref<T, AssociativeContainer> base_class;
    typedef typename base_class::container_type container_type;

    container_ref (container_type & ac) : base_class(ac) {}
};

template <typename Key
        , typename T
        , typename AssociativeContainer>
struct iterators : container::details::iterators<T, AssociativeContainer>
{
    typedef container::details::iterators<T, AssociativeContainer> base_class;
    typedef typename base_class::container_type container_type;
    typedef typename base_class::iterator iterator;
    typedef typename base_class::const_iterator const_iterator;

    iterators (container_type & ac) : base_class(ac) {}
};

template <typename Key
        , typename T
        , typename AssociativeContainer>
struct inserter : container_ref<Key, T, AssociativeContainer>
{
    typedef container_ref<Key, T, AssociativeContainer> base_class;

    inserter (typename base_class::container_type & ac) : base_class(ac) {}

    void insert (Key const & key, T const & value)
    {
        this->p->insert(key, value);
    }
};

template <typename Key
        , typename T
        , typename AssociativeContainer>
struct finder : container_ref<Key, T, AssociativeContainer>
{
    typedef container_ref<Key, T, AssociativeContainer> base_class;
    typedef typename base_class::iterator iterator;
    typedef typename base_class::const_iterator const_iterator;

    finder (typename base_class::container_type & ac) : base_class(ac) {}

    iterator find (Key const & key)
    {
        return this->p->find(key);
    }

    const_iterator find (Key const & key) const
    {
        return this->p->find(key);
    }
};

} // details

template <typename Key
        , typename T
        , typename AssociativeContainerTag>
struct type_traits;

template <typename Key
        , typename T
        , typename AssociativeContainer>
struct container_ref : details::container_ref<Key, T, AssociativeContainer>
{
    typedef details::container_ref<Key, T, AssociativeContainer> base_class;
    container_ref (typename base_class::container_type & ac) : base_class(ac) {}
};

template <typename Key
        , typename T
        , typename AssociativeContainer>
struct iterators : details::iterators<Key, T, AssociativeContainer>
{
    typedef details::iterators<Key, T, AssociativeContainer> base_class;
    typedef typename base_class::iterator iterator;
    typedef typename base_class::const_iterator const_iterator;
    iterators (typename base_class::container_type & ac) : base_class(ac) {}
};

template <typename Key
        , typename T
        , typename AssociativeContainer>
struct inserter : details::inserter<Key, T, AssociativeContainer>
{
    typedef details::inserter<Key, T, AssociativeContainer> base_class;
    inserter (typename base_class::container_type & ac) : base_class(ac) {}
};

template <typename Key
        , typename T
        , typename AssociativeContainer>
struct finder : details::finder<Key, T, AssociativeContainer>
{
    typedef details::finder<Key, T, AssociativeContainer> base_class;
    finder (typename base_class::container_type & ac) : base_class(ac) {}
};

}} // pfs::associative_container
