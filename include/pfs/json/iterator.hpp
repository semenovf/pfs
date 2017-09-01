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

template <typename JsonType>
class scalar_iterator
    : public iterator_facade<random_access_iterator_tag
                , scalar_iterator<JsonType>
                , typename JsonType::value_type
                , typename JsonType::pointer
                , typename JsonType::reference
                , typename JsonType::difference_type> // Distance
{
    friend JsonType;
    
public:
    typedef random_access_iterator_tag       iterator_category;
    typedef typename JsonType::value_type      value_type;
    typedef typename JsonType::difference_type difference_type;
    typedef typename JsonType::pointer         pointer;
    typedef typename JsonType::reference       reference;

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
    
    template <typename JsonTypeU>
    friend class scalar_iterator;
    
    // Allow iterator to const_iterator assignment
    template <typename JsonTypeU, typename EnableIf = pfs::enable_if<pfs::is_same<pfs::remove_cv<JsonType>,JsonTypeU>::value> >
    scalar_iterator & operator = (scalar_iterator<JsonTypeU> const & rhs)
    {
        if (_pvalue != rhs._pvalue)
            _pvalue = rhs._pvalue;
        return *this;
    }
};

template <typename JsonType>
class basic_iterator
    : public iterator_facade<random_access_iterator_tag
                , basic_iterator<JsonType>
                , JsonType
                , JsonType *
                , JsonType &
                , ptrdiff_t>
{
    friend JsonType;
    
public:    
    typedef random_access_iterator_tag  iterator_category;
    typedef JsonType                    value_type;
    typedef JsonType *                  pointer;
    typedef JsonType &                  reference;
    typedef typename JsonType::key_type key_type;
    typedef ptrdiff_t                   difference_type;

protected:
    typedef typename JsonType::object_type           object_type;
    typedef typename JsonType::array_type::iterator  array_iterator_type;
    typedef typename JsonType::object_type::iterator object_iterator_type;
    typedef scalar_iterator<JsonType>                scalar_iterator_type;
    
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
    
    template <typename JsonTypeU>
    friend class basic_iterator;
    
    // Allow iterator to const_iterator conversion
    template <typename JsonTypeU, typename EnableIf = pfs::enable_if<pfs::is_same<pfs::remove_cv<JsonType>,JsonTypeU>::value> >
    basic_iterator (basic_iterator<JsonTypeU> const & rhs)
        : _pvalue (rhs._pvalue)
    {
        operator = (rhs);
    }

    template <typename JsonTypeU, typename EnableIf = pfs::enable_if<pfs::is_same<pfs::remove_cv<JsonType>,JsonTypeU>::value> >
    basic_iterator & operator = (basic_iterator<JsonTypeU> const & rhs)
    {
        _pvalue = rhs._pvalue;
        
        if (_pvalue) {
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
    }
    
    key_type key () const
    {
        return _pvalue->type() == data_type::object 
                ? object_type::key(_object_it)
                : key_type();
    }
};

template <typename JsonType>
basic_iterator<JsonType>::basic_iterator (basic_iterator const & rhs)
    : _pvalue (rhs._pvalue)
{
    operator = (rhs);
}

template <typename JsonType>
basic_iterator<JsonType> & 
basic_iterator<JsonType>::operator = (basic_iterator const & rhs)
{
    if (this != & rhs) {
        
        _pvalue = rhs._pvalue;
        
        if (_pvalue) {
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
    }
    return *this;
}

template <typename JsonType>
void basic_iterator<JsonType>::__set_begin ()
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

template <typename JsonType>
void basic_iterator<JsonType>::__set_end ()
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

template <typename JsonType>
typename basic_iterator<JsonType>::reference 
basic_iterator<JsonType>::ref (basic_iterator & it)
{
    switch (it._pvalue->type()) {
	case data_type::object:
		return object_type::mapped_reference(it._object_it); //it._object_it->second;

	case data_type::array:
		return *it._array_it;
    }

    return *it._scalar_it;
}

template <typename JsonType>
typename basic_iterator<JsonType>::pointer
basic_iterator<JsonType>::ptr (basic_iterator & it)
{
    switch (it._pvalue->type()) {
    case data_type::object:
        return & object_type::mapped_reference(it._object_it);//->second;

    case data_type::array:
        return & *it._array_it;
    }

    return & *it._scalar_it;
}

template <typename JsonType>
void basic_iterator<JsonType>::increment (basic_iterator & it, difference_type n)
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

template <typename JsonType>
bool basic_iterator<JsonType>::equals (basic_iterator const & lhs
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

template <typename JsonType>
int basic_iterator<JsonType>::compare (basic_iterator const & lhs
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

template <typename JsonType>
typename basic_iterator<JsonType>::reference
basic_iterator<JsonType>::subscript (basic_iterator & it, difference_type n)
{
    switch (it._pvalue->type()) {
	case data_type::object:
        throw domain_error("json::basic_iterator::subscript(): object");
        
	case data_type::array:
		return it._array_it[n];
    }
    
    return scalar_iterator_type::subscript(it._scalar_it, n);
}

template <typename JsonType>
typename basic_iterator<JsonType>::difference_type
basic_iterator<JsonType>::diff (basic_iterator const & lhs
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
