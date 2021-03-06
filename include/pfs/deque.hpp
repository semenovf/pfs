#pragma once
#include <pfs/stdcxx/deque.hpp>

namespace pfs {

#if __cplusplus >= 201103L

template <typename T>
using deque = stdcxx::deque<T>;

#else

template <typename T>
class deque : public stdcxx::deque<T, deque<T> >
{
    typedef stdcxx::deque<T, deque<T> > base_class;

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
    deque () : base_class() {}

    deque (size_type count, const_reference value)
        : base_class(count, value)
    {}

    explicit deque (size_type count)
        : base_class(count)
    {}

    template <typename InputIt>
    deque (InputIt first, InputIt last)
        : base_class(first, last)
    {}
};

#endif

} // pfs

