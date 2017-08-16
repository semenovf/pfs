/* 
 * File:   stack.hpp
 * Author: wladt
 *
 * Created on June 26, 2017, 2:26 PM
 */

#ifndef __PFS_TRAITS_STACK_HPP__
#define __PFS_TRAITS_STACK_HPP__

namespace pfs {
namespace traits {

template <typename T, template <typename> class ContainerValueRef>
class stack
{
protected:
    typedef ContainerValueRef<T> internal_type;
    
public:
    typedef typename internal_type::native_type            native_type;
    typedef typename internal_type::native_reference       native_reference;
    typedef typename internal_type::const_native_reference const_native_reference;

    typedef typename internal_type::value_type             value_type;
    typedef typename internal_type::reference              reference;
    typedef typename internal_type::const_reference        const_reference;
    typedef typename internal_type::size_type              size_type;
    
protected:
    internal_type _p;

public:
    // Acceptable for container_value only (as opposite to container_reference)
    stack ()
    {}
    
    /**
     * Initializes with reference to native container
     */
    stack (native_reference rhs)
        : _p(rhs)
    {}

    /**
     * Casts to const native container reference
     */
    operator const_native_reference () const
    {
        return _p;
    }

    /**
     * Casts to native container reference
     */
    operator native_reference ()
    {
        return _p;
    }
    
    /**
     *  Returns true if the %stack is empty.
     */
    bool empty () const
    {
        return _p.empty();
    }

    /**  Returns the number of elements in the %stack.  */
    size_type size () const
    {
        return _p.size();
    }

    /**
     *  Returns a read/write reference to the data at the first
     *  element of the %stack.
     */
    reference top ()
    {
        //return _p.back();
        return _p.top();
    }

    /**
     *  Returns a read-only (constant) reference to the data at the first
     *  element of the %stack.
     */
    const_reference top () const
    {
        //return _p.back();
        return _p.top();
    }

    /**
     *  @brief  Add data to the top of the %stack.
     *  @param  x  Data to be added.
     *
     *  This is a typical %stack operation.  The function creates an
     *  element at the top of the %stack and assigns the given data
     *  to it.  The time complexity of the operation depends on the
     *  underlying sequence.
     */
    void push (value_type const & x)
    {
        _p.push(x);
    }

#if __cplusplus >= 201103L

    void push (value_type && x)
    {
        _p.push(x);
    }

    template <typename... Args>
    void emplace (Args &&... args)
    {
        _p.emplace(std::forward<Args>(args)...);
    }
#endif

    /**
     *  @brief  Removes first element.
     *
     *  This is a typical %stack operation.  It shrinks the %stack
     *  by one.  The time complexity of the operation depends on the
     *  underlying sequence.
     *
     *  Note that no data is returned, and if the first element's
     *  data is needed, it should be retrieved before pop() is
     *  called.
     */
    void pop ()
    {
        _p.pop();
    }

    void swap (stack & s) pfs_noexcept
    {
        _p.swap(s._p);
    }

    //
    // Non-standard methods
    //

    void clear ()
    {
        while (! this->empty())
            this->pop();
    }
};

}} //pfs::traits

#endif /* __PFS_TRAITS_STACK_HPP__ */

