#ifndef TEST_BASIC_HPP
#define TEST_BASIC_HPP

namespace test_basic {

template <typename value_type>
void test ()
{
    ADD_TESTS(26);
    
    value_type null_value;
    value_type boolean_value(true);
    value_type integer_value(-200);
    value_type uinteger_value(200);
    value_type real_value(3.14f);
    value_type string_value("Hello");
    value_type array_value(value_type::make_array());
    value_type object_value(value_type::make_object());
    
    TEST_OK(null_value.is_null());
    TEST_OK(boolean_value.is_boolean());
    TEST_OK(integer_value.is_integer());
    TEST_OK(integer_value.is_number());
    TEST_OK(uinteger_value.is_integer());
    TEST_OK(uinteger_value.is_number());
    TEST_OK(real_value.is_real());
    TEST_OK(real_value.is_number());
    TEST_OK(string_value.is_string());

    TEST_OK(null_value.is_scalar());
    TEST_OK(boolean_value.is_scalar());
    TEST_OK(integer_value.is_scalar());
    TEST_OK(integer_value.is_scalar());
    TEST_OK(uinteger_value.is_scalar());
    TEST_OK(uinteger_value.is_scalar());
    TEST_OK(real_value.is_scalar());
    TEST_OK(real_value.is_scalar());
    TEST_OK(string_value.is_scalar());
    
    TEST_OK(!null_value.is_container());
    TEST_OK(!string_value.is_container());
    
    TEST_OK(array_value.is_array());
    TEST_OK(object_value.is_object());
    
    TEST_OK(array_value.is_container());
    TEST_OK(object_value.is_container());
    
    TEST_OK(!array_value.is_scalar());
    TEST_OK(!object_value.is_scalar());
}

} // test_basic

#endif /* TEST_BASIC_HPP */

