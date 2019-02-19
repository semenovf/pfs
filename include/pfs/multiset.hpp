#pragma once
#include <pfs/stdcxx/multiset.hpp>

namespace pfs {

#if __cplusplus >= 201103L

template <typename T>
using multiset = stdcxx::multiset<T>;

#else

template <typename T>
class multiset : public stdcxx::multiset<T, multiset<T> >
{
    typedef stdcxx::multiset<T, multiset<T> > base_class;

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
    multiset () : base_class() {}

    template <typename InputIt>
    multiset (InputIt first, InputIt last)
        : base_class(first, last)
    {}
};

#endif

} // pfs

