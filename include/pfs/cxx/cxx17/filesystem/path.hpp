#pragma once
#include <pfs/config.h>

#if HAVE_STDCXX_FILESYSTEM

#include <filesystem>
#include "../system_error.hpp"

namespace pfs {
namespace filesystem {
namespace details {

class path : public std::filesystem::path
{
    typedef std::filesystem::path base_class;

public:
    typedef base_class native_path_type;
    
public:
    path ()
        : base_class()
    {}

    path (path const & other)
        : base_class(other)
    {}

    path const & operator = (path const & other)
    {
        base_class::operator = (other);
        return *this;
    }

    template <typename Source>
    path (Source const & source)
        : base_class(source)
    {}

    template <typename InputIt>
    path (InputIt first, InputIt last)
        : base_class(first, last)
    {}

    template <typename Source>
    path (Source const & source, std::locale const & loc)
        : base_class(source, loc)
    {}

    // FIXME fmt unused yet
    template <typename InputIt>
    path (InputIt first, InputIt last, std::locale const & loc)
        : base_class(first, last, loc)
    {}

    template <typename Source>
    path & operator = (Source const & source)
    {
        base_class::operator = (source);
        return *this;
    }

    // TODO Implement file_status
    //bool exists (filesystem::file_status s) noexcept;

    bool exists (std::error_code & ec) const noexcept
    {
        return std::filesystem::exists(*this, ec);
    }

    bool remove (std::error_code & ec) const noexcept
    {
        return std::filesystem::remove(*this, ec);
    }
};

inline path temp_directory_path ()
{
    return std::filesystem::temp_directory_path();
}

inline path temp_directory_path (error_code & ec)
{
    return std::filesystem::temp_directory_path(ec);
}

inline path current_path ()
{
    return std::filesystem::current_path();
}

inline path current_path (error_code & ec)
{
    return std::filesystem::current_path(ec);
}

inline void current_path (path const & p)
{
    std::filesystem::current_path(p);
}

inline void current_path (path const & p, error_code & ec)
{
    std::filesystem::current_path(p, ec);
}

#if __COMMENT__
class path : public std::filesystem::path
{
    typedef std::filesystem::path base_class;
    
public:    
    typedef base_class::value_type  value_type;
    typedef base_class::string_type string_type;
    typedef base_class::iterator    iterator;

    enum format {
          native_format   ///< Native pathname format
        , generic_format  ///< Generic pathname format
        , auto_format     ///< Implementation-defined pathname format, auto-detected where possible
    };

public:
    path() noexcept : base_class()
    {}

    path (path const & p) : base_class(p) 
    {}

    path (path && p) noexcept : base_class(p)
    {}

    // TODO implement
    // path (string_type && source, format fmt = auto_format)
    //      : base_class(source)
    // {}

    // FIXME fmt unused yet
    template <typename Source>
    path (Source const & source, format fmt = auto_format)
        : base_class(source)
    {}

    // FIXME fmt unused yet
    template <typename InputIt>
    path (InputIt first, InputIt last, format fmt = auto_format)
        : base_class(first, last)
    {}

    // FIXME fmt unused yet
    template <typename Source>
    path (Source const & source, std::locale const & loc, format fmt = auto_format)
        : base_class(source, loc)
    {}

    // FIXME fmt unused yet
    template <typename InputIt>
    path (InputIt first, InputIt last, std::locale const & loc, format fmt = auto_format)
        : base_class(first, last, loc)
    {}
    
    ~path () = default;
    
    path & operator = (path const & p)
    {
        base_class::operator = (p);
        return *this;
    }
    
    path & operator = (path && p) noexcept
    {
        base_class::operator = (p);
        return *this;
    }
    
    // TODO implement
    // path & operator = (string_type && source)
    // {}
    

    // TODO Implement
    // path & assign (string_type && source)
    // {}

    /**
     * @fn template <typename Source>
     *           path & assign (Source const & source);
     */

    /**
     * @fn template <typename InputIt>
     *           path & assign (InputIt first, InputIt last)
     */
    
    //==========================================================================
    // Concatenation
    //==========================================================================
    /**
     * @fn path & operator /= (path const & p);
     */

    /**
     * @fn template <typename Source>
     *           path & operator /= (Source const & source);
     */
    
    /**
     * @fn template <typename Source>
     *           path & append (Source const & source);
     */
    
    /**
     * @fn template <typename InputIt>
     *           path & path::append (InputIt first, InputIt last);
     */

    /**
     * @fn path & path::operator += (path const & p)
     * 
     * @param p Path to append.
     * @return 
     */

    /**
     * @fn path & path::operator += (string_type const & str);
     * 
     * @param str String to append.
     * @return @c *this
     */
    

    // TODO Implement
    //path & operator+= (std::basic_string_view<value_type> str);

    /**
     * @fn path & path::operator += (value_type const * ptr)
     * 
     * @param ptr Pointer to the beginning of a null-terminated string to append.
     * @return @c *this
     */
    
    /**
     * @fn path & path::operator += (value_type x)
     * 
     * @param x Single character to append.
     * @return @c *this
     */
    
    /**
     * @fn template <typename Source>
     *     path & path::operator += (Source const & source)
     * 
     * @param source @c path::string_type, null-terminated multi-character string,
     *        or an input iterator pointing to a null-terminated 
     *        multi-character sequence, which represents a path name 
     *        (either in portable or in native format).
     * @return @c *this
     */
    
    /**
     * @fn template <typename CharT>
     *     path & path::operator += (CharT x)
     * 
     * @param x Single character to append.
     * @return @c *this
     */
    
    /**
     * @fn template <typename Source>
     *     path & path::concat (Source const & source)
     * 
     * @param source @c path::string_type, null-terminated multi-character string,
     *        or an input iterator pointing to a null-terminated 
     *        multi-character sequence, which represents a path name 
     *        (either in portable or in native format).
     * @return @c *this
     */
    
    /**
     * @fn template <typename InputIt>
     *     path & path::concat (InputIterator first, InputIterator last)
     * 
     * @param first Begin iterator of multi-character sequence that represents a path name.
     * @param last End iterator of multi-character sequence that represents a path name.
     * @return @c *this
     */
    
    //==========================================================================
    // Modifiers
    //==========================================================================
    
    /**
     * @fn void path::clear()
     * 
     * @brief Clears the stored pathname. empty() is @c true after the call. 
     */
    
    /**
     * @fn path & path::make_preferred ()
     * 
     * @brief Converts all directory separators in the generic-format view of 
     *        the path to the preferred directory separator. 
     * 
     * @return @c *this
     */
   
    /**
     * @fn path & path::remove_filename ()
     * 
     * @brief Removes a single generic-format filename component 
     *        (as returned by filename) from the given generic-format path.
     * 
     * @return @c *this
     */
    
    /**
     * @fn path & path::replace_filename (path const & replacement)
     * 
     * @brief Replaces a single filename component with replacement.
     * 
     * @return @c *this
     */
    
    /** 
     * @fn path & replace_extension (path const & replacement = path())
     * 
     * @brief Replaces the extension with replacement or removes it when the 
     *        default value of replacement is used.
     * 
     * @return @c *this
     */
    
    /**
     * @fn void path::swap (path & other) noexcept
     * 
     * @brief Swaps the contents (both native and generic format) of @c *this 
     *        and other. 
     */
    
    //==========================================================================
    // Format observers
    //==========================================================================
    
    /**
     * @fn value_type const * path::c_str () const noexcept
     * 
     * @brief Accesses the native path name as a character string.
     *        Equivalent to native().c_str().
     */

    /**
     * @fn string_type const & path::native () const noexcept
     * 
     * @brief Accesses the native path name as a character string.
     */
    

    /**
     * @fn path::operator string_type () const
     * 
     * @brief Accesses the native path name as a character string.
     */

    /**
     * @fn template <typename CharT
     *             , typename Traits = std::char_traits<CharT>
     *             , typename Alloc = std::allocator<CharT> >
     *     std::basic_string<CharT, Traits, Alloc>
     *     path::string (Alloc const & a = Allocator()) const
     * 
     * @return The internal pathname in native pathname format, 
     *         converted to specified string type.
     */

    /**
     * @fn std::string path::string () const
     * @return 
     */

    /**
     * @fn std::wstring path::wstring () const
     * @return 
     */

    /**
     * @fn std::string path::u8string () const
     * @return 
     */

    /**
     * @fn std::u16string path::u16string () const
     * @return 
     */

    /**
     * @fn std::u32string path::u32string () const
     * @return 
     */
    
    /**
     * @fn template <typename CharT, typename Traits = std::char_traits<CharT>
     *          , typename Alloc = std::allocator<CharT> >
     *     std::basic_string<CharT, Traits, Alloc>
     *     path::generic_string (Allocator const & a = Allocator()) const
     * 
     * @return The internal pathname in generic pathname format,
     *         converted to specified string type.
     */
    
    /**
     * @fn std::string path::generic_string () const
     * @return 
     */

    /**
     * @fn std::wstring path::generic_wstring () const
     * @return 
     */

    /**
     * @fn std::string path::generic_u8string () const
     * @return 
     */

    /**
     * @fn std::u16string path::generic_u16string () const
     * @return 
     */

    /**
     * @fn std::u32string path::generic_u32string () const
     * @return 
     */
    
    //==========================================================================
    // Compare
    //==========================================================================
    
    /**
     * @fn int path::compare (path const & p) const noexcept
     * 
     * @brief Compares the lexical representations of the path and another path.
     * 
     * @param p A path to compare to.
     * @return A value less than 0 if the path is lexicographically less than the given path.
     *         A value equal to 0 if the path is lexicographically equal to the given path. 
     *         A value greater than 0 if the path is lexicographically greater than the given path. 
     */

    /**
     * @fn int path::compare (string_type const & str) const
     * 
     * @brief Compares the lexical representations of the path and another path.
     * 
     * @param str A string or string view representing path to compare to.
     * @return A value less than 0 if the path is lexicographically less than the given path.
     *         A value equal to 0 if the path is lexicographically equal to the given path. 
     *         A value greater than 0 if the path is lexicographically greater than the given path. 
     */
    
    // TODO Implement
    //int compare (std::basic_string_view<value_type> str) const;

    /**
     * @fn int path::compare (value_type const * s) const
     * 
     * @brief Compares the lexical representations of the path and another path.
     * 
     * @param s A null-terminated string representing path to compare to.
     * @return A value less than 0 if the path is lexicographically less than the given path.
     *         A value equal to 0 if the path is lexicographically equal to the given path. 
     *         A value greater than 0 if the path is lexicographically greater than the given path. 
     */

    //==========================================================================
    // Generation
    //==========================================================================
    
    // TODO Implement
    //path lexically_normal () const;
    
    // TODO Implement
    //path lexically_relative (path const & base) const;
    
    // TODO Implement
    //path lexically_proximate (path const & base) const;
    
    //==========================================================================
    // Decomposition
    //==========================================================================

    /**
     * @fn path path::root_name () const
     * @return The root name of the path.
     */
    
    /**
     * @fn path path::root_directory () const
     * @return The root directory of the path.
     */
    
    /**
     * @fn path path::root_path () const
     * @return The root path of the path.
     */
    
    /**
     * @fn path relative_path () const
     * @return Path relative to the root path.
     */
    
    /**
     * @fn path path::parent_path () const
     * @return The path to the parent directory.
     */
    
    /**
     * @fn path path::filename () const
     * @return The filename identified by the path.
     */
    
    // FIXME (For description see test_path.hpp)
    /**
     * @fn path path::stem () const
     * @return The stem of the filename identified by the path
     *         (i.e. the filename without the final extension).
     */
    
    // FIXME (For description see test_path.hpp)
    /**
     * @fn path path::extension () const
     * @return The extension of the current pathname or an empty path 
     *         if there's no extension.
     */

    //==========================================================================
    // Queries
    //==========================================================================
    
    /**
     * @fn bool path::empty () const noexcept
     * @return @c true if the path is empty, @c false otherwise.
     */
    
    /**
     * @fn bool path::has_root_path () const
     * @return Checks whether root_path() is empty.
     */

    /**
     * @fn bool path::has_root_name () const
     * @return Checks whether root_name() is empty.
     */

    /**
     * @fn bool path::has_root_directory () const
     * @return Checks whether root_directory() is empty.
     */

    /**
     * @fn bool path::has_relative_path () const
     * @return Checks whether relative_path() is empty.
     */
    
    /**
     * @fn bool path::has_parent_path () const
     * @return Checks whether parent_path() is empty.
     */

    /**
     * @fn bool path::has_filename () const
     * @return Checks whether filename() is empty.
     */

    // FIXME (For description see test_path.hpp)
    /**
     * @fn bool path::has_stem () const
     * @return Checks whether stem() is empty. 
     */

    // FIXME (For description see test_path.hpp)
    /**
     * @fn bool path::has_extension () const
     * @return Checks whether extension() is empty.
     */
    
    /**
     * @fn bool path::is_absolute () const
     * @return @c true if the path is absolute, @c false otherwise.
     * 
     * @note The path "/" is absolute on a POSIX OS, but is relative on Windows.
     */

    /**
     * @fn bool path::is_relative () const
     * @return @c false if the path is absolute, @c true otherwise.
     * 
     * @note The path "/" is absolute on a POSIX OS, but is relative on Windows.
     */
    
    //==========================================================================
    // Iterators
    //==========================================================================

    /**
     * @fn iterator path::begin () const
     * @return Iterator to the first element of the path.
     */

    /**
     * @fn iterator path::end () const
     * @return Iterator one past the end of the path.
     */
};

/**
 * @fn path operator / (path const & lhs, path const & rhs)
 * 
 * @param lhs path to concatenate.
 * @param rhs path to concatenate.
 * @return The result of path concatenation.
 */

/**
 * @param lhs Path whose state to swap 
 * @param rhs Path whose state to swap 
 */
inline void swap (path & lhs, path & rhs) noexcept
{
    std::filesystem::swap(lhs, rhs);
}

/**
 * @fn bool operator == (path const & lhs, path const & rhs ) noexcept
 * 
 * @brief Checks whether lhs and rhs are equal.
 * 
 * @param lhs Paths to compare.
 * @param rhs Paths to compare.
 * @return @c true if the corresponding comparison yields, @c false otherwise.
 */

/**
 * @fn bool operator != (path const & lhs, path const & rhs ) noexcept
 * 
 * @brief Checks whether lhs and rhs are not equal.
 * 
 * @param lhs Paths to compare.
 * @param rhs Paths to compare.
 * @return @c true if the corresponding comparison yields, @c false otherwise.
 */

/**
 * @fn bool operator <  (path const & lhs, path const & rhs ) noexcept
 * 
 * @brief Checks whether lhs is less than rhs.
 * 
 * @param lhs Paths to compare.
 * @param rhs Paths to compare.
 * @return @c true if the corresponding comparison yields, @c false otherwise.
 */

/**
 * @fn bool operator <= (path const & lhs, path const & rhs ) noexcept
 * 
 * @brief Checks whether lhs is less than or equal to rhs.
 * 
 * @param lhs Paths to compare.
 * @param rhs Paths to compare.
 * @return @c true if the corresponding comparison yields, @c false otherwise.
 */

/**
 * @fn bool operator >  (path const & lhs, path const & rhs ) noexcept
 * 
 * @brief Checks whether lhs is greater than rhs.
 * 
 * @param lhs Paths to compare.
 * @param rhs Paths to compare.
 * @return @c true if the corresponding comparison yields, @c false otherwise.
 */

/**
 * @fn bool operator >= (path const & lhs, path const & rhs ) noexcept
 * 
 * @brief Checks whether lhs is greater than rhs.
 * 
 * @param lhs Paths to compare.
 * @param rhs Paths to compare.
 * @return @c true if the corresponding comparison yields, @c false otherwise.
 */



/**
 * @fn template <typename Source>
 *     path u8path (Source const & source)
 * 
 * @brief Constructs a path p from a UTF-8 encoded sequence.
 * 
 * @param source A UTF-8 encoded string, string_view, 
 *        a pointer to a null-terminated multi-byte string, 
 *        or an input iterator with char value type that points 
 *        to a null-terminated multi-byte string.
 * @return The path constructed from the input string after conversion 
 *         from UTF-8 to the file system's native character encoding.
 */

template <typename Source>
inline path u8path (Source const & source)
{
    return std::filesystem::u8path(source);
}

/**
 * @fn template <typename InputIt>
 *     path u8path (InputIt first, InputIt last)
 * 
 * @brief Constructs a path p from a UTF-8 encoded sequence.
 * 
 * @param first InputIterator that specifies the begin of UTF-8 encoded character sequence.
 * @param last InputIterator that specifies the end of UTF-8 encoded character sequence.
 * @return The path constructed from the input string after conversion 
 *         from UTF-8 to the file system's native character encoding.
 */
template <typename InputIt>
inline path u8path (InputIt first, InputIt last)
{
    return std::filesystem::u8path(first, last);
}

#endif

}}} // pfs::filesystem::details

#else
#   include <pfs/cxx/cxx14/filesystem/path.hpp>
#endif
