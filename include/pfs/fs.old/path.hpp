#ifndef __PS_FS_PATH_HPP__
#define __PS_FS_PATH_HPP__

#include <pfs/iterator.hpp>
#include <pfs/exception.hpp>
#include <pfs/algo/split.hpp>
#include <pfs/fs/traits.hpp>
#include <pfs/fs/file_status.hpp>

namespace pfs {
namespace fs {

template <typename Traits>
class path
{
    typedef Traits traits_type;
    
public:
	typedef typename traits_type::string_type               string_type;
	typedef typename traits_type::stringlist_type           stringlist_type;
    typedef typename traits_type::string_builder_type       string_builder_type;
    typedef typename traits_type::template list<path>::type pathlist_type;

private:
	string_type _path;

public:
    static string_type separator ()
    {
        return traits_type::separator();
    }

public:
	class iterator;
    typedef iterator const_iterator;
    
    const_iterator begin () const;
    const_iterator end () const;
    
    const_iterator cbegin () const
    {
        return begin();
    }
    
    const_iterator cend () const
    {
        return begin();
    }
    
	class range
	{
		friend class path;

		stringlist_type * _pcomponents;

		range (string_type const & s)
			: _pcomponents(new stringlist_type)
		{
            split(s.cbegin()
                , s.cend()
                , separator().cbegin()
                , separator().cend()
                , keep_empty, _pcomponents);
		}

	public:
        ~range ()
        {
            delete _pcomponents;
        }
        
		iterator begin ()
		{
			return iterator_facade(_pcomponents->begin());
		}

		iterator end ()
		{
			return iterator_facade(_pcomponents->end());
		}
        
//        /**
//         * @return First component of the path or empty string if path is empty.
//         */
//        string_type first () const
//        {
//            return _pcomponents->size() > 0 ? _pcomponents->at(0) : string_type();
//        }
//
//        /**
//         * @return Last component of the path empty string if path is empty.
//         */
//        string_type last () const
//        {
//            return _pcomponents->size() > 0 ? _pcomponents->at(_pcomponents->size() - 1) : string_type();
//        }
	};

public:

	/**
	 * @brief Construct path object with empty path and platform-specific separator
	 *
	 * @note Platform specific.
	 */
	path ()
	{}

	path (path const & other)
		: _path(other._path)
	{}

	explicit path (string_type const & s)
        : _path(s)
    {}

	explicit path (char const * s)
        : _path(s)
    {}

	path & operator = (string_type const & s)
    {
        _path = s;
        return *this;
    }
    
	path & operator = (char const * s)
    {
        _path = s;
        return *this;
    }

	/**
	 * @brief Returns string representation of path.
	 *
	 * @return String representation of path.
	 */
	string_type const & str () const
	{
		return _path;
	}

	bool empty () const
	{
		return _path.empty();
	}

	bool is_absolute () const
    {
        return _path.empty()
                ? false
                : _path.starts_with(separator());
    }

	bool is_relative () const
	{
		return !is_absolute();
	}

	/**
	 * @brief Returns the canonical representation of this path.
	 */
	path canonical () const;

	range get_range () const
	{
		return range(_path, separator());
	}

public: // static
    
    /**
     * @brief Determines file type by path.
     * @param p Path.
     * @param nx Pointer to save error message(s).
     * @return File type (@see @a file_type_enum). Check @c errno if 
     *         return value is @c status_error.
     */
    static file_status status (path const & p);

    /**
     * @brief Checks if @ path exists.
     * @param p Path.
     * @param nx Pointer to save error message(s).
     * @return @c true if @c path exists, @c false otherwise.
     *
     * @note Path exists if it's status is not equals to @c status_error nor @c file_not_found.
     */
    static bool exists (path const & p)
    {
        file_status st = file_status(p);
        return st.type() != status_error && st.type() != file_not_found;
    }

    /**
     * @brief Joins two path instances.
     *
     * @return Resulting path after join.
     */
    static path join (path const & p1, path const & p2)
    {
        string_builder_type sb;
        sb.push_back(p1.str());
        sb.push_back(separator());
        sb.push_back(p2.str());
        return path(sb.template str<string_type>());
    }

    /**
     * @brief Joins path list components.
     *
     * @return Resulting path after join.
     */
    static path join (pathlist_type const & pl);

    /**
     * @brief Joins path list components and path instance.
     *
     * @return Resulting path after join.
     */
    static path join (pathlist_type const & pl, path const & p)
    {
    	path r = join(pl, separator());
    	return join(r, p, separator());
    }
    
	friend bool operator == (path const & lhs, path const & rhs)
	{
		return lhs.canonical()._path == rhs.canonical()._path;
	}

	friend bool operator != (path const & lhs, path const & rhs)
	{
		return ! operator == (lhs, rhs);
	}
};

template <typename Traits>
path<Traits> path<Traits>::canonical () const
{
    stringlist_type parts;
	range rn(_path);
	iterator it = rn.begin();
	iterator it_end = rn.end();

	while (it != it_end) {
		if (*it == string_type(".")) {
			; // ignore
		} else if (*it == string_type("..")) {
			if (parts.size() > 0)
				parts.pop_front();
		} else {
			parts.push_front(*it);
		}

		++it;
	}

	pathlist_type pl;

	while (parts.size() > 0) {
		pl.push_back(path(parts.back()));
		parts.pop_back();
	}

	if (this->is_absolute())
		return join(path(), join(pl));

	return join(pl);
}

template <typename Traits>
path<Traits> path<Traits>::join (pathlist_type const & pl)
{
	string_builder_type r;

	typename pathlist_type::const_iterator it = pl.cbegin();
	typename pathlist_type::const_iterator it_end = pl.cend();

	if (it != it_end) {
		r.push_back(it->str());
		++it;
	}

	while (it != it_end) {
		r.push_back(separator());
		r.push_back(it->str());
		++it;
	}

	return path(r.template str<string_type>());
}


/**
 * @brief Strip directory and suffix from path.
 * @param p Path to strip.
 * @return Path component with any leading directory components and suffixes removed.
 * 
 * The base name consists of all characters in the file up to (but not including) the first '.' character.
 */
//string basename (path const & p);

/**
 * @brief Strip directory from path.
 * @param p Path to strip.
 * @return Path component with any leading directory components removed.
 */
//inline string filename (path const & p)
//{
//    return p.get_range().last();
//}

/**
 * @brief Checks if @ path is regular file.
 *
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return @c true if @c path is a regular file, @c false otherwise.
 */
//inline bool is_regular_file (const path & p, error_code * ex = 0)
//{
//	return get_file_status(p, ex).type() == regular_file;
//}

//inline bool is_directory (const path & p, error_code * ex = 0)
//{
//	return get_file_status(p, ex).type() == directory_file;
//}
//
//inline bool is_symlink (const path & p, error_code * ex = 0)
//{
//	return get_file_status(p, ex).type() == symlink_file;
//}
//
//uintmax_t file_size (const path & p, error_code * ex = 0);
//
//bool remove (const path & p, error_code * ex = 0);
//
//inline bool unlink (const path & p, error_code * ex = 0)
//{
//	return remove(p, ex);
//}
//
//bool rename (const path & from, const path & to, error_code * ex = 0);
//
//path temp_directory_path (error_code * ex = 0);
//
//// FIXME Need to implement
//path unique_path (const path & model, error_code * ex = 0);
//
//inline path unique_path ()
//{
//	return unique_path(path(path::string_type("%%%%-%%%%")), 0);
//}
//
//path current_directory (error_code * ex = 0);
//
//inline bool starts_with (const path & haystack, const path & needle)
//{
//	return pfs::fs::starts_with(haystack, needle);
//}

/**
 * @brief Searches file in current directory and directories specified
 *        by @a searchdirs and returns appropriate existence path.
 *
 * @param file File name to search. May be absolute or relative.
 * @param searchdirs List of directories where to to search file.
 *
 * @return Path to file if found or empty path if @a filename is empty
 *         or file not found in list of directories specified by @a searchdirs.
 */
//path search_file (const path & file, const pathlist & searchdirs, error_code * ex = 0);

//inline string to_string (const fs::path & p)
//{
//	return p.native();
//}

}} // pfs::fs

#include <pfs/fs/iterator.hpp>

#endif /* __PFS_FS_PATH_HPP__ */

