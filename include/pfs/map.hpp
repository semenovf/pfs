#pragma once
#include <pfs/utility.hpp>
#include <pfs/stdcxx/map.hpp>

namespace pfs {

template <typename Key, typename T>
#if __cplusplus >= 201103L

using map = stdcxx::map<Key, T>;

#else

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
};

#endif

} // pfs

