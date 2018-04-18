#pragma once
#include <pfs/stdcxx/list.hpp>

namespace pfs {

template <typename T>
class list : public stdcxx::list<T, list<T> >
{
    typedef stdcxx::list<T, list> base_class;

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
    list () : base_class() {}

    list (size_type count, const_reference value)
        : base_class(count, value)
    {}

    explicit list (size_type count)
        : base_class(count)
    {}

    template <typename InputIt>
    list (InputIt first, InputIt last)
        : base_class(first, last)
    {}

    list (list const & rhs)
        : base_class(rhs)
    {}

#if __cplusplus >= 201103L
    list (list && rhs)
        : base_class(std::forward<list>(rhs))
    {}

    list (std::initializer_list<T> ilist)
        : base_class(ilist.begin(), ilist.end())
    {}
#endif
};

} // pfs
