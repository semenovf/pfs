/* 
 * File:   path.hpp
 * Author: wladt
 *
 * Created on June 30, 2017, 6:45 PM
 */

#ifndef __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__
#define __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__

namespace pfs {
namespace filesystem {

class path
{
public:    
//    typedef base_class::value_type  value_type;
//    typedef base_class::string_type string_type;
//    typedef base_class::iterator    iterator;

    enum format {
          native_format   ///< Native pathname format
        , generic_format  ///< Generic pathname format
        , auto_format     ///< Implementation-defined pathname format, auto-detected where possible
    };
    
public:
    path ();
    path (path const & p);

    // TODO implement
    // path (string_type && source, format fmt = auto_format)
    //      : base_class(source)
    // {}

    // FIXME fmt unused yet
    template <typename Source>
    path (Source const & source, format fmt = auto_format);

    // FIXME fmt unused yet
    template <typename InputIt>
    path (InputIt first, InputIt last, format fmt = auto_format);

    // FIXME fmt unused yet
    template <typename Source>
    path (Source const & source, std::locale const & loc, format fmt = auto_format);

    // FIXME fmt unused yet
    template <typename InputIt>
    path (InputIt first, InputIt last, std::locale const & loc, format fmt = auto_format);
    
    ~path ();
    
    path & operator = (path const & p);
    
    // TODO implement
    // path & operator = (string_type && source)
    // {}
    
    template <typename Source>
    path & operator = (Source const & source);

    template <typename Source>
    path & assign (Source const & source);

    template <typename InputIt>
    path & assign (InputIt first, InputIt last);
    
    //==========================================================================
    // Concatenation
    //==========================================================================
    path & operator /= (path const & p);

    template <typename Source>
    path & operator /= (Source const & source);
    
    template <typename Source>
    path & append (Source const & source);
    
    template <typename InputIt>
    path & path::append (InputIt first, InputIt last);

    path & path::operator += (path const & p);

    path & path::operator += (string_type const & str);

    // TODO Implement
    //path & operator+= (std::basic_string_view<value_type> str);

    path & path::operator += (value_type const * ptr);
    
    path & path::operator += (value_type x);
    
    template <typename Source>
    path & path::operator += (Source const & source);
    
    template <typename CharT>
    path & path::operator += (CharT x);
    
    template <typename Source>
    path & path::concat (Source const & source);
    
    template <typename InputIt>
    path & path::concat (InputIt first, InputIt last);
    
    //==========================================================================
    // Modifiers
    //==========================================================================
    
    void path::clear ();
    path & path::make_preferred ();
    path & path::remove_filename ();
    path & path::replace_filename (path const & replacement);
    path & replace_extension (path const & replacement = path());
    void path::swap (path & other);
    
    //==========================================================================
    // Format observers
    //==========================================================================
    
    value_type const * c_str () const;
    string_type const & native () const;
    path operator string_type () const;

    template <typename CharT
            , typename Traits = pfs::char_traits<CharT>
            , typename Alloc = pfs::allocator<CharT> >
    std::basic_string<CharT, Traits, Alloc>
    path::string (Alloc const & a = Allocator()) const;
    
    std::string path::string () const;
    std::wstring path::wstring () const;
    std::string path::u8string () const;
    
//    std::u16string path::u16string () const;
//    std::u32string path::u32string () const;
     
    template <typename CharT, typename Traits = pfs::char_traits<CharT>
            , typename Alloc = pfs::allocator<CharT> >
    std::basic_string<CharT, Traits, Alloc>
    path::generic_string (Allocator const & a = Allocator()) const;

    std::string path::generic_string () const;
    std::wstring path::generic_wstring () const;
    std::string path::generic_u8string () const;
//    std::u16string path::generic_u16string () const
//    std::u32string path::generic_u32string () const
    
    //==========================================================================
    // Compare
    //==========================================================================
    
    int path::compare (path const & p) const;
    int path::compare (string_type const & str) const;

     // TODO Implement
    //int compare (std::basic_string_view<value_type> str) const;

    int path::compare (value_type const * s) const;

    //==========================================================================
    // Generation
    //==========================================================================
    
    // TODO Implement
    path lexically_normal () const;
    
    // TODO Implement
    path lexically_relative (path const & base) const;
    
    // TODO Implement
    path lexically_proximate (path const & base) const;
    
    //==========================================================================
    // Decomposition
    //==========================================================================

    path path::root_name () const;
    path path::root_directory () const;
    path path::root_path () const;
    path relative_path () const;
    path path::parent_path () const;
    path path::filename () const;

    // FIXME (For description see test_path.hpp)
    path path::stem () const;
    
    // FIXME (For description see test_path.hpp)
    path path::extension () const;

    //==========================================================================
    // Queries
    //==========================================================================
    
    bool path::empty () const;
    bool path::has_root_path () const;
    bool path::has_root_name () const;
    bool path::has_root_directory () const;
    bool path::has_relative_path () const;
    bool path::has_parent_path () const;
    bool path::has_filename () const;

    // FIXME (For description see test_path.hpp)
    bool path::has_stem () const;

    // FIXME (For description see test_path.hpp)
    bool path::has_extension () const;
    bool path::is_absolute () const;
    bool path::is_relative () const;
    
    //==========================================================================
    // Iterators
    //==========================================================================

    iterator path::begin () const;
    iterator path::end () const;
};

path operator / (path const & lhs, path const & rhs);

inline void swap (path & lhs, path & rhs);

bool operator == (path const & lhs, path const & rhs);
bool operator != (path const & lhs, path const & rhs);
bool operator <  (path const & lhs, path const & rhs);
bool operator <= (path const & lhs, path const & rhs);
bool operator >  (path const & lhs, path const & rhs);
bool operator >= (path const & lhs, path const & rhs);
 
template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits> &
operator << (std::basic_ostream<CharT,Traits> & os, path const & p);

template <typename CharT, typename Traits>
std::basic_istream<CharT, Traits> &
operator >> (std::basic_istream<CharT, Traits> & is, path & p);

template <typename Source>
path u8path (Source const & source);

template <typename InputIt>
path u8path (InputIt first, InputIt last);

// TODO Implement file_status
bool exists (filesystem::file_status s);

bool exists (path const & p);
bool exists (path const & p, pfs::error_code & ec);

}} // pfs::filesystem

#endif /* __PFS_CXX_CXX98_FILESYSTEM_PATH_HPP__ */

