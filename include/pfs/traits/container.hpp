/* 
 * File:   container.hpp
 * Author: wladt
 *
 * Created on May 5, 2017, 8:49 PM
 */

#ifndef __PFS_TRAITS_CONTAINER_HPP__
#define __PFS_TRAITS_CONTAINER_HPP__

/*
 *  1 - array
 *  2 - vector
 *  3 - deque
 *  4 - forward_list
 *  5 - list
 *  6 - set
 *  7 - multiset
 *  8 - map
 *  9 - multimap
 * 10 - unordered_set
 * 11 - unordered_multiset
 * 12 - unordered_map
 * 13 - unordered_multimap
 * 14 - stack
 * 15 - queue
 * 16 - priority_queue
 * 
 * Iterators
 * 1 - OutputIterator
 * 2 - InputIterator
 * 3 - ForwardIterator
 * 4 - BidirectionalIterator
 * 5 - RandomAccessIterator
 *                                                                     u       u
 *                                                                     n       n
 *                                                                     o       o
 *                                                                     r       r
 *                                                                     d       d           p
 *                                                                 u   e   u   e           r
 *                                         f                       n   r   n   r           i
 *                                         o                       o   e   o   e           o
 *                                         r                       r   d   r   d           r
 *                                         w                       d   _   d   _           i
 *                                         a           m       m   e   m   e   m           t
 *                                         r           u       u   r   u   r   u           y
 *                                 v       d           l       l   e   l   e   l           _
 *                             a   e   d   _           t       t   d   t   d   t   s   q   q
 *                             r   c   e   l   l       i       i   _   i   _   i   t   u   u
 *                             r   t   q   i   i   s   s   m   m   s   s   m   m   a   e   e
 *                             a   o   u   s   s   e   e   a   a   e   e   a   a   c   u   u
 *                             y   r   e   t   t   t   t   p   p   t   t   p   p   k   e   e
 * ==========================================================================================|
 *                           | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12| 13| 14| 15| 16|
 * =====================================MEMBER TYPES=========================================|
 * container_type            |   |   |   |   |   |   |   |   |   |   |   |   |   | + | + | + |
 * key_type                  |   |   |   |   |   | + | + | + | + | + | + | + | + |   |   |   |
 * value_type                | + | + | + | + | + | + | + | + | + | + | + | + | + | + | + | + |
 * mapped_type               |   |   |   |   |   |   |   | + | + |   |   | + | + |   |   |   |
 * allocator_type            |   | + | + | + | + | + | + | + | + | + | + | + | + |   |   |   |
 * size_type                 | + | + | + | + | + | + | + | + | + | + | + | + | + | + | + | + |
 * difference_type           | + | + | + | + | + | + | + | + | + | + | + | + | + |   |   |   |
 * hasher                    |   |   |   |   |   |   |   |   |   | + | + | + | + |   |   |   |
 * key_equal                 |   |   |   |   |   |   |   |   |   | + | + | + | + |   |   |   |
 * key_compare               |   |   |   |   |   | + | + | + | + |   |   |   |   |   |   |   |
 * value_compare             |   |   |   |   |   | + | + |   |   |   |   |   |   |   |   | + |
 * reference                 | + | + | + | + | + | + | + | + | + | + | + | + | + | + | + | + |
 * const_reference           | + | + | + | + | + | + | + | + | + | + | + | + | + | + | + | + |
 * pointer                   | + | + | + | + | + | + | + | + | + | + | + | + | + |   |   |   |
 * const_pointer             | + | + | + | + | + | + | + | + | + | + | + | + | + |   |   |   |
 * iterator                  | 5 | 5 | 5 | 3 | + | 4 | 4 | 4 | 4 | 3 | 3 | 3 | 3 |   |   |   |
 * const_iterator            | 5 | 5 | 5 | 3 | + | 4 | 4 | 4 | 4 | 3 | 3 | 3 | 3 |   |   |   |
 * reverse_iterator          | 5 | 5 | 5 |   | + | 4 | 4 | 4 | 4 |   |   |   |   |   |   |   |
 * const_reverse_iterator    | 5 | 5 | 5 |   | + | 4 | 4 | 4 | 4 |   |   |   |   |   |   |   |
 * local_iterator            |   |   |   |   |   |   |   |   |   | + | + | + | + |   |   |   |
 * const_local_iterator      |   |   |   |   |   |   |   |   |   | + | + | + | + |   |   |   |
 * node_type (C++17)         |   |   |   |   |   | + | + | + | + | + | + | + | + |   |   |   |
 * insert_return_type (C++17)|   |   |   |   |   | + |   | + |   | + |   | + |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * ===================================== ELEMENT ACCESS======================================|
 * operator =                |   |   | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * assign                    |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * get_allocator             |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * at                        | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * operator[]                | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * front                     | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * back                      | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * data                      | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * =========================================ITERATORS========================================|
 * begin                     | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * cbegin                    | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * end                       | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * cend                      | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * rbegin                    | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * crbegin                   | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * rend                      | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * crend                     | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * =========================================CAPACITY=========================================|
 * empty                     | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * size                      | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * max_size                  | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * reserve                   |   | + |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * capacity                  |   | + |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * shrink_to_fit             |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * ========================================MODIFIERS=========================================|
 * clear                     |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * insert                    |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * emplace                   |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * erase                     |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * push_back                 |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * emplace_back              |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * pop_back                  |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   | 
 * push_front                |   |   | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * emplace front             |   |   | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * pop_front                 |   |   | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * resize                    |   | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * swap                      | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * ========================================OPERATIONS========================================|
 * fill                      | + |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * ===================================NON-MEMBER FUNCTIONS===================================|
 * operator ==               | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * operator !=               | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * operator <                | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * operator <=               | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * operator >                | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * operator >=               | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * get                       | + |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * swap                      | + | + | + |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *                           |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 */

namespace pfs {
namespace traits {

template <typename RepContainerTraits>
class container
{
public:
    typedef RepContainerTraits                    traits_type;
    typedef typename traits_type::rep_type        rep_type;
    typedef typename traits_type::size_type       size_type;
    typedef typename traits_type::reference       reference;
    typedef typename traits_type::const_reference const_reference;
};

}}

#endif /* __PFS_TRAITS_CONTAINER_HPP__ */

