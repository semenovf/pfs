/* 
 * File:   iterator.hpp
 * Author: wladt
 *
 * Created on December 30, 2016, 11:04 AM
 */

#ifndef __PFS_CXX11_ITERATOR_HPP__
#define __PFS_CXX11_ITERATOR_HPP__

#include <pfs/cxx/cxx98/iterator.hpp>

namespace pfs {

template <typename Iterator>
using reverse_iterator = std::reverse_iterator<Iterator>;

template <typename Container>
using back_insert_iterator = std::back_insert_iterator<Container>;

template <typename Container>
inline back_insert_iterator<Container> 
back_inserter (Container & c)
{
    return std::back_insert_iterator<Container>(c);
}

} // pfs

#endif /* __PFS_CXX11_ITERATOR_HPP__ */

