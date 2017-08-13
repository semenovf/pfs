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

#endif // __COMMENT__

}} // pfs::json

#endif /* __PFS_JSON_VALUE_INC_HPP__ */

