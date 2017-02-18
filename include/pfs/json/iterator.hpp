/*
 * iterator.hpp
 *
 *  Created on: Jan 9, 2016
 *      Author: wladt
 */

namespace pfs { namespace json {

template <typename CharT>
void value<CharT>::const_iterator::set_begin ()
{
	switch (_pvalue->_type) {
	case value::type_object:
		_it.object_it = _pvalue->_value.object->begin();
		break;

	case value::type_array:
		_it.array_it = _pvalue->_value.array->begin();
		break;

	case value::type_null:
		_it.scalar_it.set_end();
		break;

	default:
		_it.scalar_it.set_begin();
		break;
	}
}

template <typename CharT>
void value<CharT>::const_iterator::set_end ()
{
	switch (_pvalue->_type) {
	case value::type_object:
		_it.object_it = _pvalue->_value.object->end();
		break;

	case value::type_array:
		_it.array_it = _pvalue->_value.array->end();
		break;

	default:
		_it.scalar_it.set_end();
		break;
	}
}

template <typename CharT>
value<CharT>::const_iterator::const_iterator (pointer ptr)
	: _pvalue(ptr)
{
    switch (_pvalue->_type) {
        case value::type_object:
            _it.object_it = object_type::iterator();
            break;

        case value::type_array:
            _it.array_it = array_type::iterator();
            break;

        default:
            _it.scalar_it = scalar_iterator();
            break;
    }
}

template <typename CharT>
value<CharT>::const_iterator::const_iterator (value::iterator const & other)
	: _pvalue(other._pvalue)
{
    switch (_pvalue->_type) {
    case value::type_object:
    	_it.object_it = other._it.object_it;
    	break;

    case value::type_array:
        _it.array_it = other._it.array_it;
            break;

    default:
    	_it.scalar_it = other._it.scalar_it;
            break;
    }
}

template <typename CharT>
value<CharT>::const_iterator::reference value<CharT>::const_iterator::ref (const_iterator & it);
{
    switch (it._pvalue->_type) {
	case value::type_object:
		return it._it.object_it->second;

	case value::type_array:
		return *it._it.array_it;

	case value::type_null:
		throw out_of_range("json::const_iterator::ref(): cannot get value");
		break;

	default:
		if (it._it.scalar_it.is_begin())
			break; // return *_value_ptr;
		else
            throw out_of_range("json::const_iterator::ref(): cannot get value");
    }

    return *it._pvalue;
}

template <typename CharT>
value<CharT>::const_iterator::pointer value<CharT>::const_iterator::ptr (const_iterator & it)
{
    switch (it._pvalue->_type) {
	case (value::type_object):
		return & it._it.object_it->second;

	case value::type_array:
		return & *it._it.array_it;

	default:
		if (it._it.scalar_it.is_begin())
			break; // return _value_ptr;
		else
			throw out_of_range("json::const_iterator::ptr(): cannot get value");
    }

    return it._pvalue;
}

template <typename CharT>
value<CharT>::const_iterator & 
value<CharT>::const_iterator::increment (const_iterator & it
        , difference_type n)
{
    switch (it._pvalue->_type) {
    case value::type_object:
        if (n == 0)
            ;
        else if (n == 1)
            ++it._it.object_it;
        else if (n == -1)
            --it._it.object_it;
        else
            throw invalid_argument("json::const_iterator::increment(): "
                    "increment value too big for object");
        break;

    case value::type_array:
    	it._it.array_it += n;
        break;

    default:
    	it._it.scalar_it += n;
    	break;
    }

    return *this;
}

template <typename CharT>
int value<CharT>::const_iterator::compare (const_iterator const & lhs
        , const_iterator const & rhs)
{
    if (lhs._pvalue != rhs._pvalue)
        throw invalid_argument("json::const_iterator::compare()");

  	switch (lhs._pvalue->_type) {
	case value::type_object:
		return lhs._it.object_it == rhs._it.object_it;

	case value::type_array:
		return lhs._it.array_it == rhs._it.array_it;

	default:
		break;
	}

	return lhs._it.scalar_it == rhs._it.scalar_it;
}

template <typename CharT>
int value<CharT>::const_iterator::compare (const_iterator const & lhs
        , const_iterator const & rhs)
{
    if (lhs._pvalue != rhs._pvalue)
        throw invalid_argument("json::const_iterator::compare()");

    if (lhs._pvalue->_type != value::type_object)
        throw invalid_argument("json::const_iterator::compare()");

  	switch (_pvalue->_type) {
	case value::type_array:
		if (lhs._it.array_it == rhs._it.array_it)
            return 0;

	default:
        if (lhs._it.scalar_it == rhs._it.scalar_it)
            return 0;
		break;
	}

	return 0;
}


bool value::const_iterator::operator < (const const_iterator & other) const
{
	PFS_ASSERT(_pvalue == other._pvalue);
	PFS_ASSERT(_pvalue->_type != value::type_object);

    switch (_pvalue->_type) {
	case value::type_object:
		break; // see above

	case value::type_array:
		return _it.array_it < other._it.array_it;

	default:
		break;
    }

    return _it.scalar_it < other._it.scalar_it;
}

value::const_iterator::difference_type value::const_iterator::operator - (const const_iterator & other) const
{
	PFS_ASSERT(_pvalue->_type != value::type_object);

    switch (_pvalue->_type) {
	case value::type_object:
		break; // see above

	case value::type_array:
		return _it.array_it - other._it.array_it;

	default:
		break;
    }

    return _it.scalar_it - other._it.scalar_it;
}

value::const_iterator::reference value::const_iterator::operator [] (difference_type n) const
{
	PFS_ASSERT(_pvalue->_type != value::type_object);

    switch (_pvalue->_type) {
	case value::type_object:
		break; // see above

	case value::type_array:
		return *(_it.array_it + n);

	case value::type_null:
		PFS_ASSERT_RANGE(false);
		break;

	default:
		if (_it.scalar_it == -n) {
			break;
		} else {
			PFS_ASSERT_RANGE(false);
			break;
		}
    }

    return *_pvalue;
}

value::object_type::key_type value::const_iterator::key () const
{
    switch (_pvalue->_type) {
	case value::type_object:
		return _it.object_it->first;

	default:
		PFS_ASSERT_DOMAIN("Cannot use key() for non-object iterators");
		break;
    }
}

}} // pfs::json
