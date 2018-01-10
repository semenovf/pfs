#ifndef __PFS_FILESYSTEM_PATH_HPP__
#define __PFS_FILESYSTEM_PATH_HPP__

#include <ostream>
#include <istream>
#include <pfs/cxxlang.hpp>
#include <pfs/string.hpp>
#include <pfs/filesystem/filesystem_error.hpp>
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(filesystem/path)

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

template <typename Impl>
class basic_path
{
    typedef Impl impl_type;
    
public:
    typedef typename impl_type::native_path_type native_path_type;
    typedef typename impl_type::value_type  value_type;
    typedef typename impl_type::string_type string_type;
    typedef typename impl_type::iterator    iterator;
    //typedef typename impl_type::format      format;

protected:
    impl_type _d;

public:
    basic_path ()
        : _d()
    {}
        
    basic_path (basic_path const & p)
        : _d(p._d)
    {}

    basic_path (string_type const & source/*, format fmt = auto_format*/)
        : _d(source)
    {}

    basic_path (char const * source/*, format fmt = auto_format*/)
        : _d(source)
    {}

    basic_path (wchar_t const * source/*, format fmt = auto_format*/)
        : _d(source)
    {}

    basic_path (native_path_type const & d)
        : _d(d)
    {}

    // TODO implement
    // path (string_type && source, format fmt = auto_format)
    //      : base_class(source)
    // {}

    /**
     * @brief Constructs the path from a character sequence.
     * 
     * @details Constructs the path from a character sequence
     *        (format interpreted as specified by @arg fmt) provided by source,
     *        which is a pointer or an input iterator to a null-terminated 
     *        character/wide character sequence, an std::basic_string or 
     *        an std::basic_string_view. Any of the four character types 
     *        @c char, @c char16_t, @c char32_t, @c wchar_t is allowed, 
     *        and the method of conversion to the native character set 
     *        depends on the character type used by source.
     * 
     *        * If the source character type is @c char, the encoding of the 
     *        source is assumed to be the native narrow encoding 
     *        (so no conversion takes place on POSIX systems).
     * 
     *        * If the source character type is @c char16_t, conversion from 
     *        UTF-16 to native filesystem encoding is used. 
     * 
     *        * If the source character type is @c char32_t, conversion from 
     *        UTF-32 to native filesystem encoding is used.
     *        
     *        * If the source character type is @c wchar_t, the input is assumed
     *        to be the native wide encoding (so no conversion takes places
     *        on Windows).
     * @param source 
     */
//    template <typename Source>
//    basic_path (Source const & source/*, format fmt = auto_format*/)
//        :_d(source)
//    {}

    template <typename StringImpl>
    basic_path (string<StringImpl> const & source/*, format fmt = auto_format*/);

    template <typename InputIt>
    basic_path (InputIt first, InputIt last)
        : _d(first, last)
    {}

//    template <typename Source>
//    basic_path (Source const & source, std::locale const & loc)
//        : _d(source, loc)
//    {}

//    template <typename InputIt>
//    basic_path (InputIt first, InputIt last, std::locale const & loc)
//        : _d(first, last, loc)
//    {}
    
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
        _d.template operator = <Source>(source);
        return *this;
    }
    
    native_path_type const & native_path () const
    {
        return _d;
    }
    
//    operator native_path_type () const
//    {
//        return _d;
//    }

    template <typename Source>
    basic_path & assign (Source const & source)
    {
        _d.template assign<Source>(source);
        return *this;
    }

    template <typename InputIt>
    basic_path & assign (InputIt first, InputIt last)
    {
        _d.template assign<InputIt>(first, last);
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
        _d.template operator /= <Source> (source);
        return *this;
    }
    
    template <typename Source>
    basic_path & append (Source const & source)
    {
        _d.template append<Source>(source);
        return *this;
    }
    
    template <typename InputIt>
    basic_path & append (InputIt first, InputIt last)
    {
        _d.template append<InputIt>(first, last);
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
        _d.template operator += <Source> (source);
        return *this;
    }
    
    template <typename CharT>
    basic_path & operator += (CharT x)
    {
        _d.template operator += <CharT> (x);
        return *this;
        
    }
    
    template <typename Source>
    basic_path & concat (Source const & source)
    {
        _d.template concat<Source>(source);
        return *this;
    }
    
    template <typename InputIt>
    basic_path & concat (InputIt first, InputIt last)
    {
        _d.template concat<InputIt>(first, last);
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
    
    /**
     * @brief Replaces a single filename component with replacement.
     * 
     * @param replacement Replacement path.
     * @return @c *this.
     */
    basic_path & replace_filename (basic_path const & replacement)
    {
        _d.replace_filename(replacement._d);
        return *this;
    }
    
    // FIXME for std::experimental::filesystem and boost::filesystem implementations
    basic_path & replace_extension (basic_path const & replacement = basic_path())
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
    
    basic_path lexically_relative (basic_path const & base) const
    {
        return basic_path(_d.lexically_relative());
    }
    
    basic_path lexically_proximate (basic_path const & base) const
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

    // FIXME (For description see test_path.hpp)
    // std::experimental::filesystem::path(".hidden").stem() returns ""
    // boost::filesystem::path(".hidden").stem()             returns ""
    // std::filesystem::path(".hidden").stem()               returns ".hidden"
    basic_path stem () const
    {
        return basic_path(_d.stem());
    }
    
    // FIXME (For description see test_path.hpp)
    // std::experimental::filesystem::path(".hidden").extension() returns ".hidden"
    // boost::filesystem::path(".hidden").extension()             returns ".hidden"
    // std::filesystem::path(".hidden").extension()               returns ""
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
    
    bool exists (pfs::error_code & ec) const pfs_noexcept
    {
        return _d.exists(ec);
    }

    bool remove (pfs::error_code & ec) const pfs_noexcept
    {
        return _d.remove(ec);
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
    
    friend inline basic_path operator / (basic_path const & lhs, basic_path const & rhs)
    {
        basic_path r(lhs);
        r._d /= rhs._d;
        return r;
    }
    
    friend void swap (basic_path & lhs, basic_path & rhs)
    {
        lhs._d.swap(rhs._d);
    }
    
    friend bool operator == (string_type const & lhs, basic_path const & rhs)
    {
        return lhs == rhs._d;
    }

    friend bool operator == (basic_path const & lhs, string_type const & rhs)
    {
        return lhs._d == rhs;
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
    
    /**
     * @fn template <typename CharT, typename Traits>
     *     std::basic_ostream<CharT, Traits> &
     *     operator << (std::basic_ostream<CharT,Traits> & os, const path& p )
     * 
     * @param os Stream to perform output on.
     * @param p Path to insert.
     * @return @a os.
     */
    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &
    operator << (std::basic_ostream<CharT,Traits> & os, basic_path const & p)
    {
        return os << p._d;
    }

    /**
     * @fn template <typename CharT, typename Traits>
           std::basic_istream<CharT, Traits> &
           operator >> (std::basic_istream<CharT, Traits> & is, path & p)

     * @param is Stream to perform input on.
     * @param p Path to extract.
     * @return @a is.
     */
    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &
    operator >> (std::basic_istream<CharT, Traits> & is, basic_path & p)
    {
        return is >> p._d;
    }
};

typedef basic_path<details::path> path;

inline bool exists (path const & p)
{
    pfs::error_code ec;
    bool r = p.exists(ec);
    
    if (ec && ec != pfs::make_error_code(errc::no_such_file_or_directory)) {
        throw filesystem_error(p, ec);
    }
    
    return r;
}

inline bool exists (path const & p, pfs::error_code & ec) pfs_noexcept
{
    bool r = p.exists(ec);
    if (ec && ec == pfs::make_error_code(errc::no_such_file_or_directory))
        ec.clear();
    return r;
}

inline bool remove (path const & p)
{
    pfs::error_code ec;
    bool r = p.remove(ec);
    
    if (ec) {
        throw filesystem_error(p, ec);
    }
    
    return r;
}
    
inline bool remove (path const & p, error_code & ec)
{
    return p.remove(ec);
}

inline path temp_directory_path ()
{
    return path(details::temp_directory_path());
}

inline path temp_directory_path (error_code & ec)
{
    return path(details::temp_directory_path(ec));
}

inline path current_path ()
{
    return path(details::current_path());
}

inline path current_path (error_code & ec)
{
    return path(details::current_path(ec));
}

inline void current_path (path const & p)
{
    details::current_path(p.native_path());
}

inline void current_path (path const & p, error_code & ec)
{
    details::current_path(p.native_path(), ec);
}

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
StringType to_string (filesystem::path const & p)
{
    return StringType(p.native());
}

//template <typename StringType>
//filesystem::path lexical_cast (StringType const & s);

//template <typename PathType, typename StringType>
//inline typename enable_if<is_same<PathType, filesystem::path>::value
//        && is_same<typename PathType::value_type, char>::value, PathType>::type
//lexical_cast (StringType const & s)
//{
//    return filesystem::path(s.to_std_string());
//}
//
//template <typename PathType, typename StringType>
//inline typename enable_if<is_same<PathType, filesystem::path>::value
//        && is_same<typename PathType::value_type, wchar_t>::value, PathType>::type
//lexical_cast (StringType const & s)
//{
//    return filesystem::path(s.to_std_wstring());
//}

} // pfs

#endif /* __PFS_FILESYSTEM_PATH_HPP__ */

