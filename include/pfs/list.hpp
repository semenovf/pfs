#pragma once
#include <pfs/stdcxx/list.hpp>

namespace pfs {
    
template <typename T>
class list : public stdcxx::list<T, list>
{
    typedef stdcxx::list<T, list> base_class;

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
        : base_class<InputIt>(first, last)
    {}
    
    list (list const & other)
        : base_class(other)
    {}
};
    
} // pfs
