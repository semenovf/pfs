/* 
 * File:   filesystem.hpp
 * Author: wladt
 *
 * Created on June 20, 2017, 11:05 AM
 */

#ifndef __PFS_FILESYSTEM_HPP__
#define __PFS_FILESYSTEM_HPP__

#include <ostream>
#include <istream>
#include <pfs/compiler.hpp> // Implementation may depends on compiler version.
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(filesystem)

namespace pfs {
namespace filesystem {

/** 
 * @class path
 * @brief Objects of type path represent paths on a filesystem.
 *
 * @details Only syntactic aspects of paths are handled: 
 *          the pathname may represent a non-existing path or even 
 *          one that is not allowed to exist on the current file system or OS...
 *          Details see for [std::filesystem::path](http://en.cppreference.com/w/cpp/filesystem/path)
 */

template <typename PathImpl>
class basic_path
{
public:    
    typedef typename PathImpl::value_type  value_type;
    typedef typename PathImpl::string_type string_type;
    typedef typename PathImpl::iterator    iterator;

    enum format {
          native_format   ///< Native pathname format
        , generic_format  ///< Generic pathname format
        , auto_format     ///< Implementation-defined pathname format, auto-detected where possible
    };

protected:
    PathImpl _d;
    
    basic_path (PathImpl const & d)
        : _d(d)
    {}
    
public:
    basic_path ()
        : _d()
    {}
        
    basic_path (basic_path const & p)
        : _d(p._d)
    {}

    // TODO implement
    // path (string_type && source, format fmt = auto_format)
    //      : base_class(source)
    // {}

    template <typename Source>
    basic_path (Source const & source, format fmt = auto_format)
        :_d(source, fmt)
    {}

    template <typename InputIt>
    basic_path (InputIt first, InputIt last, format fmt = auto_format)
        : _d(first, last, fmt)
    {}

    template <typename Source>
    basic_path (Source const & source, std::locale const & loc, format fmt = auto_format)
        : _d(source, loc, fmt)
    {}

    // FIXME fmt unused yet
    template <typename InputIt>
    basic_path (InputIt first, InputIt last, std::locale const & loc, format fmt = auto_format)
        : _d(first, last, loc, fmt)
    {}
    
    ~basic_path () {}
    
    basic_path & operator = (basic_path const & p)
    {
        _d = p._d;
        return *this;
    }
    
    // TODO implement
    // path & operator = (string_type && source)
    // {}
    
    template <typename Source>
    basic_path & operator = (Source const & source)
    {
        _d.operator = <Source>(source);
        return *this;
    }

    template <typename Source>
    basic_path & assign (Source const & source)
    {
        _d.assign<Source>(source);
        return *this;
    }

    template <typename InputIt>
    basic_path & assign (InputIt first, InputIt last)
    {
        _d.assign<InputIt>(first, last);
        return *this;
    }
    
    //==========================================================================
    // Concatenation
    //==========================================================================
    basic_path & operator /= (basic_path const & p)
    {
        _d.operator /= (p._d);
        return *this;
    }

    template <typename Source>
    basic_path & operator /= (Source const & source)
    {
        _d.operator /= <Source> (source);
        return *this;
    }
    
    template <typename Source>
    basic_path & append (Source const & source)
    {
        _d.append<Source>(source);
        return *this;
    }
    
    template <typename InputIt>
    basic_path & append (InputIt first, InputIt last)
    {
        _d.append<InputIt>(first, last);
        return *this;
    }

    basic_path & operator += (basic_path const & p)
    {
        _d.operator += (p._d);
        return *this;
    }

    basic_path & operator += (string_type const & str)
    {
        _d.operator += (str);
        return *this;
    }

    // TODO Implement
    //path & operator+= (std::basic_string_view<value_type> str);

    basic_path & operator += (value_type const * ptr)
    {
        _d += ptr;
        return *this;
    }
    
    basic_path & operator += (value_type x)
    {
        _d += x;
        return *this;
    }
    
    
    template <typename Source>
    basic_path & operator += (Source const & source)
    {
        _d.operator += <Source> (source);
        return *this;
    }
    
    template <typename CharT>
    basic_path & operator += (CharT x)
    {
        _d.operator += <CharT> (x);
        return *this;
        
    }
    
    template <typename Source>
    basic_path & concat (Source const & source)
    {
        _d.concat<Source>(source);
        return *this;
    }
    
    template <typename InputIt>
    basic_path & concat (InputIt first, InputIt last)
    {
        _d.concat<InputIt>(first, last);
        return *this;
    }
    
    //==========================================================================
    // Modifiers
    //==========================================================================
    
    void clear ()
    {
        _d.clear();
    }
    
    basic_path & make_preferred ()
    {
        _d.make_preferred();
        return *this;
    }
    
    basic_path & remove_filename ()
    {
        _d.remove_filename();
        return *this;
    }
    
    basic_path & replace_filename (basic_path const & replacement)
    {
        _d.replace_filename(replacement._d);
        return *this;
    }
    
    basic_path & replace_extension (basic_path const & replacement = path())
    {
        _d.replace_extension(replacement._d);
        return *this;
    }
    
    void swap (basic_path & other)
    {
        _d.swap(other._d);
    }
    
    //==========================================================================
    // Format observers
    //==========================================================================
    
    value_type const * c_str () const
    {
        return _d.c_str();
    }
    
    string_type const & native () const
    {
        return _d.native();
    }
    
    operator string_type () const
    {
        return _d.operator string_type ();
    }

//    template <typename CharT
//            , typename Traits = pfs::char_traits<CharT>
//            , typename Alloc = pfs::allocator<CharT> >
//    std::basic_string<CharT, Traits, Alloc>
//    path::string (Alloc const & a = Allocator()) const;
//    
//    std::string path::string () const;
//    std::wstring path::wstring () const;
//    std::string path::u8string () const;
    
//    std::u16string path::u16string () const;
//    std::u32string path::u32string () const;
     
//    template <typename CharT, typename Traits = pfs::char_traits<CharT>
//            , typename Alloc = pfs::allocator<CharT> >
//    std::basic_string<CharT, Traits, Alloc>
//    path::generic_string (Allocator const & a = Allocator()) const;
//
//    std::string path::generic_string () const;
//    std::wstring path::generic_wstring () const;
//    std::string path::generic_u8string () const;
//    std::u16string path::generic_u16string () const
//    std::u32string path::generic_u32string () const
    
    //==========================================================================
    // Compare
    //==========================================================================
    
    int compare (basic_path const & p) const
    {
        return _d.compare(p._d);
    }
    
    int compare (string_type const & str) const
    {
        return _d.compare(str);
    }

     // TODO Implement
    //int compare (std::basic_string_view<value_type> str) const;

    int compare (value_type const * s) const
    {
        return _d.compare(s);
    }

    //==========================================================================
    // Generation
    //==========================================================================
    
    basic_path lexically_normal () const
    {
        return basic_path(_d.lexically_normal());
    }
    
    basic_path lexically_relative (path const & base) const
    {
        return basic_path(_d.lexically_relative());
    }
    
    basic_path lexically_proximate (path const & base) const
    {
        return basic_path(_d.lexically_proximate());
    }
    
    //==========================================================================
    // Decomposition
    //==========================================================================

    basic_path root_name () const
    {
        return basic_path(_d.root_name());
    }
    
    basic_path root_directory () const
    {
        return basic_path(_d.root_directory());
    }
    
    basic_path root_path () const
    {
        return basic_path(_d.root_path());
    }
    
    basic_path relative_path () const
    {
        return basic_path(_d.relative_path());
    }
    
    basic_path parent_path () const
    {
        return basic_path(_d.parent_path());
    }
    
    basic_path filename () const
    {
        return basic_path(_d.filename());
    }

    basic_path stem () const
    {
        return basic_path(_d.stem());
    }
    
    basic_path extension () const
    {
        return basic_path(_d.extension());
    }

    //==========================================================================
    // Queries
    //==========================================================================
    
    bool empty () const
    {
        return _d.empty();
    }
    
    bool has_root_path () const
    {
        return _d.has_root_path();
    }
    
    bool has_root_name () const
    {
        return _d.has_root_name();
    }
    
    bool has_root_directory () const
    {
        return _d.has_root_directory();
    }
    
    bool has_relative_path () const
    {
        return _d.has_relative_path();
    }
    
    bool has_parent_path () const
    {
        return _d.has_parent_path();
    }
    
    bool has_filename () const
    {
        return _d.has_filename();
    }

    bool has_stem () const
    {
        return _d.has_stem();
    }

    bool has_extension () const
    {
        return _d.has_extension();
    }
    
    bool is_absolute () const
    {
        return _d.is_absolute();
    }
    
    bool is_relative () const
    {
        return _d.is_relative();
    }
    
    //==========================================================================
    // Iterators
    //==========================================================================

    iterator begin () const
    {
        return _d.begin();
    }
    
    iterator end () const
    {
        return _d.end();
    }
    
    friend basic_path operator / (basic_path const & lhs, basic_path const & rhs)
    {
        return basic_path(lhs._d / rhs._d);
    }
    
    friend void swap (basic_path & lhs, basic_path & rhs)
    {
        lhs._d.swap(rhs._d);
    }
    
    friend bool operator == (basic_path const & lhs, basic_path const & rhs)
    {
        return lhs._d == rhs._d;
    }
    
    friend bool operator != (basic_path const & lhs, basic_path const & rhs)
    {
        return lhs._d != rhs._d;
    }
    
    friend bool operator <  (basic_path const & lhs, basic_path const & rhs)
    {
        return lhs._d < rhs._d;
    }
    
    friend bool operator <= (basic_path const & lhs, basic_path const & rhs)
    {
        return lhs._d <= rhs._d;
    }
    
    friend bool operator >  (basic_path const & lhs, basic_path const & rhs)
    {
        return lhs._d > rhs._d;    
    }
    
    friend bool operator >= (basic_path const & lhs, basic_path const & rhs)
    {
        return lhs._d >= rhs._d;
    }
    
    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &
    operator << (std::basic_ostream<CharT,Traits> & os, basic_path const & p)
    {
        return os << (os, p._d);
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &
    operator >> (std::basic_istream<CharT, Traits> & is, basic_path & p)
    {
        return is >> p._d;
    }
};
 
//
//template <typename Source>
//path u8path (Source const & source);
//
//template <typename InputIt>
//path u8path (InputIt first, InputIt last);
//
//// TODO Implement file_status
//bool exists (filesystem::file_status s);
//
//bool exists (path const & p);
//bool exists (path const & p, pfs::error_code & ec);

}} // pfs::filesystem

namespace pfs {

template <typename StringType>
inline StringType to_string (filesystem::path const & p)
{
    return StringType(p.native());
}

} // pfs

#endif /* __PFS_FILESYSTEM_HPP__ */
