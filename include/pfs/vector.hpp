#pragma once
#include <pfs/stdcxx/vector.hpp>

namespace pfs {

#if __cplusplus >= 201103L

template <typename T>
using vector = stdcxx::vector<T>;

#else

template <typename T>
class vector : public stdcxx::vector<T, vector<T> >
{
    typedef stdcxx::vector<T, vector<T> > base_class;

public:
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
    vector () : base_class() {}

    vector (size_type count, const_reference value)
        : base_class(count, value)
    {}

    explicit vector (size_type count)
        : base_class(count)
    {}

    template <typename InputIt>
    vector (InputIt first, InputIt last)
        : base_class(first, last)
    {}
};

#endif

} // pfs


