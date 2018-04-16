#ifndef __PFS_FS_ITERATOR_HPP__
#define __PFS_FS_ITERATOR_HPP__

#include <pfs/iterator.hpp>

namespace pfs {
namespace fs {

template <typename Traits>
class path<Traits>::iterator : public iterator_facade<
          bidirectional_iterator_tag // category
        , path>                    // value type
{
    friend class path;

    path                            _p;
    path const *                    _owner;
    typename string_type::size_type _pos;

//    iterator (const pointer & ptr)
//    : _p (ptr)
//    {
//    }
private:
    static void increment (iterator_facade & it);
    
public:

//    iterator (const iterator & other)
//    : _p (other._p)
//    {
//    }
//
//    iterator & operator= (const iterator & other)
//    {
//        _p = other._p;
//        return *this;
//    }
//
//    iterator & operator++ ()
//    {
//        ++_p;
//        return *this;
//    }
//
//    //
//    // input iterator traits
//    //
//
//    iterator operator++ (int)
//    {
//        return iterator(_p++);
//    }
//
//    reference operator* ()
//    {
//        return *_p;
//    }
//
//    //
//    // bidirectional iterator traits
//    //
//
//    iterator & operator-- ()
//    {
//        --_p;
//        return *this;
//    }
//
//    iterator operator-- (int)
//    {
//        return iterator(_p--);
//    }
//
//    pointer base () const
//    {
//        return _p;
//    }
//
    friend bool operator == (iterator_facade const & lhs, iterator_facade const & rhs)
    {
        return lhs._owner == rhs._owner 
                && lhs._pos == rhs._pos;
    }

    friend bool operator != (iterator_facade const & lhs, iterator_facade const & rhs)
    {
        return ! (lhs == rhs);
    }
};

template <typename Traits>
void path<Traits>::iterator::increment (iterator_facade & it)
{
    PFS_ASSERT(it._pos < it._owner->m_pathname.size(),
      "path::basic_iterator increment past end()");

    // increment to position past current element; if current element is implicit dot,
    // this will cause it.m_pos to represent the end iterator
    it.m_pos += it.m_element.m_pathname.size();

    // if the end is reached, we are done
    if (it.m_pos == it.m_path_ptr->m_pathname.size())
    {
      it.m_element.clear();  // aids debugging, may release unneeded memory
      return;
    }

    // both POSIX and Windows treat paths that begin with exactly two separators specially
    bool was_net(it.m_element.m_pathname.size() > 2
      && is_separator(it.m_element.m_pathname[0])
      && is_separator(it.m_element.m_pathname[1])
      && !is_separator(it.m_element.m_pathname[2]));

    // process separator (Windows drive spec is only case not a separator)
    if (is_separator(it.m_path_ptr->m_pathname[it.m_pos]))
    {
      // detect root directory
      if (was_net
#       ifdef BOOST_WINDOWS_API
        // case "c:/"
        || it.m_element.m_pathname[it.m_element.m_pathname.size()-1] == colon
#       endif
         )
      {
        it.m_element.m_pathname = separator;  // generic format; see docs
        return;
      }

      // skip separators until it.m_pos points to the start of the next element
      while (it.m_pos != it.m_path_ptr->m_pathname.size()
        && is_separator(it.m_path_ptr->m_pathname[it.m_pos]))
        { ++it.m_pos; }

      // detect trailing separator, and treat it as ".", per POSIX spec
      if (it.m_pos == it.m_path_ptr->m_pathname.size()
        && !is_root_separator(it.m_path_ptr->m_pathname, it.m_pos-1)) 
      {
        --it.m_pos;
        it.m_element = detail::dot_path();
        return;
      }
    }

    // get m_element
    size_type end_pos(it.m_path_ptr->m_pathname.find_first_of(separators, it.m_pos));
    if (end_pos == string_type::npos)
      end_pos = it.m_path_ptr->m_pathname.size();
    it.m_element = it.m_path_ptr->m_pathname.substr(it.m_pos, end_pos - it.m_pos);    
}


}} // pfs::fs

#endif /* __PFS_FS_ITERATOR_HPP__ */

