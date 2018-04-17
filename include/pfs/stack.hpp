#pragma once
#include <pfs/stdcxx/stack.hpp>
#include <pfs/deque.hpp>

namespace pfs {

template <typename T, typename Container = pfs::deque<T> >
class stack : public stdcxx::stack<T, Container, stack<T, Container> >
{
    typedef stdcxx::stack<T, Container, stack<T, Container> > base_class;

public:
    typedef Container                           container_type;
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;

public:
    ///////////////////////////////////////////////////////////////////////////
    // Constructors                                                          //
    ///////////////////////////////////////////////////////////////////////////
    
    stack () : base_class() {}
    
    explicit stack (Container const & cont)
        : base_class(cont)
    {}
    
    stack (stack const & rhs)
        : base_class(rhs)
    {}

#if __cplusplus >= 201103L

    stack (stack && rhs)
        : base_class(std::forward<stack>(rhs))
    {}

    explicit stack (Container && cont)
        : base_class(std::forward<Container>(cont))
    {}

#endif
};

} //pfs
