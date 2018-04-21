#pragma once
#include <pfs/stdcxx/set.hpp>

namespace pfs {

template <typename Key>
class set : public stdcxx::set<Key, set<Key> >
{
    typedef stdcxx::set<Key, set<Key> > base_class;

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
    set () : base_class() {}

    template <typename InputIt>
    set (InputIt first, InputIt last)
        : base_class(first, last)
    {}

    set (set const & rhs)
        : base_class(rhs)
    {}

#if __cplusplus >= 201103L
    set (set && rhs)
        : base_class(std::forward<set>(rhs))
    {}

    set (std::initializer_list<value_type> ilist)
        : base_class(ilist)
    {}
#endif
};

} // pfs
