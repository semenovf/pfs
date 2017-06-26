/*
 * iterator.hpp
 *
 *  Created on: Jan 9, 2016
 *      Author: wladt
 */

#include <pfs/type_traits.hpp>
#include <pfs/iterator.hpp>
#include <pfs/exception.hpp>
#include <pfs/json/constants.hpp>

namespace pfs { namespace json {

template <typename ValueT>
class scalar_iterator
    : public iterator_facade<random_access_iterator_tag
                , scalar_iterator<ValueT>
                , typename ValueT::value_type
                , typename ValueT::pointer
                , typename ValueT::reference
                , typename ValueT::difference_type> // Distance
{
    friend ValueT;
    
public:
    typedef random_access_iterator_tag       iterator_category;
    typedef typename ValueT::value_type      value_type;
    typedef typename ValueT::difference_type difference_type;
    typedef typename ValueT::pointer         pointer;
    typedef typename ValueT::reference       reference;

protected:
    pointer _pvalue;

protected:
    scalar_iterator (pointer pvalue)
        : _pvalue(pvalue)
    {}
    
public: 
    static reference ref (scalar_iterator & it)
    {
        return *it._pvalue;
    }
    
    static pointer ptr (scalar_iterator & it)
    {
        return it._pvalue;
    }
    
    static void increment (scalar_iterator &, difference_type n = 1)
    {
        throw domain_error("json::scalar_iterator::increment()");
    }

    static void decrement (scalar_iterator & it, difference_type n = 1)
    {
        throw domain_error("json::scalar_iterator::decrement()");
    }

    static bool equals (scalar_iterator const & lhs
            , scalar_iterator const & rhs)
    {
        return lhs._pvalue == rhs._pvalue;
    }
    
    static int compare (scalar_iterator const & lhs
            , scalar_iterator const & rhs)
    {
        throw domain_error("json::scalar_iterator::compare()");
    }
    
    static reference subscript (scalar_iterator & it
            , difference_type n)
    {
        throw domain_error("json::scalar_iterator::subscript()");
    }
    
    static difference_type diff (scalar_iterator const & lhs
            , scalar_iterator const & rhs)
    {
        throw domain_error("json::scalar_iterator::diff()");
    }
    
public:
    scalar_iterator ()
        : _pvalue(0)
    {}
        
    scalar_iterator & operator = (scalar_iterator const & rhs)
    {
        if (_pvalue != rhs._pvalue)
            _pvalue = rhs._pvalue;
        return *this;
    }
    
    template <typename ValueU>
    friend class scalar_iterator;
    
    // Allow iterator to const_iterator assignment
    template <typename ValueU, typename EnableIf = pfs::enable_if<pfs::is_same<pfs::remove_cv<ValueT>,ValueU>::value> >
    scalar_iterator & operator = (scalar_iterator<ValueU> const & rhs)
    {
        if (_pvalue != rhs._pvalue)
            _pvalue = rhs._pvalue;
        return *this;
    }

};

template <typename ValueT>
class basic_iterator
    : public iterator_facade<random_access_iterator_tag
                , basic_iterator<ValueT>
                , ValueT
                , ValueT *
                , ValueT &
                , ptrdiff_t>
{
    friend ValueT;
    
public:    
    typedef random_access_iterator_tag iterator_category;
    typedef ValueT    value_type;
    typedef ValueT *  pointer;
    typedef ValueT &  reference;
    typedef ptrdiff_t difference_type;

protected:
    typedef typename ValueT::array_type::iterator  array_iterator_type;
    typedef typename ValueT::object_type::iterator object_iterator_type;
    typedef scalar_iterator<ValueT>                scalar_iterator_type;
    
    pointer              _pvalue;
    array_iterator_type  _array_it;
    object_iterator_type _object_it;
    scalar_iterator_type _scalar_it;

public: 
    static reference ref (basic_iterator & it);
    static pointer ptr (basic_iterator & it);
    static void increment (basic_iterator &, difference_type n = 1);

    static void decrement (basic_iterator & it, difference_type n = 1)
    {
        increment(it, -n);
    }

    static bool equals (basic_iterator const & lhs, basic_iterator const & rhs);
    static int compare (basic_iterator const & lhs, basic_iterator const & rhs);
    static reference subscript (basic_iterator & it, difference_type n);
    static difference_type diff (basic_iterator const & lhs, basic_iterator const & rhs);

protected:
    struct set_begin {};
    struct set_end {};
    
    basic_iterator (pointer pvalue, set_begin begin_tag)
        : _pvalue(pvalue)
    {
        __set_begin();
    }

    basic_iterator (pointer pvalue, set_end end_tag)
        : _pvalue(pvalue)
    {
        __set_end();
    }

    basic_iterator (pointer pvalue, array_iterator_type it)
        : _pvalue(pvalue)
        , _array_it(it)
    {}

    basic_iterator (pointer pvalue, object_iterator_type it)
        : _pvalue(pvalue)
        , _object_it(it)
    {}

    basic_iterator (pointer pvalue, scalar_iterator_type it)
        : _pvalue(pvalue)
        , _scalar_it(it)
    {}

    void __set_begin ();
    void __set_end ();

public:
    basic_iterator ()
        : _pvalue (0)
    {}

    basic_iterator (basic_iterator const & rhs);
    basic_iterator & operator = (basic_iterator const & rhs);
    
    template <typename ValueU>
    friend class basic_iterator;
    
    // Allow iterator to const_iterator conversion
    template <typename ValueU, typename EnableIf = pfs::enable_if<pfs::is_same<pfs::remove_cv<ValueT>,ValueU>::value> >
    basic_iterator (basic_iterator<ValueU> const & rhs)
        : _pvalue (rhs._pvalue)
    {
        operator = (rhs);
    }

    template <typename ValueU, typename EnableIf = pfs::enable_if<pfs::is_same<pfs::remove_cv<ValueT>,ValueU>::value> >
    basic_iterator & operator = (basic_iterator<ValueU> const & rhs)
    {
        switch (_pvalue->type()) {
        case data_type::object:
            _object_it = rhs._object_it;
            break;

        case data_type::array:
            _array_it = rhs._array_it;
            break;

        default:
            _scalar_it = rhs._scalar_it;
            break;
        }
        return *this;
    }

};

template <typename ValueT>
basic_iterator<ValueT>::basic_iterator (basic_iterator const & rhs)
    : _pvalue (rhs._pvalue)
{
    operator = (rhs);
}

template <typename ValueT>
basic_iterator<ValueT> & 
basic_iterator<ValueT>::operator = (basic_iterator const & rhs)
{
    if (this != & rhs) {
        switch (_pvalue->type()) {
        case data_type::object:
            _object_it = rhs._object_it;
            break;

        case data_type::array:
            _array_it = rhs._array_it;
            break;

        default:
            _scalar_it = rhs._scalar_it;
            break;
        }
    }
    return *this;
}

template <typename ValueT>
void basic_iterator<ValueT>::__set_begin ()
{
    switch (_pvalue->type()) {
	case data_type::object:
		_object_it = _pvalue->_d.object->begin();
        break;

	case data_type::array:
		_array_it = _pvalue->_d.array->begin();
        break;
    }
}

template <typename ValueT>
void basic_iterator<ValueT>::__set_end ()
{
    switch (_pvalue->type()) {
	case data_type::object:
		_object_it = _pvalue->_d.object->end();
        break;

	case data_type::array:
		_array_it = _pvalue->_d.array->end();
        break;
    }
}

template <typename ValueT>
typename basic_iterator<ValueT>::reference 
basic_iterator<ValueT>::ref (basic_iterator & it)
{
    switch (it._pvalue->type()) {
	case data_type::object:
		return it._object_it->second;

	case data_type::array:
		return *it._array_it;
    }

    return *it._scalar_it;
}

template <typename ValueT>
typename basic_iterator<ValueT>::pointer
basic_iterator<ValueT>::ptr (basic_iterator & it)
{
//    return it._pvalue;
    switch (it._pvalue->type()) {
    case data_type::object:
        return & it._object_it->second;

    case data_type::array:
        return & *it._array_it;
    }

    return & *it._scalar_it;
}

template <typename ValueT>
void basic_iterator<ValueT>::increment (basic_iterator & it, difference_type n)
{
    switch (it._pvalue->type()) {
	case data_type::object:
        if (n == 0)
            ;
        else if (n == 1)
            ++it._object_it;
        else if (n == -1)
            --it._object_it;
        else
            throw out_of_range("json::basic_iterator::increment()");
        break;

	case data_type::array:
		it._array_it += n;
        break;
        
    default:
        it._scalar_it += n;
        break;
    }
}

template <typename ValueT>
bool basic_iterator<ValueT>::equals (basic_iterator const & lhs
        , basic_iterator const & rhs)
{
    switch (lhs._pvalue->type()) {
	case data_type::object:
        return lhs._object_it == rhs._object_it;
        
	case data_type::array:
		return lhs._array_it == rhs._array_it;
    }
    
    return lhs._scalar_it == rhs._scalar_it;
}

template <typename ValueT>
int basic_iterator<ValueT>::compare (basic_iterator const & lhs
        , basic_iterator const & rhs)
{
    switch (lhs._pvalue->type()) {
	case data_type::object:
        throw domain_error("json::basic_iterator::compare(): object");
        
	case data_type::array:
		return lhs._array_it - rhs._array_it;
    }
    
    return scalar_iterator_type::compare(lhs._scalar_it, rhs._scalar_it);
}

template <typename ValueT>
typename basic_iterator<ValueT>::reference
basic_iterator<ValueT>::subscript (basic_iterator & it, difference_type n)
{
    switch (it._pvalue->type()) {
	case data_type::object:
        throw domain_error("json::basic_iterator::subscript(): object");
        
	case data_type::array:
		return it._array_it[n];
    }
    
    return scalar_iterator_type::subscript(it._scalar_it, n);
}

template <typename ValueT>
typename basic_iterator<ValueT>::difference_type
basic_iterator<ValueT>::diff (basic_iterator const & lhs
        , basic_iterator const & rhs)
{
    switch (lhs._pvalue->type()) {
	case data_type::object:
        throw domain_error("json::basic_iterator::diff(): object");
        
	case data_type::array:
		return lhs._array_it - rhs._array_it;
    }
    
    return scalar_iterator_type::diff(lhs._scalar_it, rhs._scalar_it);
}

}} // pfs::json
