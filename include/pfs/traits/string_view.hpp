/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string_view.hpp
 * Author: wladt
 *
 * Created on February 7, 2017, 2:50 PM
 */

#ifndef __PFS_TRAITS_STRING_VIEW_HPP__
#define __PFS_TRAITS_STRING_VIEW_HPP__

#include <pfs/cxxlang.hpp>
#include <pfs/exception.hpp>

namespace pfs {
namespace traits {

template <typename CharT>
struct string_view_traits
{
    typedef CharT const * native_type;
    typedef size_t        size_type;
    typedef CharT const & const_reference;
    typedef CharT const * const_pointer;
    typedef CharT const * const_iterator;
    typedef CharT const * const_reverse_iterator;
    typedef native_type   data_type;

    static pfs_constexpr size_type xsize (data_type const & d);
    static pfs_constexpr const_iterator xbegin (data_type const & d);
    static pfs_constexpr const_iterator xend (data_type const & d);
    static pfs_constexpr const_reverse_iterator xrbegin (data_type const & d);
    static pfs_constexpr const_reverse_iterator xrend (data_type const & d);
    static pfs_constexpr const_reference xsubscript (data_type const & d, size_type pos);
    static pfs_constexpr const_reference xat (data_type const & d, size_type pos);
    static pfs_constexpr const_pointer xdata (data_type const & d);
    static pfs_constexpr void xremove_prefix (data_type & d, size_type n);
    static pfs_constexpr void xremove_suffix (data_type & d, size_type n);
    static pfs_constexpr void xswap (data_type & d1, data_type & d2);
    static size_type xcopy (data_type & d, CharT * dest, size_type count, size_type pos);
    static pfs_constexpr void xsubstr (data_type & d, data_type & dest, size_type pos, size_type count);
    static pfs_constexpr int xcompare (data_type const & d1, data_type const & d2);
    static pfs_constexpr size_type xfind (data_type const & d1, data_type const & d2, size_type pos) pfs_noexcept;
    static pfs_constexpr size_type xrfind (data_type const & d1, data_type const & d2, size_type pos) pfs_noexcept;
    static pfs_constexpr size_type xfind_first_of (data_type const & d1, data_type const & d2, size_type pos) pfs_noexcept;
    static pfs_constexpr size_type xfind_last_of (data_type const & d1, data_type const & d2, size_type pos) pfs_noexcept;
    static pfs_constexpr size_type xfind_first_not_of (data_type const & d1, data_type const & d2, size_type pos) pfs_noexcept;
    static pfs_constexpr size_type xfind_last_not_of (data_type const & d1, data_type const & d2, size_type pos) pfs_noexcept;
};

template <typename CharT>
class string_view
{
public:
    typedef string_view_traits<CharT>                    traits_type;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::const_reference        const_reference;
    typedef typename traits_type::const_pointer          const_pointer;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::data_type              data_type;
    
private:
    data_type _d;
    
public:
    pfs_constexpr basic_string_view ()
            : _d()
    {}
            
    pfs_constexpr basic_string_view (CharT const * s, size_type count)
        : _d(s, count)
    {}
    
    pfs_constexpr basic_string_view (CharT const * s)
        : _d(s)
    {}
    
    pfs_constexpr const_iterator begin () const
    {
        return traits_type::xbegin(_d);
    }
    
    pfs_constexpr const_iterator cbegin() const
    {
        return cbegin();
    }
    
    pfs_constexpr const_iterator end () const
    {
        return traits_type::xend(_d);
    }

    pfs_constexpr const_iterator cend () const
    {
        return end();
    }
    
    pfs_constexpr const_reverse_iterator rbegin () const
    {
        return traits_type::xrbegin(_d);
    }

    pfs_constexpr const_reverse_iterator crbegin () const
    {
        return rbegin();
    }
    
    pfs_constexpr const_reverse_iterator rend () const
    {
        return traits_type::xrend(_d);
    }

    pfs_constexpr const_reverse_iterator crend () const
    {
        return rend();
    }
    
    pfs_constexpr const_reference operator [] (size_type pos) const
    {
        return traits_type::xsubscript(_d, pos);
    }
    
    pfs_constexpr const_reference at (size_type pos) const
    {
        if (pos >= size())
            throw out_of_range("string_view::at");
        return traits_type::xat(_d, pos);
    }
    
    pfs_constexpr const_reference front () const
    {
        return traits_type::xsubscript(_d, 0);
    }
    
    pfs_constexpr const_reference back () const
    {
        return traits_type::xsubscript(_d, size() - 1);
    }
    
    pfs_constexpr const_pointer data () const
    {
        return traits_type::xdata(_d);
    }
    
    pfs_constexpr size_type size () const pfs_noexcept
    {
        return traits_type::xsize(_d);
    }
	
    constexpr size_type length () const pfs_noexcept
    {
        return size();
    }
    
    pfs_constexpr bool empty() const pfs_noexcept
    {
        return size() == 0;
    }
    
    pfs_constexpr void remove_prefix (size_type n)
    {
        traits_type::xremove_prefix(_d, n);
    }
    
    pfs_constexpr void remove_suffix (size_type n)
    {
        traits_type::xremove_suffix(_d, n);
    }
    
    pfs_constexpr void swap (string_view & v) pfs_noexcept
    {
        traits_type::xswap(_d, v._d);
    }
    
    size_type copy (CharT * dest, size_type count, size_type pos = 0) const
    {
        if (pos > size())
            throw out_of_range("string_view::copy");

        traits_type::xcopy(_d, dest, count, pos);
    }
    
    pfs_constexpr string_view substr (size_type pos = 0, size_type count = size_type(-1)) const
    {
        if (pos > size())
            throw out_of_range("string_view::substr");
        
        string_view r;
        traits_type::xsubstr(_d, r, pos, count);
        return r;
    }
    
    pfs_constexpr int compare (string_view v) const
    {
        return traits_type::xcompare(_d, v._d);
    }
    
    pfs_constexpr int compare (size_type pos1, size_type count1
                  , string_view v) const
    {
        return substr(pos1, count1).compare(v);
    }
    
    pfs_constexpr int compare (size_type pos1, size_type count1, string_view v
                , size_type pos2, size_type count2) const
    {
        return substr(pos1, count1).compare(v.substr(pos2, count2));
    }
    
    pfs_constexpr int compare (CharT const * s) const
    {
        return compare(string_view(s));
    }
    
    pfs_constexpr int compare (size_type pos1, size_type count1
                , CharT const * s) const
    {
        return substr(pos1, count1).compare(string_view(s));
    }
    
    pfs_constexpr int compare (size_type pos1, size_type count1
                , CharT const * s, size_type count2) const
    {
        return substr(pos1, count1).compare(basic_string_view(s, count2));
    }
    
    pfs_constexpr size_type find (string_view v, size_type pos = 0) const pfs_noexcept
    {
        return traits_type::xfind(_d, v._d, pos);
    }

    pfs_constexpr size_type find (CharT c, size_type pos = 0) const pfs_noexcept
    {
        return find(string_view(& c, 1), pos);
    }

    pfs_constexpr size_type find (CharT const * s, size_type pos, size_type count) const
	{
        return find(string_view(s, count), pos);
    }

    pfs_constexpr size_type find (CharT const * s, size_type pos = 0) const
    {
        return find(basic_string_view(s), pos);
    }
    
    pfs_constexpr size_type rfind (string_view v, size_type pos = size_type(-1)) const pfs_noexcept
    {
        return traits_type::xrfind(_d, v._d, pos);
    }

    pfs_constexpr size_type rfind (CharT c, size_type pos = size_type(-1)) const pfs_noexcept
    {
        return rfind(string_view(& c, 1), pos);
    }

    pfs_constexpr size_type rfind (CharT const * s, size_type pos, size_type count) const
    {
        return rfind(string_view(s, count), pos);
    }
    
    pfs_constexpr size_type rfind (CharT const * s, size_type pos = size_type(-1)) const
    {
        return rfind(string_view(s), pos);
    }
    
    pfs_constexpr size_type find_first_of (string_view v, size_type pos = 0) const pfs_noexcept
    {
        return traits_type::xfind_first_of(_d, v._d, pos);
    }

    pfs_constexpr size_type find_first_of (CharT c, size_type pos = 0) const  pfs_noexcept
    {
        return find_first_of(string_view(& c, 1), pos);
    }

    pfs_constexpr size_type find_first_of (CharT const * s, size_type pos, size_type count) const
    {
        return find_first_of(string_view(s, count), pos);
    }

    pfs_constexpr size_type find_first_of (CharT const * s, size_type pos = 0) const
    {
        return find_first_of(string_view(s), pos);
    }
    
    pfs_constexpr size_type find_last_of (string_view v, size_type pos = size_type(-1)) const pfs_noexcept
    {
        return traits_type::xfind_last_of(_d, v._d, pos);
    }
	
    pfs_constexpr size_type find_last_of (CharT c, size_type pos = size_type(-1)) const pfs_noexcept
    {
        return find_last_of(string_view(& c, 1), pos);
    }

    pfs_constexpr size_type find_last_of (CharT const * s, size_type pos, size_type count) const
    {
        return find_last_of(string_view(s, count), pos);
    }

    pfs_constexpr size_type find_last_of (CharT const * s, size_type pos = size_type(-1)) const
    {
        return find_last_of(basic_string_view(s), pos);
    }
    
    pfs_constexpr size_type find_first_not_of (string_view v, size_type pos = 0) const pfs_noexcept
    {
        return traits_type::xfind_first_not_of(_d, v._d, pos);
    }

    pfs_constexpr size_type find_first_not_of (CharT c, size_type pos = 0) const pfs_noexcept
    {
        return find_first_not_of(string_view(& c, 1), pos);
    }

    pfs_constexpr size_type find_first_not_of (CharT const * s, size_type pos, size_type count) const
    {
        return find_first_not_of(string_view(s, count), pos);
    }

    pfs_constexpr size_type find_first_not_of (CharT const * s, size_type pos = 0) const
    {
        return find_first_not_of(string_view(s), pos);
    }
    
    pfs_constexpr size_type find_last_not_of (string_view v, size_type pos = size_type(-1)) const pfs_noexcept
    {
        return traits_type::xfind_last_not_of(_d, v._d, pos);
    }

    pfs_constexpr size_type find_last_not_of (CharT c, size_type pos = size_type(-1)) const pfs_noexcept
    {
        return find_last_not_of(string_view(&c, 1), pos);
    }

    pfs_constexpr size_type find_last_not_of (CharT const * s, size_type pos, size_type count) const
    {
        return find_last_not_of(string_view(s, count), pos);
    }

    pfs_constexpr size_type find_last_not_of (CharT const * s, size_type pos = size_type(-1)) const
    {
        return find_last_not_of(string_view(s), pos);
    }
};

template <typename CharT>
pfs_constexpr bool operator == (string_view<CharT> lhs, string_view<CharT> rhs) pfs_noexcept
{
    return lhs.compare(rhs) == 0;
}

template <typename CharT>
pfs_constexpr bool operator != (string_view<CharT> lhs, string_view<CharT> rhs) pfs_noexcept
{
    return lhs.compare(rhs) != 0;
}

template <typename CharT>
pfs_constexpr bool operator < (string_view<CharT> lhs, string_view<CharT> rhs) pfs_noexcept
{
    return lhs.compare(rhs) < 0 ;
}

template <typename CharT>
pfs_constexpr bool operator <= (string_view<CharT> lhs, string_view<CharT> rhs) pfs_noexcept
{
    return lhs.compare(rhs) <= 0;
}

template <typename CharT>
pfs_constexpr bool operator > (string_view <CharT> lhs, string_view<CharT> rhs) pfs_noexcept
{
    return lhs.compare(rhs) > 0 ;
}

template <typename CharT>
pfs_constexpr bool operator >= (string_view <CharT> lhs, string_view<CharT> rhs) pfs_noexcept
{
    return lhs.compare(rhs) >= 0;
}

}} // pfs::traits

namespace std {

template <typename CharT>
basic_ostream<CharT> & operator << (basic_ostream<CharT> & os
        , pfs::traits::string_view<CharT> v);

} // std

#endif /* __PFS_TRAITS_STRING_VIEW_HPP__ */

