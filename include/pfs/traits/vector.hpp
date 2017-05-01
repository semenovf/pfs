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
struct vector_rep
{
    typedef VectorT<T>                                   native_type;
    typedef native_type const &                          const_native_reference;
    typedef typename native_type::size_type              size_type;
    typedef typename native_type::value_type             value_type;
    typedef typename native_type::reference              reference;
    typedef typename native_type::const_reference        const_reference;
    typedef typename native_type::pointer                pointer;
    typedef typename native_type::const_pointer          const_pointer;
    typedef typename native_type::iterator               iterator;
    typedef typename native_type::const_iterator         const_iterator;
    typedef typename native_type::reverse_iterator       reverse_iterator;
    typedef typename native_type::const_reverse_iterator const_reverse_iterator;
    
    native_type d;
};

template <typename T, template <typename> class VectorT>
class vector
{
public:
    typedef vector_rep<T, VectorT>                    rep_type;
    typedef typename rep_type::native_type            native_type;
    typedef typename rep_type::const_native_reference const_native_reference;
    typedef typename rep_type::size_type              size_type;
    typedef typename rep_type::value_type             value_type;
    typedef typename rep_type::reference              reference;
    typedef typename rep_type::const_reference        const_reference;
    typedef typename rep_type::pointer                pointer;
    typedef typename rep_type::const_pointer          const_pointer;
    typedef typename rep_type::iterator               iterator;
    typedef typename rep_type::const_iterator         const_iterator;
    typedef typename rep_type::reverse_iterator       reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    
private:
    rep_type _d;
    
public:
    vector ()
        : _d()
    {}
        
    vector (vector const & other)
        : _d(other._d)
    {}

    vector (size_type count, T const & value)
        : _d(count, value)
    {}
    
    explicit vector (size_type count)
        : _d(count)
    {}

    template <typename InputIt>
    vector (InputIt first, InputIt last)
    {
        for (; first != last; ++first)
            push_back(*first);
    }

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
        if (this != & rhs)
            _d = rhs._d;
        return *this;
    }

    vector & operator = (const_native_reference rhs)
    {
        _d = rhs;
        return *this;
    }
    
    operator const_native_reference () const pfs_noexcept
    {
        return _d;
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
        return _d.at(pos);
    }
		
    const_reference at (size_type pos) const
    {
        if (pos >= this->size())
            throw out_of_range("string::at()");
        return _d.at(pos);
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
        return _d.at(pos);
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
        return _d.at(pos);
    }
	
    /**
     * @return Reference to the first element.
     */
    reference front ()
    {
        return *this->begin();
    }

    /**
     * @return Reference to the first element.
     */
    const_reference front () const
    {
        return *this->begin();
    }
	
    reference back ()
    {
        iterator it = this->end();
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
        return _d.data();
    }

    const_pointer  data () const
    {
        return _d.data();   
    }

    iterator begin ()
    {
        return _d.begin();
    }

    const_iterator begin () const
    {
        return _d.begin();
    }
		
    const_iterator cbegin () const
    {
        return this->begin();
    }
    
    iterator end ()
    {
        return _d.end();
    }
		
    const_iterator end () const
    {
        return _d.end();
    }
		
    const_iterator cend () const
    {
        return this->end();
    }
    
    reverse_iterator rbegin ()
    {
        return _d.rbegin();
    }
		
    const_reverse_iterator rbegin() const
    {
        return _d.rbegin();
    }
		
    const_reverse_iterator crbegin () const
    {
        return this->rbegin();
    }
    
    reverse_iterator rend ()
    {
        return _d.rend();
    }
		
    const_reverse_iterator rend () const
    {
        return _d.rend();
    }
		
    const_reverse_iterator crend () const
    {
        return this->rend();
    }
    
    bool empty () const
    {
    	return this->size() == 0;
    }

    size_type size () const pfs_noexcept
    {
        return _d.size();
    }
    
	size_type max_size () const
    {
        return _d.max_size();
    }
    
    void reserve (size_type new_capacity)
    {
        if (new_capacity > this->max_size())
            throw pfs::length_error("vector::reserve()");
                    
        _d.reserve(new_capacity);
    }
	
    size_type capacity () const pfs_noexcept
    {
        return _d.capacity();
    }

// TODO Implement	
//    void shrink_to_fit ()
//    {
//        _d.shrink_to_fit();
//    }
    
    void clear ()
    {
        _d.clear();
    }
   
    iterator insert (const_iterator pos, const_reference value)
    {
        return this->insert(pos, 1, value);
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
        return this->erase(pos, last);
    }
    
    iterator erase (const_iterator first, const_iterator last)
    {
        return _d.erase(first, last);
    }
    
    void push_back (const_reference value)
    {
        _d.push_back(value);
    }

#if __cplusplus >= 201103L    
//    TODO Implement
//    void push_back (T && value);
    
//    template <typename... Args>
//    reference emplace_back (Args &&... args);
#endif
    
    void pop_back ()
    {
        _d.pop_back();
    }
    
	void resize (size_type count)
    {
        this->resize(count, value_type());
    }
    
    void resize (size_type count, const_reference value)
    {
        _d.resize(count, value);
    }
    
    const_native_reference cast () const
    {
        return _d.cast();
    }
    
    int compare (vector const & rhs) const
    {
        return _d.compare(rhs._d);
    }
    
    void swap (vector & rhs)
    {
        _d.swap(rhs._d);
    }
};

template <typename T, template <typename> class VectorT>
inline bool
operator == (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) == 0;
}

template <typename T, template <typename> class VectorT>
inline bool
operator != (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) != 0;
}

template <typename T, template <typename> class VectorT>
inline bool
operator < (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) < 0;
}

template <typename T, template <typename> class VectorT>
inline bool
operator <= (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) <= 0;
}

template <typename T, template <typename> class VectorT>
inline bool
operator > (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) > 0;
}

template <typename T, template <typename> class VectorT>
inline bool
operator >= (vector<T, VectorT> const & lhs
        , vector<T, VectorT> const & rhs)
{
    return lhs.compare(rhs) >= 0;
}

}} // pfs::traits

#endif /* __PFS_TRAITS_VECTOR_HPP__ */
