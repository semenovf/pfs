/* 
 * File:   filesystem.hpp
 * Author: wladt
 *
 * Created on June 20, 2017, 11:07 AM
 */

#ifndef __PFS_CXX_CXX11_FILESYSTEM_HPP__
#define __PFS_CXX_CXX11_FILESYSTEM_HPP__

#include <experimental/filesystem>

namespace pfs {
namespace filesystem {

class path : public std::experimental::filesystem::path
{
    typedef std::experimental::filesystem::path base_class;
    
public:    
    typedef base_class::value_type  value_type;
    typedef base_class::string_type string_type;

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
//    path (string_type && source, format fmt = auto_format)
//        : base_class(source)
//    {}

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
//    path & operator = (string_type && source)
//    {}
    
    template <typename Source>
    path & operator = (Source const & source)
    {
        base_class::operator = (source);
        return *this;
    }

    // TODO Implement
//    path & assign (string_type && source)
//    {}

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

};

/**
 * @function path operator / (path const & lhs, path const & rhs)
 * 
 * @param lhs path to concatenate.
 * @param rhs path to concatenate.
 * @return The result of path concatenation.
 */


}} // pfs

#endif /* __PFS_CXX_CXX11_FILESYSTEM_HPP__ */

