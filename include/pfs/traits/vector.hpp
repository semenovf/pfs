/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 11:44 AM
 */

#ifndef __PFS_TRAITS_VECTOR_HPP__
#define __PFS_TRAITS_VECTOR_HPP__

#include <pfs/exception.hpp>
#include <pfs/cxxlang.hpp>

namespace pfs {
namespace traits {

template <typename T, template <typename> class VectorT>
struct vector_traits
{
    typedef struct __Use_Specialized_Traits__ {} native_type;
    typedef native_type const & const_native_reference;
    typedef size_t              size_type;
    typedef T                   value_type;
    typedef T &                 reference;
    typedef T const &           const_reference;
    typedef T *                 pointer;
    typedef T const *           const_pointer;
    typedef T *                 iterator;
    typedef T const *           const_iterator;
    typedef T *                 reverse_iterator;
    typedef T const *           const_reverse_iterator;
    typedef native_type         data_type;

    static void xassign (data_type & d, const_native_reference lhs);
    static void xassign (data_type & d, const_pointer lhs, size_type n);
    static size_type xsize (data_type const & d);
    static size_type xmax_size (data_type const & d);
    static iterator xbegin (data_type & d);
    static const_iterator xbegin (data_type const & d);
    static iterator xend (data_type & d);
    static const_iterator xend (data_type const & d);
    static reverse_iterator xrbegin (data_type & d);
    static const_reverse_iterator xrbegin (data_type const & d);
    static reverse_iterator xrend (data_type & d);
    static const_reverse_iterator xrend (data_type const & d);
    static reference xat (data_type const & d, size_type pos);
    static void xreserve (data_type & d, size_type new_capacity);
    static size_type xcapacity (data_type const & d);
    static iterator xerase (data_type & d, const_iterator first, const_iterator last);
    static void xclear (data_type & d);
    static void xpush_back (data_type & d, const_reference value);
    static void xpop_back (data_type & d);
    static void xresize (data_type & d, size_type count, const_reference value);
    static int xcompare (data_type const & lhs, data_type const & rhs);
    static void xswap (data_type & lhs, data_type & rhs);
    static pointer xdata (data_type & d);
    static const_native_reference xcast (data_type const & d);
};

template <typename T, template <typename> class VectorT>
class vector
{
public:
    typedef vector_traits<T, VectorT>                    traits_type;
    typedef typename traits_type::native_type            native_type;
    typedef typename traits_type::const_native_reference const_native_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::value_type             value_type;
    typedef typename traits_type::reference              reference;
    typedef typename traits_type::const_reference        const_reference;
    typedef typename traits_type::pointer                pointer;
    typedef typename traits_type::const_pointer          const_pointer;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::data_type              data_type;
    
private:
    data_type _d;
    
public:
    explicit vector ()
    {}

    vector (size_type count, T const & value)
        : _d(count, value)
    {}
    
    explicit vector (size_type count)
        : _d(count)
    {}

    template <typename InputIt>
    vector (InputIt first, InputIt last);

    vector (vector const & other)
        : _d(other._d)
    {}

    explicit vector (const_native_reference s)
        : _d(s)
    {}

#if __cplusplus >= 201103L    
    // TODO Implement
    vector (vector && other);
    vector (std::initializer_list<T> init);
    vector & operator = (vector && other);
#endif    

    vector & operator = (vector const & rhs)
    {
        traits_type::xassign(_d, traits_type::xcast(rhs._d));
        return *this;
    }

    vector & operator = (const_native_reference rhs)
    {
        traits_type::xassign(_d, rhs);
        return *this;
    }

    vector & operator = (const_pointer rhs)
    {
        traits_type::xassign(_d, rhs, size_type(-1));
        return *this;
    }

    // TODO Implement
//    void assign( size_type count, const T& value );
//    template< class InputIt >
//    void assign( InputIt first, InputIt last );
//    void assign( std::initializer_list<T> ilist );
    
    reference at (size_type pos)
    {
        if (pos >= this->size())
            throw out_of_range("string::at()");
        return traits_type::xassign(_d, pos);
    }
		
    const_reference at (size_type pos) const
    {
        if (pos >= this->size())
            throw out_of_range("string::at()");
        return traits_type::xassign(_d, pos);
    }
	
    /**
     * @brief Returns a reference to the element at specified location pos. 
     *        No bounds checking is performed. 
     * 
     * @param pos Position of the element to return.
     * @return Reference to the requested element.
     * 
     * @note This operator never inserts a new element into the container. 
     */
    reference operator [] (size_type pos)
    {
        return traits_type::xassign(_d, pos);
    }

    /**
     * @brief Returns a reference to the element at specified location pos. 
     *        No bounds checking is performed. 
     * 
     * @param pos Position of the element to return.
     * @return Constant reference to the requested element.
     * 
     * @note This operator never inserts a new element into the container. 
     */
    const_reference operator [] (size_type pos) const
    {
        return traits_type::xassign(_d, pos);
    }
	
    /**
     * @return Reference to the first element.
     */
    reference front ()
    {
        return *begin();
    }

    /**
     * @return Reference to the first element.
     */
    const_reference front () const
    {
        return *begin();
    }
	
    reference back ()
    {
        iterator it = end();
        --it;
        return *it;
    }
		
    const_reference back () const
    {
        const_iterator it = end();
        --it;
        return *it;
    }
	
    pointer data ()
    {
        return traits_type::xdata(_d);
    }

    const_pointer  data () const
    {
        return traits_type::xdata(_d);   
    }

    iterator begin ()
    {
        return traits_type::xbegin(_d);
    }

    const_iterator begin () const
    {
        return traits_type::xbegin(_d);
    }
		
    const_iterator cbegin () const
    {
        return begin();
    }
    
    iterator end ()
    {
        return traits_type::xend(_d);
    }
		
    const_iterator end () const
    {
        return traits_type::xend(_d);
    }
		
    const_iterator cend () const
    {
        return end();
    }
    
    reverse_iterator rbegin ()
    {
        return traits_type::xrbegin(_d);
    }
		
    const_reverse_iterator rbegin() const
    {
        return traits_type::xrbegin(_d);
    }
		
    const_reverse_iterator crbegin () const
    {
        return rbegin();
    }
    
    reverse_iterator rend ()
    {
        return traits_type::xrend(_d);
    }
		
    const_reverse_iterator rend () const
    {
        return traits_type::xrend(_d);
    }
		
    const_reverse_iterator crend () const
    {
        return rend();
    }
    
    bool empty () const
    {
    	return size() == 0;
    }

    size_type size () const pfs_noexcept
    {
        return traits_type::xsize(_d);
    }
    
	size_type max_size () const
    {
        return traits_type::xmax_size(_d);
    }
    
    void reserve (size_type new_capacity)
    {
        if (new_capacity > max_size())
            throw pfs::length_error("vector::reserve()");
                    
        traits_type::xreserve(_d, new_capacity);
    }
	
    size_type capacity () const pfs_noexcept
    {
        return traits_type::xcapacity(_d);
    }

// TODO Implement	
//    void shrink_to_fit ()
//    {
//        traits_type::xshrink_to_fit(_d);
//    }
    
    void clear ()
    {
        traits_type::xclear(_d);
    }
   
    iterator insert (const_iterator pos, const_reference value)
    {
        return insert(pos, 1, value);
    }
    
    // TODO Implement
    //iterator insert (const_iterator pos, T && value);
    
    iterator insert (const_iterator pos, size_type count, const_reference value);
    
// TODO Implement
//    template <typename InputIt>
//    void insert (const_iterator pos, InputIt first, InputIt last);
//    
//    iterator insert (const_iterator pos, std::initializer_list<T> ilist);
//
//    template< class... Args >
//    iterator emplace( const_iterator pos, Args&&... args );    
    
    
    iterator erase (const_iterator pos)
    {
        const_iterator last(pos);
        ++last;
        return erase(pos, last);
    }
    
    iterator erase (const_iterator first, const_iterator last)
    {
        return traits_type::xerase(_d, first, last);
    }
    
    void push_back (const_reference value)
    {
        traits_type::xpush_back(_d, value);
    }

#if __cplusplus >= 201103L    
//    TODO Implement
//    void push_back (T && value);
    
//    template <typename... Args>
//    reference emplace_back (Args &&... args);
#endif
    
    void pop_back ()
    {
        traits_type::xpop_back(_d);
    }
    
	void resize (size_type count)
    {
        resize(count, value_type());
    }
    
    void resize (size_type count, const_reference value)
    {
        traits_type::xresize(_d, count, value);
    }
    
    const_native_reference cast () const
    {
        return traits_type::xcast(_d);
    }
    
    int compare (vector const & rhs) const
    {
        return traits_type::xcompare(_d, rhs._d);
    }
    
    void swap (vector & rhs)
    {
        this->xswap(rhs);
    }
};

template <typename T, template <typename> class VectorT>
inline bool operator == (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) == 0;
}

template <typename T, template <typename> class VectorT>
inline bool operator != (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) != 0;
}

template <typename T, template <typename> class VectorT>
inline bool operator < (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) < 0;
}

template <typename T, template <typename> class VectorT>
inline bool operator <= (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) <= 0;
}

template <typename T, template <typename> class VectorT>
inline bool operator > (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) > 0;
}

template <typename T, template <typename> class VectorT>
inline bool operator >= (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) >= 0;
}

}} // pfs::traits

#endif /* __PFS_TRAITS_VECTOR_HPP__ */

