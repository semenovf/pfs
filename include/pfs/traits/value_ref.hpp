#ifndef __PFS_TRAITS_VALUE_REF_HPP__
#define __PFS_TRAITS_VALUE_REF_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/utility.hpp>

namespace pfs {
namespace traits {

template <typename NativeType>
struct reference;

template <typename NativeType>
struct value
{
    typedef NativeType          native_type;
    typedef native_type &       native_reference;
    typedef native_type const & const_native_reference;

    typedef value<NativeType>     value_type;
    typedef reference<NativeType> reference_type;

    native_type v;

    value ()
    {}

    value (native_reference rhs)
        : v(rhs)
    {}

    value (const_native_reference rhs)
        : v(rhs)
    {}

    native_reference operator * ()
    {
        return v;
    }

    const_native_reference operator * () const
    {
        return v;
    }

    native_type * operator -> ()
    {
        return & v;
    }

    native_type const * operator -> () const
    {
        return & v;
    }
};

template <typename NativeType>
struct reference
{
    typedef NativeType          native_type;
    typedef native_type &       native_reference;
    typedef native_type const & const_native_reference;

    typedef value<NativeType>     value_type;
    typedef reference<NativeType> reference_type;

    native_type * p;

    reference (native_reference rhs)
        : p(& rhs)
    {}

    native_reference operator * ()
    {
        return *p;
    }

    const_native_reference operator * () const
    {
        return *p;
    }

    native_type * operator -> ()
    {
        return p;
    }

    native_type const * operator -> () const
    {
        return p;
    }

private:
    reference ();
    reference (const_native_reference rhs);
};

template <typename T, template <typename> class NativeContainerWrapper>
struct container_reference;

template <typename T, template <typename> class NativeContainerWrapper>
struct container_value
{
    typedef typename NativeContainerWrapper<T>::type native_type;
    typedef native_type &       native_reference;
    typedef native_type const & const_native_reference;

    typedef container_value<T, NativeContainerWrapper> container_value_type;
    typedef container_reference<T, NativeContainerWrapper>   container_reference_type;

    native_type v;

    container_value ()
    {}

    container_value (native_reference rhs)
        : v(rhs)
    {}

    container_value (const_native_reference rhs)
        : v(rhs)
    {}

    native_reference operator * ()
    {
        return v;
    }

    const_native_reference operator * () const
    {
        return v;
    }

    native_type * operator -> ()
    {
        return & v;
    }

    native_type const * operator -> () const
    {
        return & v;
    }
};

template <typename T, template <typename> class NativeContainerWrapper>
struct container_reference
{
    typedef typename NativeContainerWrapper<T>::type native_type;
    typedef native_type &       native_reference;
    typedef native_type const & const_native_reference;

    typedef container_value<T, NativeContainerWrapper>     container_value_type;
    typedef container_reference<T, NativeContainerWrapper> container_reference_type;

    native_type * p;
    

    container_reference (native_reference rhs)
        : p(& rhs)
    {}

    native_reference operator * ()
    {
        return *p;
    }
    
    const_native_reference operator * () const
    {
        return *p;
    }

    native_type * operator -> ()
    {
        return p;
    }
    
    native_type const * operator -> () const
    {
        return p;
    }
    
private:
    container_reference ();
    container_reference (const_native_reference rhs);
};

}} // pfs::traits

#endif /* __PFS_TRAITS_VALUE_REF_HPP__ */

