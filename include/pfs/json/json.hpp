/*
 * json.hpp
 *
 *  Created on: Nov 30, 2015
 *      Author: wladt
 */

#ifndef __PFS_JSON_JSON_HPP__
#define __PFS_JSON_JSON_HPP__

#include <cstring>
#include <pfs/assert.hpp>
#include <pfs/utility.hpp>
#include <pfs/memory.hpp>
#include <pfs/string.hpp>
#include <pfs/traits/associative_container.hpp>
#include <pfs/traits/sequence_container.hpp>
#include <pfs/json/constants.hpp>
#include <pfs/json/iterator.hpp>
#include <pfs/json/exception.hpp>
#include <pfs/json/cast.hpp>
#include <pfs/json/rfc4627.hpp>

namespace pfs {
namespace json {

template <typename BoolType
        , typename IntType
        , typename FloatType
        , typename StringType
        , template <typename> class SequenceContainerImplType
        , template <typename> class AssociativeContainerImplType>
class json
{
public:
    typedef ptrdiff_t difference_type;
    typedef size_t    size_type;

    typedef StringType                            string_type;
    typedef BoolType                              boolean_type;
    typedef IntType                               integer_type;
    typedef typename make_unsigned<IntType>::type uinteger_type;
    typedef FloatType                             float_type;
    typedef pfs::traits::sequence_container<json
            , SequenceContainerImplType>          array_type;

    typedef pfs::traits::associative_container<
              pfs::traits::kv<string_type, json>
            , AssociativeContainerImplType>       object_type;

    typedef typename object_type::key_type        key_type;

    struct value_rep
    {
        data_type_t type;
        
        union {
            boolean_type  boolean;
            integer_type  integer;
            float_type    real;
            string_type * string;
            array_type *  array;
            object_type * object;
        };

        value_rep ()
            : type(data_type::null)
        {}

        value_rep (boolean_type v)
            : type(data_type::boolean)
            , boolean(v)
        {}

        value_rep (integer_type v)
            : type(data_type::integer)
            , integer(v)
        {}

        value_rep (uinteger_type v)
            : type(data_type::uinteger)
            , integer(v)
        {}

        value_rep (float_type v) 
            : type(data_type::real)
            , real(v)
        {}

        value_rep (string_type const & v)
            : type(data_type::string)
        {
            pfs::allocator<string_type> alloc;
            string = alloc.allocate(1);
            alloc.construct(string, v);
        }

        value_rep (array_type const & v)
            : type(data_type::array)
        {
            pfs::allocator<array_type> alloc;
            array = alloc.allocate(1);
            alloc.construct(array, v);
        }

        value_rep (object_type const & v)
            : type(data_type::object)
        {
            pfs::allocator<object_type> alloc;
            object = alloc.allocate(1);
            alloc.construct(object, v);
        }
    };
    
    typedef value_rep rep_type;
   
    typedef json                       value_type;
    typedef json *                     pointer;
    typedef json const *               const_pointer;
    typedef json &                     reference;
    typedef json const &               const_reference;
    typedef basic_iterator<json>       iterator;
    typedef basic_iterator<json const> const_iterator;

    friend class basic_iterator<json>;
    friend class basic_iterator<json const>;

protected:
    rep_type _d;

public:

    /**
     * @brief Constructs null value.
     */
    json () 
        : _d()
    {}

    /**
     * @brief Constructs boolean value.
     */
    json (bool v) 
        : _d(v)
    {}

    /**
     * @brief Constructs integer numeric value from char value.
     */
    json (char v) 
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from signed char value.
     */
    json (signed char v) 
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned char value.
     */
    json (unsigned char v) 
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from short value.
     */
    json (short v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from short value.
     */
    json (unsigned short v)
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from int value.
     */
    json (int v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned int value.
     */
    json (unsigned int v)
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from long value.
     */
    json (long v) 
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned long value.
     */
    json (unsigned long v) 
        : _d(static_cast<uinteger_type>(v))
    {}

#ifdef PFS_HAVE_LONGLONG

    /**
     * @brief Constructs integer numeric value from long long value.
     */
    json (long long v) 
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned long long value.
     */
    json (unsigned long long v)
        : _d(static_cast<uinteger_type>(v))
    {}
#endif

    /**
     * @brief Constructs number value from float value.
     */
    json (float v) 
        : _d(static_cast<float_type>(v))
    {}

    /**
     * @brief Constructs number value from double value.
     */
    json (double v)
        : _d(static_cast<float_type>(v))
    {}

#ifdef PFS_HAVE_LONG_DOUBLE

    /**
     * @brief Constructs number value from double value.
     */
    json (long double v)
        : _d(static_cast<float_type>(v))
    {}
#endif

    /**
     * @brief Constructs string value from string.
     */
    json (string_type const & v)
        : _d(v)
    {}

    /**
     * @brief Constructs string value from C-string.
     */
    explicit json (char const * v) 
        : _d(string_type(v))
    {}

    /**
     * @brief Constructs array value from other type_array value.
     */
    json (array_type const & v) 
        : _d(v)
    {}

    /**
     * @brief Constructs object value from other type_object value.
     */
    json (object_type const & v) 
        : _d(v)
    {}

//    value (size_type n, const value & v);
//    value (const value & other);

    ~json ()
    {
        switch (_d.type) {
        case data_type::string: {
            std::allocator<string_type> alloc;
            alloc.destroy(_d.string);
            alloc.deallocate(_d.string, 1);
            _d.string = 0;
            break;
        }

        case data_type::array: {
            std::allocator<array_type> alloc;
            alloc.destroy(_d.array);
            alloc.deallocate(_d.array, 1);
            _d.array = 0;
            break;
        }

        case data_type::object: {
            std::allocator<object_type> alloc;
            alloc.destroy(_d.object);
            alloc.deallocate(_d.object, 1);
            _d.object = 0;
            break;
        }

        default:
            break;
        }
    }    

//    value & operator = (const value & other);

    data_type_t type () const
    {
        return _d.type;
    }

    bool is_null () const
    {
        return _d.type == data_type::null;
    }

    bool is_boolean () const
    {
        return _d.type == data_type::boolean;
    }

    bool is_integer () const
    {
        return _d.type == data_type::integer 
                || _d.type == data_type::uinteger;
    }

    bool is_real () const
    {
        return _d.type == data_type::real;
    }

    bool is_number () const
    {
        return is_integer() || is_real();
    }

    bool is_string () const
    {
        return _d.type == data_type::string;
    }

    bool is_array () const
    {
        return _d.type == data_type::array;
    }

    bool is_object () const
    {
        return _d.type == data_type::object;
    }

    bool is_scalar () const
    {
        return is_null() || is_string() || is_boolean() || is_number();
    }

    /**
     * @brief Checks if value is a container (array or object)
     */
    bool is_container () const
    {
        return is_array() || is_object();
    }

    size_type size () const
    {
        switch (_d.type) {
        case data_type::null:   return 0;
        case data_type::array:  return _d.array->size();
        case data_type::object: return _d.object->size();
        default: break;
        }
        return 1;
    }

//    reference at (size_t index)
//    {
//        PFS_ASSERT(_type == type_array);
//        PFS_ASSERT_RANGE(index < _value.array->size());
//        return _value.array->at(index);
//    }
//
//    const_reference at (size_t index) const
//    {
//        PFS_ASSERT(_type == type_array);
//        PFS_ASSERT_RANGE(index < _value.array->size());
//        return _value.array->at(index);
//    }
//
//    reference at (const object_type::key_type & key)
//    {
//        PFS_ASSERT(_type == type_object);
//        return _value.object->at(key);
//    }
//
//    const_reference at (const object_type::key_type & key) const
//    {
//        PFS_ASSERT(_type == type_object);
//        return _value.object->at(key);
//    }

    reference operator [] (size_type index)
    {
        // implicitly convert null to object
        if (_d.type == data_type::null) {
            _d = array_type();
//            std::allocator<array_type> alloc;
//            _value.array = alloc.allocate(1);
//            alloc.construct(_value.array, array_type());
        }

        PFS_ASSERT(_d.type == data_type::array);

        for (size_t i = _d.array->size(); i <= index; ++i) {
            _d.array->push_back(json());
        }

        typename array_type::iterator it = _d.array->begin();
        pfs::advance(it, index);
        return *it;
    }
    
    json operator [] (size_type index) const
    {
        if (_d.type == data_type::array) {
            typename array_type::iterator it = _d.array->begin();
            pfs::advance(it, index);
            return *it;
        }
        return json();
    }
    
    reference operator [] (key_type const & key)
    {
        if (_d.type == data_type::null) {
            _d = object_type();
//            pfs::allocator<object_type> alloc;
//            _d.type = data_type::object;
//            _d.object = alloc.allocate(1);
//            alloc.construct(_d.object, object_type());
        }

        PFS_ASSERT(_d.type == data_type::object);

        typename object_type::iterator it = _d.object->find(key);

        if (it == _d.object->end()) {
            pfs::pair<typename object_type::iterator, bool> result 
                = _d.object->insert(key, json());
            it = result.first;

            PFS_ASSERT(it != _d.object->end());
        }

        return object_type::mapped_reference(it);
    }

    json operator [] (key_type const & key) const
    {
        if (_d.type == data_type::object) {
            typename object_type::const_iterator it = _d.object->find(key);
            if (it != _d.object->cend())
                return object_type::mapped_reference(it);
        }
        return json();
    }
    
    reference operator [] (const char * key)
    {
        return operator [] (key_type(key));
    }

    json operator [] (const char * key) const
    {
        return operator [] (key_type(key));
    }

//    void erase (const size_type index)
//    {
//        PFS_ASSERT(_type == type_array);
//        PFS_ASSERT_RANGE(index < _value.array->size());
//        _value.array->erase(_value.array->begin() + static_cast<difference_type> (index));
//    }
//
//    size_type erase (const object_type::key_type & key)
//    {
//        PFS_ASSERT(_type == type_object);
//        return _value.object->erase(key);
//    }
//
//    bool empty () const
//    {
//        return _type == type_null
//                || (_type == type_array && _value.array->empty())
//                || (_type == type_object && _value.object->empty())
//                ? true
//                : false;
//    }
//
//    /**
//     * @brief
//     */
//    size_type size () const;
//
//    /**
//     * @brief Clear value content.
//     */
//    void clear ()
//    {
//        value v;
//        this->swap(v);
//    }
//
    void push_back (json const & v)
    {
        if (_d.type == data_type::null)
            _d = array_type();

        PFS_ASSERT(_d.type == data_type::array);

        _d.array->push_back(v);
    }

    iterator begin ()
    {
        return iterator(this, typename iterator::set_begin());
    }

    const_iterator begin () const
    {
        return const_iterator(this, typename const_iterator::set_begin());
    }

    const_iterator cbegin () const
    {
        return begin();
    }

    iterator end ()
    {
        return iterator(this, typename iterator::set_end());
    }

    const_iterator end () const
    {
        return const_iterator(this, typename const_iterator::set_end());
    }

    const_iterator cend () const
    {
        return end();
    }

    iterator find (key_type const & key)
    {
        if (is_object()) {
            typename object_type::iterator it = _d.object->find(key);

            if (it != _d.object->end())
                return iterator(this, it);
        }

        return end();
    }

    const_iterator find (key_type const & key) const
    {
        if (is_object()) {
            typename object_type::iterator it = _d.object->find(key);

            if (it != _d.object->end())
                return const_iterator(this, it);
        }

        return end();
    }
    
    iterator find (char const * key)
    {
        return find(key_type(key));
    }

    const_iterator find (char const * key) const
    {
        return find(key_type(key));
    }

    bool contains (key_type const & key) const
    {
        return find(key) != end();
    }

    bool contains (char const * key) const
    {
        return find(key) != end();
    }

    template <typename T>
    struct cast_traits : public details::cast_traits<T, json>
    {};

    template <typename T>
    pfs::pair<bool, T> fetch () const
    {
        switch (type()) {
        case data_type::boolean:  return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(_d.boolean));
        case data_type::integer:
        case data_type::uinteger: return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(_d.integer));
        case data_type::real:     return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(_d.real));
        case data_type::string:   return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(*_d.string));
        case data_type::array:    return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(*_d.array));
        case data_type::object:   return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(*_d.object));
        case data_type::null:
        default:
            break;
        }

        return pfs::make_pair<bool,T>(true, cast_traits<T>::cast());
    }

    template <typename T>
    T get (T const & default_value = T()) const
    {
        pfs::pair<bool, T> r = fetch<T>();
        return r.first ? r.second : default_value;
    }
    
//    void swap (value & other)
//    {
//        pfs::swap(_d, other._d);
//    }

    /**
     * @brief Deserializer
     * @param s
     * @return 
     */
    error_code parse (string_type const & s)
    {
        return parse(s.cbegin(), s.cend());
    }

    error_code parse (typename string_type::const_iterator first
            , typename string_type::const_iterator last)
    {
        typedef grammar<json> grammar_type;
        typedef typename grammar_type::fsm_type fsm_type;

        // Initialize grammar's static members
        static grammar_type grammar;
        typename grammar_type::parse_context context;

        fsm_type fsm(grammar.p_json_tr, & context);
        typename fsm_type::result_type r = fsm.exec(0, first, last);

        if (r.first && r.second == last)
            return true;

        return false;
    }

//    error_code write (string_type & s);
    
    static json make_object ()
    {
        return json(json::object_type());
    }
    
    static json make_array ()
    {
        return json(json::array_type());
    }
    
};


//enum brace_position_enum
//{
//    brace_same_line
//    , brace_next_line
//};
//
//enum comma_position_enum
//{
//    comma_same_line
//    , comma_next_line
//};
//
////struct print_format
////{
////    int                 base_indent;    // base indent in spaces, -1 - compact style
////    brace_position_enum bracepos;
////    comma_position_enum commapos;
////    string              key_separator;
////    string              new_line;
////};
//
//struct print_format
//{
//    string ws_symbol; //<! whitespace symbol, default is ' ' (space)
//    size_t base_indent; //<! base indent in symbols, default is 4 
//    size_t brace_indent; //<! indent in symbols for brace, default is 0
//    size_t first_item_indent; //<! indent for first item (for comma_next_line), default is 0
//    size_t ws_before_vseparator; //<! whitespaces before value separator (comma), default is 0
//    size_t ws_after_vseparator; //<! whitespaces after value separator (comma), default is 1
//    size_t ws_before_kvseparator; //<! whitespaces before key/value separator (colon), default is 0
//    size_t ws_after_kvseparator; //<! whitespaces after key/value separator (colon), default is 1
//    brace_position_enum brace_position; //<! brace position, default is @c brace_same_line
//    comma_position_enum comma_position; //<! vseparator position, default is @c comma_same_line
//};
//
//// See description at https://en.wikipedia.org/wiki/Indent_style
////
//
//enum print_style_enum
//{
//    style_plain // all in one line
//    , style_kr // Kernighan and Ritchie, base_indent = 4, brace_position = brace_same_line, comma_position = comma_same_line
//    , style_bsd
//    , style_allman = style_bsd // BSD (Allman), base_indent = 4, brace_position = brace_next_line, comma_position = comma_same_line
//    , style_gnu // base_indent = 2, brace_position = brace_next_line, comma_position = comma_same_line
//    , style_whitesmiths // base_indent = 4, brace_position = brace_next_line_indent, comma_position = comma_same_line
//    , style_favorite // base_indent = 4, brace_position = brace_same_line, comma_position = comma_next_line
//};
//
}} // pfs::json
//
//
namespace pfs {

template <typename JsonType, typename T>
JsonType to_json (T const & v, bool plain = false);

//
//string to_string (json::value const & v, json::print_style_enum style);
//string to_string (json::value const & v, json::print_format const & format);
//
//inline string to_string (json::value const & v)
//{
//    return to_string(v, json::style_plain);
//}
//
//template <>
//void pack (pack_context & ctx, json::value const & v);
//
//template <>
//bool unpack (unpack_context & ctx, json::value & v);

} // pfs

#endif /* __PFS_JSON_JSON_HPP__ */
