#pragma once
#include <stack>

namespace pfs {
namespace stdcxx {

#if __cplusplus >= 201103L
    template <typename T, typename Container>
#else
    template <typename T, typename Container, typename DerivedT>
#endif
class stack : public std::stack<T, Container>
{
    typedef std::stack<T, Container> base_class;

public:
#if __cplusplus >= 201103L
    using DerivedT = stack;
#endif
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

    ///////////////////////////////////////////////////////////////////////////
    // Assign operators and methods                                          //
    ///////////////////////////////////////////////////////////////////////////

    DerivedT & operator = (DerivedT const & rhs)
    {
        base_class::operator = (rhs);
        return *this;
    }

#if __cplusplus >= 201103L
    DerivedT & operator = (DerivedT && rhs)
    {
        base_class::operator = (std::forward<DerivedT>(rhs));
        return *this;
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Element access                                                        //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn reference top ()
     */

    /**
     * @fn const_reference top() const
     */

    ///////////////////////////////////////////////////////////////////////////
    // Capacity                                                              //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn bool empty () const
     */

    /**
     * @fn size_type size () const
     */

    ///////////////////////////////////////////////////////////////////////////
    // Modifiers                                                             //
    ///////////////////////////////////////////////////////////////////////////

    /**
     * @fn void push (const_reference value)
     */

#if __cplusplus >= 201103L
    /**
     * @fn void push (value_type && value)
     */
#endif

#if __cplusplus >= 201103L
    /**
     * @fn template <typename... Args>
     *     void emplace (Args &&... args)
     */
#endif

    /**
     * @fn void pop ()
     */

    // TODO Implement for C++98
    // void swap( stack& other )
};

}} // pfs::stdcxx
