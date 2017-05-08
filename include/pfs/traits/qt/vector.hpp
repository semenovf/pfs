/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on February 26, 2017, 10:58 AM
 */

#ifndef __PFS_TRAITS_QT_VECTOR_HPP__
#define __PFS_TRAITS_QT_VECTOR_HPP__

#include <QVector>
#include <pfs/traits/qt/container.hpp>

namespace pfs {
namespace traits {
namespace qt {

template <typename T>
struct vector_wrapper
{
    typedef QVector<T> type;
};

template <typename T>
class vector : public container<T, vector_wrapper>
{
    typedef container<T, vector_wrapper> base_class;
    
public:
    typedef typename base_class::native_type      native_type;
    typedef typename base_class::native_reference native_reference;
    typedef typename base_class::size_type        size_type;
    typedef typename native_type::value_type      value_type;
    typedef typename native_type::reference       reference;
    typedef typename native_type::const_reference const_reference;

public:
    vector (native_reference rhs)
        : base_class(rhs)
    {}

    size_type max_size () const pfs_noexcept
    {
        return (INT_MAX)/sizeof(T) - sizeof(native_type);
    }
};

}}} // pfs::traits::qt

#if __OBSOLETE__

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

#endif

#endif /* __PFS_TRAITS_QT_VECTOR_HPP__ */
