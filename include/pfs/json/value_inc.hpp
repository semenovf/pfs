/* 
 * File:   value_inc.hpp
 * Author: wladt
 *
 * Created on June 22, 2017, 2:37 PM
 */

#ifndef __PFS_JSON_VALUE_INC_HPP__
#define __PFS_JSON_VALUE_INC_HPP__

#ifndef __PFS_JSON_VALUE_HPP__
#   error "Do not include directly"
#endif

namespace pfs {
namespace json {

#if __COMMENT__

value::value_rep::value_rep (value::type_enum t)
{
	switch (t) {
	case value::type_null:
		break;

	case value::type_boolean:
		boolean = false;
		break;

	case value::type_integer:
	case value::type_uinteger:
		integer = integer_type(0);
		break;

	case value::type_float:
		flt = float_type(0.0f);
		break;

	case value::type_string: {
		std::allocator<string_type> alloc;
	    string = alloc.allocate(1);
	    alloc.construct(string, string_type());
	    break;
	}

	case value::type_object: {
		std::allocator<object_type> alloc;
	    object = alloc.allocate(1);
	    alloc.construct(object, object_type());
	    break;
	}

	case value::type_array: {
		std::allocator<array_type> alloc;
	    array = alloc.allocate(1);
	    alloc.construct(array, array_type());
	    break;
	}}
}


value::value (const value & other)
        : _type(other._type)
{
	switch (_type)	{
	case type_null:
		break;

	case type_object:
		_value = *other._value.object;
		break;

	case type_array:
		_value = *other._value.array;
		break;

	case type_string:
		_value = *other._value.string;
		break;

	case type_boolean:
		_value = other._value.boolean;
		break;

	case type_integer:
	case type_uinteger:
		_value = other._value.integer;
		break;

	case type_float:
		_value = other._value.flt;
		break;
	}
}

value::value (size_type n, const value & v)
	: _type(type_array)
{
	std::allocator<array_type> alloc;
	_value.array = alloc.allocate(1);
#if __cplusplus >= 201103L
	alloc.construct(_value.array, n, v);
#else
	alloc.construct(_value.array, array_type());

	for (size_type i = 0; i < n; ++i) {
		_value.array->push_back(v);
	}
#endif
}


value & value::operator = (const value & other)
{
	if (this != & other) {
		value v;
		this->swap(v);

		_type = other._type;

		switch (_type)	{
		case type_null:
			break;

		case type_object:
			_value = *other._value.object;
			break;

		case type_array:
			_value = *other._value.array;
			break;

		case type_string:
			_value = *other._value.string;
			break;

		case type_boolean:
			_value = other._value.boolean;
			break;

		case type_integer:
		case type_uinteger:
			_value = other._value.integer;
			break;

		case type_float:
			_value = other._value.flt;
			break;
		}
	}

	return *this;

}

value::reference value::operator [] (size_type index)
{
	// implicitly convert null to object
	if (_type == type_null) {
		_type = type_array;
		std::allocator<array_type> alloc;
		_value.array = alloc.allocate(1);
		alloc.construct(_value.array, array_type());
	}

	PFS_ASSERT(_type == type_array);

	for (size_t i = _value.array->size(); i <= index; ++i) {
		_value.array->push_back(value());
	}

	return _value.array->operator[](index);
}

template <>
std::pair<bool,bool> value::fetch<bool> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,bool>(true, false);
	case type_boolean:
		return std::pair<bool,bool>(true, _value.boolean);
	case type_integer:
	case type_uinteger:
		return std::pair<bool,bool>(true, _value.integer ? true : false);
	case type_float:
		return std::pair<bool,bool>(true, _value.flt != float_type(0.0f) ? true : false);
	case type_string:
		return std::pair<bool,bool>(true, pfs::lexical_cast<bool>(*_value.string));
	case type_object:
		return std::pair<bool,bool>(true, _value.object ? _value.object->size() != 0 : false);
	case type_array:
		return std::pair<bool,bool>(true, _value.array ? _value.array->size() != 0 : false);
	}

	return std::pair<bool,bool>(false, false);
}

template <>
std::pair<bool,intmax_t> value::fetch<intmax_t> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,intmax_t>(true, 0);
	case type_boolean:
		return std::pair<bool,intmax_t>(true, _value.boolean ? 1 : 0);
	case type_integer:
	case type_uinteger:
		return std::pair<bool,intmax_t>(true, _value.integer);
	case type_float:
		return std::pair<bool,intmax_t>(true
				, static_cast<intmax_t>(_value.flt));
	case type_string: {
		bool ok;
		intmax_t r = pfs::lexical_cast<intmax_t>(*_value.string, 10, & ok);
		return std::pair<bool,intmax_t>(true
				, ok ? r : 0);
	}
	case type_object:
		return std::pair<bool,intmax_t>(true
				, _value.object ? static_cast<intmax_t>(_value.object->size()) : 0);
	case type_array:
		return std::pair<bool,intmax_t>(true
				, _value.array ? static_cast<intmax_t>(_value.array->size()) : 0);
	}

	return std::pair<bool,intmax_t>(false, 0);
}

template <>
std::pair<bool,uintmax_t> value::fetch<uintmax_t> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,uintmax_t>(true, 0);
	case type_boolean:
		return std::pair<bool,uintmax_t>(true
				, _value.boolean ? 1 : 0);
	case type_integer:
	case type_uinteger:
		return std::pair<bool,uintmax_t>(true
				, _value.integer);
	case type_float:
		return std::pair<bool,uintmax_t>(true
				, static_cast<uintmax_t>(_value.flt));
	case type_string: {
		bool ok;
		uintmax_t r = pfs::lexical_cast<uintmax_t>(*_value.string, 10, & ok);
		return std::pair<bool,uintmax_t>(true
				, ok ? r : 0);
	}
	case type_object:
		return std::pair<bool,uintmax_t>(true
				, _value.object ? _value.object->size() : 0);
	case type_array:
		return std::pair<bool,uintmax_t>(true
				, _value.array ? _value.array->size() : 0);
	}

	return std::pair<bool,uintmax_t>(false, 0);
}

template <>
std::pair<bool,real_t> value::fetch<real_t> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,real_t>(true, real_t(0));
	case type_boolean:
		return std::pair<bool,real_t>(true
				, _value.boolean ? real_t(1) : real_t(0));
	case type_integer:
		return std::pair<bool,real_t>(true
				, static_cast<real_t>(static_cast<integer_type>(_value.integer)));
	case type_uinteger:
		return std::pair<bool,real_t>(true
				, static_cast<real_t>(static_cast<uinteger_type>(_value.integer)));
	case type_float:
		return std::pair<bool,real_t>(true, _value.flt);
	case type_string: {
		bool ok;
		real_t r = pfs::lexical_cast<real_t>(*_value.string, & ok);
		return std::pair<bool,real_t>(true
				, ok ? r : real_t(0));
	}
	case type_object:
		return std::pair<bool,real_t>(true
				, _value.object ? static_cast<real_t>(_value.object->size()) : real_t(0));
	case type_array:
		return std::pair<bool,real_t>(true
				, _value.array ? static_cast<real_t>(_value.array->size()) : real_t(0));
	}

	return std::pair<bool,real_t>(false, 0);
}


template <>
std::pair<bool,string> value::fetch<string> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,string>(true, string());
	case type_boolean:
		return std::pair<bool,string>(true, to_string(_value.boolean));
	case type_integer:
		return std::pair<bool,string>(true
				, to_string(static_cast<integer_type>(_value.integer)));
	case type_uinteger:
		return std::pair<bool,string>(true
				, to_string(static_cast<uinteger_type>(_value.integer)));
	case type_float:
		return std::pair<bool,string>(true, to_string(_value.flt));
	case type_string:
		return std::pair<bool,string>(true, *_value.string);
	case type_object:
		return std::pair<bool,string>(false, string());
	case type_array:
		return std::pair<bool,string>(false, string());
	}

	return std::pair<bool,string>(false, string());
}

value::value_rep::value_rep (value::type_enum t)
{
	switch (t) {
	case value::type_null:
		break;

	case value::type_boolean:
		boolean = false;
		break;

	case value::type_integer:
	case value::type_uinteger:
		integer = integer_type(0);
		break;

	case value::type_float:
		flt = float_type(0.0f);
		break;

	case value::type_string: {
		std::allocator<string_type> alloc;
	    string = alloc.allocate(1);
	    alloc.construct(string, string_type());
	    break;
	}

	case value::type_object: {
		std::allocator<object_type> alloc;
	    object = alloc.allocate(1);
	    alloc.construct(object, object_type());
	    break;
	}

	case value::type_array: {
		std::allocator<array_type> alloc;
	    array = alloc.allocate(1);
	    alloc.construct(array, array_type());
	    break;
	}}
}

value::~value ()
{
	switch (_type) {
	case type_string: {
		std::allocator<string_type> alloc;
        alloc.destroy(_value.string);
        alloc.deallocate(_value.string, 1);
        _value.string = 0;
		break;
	}

	case type_array: {
		std::allocator<array_type> alloc;
        alloc.destroy(_value.array);
        alloc.deallocate(_value.array, 1);
        _value.array = 0;
		break;
	}

	case type_object: {
		std::allocator<object_type> alloc;
		alloc.destroy(_value.object);
		alloc.deallocate(_value.object, 1);
		_value.object = 0;
		break;
	}

	default:
		break;
	}
}

value::value (const value & other)
        : _type(other._type)
{
	switch (_type)	{
	case type_null:
		break;

	case type_object:
		_value = *other._value.object;
		break;

	case type_array:
		_value = *other._value.array;
		break;

	case type_string:
		_value = *other._value.string;
		break;

	case type_boolean:
		_value = other._value.boolean;
		break;

	case type_integer:
	case type_uinteger:
		_value = other._value.integer;
		break;

	case type_float:
		_value = other._value.flt;
		break;
	}
}

value::value (size_type n, const value & v)
	: _type(type_array)
{
	std::allocator<array_type> alloc;
	_value.array = alloc.allocate(1);
#if __cplusplus >= 201103L
	alloc.construct(_value.array, n, v);
#else
	alloc.construct(_value.array, array_type());

	for (size_type i = 0; i < n; ++i) {
		_value.array->push_back(v);
	}
#endif
}


value & value::operator = (const value & other)
{
	if (this != & other) {
		value v;
		this->swap(v);

		_type = other._type;

		switch (_type)	{
		case type_null:
			break;

		case type_object:
			_value = *other._value.object;
			break;

		case type_array:
			_value = *other._value.array;
			break;

		case type_string:
			_value = *other._value.string;
			break;

		case type_boolean:
			_value = other._value.boolean;
			break;

		case type_integer:
		case type_uinteger:
			_value = other._value.integer;
			break;

		case type_float:
			_value = other._value.flt;
			break;
		}
	}

	return *this;

}

value::reference value::operator [] (size_type index)
{
	// implicitly convert null to object
	if (_type == type_null) {
		_type = type_array;
		std::allocator<array_type> alloc;
		_value.array = alloc.allocate(1);
		alloc.construct(_value.array, array_type());
	}

	PFS_ASSERT(_type == type_array);

	for (size_t i = _value.array->size(); i <= index; ++i) {
		_value.array->push_back(value());
	}

	return _value.array->operator[](index);
}

value::reference value::operator [] (const object_type::key_type & key)
{
	if (_type == type_null) {
		_type = type_object;
		std::allocator<object_type> alloc;
		_value.object = alloc.allocate(1);
		alloc.construct(_value.object, object_type());
	}

	PFS_ASSERT(_type == type_object);

	return _value.object->operator[](key);
}

value::size_type value::size () const
{
	switch (_type) {
	case type_null  : return 0;
	case type_array : return _value.array->size();
	case type_object: return _value.object->size();
	default: break;
    }
	return 1;
}


template <>
std::pair<bool,bool> value::fetch<bool> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,bool>(true, false);
	case type_boolean:
		return std::pair<bool,bool>(true, _value.boolean);
	case type_integer:
	case type_uinteger:
		return std::pair<bool,bool>(true, _value.integer ? true : false);
	case type_float:
		return std::pair<bool,bool>(true, _value.flt != float_type(0.0f) ? true : false);
	case type_string:
		return std::pair<bool,bool>(true, pfs::lexical_cast<bool>(*_value.string));
	case type_object:
		return std::pair<bool,bool>(true, _value.object ? _value.object->size() != 0 : false);
	case type_array:
		return std::pair<bool,bool>(true, _value.array ? _value.array->size() != 0 : false);
	}

	return std::pair<bool,bool>(false, false);
}

template <>
std::pair<bool,intmax_t> value::fetch<intmax_t> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,intmax_t>(true, 0);
	case type_boolean:
		return std::pair<bool,intmax_t>(true, _value.boolean ? 1 : 0);
	case type_integer:
	case type_uinteger:
		return std::pair<bool,intmax_t>(true, _value.integer);
	case type_float:
		return std::pair<bool,intmax_t>(true
				, static_cast<intmax_t>(_value.flt));
	case type_string: {
		bool ok;
		intmax_t r = pfs::lexical_cast<intmax_t>(*_value.string, 10, & ok);
		return std::pair<bool,intmax_t>(true
				, ok ? r : 0);
	}
	case type_object:
		return std::pair<bool,intmax_t>(true
				, _value.object ? static_cast<intmax_t>(_value.object->size()) : 0);
	case type_array:
		return std::pair<bool,intmax_t>(true
				, _value.array ? static_cast<intmax_t>(_value.array->size()) : 0);
	}

	return std::pair<bool,intmax_t>(false, 0);
}

template <>
std::pair<bool,uintmax_t> value::fetch<uintmax_t> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,uintmax_t>(true, 0);
	case type_boolean:
		return std::pair<bool,uintmax_t>(true
				, _value.boolean ? 1 : 0);
	case type_integer:
	case type_uinteger:
		return std::pair<bool,uintmax_t>(true
				, _value.integer);
	case type_float:
		return std::pair<bool,uintmax_t>(true
				, static_cast<uintmax_t>(_value.flt));
	case type_string: {
		bool ok;
		uintmax_t r = pfs::lexical_cast<uintmax_t>(*_value.string, 10, & ok);
		return std::pair<bool,uintmax_t>(true
				, ok ? r : 0);
	}
	case type_object:
		return std::pair<bool,uintmax_t>(true
				, _value.object ? _value.object->size() : 0);
	case type_array:
		return std::pair<bool,uintmax_t>(true
				, _value.array ? _value.array->size() : 0);
	}

	return std::pair<bool,uintmax_t>(false, 0);
}

template <>
std::pair<bool,real_t> value::fetch<real_t> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,real_t>(true, real_t(0));
	case type_boolean:
		return std::pair<bool,real_t>(true
				, _value.boolean ? real_t(1) : real_t(0));
	case type_integer:
		return std::pair<bool,real_t>(true
				, static_cast<real_t>(static_cast<integer_type>(_value.integer)));
	case type_uinteger:
		return std::pair<bool,real_t>(true
				, static_cast<real_t>(static_cast<uinteger_type>(_value.integer)));
	case type_float:
		return std::pair<bool,real_t>(true, _value.flt);
	case type_string: {
		bool ok;
		real_t r = pfs::lexical_cast<real_t>(*_value.string, & ok);
		return std::pair<bool,real_t>(true
				, ok ? r : real_t(0));
	}
	case type_object:
		return std::pair<bool,real_t>(true
				, _value.object ? static_cast<real_t>(_value.object->size()) : real_t(0));
	case type_array:
		return std::pair<bool,real_t>(true
				, _value.array ? static_cast<real_t>(_value.array->size()) : real_t(0));
	}

	return std::pair<bool,real_t>(false, 0);
}


template <>
std::pair<bool,string> value::fetch<string> () const
{
	switch (_type) {
	case type_null:
		return std::pair<bool,string>(true, string());
	case type_boolean:
		return std::pair<bool,string>(true, to_string(_value.boolean));
	case type_integer:
		return std::pair<bool,string>(true
				, to_string(static_cast<integer_type>(_value.integer)));
	case type_uinteger:
		return std::pair<bool,string>(true
				, to_string(static_cast<uinteger_type>(_value.integer)));
	case type_float:
		return std::pair<bool,string>(true, to_string(_value.flt));
	case type_string:
		return std::pair<bool,string>(true, *_value.string);
	case type_object:
		return std::pair<bool,string>(false, string());
	case type_array:
		return std::pair<bool,string>(false, string());
	}

	return std::pair<bool,string>(false, string());
}

value::iterator value::find (object_type::key_type const & key)
{
    if (is_object()) {
        object_type::iterator it = _value.object->find(key);
        
        if (it != _value.object->end()) {
            iterator result(this);
            result._it.object_it = it;
            return result;
        }
    }
    
    return end();
}

value::const_iterator value::find (object_type::key_type const & key) const
{
    if (is_object()) {
        object_type::iterator it = _value.object->find(key);
        
        if (it != _value.object->end()) {
            iterator result(const_cast<value *>(this));
            result._it.object_it = it;
            return result;
        }
    }
    
    return end();
}

#endif // __COMMENT__

}} // pfs::json

#endif /* __PFS_JSON_VALUE_INC_HPP__ */

