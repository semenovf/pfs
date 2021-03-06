#ifndef __PFS_TRAITS_STRING_VALUE_REF_HPP__
#define __PFS_TRAITS_STRING_VALUE_REF_HPP__

namespace pfs {
namespace traits {

template <typename CharT, typename NativeString>
struct string_ref;

template <typename CharT, typename NativeString>
struct string_value
{
    typedef NativeString        native_type;
    typedef native_type &       native_reference;
    typedef native_type const & const_native_reference;
    typedef CharT               value_type;
    
    typedef string_value<CharT, NativeString> string_value_type;
    typedef string_ref<CharT, NativeString>   string_reference_type;
    
    native_type v;

    string_value ()
        : v()
    {}

    string_value (native_reference rhs)
        : v(rhs)
    {}

    string_value (const_native_reference rhs)
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

template <typename CharT, typename NativeString>
struct string_ref
{
    typedef NativeString        native_type;
    typedef native_type &       native_reference;
    typedef native_type const & const_native_reference;

    typedef string_value<CharT, NativeString> string_value_type;
    typedef string_ref<CharT, NativeString>   string_reference_type;

    native_type * p;
    
    string_ref (const_native_reference rhs)
        : p(& rhs)
    {}
    
    string_ref (native_reference rhs)
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
    string_ref ();
};

}} // pfs::traits

#endif /* __PFS_TRAITS_STRING_VALUE_REF_HPP__ */

