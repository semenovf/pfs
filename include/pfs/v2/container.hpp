#pragma once
#include <pfs/cxxlang.hpp>

//
// Based on [C++ concepts: Container](http://en.cppreference.com/w/cpp/concept/Container)
//

namespace pfs {
namespace container {
namespace details {

template <typename T, typename Container>
struct container_ref
{
    typedef Container container_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;
    typedef T value_type;

    container_type * p;

    container_ref (container_type & ac) : p(& ac) {}
};

template <typename T, typename Container>
struct iterators : container_ref<T, Container>
{
    typedef container_ref<T, Container> base_class;
    typedef typename base_class::container_type container_type;
    typedef typename base_class::iterator iterator;
    typedef typename base_class::const_iterator const_iterator;
    typedef typename base_class::value_type value_type;

    iterators (container_type & ac) : base_class(ac) {}

    inline iterator begin ()
    {
        this->p->begin();
    }

    inline const_iterator begin () const
    {
        this->p->begin();
    }

    inline const_iterator cbegin () const
    {
        this->p->begin();
    }

    inline iterator end ()
    {
        this->p->end();
    }

    inline const_iterator end () const
    {
        this->p->end();
    }

    inline const_iterator cend () const
    {
        this->p->end();
    }

    value_type & value (iterator it)
    {
        return it->second;
    }

    value_type const & value (iterator it) const
    {
        return *it;
    }

    value_type const & value (const_iterator it) const
    {
        return *it;
    }
};

} // details

template <typename T, typename ContainerTag>
struct type_traits;

template <typename T, typename Container>
struct container_ref : details::container_ref<T, Container>
{
    typedef details::container_ref<T, Container> base_class;
    container_ref (typename base_class::container_type & ac) : base_class(ac) {}
};

template <typename T, typename Container>
struct iterators : details::iterators<T, Container>
{
    typedef details::iterators<T, Container> base_class;
    iterators (typename base_class::container_type & ac) : base_class(ac) {}
};

}} // pfs::container

