/*
 * value.hpp
 *
 *  Created on: Nov 30, 2015
 *      Author: wladt
 */

#ifndef __PFS_JSON_VALUE_HPP__
#define __PFS_JSON_VALUE_HPP__

#include <pfs/assert.hpp>
#include <pfs/utility.hpp>
#include <pfs/memory.hpp>
#include <pfs/traits/string.hpp>
#include <pfs/traits/associative_container.hpp>
#include <pfs/traits/contigous_container.hpp>
#include <pfs/traits/sequence_container.hpp>
#include <pfs/json/constants.hpp>
#include <pfs/json/iterator.hpp>

namespace pfs {
namespace json {

template <typename BoolT
        , typename IntegerT
        , typename UIntegerT
        , typename RealT
        , typename StringT
        , template <typename> class ArrayT
        , template <typename> class ObjectT>
class value
{
public:
    typedef ptrdiff_t difference_type;
    typedef size_t    size_type;

    typedef pfs::traits::string<StringT>    string_type;
    typedef BoolT                           boolean_type;
    typedef IntegerT                        integer_type;
    typedef UIntegerT                       uinteger_type;
    typedef RealT                           real_type;
    typedef pfs::traits::sequence_container<value, ArrayT>
                                            array_type;
    typedef pfs::traits::associative_container<
            pfs::traits::kv<string_type, value>, ObjectT>
                                            object_type;

    typedef typename object_type::key_type  key_type;

    struct value_rep
    {
        data_type_t type;
        
        union {
            boolean_type  boolean;
            integer_type  integer;
            real_type     real;
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

        value_rep (real_type v) 
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
   
    typedef value                       value_type;
    typedef value *                     pointer;
    typedef value const *               const_pointer;
    typedef value &                     reference;
    typedef value const &               const_reference;
    typedef basic_iterator<value>       iterator;
    typedef basic_iterator<value const> const_iterator;

    friend class basic_iterator<value>;
    friend class basic_iterator<value const>;

protected:
    rep_type _d;

//public:
//    class iterator;
//
//    class const_iterator 
//        : iterator_facade<random_access_iterator_tag
//                , const_iterator
//                , value const
//                , value const *    // Pointer
//                , value const &    // Reference
//                , difference_type> // Distance
////        : public std::iterator<std::random_access_iterator_tag, const value>
//    {
//        friend class value;
////
////    public:
////        typedef value::value_type value_type;
////        typedef value::difference_type difference_type;
////        typedef value::const_pointer pointer;
////        typedef value::const_reference reference;
////        typedef std::random_access_iterator_tag iterator_category;
////
//    private:
//        pointer         _pvalue;
//        helper_iterator _it;
//
//    private:
//        void set_begin ();
//        void set_end ();
//
//        static reference ref (const_iterator &);
//        static pointer   ptr (const_iterator &);
//        static void increment (const_iterator &, difference_type n = 1);
//        
//        static void decrement (const_iterator & it, difference_type n = 1)
//        {
//            increment(it, -n);
//        }
//        
//        static bool equals (Derived const & lhs, Derived const & rhs);
//        static int compare (const_iterator const & lhs, const_iterator const & rhs);
//        static reference subscript (const_iterator &, difference_type n);
//        static difference_type diff (const_iterator const & lhs, const_iterator const & rhs);
//
//    public:
//        const_iterator ()
//            : _pvalue (0)
//            , _it ()
//        {}
//
//        const_iterator (pointer ptr);
//
//        const_iterator (const value::iterator & other);
//
//        const_iterator (const const_iterator & other)
//            : _pvalue (other._pvalue)
//            , _it (other._it)
//        {}
//
//        const_iterator & operator= (const_iterator other)
//        {
//            pfs::swap(_pvalue, other._pvalue);
//            pfs::swap(_it, other._it);
//            return *this;
//        }
//
//        object_type::key_type key () const;
//    };
//
//    class iterator : public const_iterator
//    {
////    public:
////        typedef const_iterator base_class;
////        typedef value::pointer pointer;
////        typedef value::reference reference;
////
////        iterator ()
////        : base_class ()
////        {
////        }
////
////        iterator (pointer object)
////        : base_class (object)
////        {
////        }
////
////        iterator (const iterator & other)
////        : base_class (other)
////        {
////        }
////
////        iterator & operator= (iterator other)
////        {
////            base_class::operator= (other);
////            return *this;
////        }
////
////        reference operator* ()
////        {
////            return const_cast<reference> (base_class::operator* ());
////        }
////
////        pointer operator-> ()
////        {
////            return const_cast<pointer> (base_class::operator-> ());
////        }
////
////        iterator operator++ (int)
////        {
////            iterator result = *this;
////            base_class::operator++ ();
////            return result;
////        }
////
////        iterator & operator++ ()
////        {
////            base_class::operator++ ();
////            return *this;
////        }
////
////        iterator operator-- (int)
////        {
////            iterator result = *this;
////            base_class::operator-- ();
////            return result;
////        }
////
////        iterator & operator-- ()
////        {
////            base_class::operator-- ();
////            return *this;
////        }
////
////        iterator & operator+= (difference_type i)
////        {
////            base_class::operator+= (i);
////            return *this;
////        }
////
////        iterator & operator-= (difference_type i)
////        {
////            base_class::operator-= (i);
////            return *this;
////        }
////
////        iterator operator+ (difference_type i)
////        {
////            iterator result = *this;
////            result += i;
////            return result;
////        }
////
////        iterator operator- (difference_type i)
////        {
////            iterator result = *this;
////            result -= i;
////            return result;
////        }
////
////        difference_type operator- (const iterator & other) const
////        {
////            return base_class::operator- (other);
////        }
////
////        reference operator[] (difference_type n) const
////        {
////            return const_cast<reference> (base_class::operator[] (n));
////        }
//    };

public:

    /**
     * @brief Constructs null value.
     */
    value () 
        : _d()
    {}

    /**
     * @brief Constructs boolean value.
     */
    value (bool v) 
        : _d(v)
    {}

    /**
     * @brief Constructs integer numeric value from char value.
     */
    value (char v) 
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from signed char value.
     */
    value (signed char v) 
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned char value.
     */
    value (unsigned char v) 
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from short value.
     */
    value (short v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from short value.
     */
    value (unsigned short v)
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from int value.
     */
    value (int v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned int value.
     */
    value (unsigned int v)
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from long value.
     */
    value (long v) 
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned long value.
     */
    value (unsigned long v) 
        : _d(static_cast<uinteger_type>(v))
    {}

#ifdef PFS_HAVE_LONGLONG

    /**
     * @brief Constructs integer numeric value from long long value.
     */
    value (long long v) 
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned long long value.
     */
    value (unsigned long long v)
        : _d(static_cast<uinteger_type>(v))
    {}
#endif

    /**
     * @brief Constructs number value from float value.
     */
    value (float v) 
        : _d(static_cast<real_type>(v))
    {}

    /**
     * @brief Constructs number value from double value.
     */
    value (double v)
        : _d(static_cast<real_type>(v))
    {}

#ifdef PFS_HAVE_LONG_DOUBLE

    /**
     * @brief Constructs number value from double value.
     */
    value (long double v)
        : _d(static_cast<real_type>(v))
    {}
#endif

    /**
     * @brief Constructs string value from string.
     */
    value (string_type const & v)
        : _d(v)
    {}

    /**
     * @brief Constructs string value from C-string.
     */
    explicit value (char const * v) 
        : _d(string_type(v))
    {}

    /**
     * @brief Constructs array value from other type_array value.
     */
    value (array_type const & v) 
        : _d(v)
    {}

    /**
     * @brief Constructs object value from other type_object value.
     */
    value (object_type const & v) 
        : _d(v)
    {}

//    value (size_type n, const value & v);
//
//    value (const value & other);
//
//    ~value ();
//
//    value & operator = (const value & other);
//
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

//    reference operator [] (size_type index)
//    {
//        
//    }

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
                = _d.object->insert(pfs::make_pair(key, value()));
            it = result.first;

            PFS_ASSERT(it != _d.object->end());
        }

        return it->second;
    }

    reference operator [] (const char * key)
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
    void push_back (value const & v)
    {
        if (_d.type == data_type::null)
            _d = array_type();

        PFS_ASSERT(_d.type == data_type::array);

        _d.array->push_back(v);
    }

    
//    template <typename T>
//    T get () const;
//
//    // TODO Implement
//    //	template <typename T>
//    //	T get (const T default_value) const;
//
//    template <typename T>
//    std::pair<bool, T> fetch () const;

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

//    void swap (value & other)
//    {
//        pfs::swap(_d, other._d);
//    }

    static value make_object ()
    {
        return value(value::object_type());
    }
    
    static value make_array ()
    {
        return value(value::array_type());
    }
};

//// Forward declaration
////
//template <>
//std::pair<bool, bool> value::fetch<bool> () const;
//
//// Forward declaration
////
//template <>
//std::pair<bool, intmax_t> value::fetch<intmax_t> () const;
//
//// Forward declaration
////
//template <>
//std::pair<bool, uintmax_t> value::fetch<uintmax_t> () const;
//
//// Forward declaration
////
//template <>
//std::pair<bool, real_t> value::fetch<real_t> () const;
//
//// Forward declaration
////
//template <>
//std::pair<bool, string> value::fetch<string> () const;
//
//template <>
//inline std::pair<bool, char> value::fetch<char> () const
//{
//    std::pair<bool, intmax_t> r = fetch<intmax_t>();
//    return std::pair<bool, char>(r.first
//            , static_cast<char> (r.second));
//}
//
//template <>
//inline std::pair<bool, unsigned char> value::fetch<unsigned char> () const
//{
//    std::pair<bool, uintmax_t> r = fetch<uintmax_t>();
//    return std::pair<bool, unsigned char>(r.first
//            , static_cast<unsigned char> (r.second));
//}
//
//template <>
//inline std::pair<bool, short> value::fetch<short> () const
//{
//    std::pair<bool, intmax_t> r = fetch<intmax_t>();
//    return std::pair<bool, short>(r.first
//            , static_cast<short> (r.second));
//}
//
//template <>
//inline std::pair<bool, unsigned short> value::fetch<unsigned short> () const
//{
//    std::pair<bool, uintmax_t> r = fetch<uintmax_t>();
//    return std::pair<bool, unsigned short>(r.first
//            , static_cast<unsigned short> (r.second));
//}
//
//template <>
//inline std::pair<bool, int> value::fetch<int> () const
//{
//    std::pair<bool, intmax_t> r = fetch<intmax_t>();
//    return std::pair<bool, int>(r.first
//            , static_cast<int> (r.second));
//}
//
//template <>
//inline std::pair<bool, unsigned int> value::fetch<unsigned int> () const
//{
//    std::pair<bool, uintmax_t> r = fetch<uintmax_t>();
//    return std::pair<bool, unsigned int>(r.first
//            , static_cast<unsigned int> (r.second));
//}
//
//// TODO Need to recognize when 'long' == 'intmax_t' (for 64 bit OS)
////template <>
////inline std::pair<bool,long> value::fetch<long> () const
////{
////	std::pair<bool,intmax_t> r = fetch<intmax_t>();
////	return std::pair<bool,long>(r.first
////			, static_cast<long>(r.second));
////}
////
////
////template <>
////inline std::pair<bool,unsigned long> value::fetch<unsigned long> () const
////{
////	std::pair<bool,uintmax_t> r = fetch<uintmax_t>();
////	return std::pair<bool,unsigned long>(r.first
////			, static_cast<unsigned long>(r.second));
////}
//
//template <>
//inline std::pair<bool, float> value::fetch<float> () const
//{
//    std::pair<bool, real_t> r = fetch<real_t>();
//    return std::pair<bool, float>(r.first
//            , static_cast<float> (r.second));
//}
//
//template <>
//inline std::pair<bool, double> value::fetch<double> () const
//{
//    std::pair<bool, real_t> r = fetch<real_t>();
//    return std::pair<bool, double>(r.first
//            , static_cast<double> (r.second));
//}
//
//template <>
//inline bool value::get<bool> () const
//{
//    return fetch<bool>().second;
//}
//
//template <>
//inline char value::get<char> () const
//{
//    return fetch<char>().second;
//}
//
//template <>
//inline unsigned char value::get<unsigned char> () const
//{
//    return fetch<unsigned char>().second;
//}
//
//template <>
//inline short value::get<short> () const
//{
//    return fetch<short>().second;
//}
//
//template <>
//inline unsigned short value::get<unsigned short> () const
//{
//    return fetch<unsigned short>().second;
//}
//
//template <>
//inline int value::get<int> () const
//{
//    return fetch<int>().second;
//}
//
//template <>
//inline unsigned int value::get<unsigned int> () const
//{
//    return fetch<unsigned int>().second;
//}
//
//template <>
//inline long value::get<long> () const
//{
//    return fetch<long>().second;
//}
//
//template <>
//inline unsigned long value::get<unsigned long> () const
//{
//    return fetch<unsigned long>().second;
//}
//
//#ifdef PFS_HAVE_LONGLONG
//
//template <>
//inline long long value::get<long long> () const
//{
//    return fetch<long long>().second;
//}
//
//template <>
//inline unsigned long long value::get<unsigned long long> () const
//{
//    return fetch<unsigned long long>().second;
//}
//
//#endif
//
//template <>
//inline float value::get<float> () const
//{
//    return fetch<float>().second;
//}
//
//template <>
//inline double value::get<double> () const
//{
//    return fetch<double>().second;
//}
//
//#ifdef PFS_HAVE_LONG_DOUBLE
//
//template <>
//inline long double value::get<long double> () const
//{
//    return fetch<long double>().second;
//}
//
//#endif
//
//template <>
//inline string value::get<string> () const
//{
//    return fetch<string>().second;
//}
//
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

#endif /* __PFS_JSON_VALUE_HPP__ */
