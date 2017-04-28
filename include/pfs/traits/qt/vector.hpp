/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on February 26, 2017, 10:58 AM
 */

#ifndef __PFS_TRAITS_QT_VECTOR_HPP__
#define __PFS_TRAITS_QT_VECTOR_HPP__

#include <QVector>
#include <pfs/algo/compare.hpp>
#include <pfs/traits/vector.hpp>

namespace pfs {
namespace traits {

template <typename T>
struct vector_rep<T, QVector> : public QVector<T>
{
    typedef QVector<T>                                     base_class;
    typedef QVector<T>                                     native_type;
    typedef native_type const &                            const_native_reference;
    typedef typename native_type::size_type                size_type;
    typedef typename native_type::value_type               value_type;
    typedef typename native_type::reference                reference;
    typedef typename native_type::const_reference          const_reference;
    typedef typename native_type::pointer                  pointer;
    typedef typename native_type::const_pointer            const_pointer;
    typedef typename native_type::iterator                 iterator;
    typedef typename native_type::const_iterator           const_iterator;
    typedef typename std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;

    size_type max_size ()
    {
        return (INT_MAX)/sizeof(T) - sizeof(native_type);
    }
    
    reverse_iterator rbegin ()
    {
        return reverse_iterator(this->end());
    }

    inline const_reverse_iterator rbegin () const
    {
        return const_reverse_iterator(this->end());
    }

    inline reverse_iterator rend ()
    {
        return reverse_iterator(this->begin());
    }

    const_reverse_iterator rend () const
    {
        return const_reverse_iterator(this->begin());
    }

//    reference at (size_type pos)
//    {
//        return d[pos];
//    }
//    
//    int compare (vector_rep const & rhs)
//    {
//        return pfs::compare(this->begin(), this->end(), rhs.begin(), rhs.end());
//    }
};

}} // pfs::traits

#endif /* __PFS_TRAITS_QT_VECTOR_HPP__ */
