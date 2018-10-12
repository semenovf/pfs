#pragma once
#include <cstring>
#include <pfs/assert.hpp>
#include <pfs/compare.hpp>
#include <pfs/utility.hpp>
#include <pfs/memory.hpp>
#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include <pfs/json/constants.hpp>
#include <pfs/json/iterator.hpp>
#include <pfs/json/exception.hpp>
#include <pfs/json/cast.hpp>
#include <pfs/json/rfc7159.hpp>

namespace pfs {
namespace json {

#define PFS_JSON_TEMPLETE_SIGNATURE typename BoolT                             \
    , typename IntT                                                            \
    , typename RealT                                                           \
    , typename StringT                                                         \
    , template <typename> class SequenceContainer                              \
    , template <typename, typename> class AssociativeContainer

#define PFS_JSON_TEMPLETE_ARGS BoolT                                           \
    , IntT                                                                     \
    , RealT                                                                    \
    , StringT                                                                  \
    , SequenceContainer                                                        \
    , AssociativeContainer

template <typename BoolT = bool
        , typename IntT = intmax_t
        , typename RealT = double
        , typename StringT = pfs::string
        , template <typename> class SequenceContainer = pfs::vector
        , template <typename, typename> class AssociativeContainer = pfs::map>
class json
{
public:
    typedef ptrdiff_t difference_type;
    typedef size_t    size_type;
    typedef StringT   string_type;
    typedef BoolT     boolean_type;
    typedef IntT      integer_type;
    typedef RealT     real_type;
    typedef typename make_unsigned<IntT>::type uinteger_type;

    typedef SequenceContainer<json>                 array_type;
    typedef AssociativeContainer<string_type, json> object_type;
    typedef typename object_type::key_type          key_type;

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
            , integer(0)
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
            : type(data_type::integer)
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
    explicit json (bool v)
        : _d(v)
    {}

    /**
     * @brief Constructs integer numeric value from char value.
     */
    explicit json (char v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from signed char value.
     */
    explicit json (signed char v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned char value.
     */
    explicit json (unsigned char v)
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from short value.
     */
    explicit json (short v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from short value.
     */
    explicit json (unsigned short v)
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from int value.
     */
    explicit json (int v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned int value.
     */
    explicit json (unsigned int v)
        : _d(static_cast<uinteger_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from long value.
     */
    explicit json (long v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned long value.
     */
    explicit json (unsigned long v)
        : _d(static_cast<uinteger_type>(v))
    {}

#ifdef PFS_HAVE_LONGLONG

    /**
     * @brief Constructs integer numeric value from long long value.
     */
    explicit json (long long v)
        : _d(static_cast<integer_type>(v))
    {}

    /**
     * @brief Constructs integer numeric value from unsigned long long value.
     */
    explicit json (unsigned long long v)
        : _d(static_cast<uinteger_type>(v))
    {}
#endif

    /**
     * @brief Constructs number value from float value.
     */
    explicit json (float v)
        : _d(static_cast<real_type>(v))
    {}

    /**
     * @brief Constructs number value from double value.
     */
    explicit json (double v)
        : _d(static_cast<real_type>(v))
    {}

#ifdef PFS_HAVE_LONG_DOUBLE

    /**
     * @brief Constructs number value from double value.
     */
    explicit json (long double v)
        : _d(static_cast<real_type>(v))
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

    json & assign (json const & other);

    json (json const & other)
    {
        assign(other);
    }

    json & operator = (json const & other)
    {
        return assign(other);
    }

#if __cplusplus >= 201103L
    template <typename T, typename EnableIf = pfs::enable_if<!pfs::is_same<pfs::remove_cv<T>,json>::value> >
    json & operator = (T v)
#else
    template <typename T>
    json & operator = (T v)
#endif
    {
        return assign(json(v));
    }

#if __cplusplus >= 201103L

    // TODO Need to implement
    //json (json && other);

    // TODO Need to implement
    //json & operator = (json && other);

#endif

    ~json ()
    {
        switch (_d.type) {
        case data_type::string: {
            std::allocator<string_type> alloc;
            alloc.destroy(_d.string);
            alloc.deallocate(_d.string, 1);
            break;
        }

        case data_type::array: {
            std::allocator<array_type> alloc;
            alloc.destroy(_d.array);
            alloc.deallocate(_d.array, 1);
            break;
        }

        case data_type::object: {
            std::allocator<object_type> alloc;
            alloc.destroy(_d.object);
            alloc.deallocate(_d.object, 1);
            break;
        }

        default:
            break;
        }
    }

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
        return _d.type == data_type::integer;
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

    boolean_type const & boolean_data () const
    {
        return _d.boolean;
    }

    integer_type const & integer_data () const
    {
        return _d.integer;
    }

    real_type const & real_data () const
    {
        return _d.real;
    }

    /**
     * For safety use this method if is_string() returns true.
     *
     * @return const reference to string.
     */
    string_type const & string_data () const
    {
        return *_d.string;
    }

    /**
     * For safety use this method if is_array() returns true.
     *
     * @return const reference to array.
     */
    array_type const & array_data () const
    {
        return *_d.array;
    }

    /**
     * For safety use this method if is_object() returns true.
     *
     * @return const reference to object.
     */
    object_type const & object_data () const
    {
        return *_d.object;
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

    // For avoid ambiguous overload of operator[] with `0` value
    reference operator [] (int index)
    {
        return operator [] (static_cast<size_type>(index));
    }

    // For avoid ambiguous overload of operator[] with `0` value
    const_reference operator [] (int index) const
    {
        return operator [] (static_cast<size_type>(index));
    }

    reference operator [] (size_type index)
    {
        // implicitly convert null to array
        if (_d.type == data_type::null)
            _d = array_type();

        if (_d.type != data_type::array)
            PFS_THROW(json_exception(make_error_code(json_errc::array_expected)));

        for (size_t i = _d.array->size(); i <= index; ++i)
            _d.array->push_back(json());

        typename array_type::iterator it = _d.array->begin();
        pfs::advance(it, index);
        return *it;
    }

    const_reference operator [] (size_type index) const
    {
        if (_d.type != data_type::array)
            PFS_THROW(json_exception(make_error_code(json_errc::array_expected)));

        if (index >= _d.array->size())
            PFS_THROW(json_exception(make_error_code(json_errc::range)));

        typename array_type::iterator it = _d.array->begin();
        pfs::advance(it, index);
        return *it;
    }

    reference operator [] (key_type const & key)
    {
        if (_d.type == data_type::null)
            _d = object_type();

        if (_d.type != data_type::object)
            PFS_THROW(json_exception(make_error_code(json_errc::object_expected)));

        typename object_type::iterator it = _d.object->find(key);

        if (it == _d.object->end()) {
            pfs::pair<typename object_type::iterator, bool> result
                = _d.object->insert(key, json());
            it = result.first;

            PFS_ASSERT(it != _d.object->end());
        }

        return object_type::mapped_reference(it);
    }

    const_reference operator [] (key_type const & key) const
    {
        if (_d.type != data_type::object)
            PFS_THROW(json_exception(make_error_code(json_errc::object_expected)));

        typename object_type::const_iterator it = _d.object->find(key);

        if (it == _d.object->cend())
            PFS_THROW(json_exception(make_error_code(json_errc::range)));

        return object_type::mapped_reference(it);
    }

    reference operator [] (const char * key)
    {
        return operator [] (key_type(key));
    }

    const_reference operator [] (const char * key) const
    {
        return operator [] (key_type(key));
    }

    void erase (int index)
    {
        if (index < 0) return;
        erase(static_cast<size_type>(index));
    }

    void erase (size_type index)
    {
        if (_d.type() != data_type::array)
            PFS_THROW(json_exception(make_error_code(json_errc::array_expected)));
        if (index >= _d.array->size())
            PFS_THROW(json_exception(make_error_code(json_errc::range)));
        _d.array->erase(index);
    }

    void erase (key_type const & key)
    {
        if (_d.type() != data_type::object)
            PFS_THROW(json_exception(make_error_code(json_errc::object_expected)));

        _d.object->erase(key);
    }

    void erase (char const * key)
    {
        this->erase(key_type(key));
    }

    bool empty () const
    {
        return (_d.type == data_type::null
                || (_d.type == data_type::array && _d.array->empty())
                || (_d.type == data_type::object && _d.object->empty()));
    }

    /**
     * @brief Clear value content.
     */
    void clear ()
    {
        json v;
        this->swap(v);
    }

    void push_back (json const & v)
    {
        if (_d.type == data_type::null)
            _d = array_type();

        PFS_ASSERT(_d.type == data_type::array);

        _d.array->push_back(v);
    }

#if __cplusplus >= 201103L
    template <typename T, typename EnableIf = pfs::enable_if<!pfs::is_same<pfs::remove_cv<T>,json>::value> >
    void push_back (T v)
#else
    template <typename T>
    void push_back (T v)
#endif
    {
        this->push_back(json(v));
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

    iterator find (int index)
    {
        return index < 0
                ? end()
                : find(static_cast<size_type>(index));
    }

    iterator find (size_type index)
    {
        if (is_array()) {
            if (index < _d.array->size()) {
                typename array_type::iterator it = _d.array->begin();
                pfs::advance(it, index);
                return iterator(this, it);
            }
        }

        return end();
    }

    const_iterator find (int index) const
    {
        return index < 0
                ? end()
                : find(static_cast<size_type>(index));
    }

    const_iterator find (size_type index) const
    {
        if (is_array()) {
            if (index < _d.array->size()) {
                typename array_type::iterator it = _d.array->begin();
                pfs::advance(it, index);
                return const_iterator(this, it);
            }
        }

        return end();
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

// private:
//     template <typename T>
//     pfs::pair<bool, T> fetch () const
//     {
//         switch (type()) {
//         case data_type::boolean:  return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(_d.boolean));
//         case data_type::integer:
//         case data_type::uinteger: return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(_d.integer));
//         case data_type::real:     return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(_d.real));
//         case data_type::string:   return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(*_d.string));
//         case data_type::array:    return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(*_d.array));
//         case data_type::object:   return pfs::make_pair<bool,T>(true, cast_traits<T>::cast(*_d.object));
//         case data_type::null:     return pfs::make_pair<bool,T>(true, cast_traits<T>::cast());
//         default:
//             break;
//         }
//
//         return pfs::make_pair<bool,T>(false, cast_traits<T>::cast());
//     }

public:
    template <typename T>
    //T get (T const & default_value = T()) const
    T get () const
    {
//         pfs::pair<bool, T> r = fetch<T>();
//         return r.first ? r.second : default_value;
        switch (type()) {
        case data_type::boolean:  return cast_traits<T>::cast(_d.boolean);
        case data_type::integer:  return cast_traits<T>::cast(_d.integer);
        case data_type::real:     return cast_traits<T>::cast(_d.real);
        case data_type::string:   return cast_traits<T>::cast(*_d.string);
        case data_type::array:    return cast_traits<T>::cast(*_d.array);
        case data_type::object:   return cast_traits<T>::cast(*_d.object);
        case data_type::null:     return cast_traits<T>::cast();
        default:
            break;
        }
    }

    string_type get_string () const
    {
        return this->get<string_type>();
    }

    void swap (json & other)
    {
        pfs::swap(_d, other._d);
    }

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

        dom_builder_context<json> sax(*this);
        typename grammar_type::parse_context context;

        //context.is_json_begin = true;
        context.sax           = & sax;

//        sax.s.push(this);

        fsm_type fsm(grammar.p_json_tr, & context);
        typename fsm_type::result_type r = fsm.exec(0, first, last);

        if (r.first && r.second == last)
            return error_code();

        if (r.first && r.second != last)
            context.ec = make_error_code(json_errc::excess_source);

        json j;
        this->swap(j);
        return context.ec;
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

    friend void swap (json & lhs, json & rhs)
    {
        lhs.swap(rhs);
    }

    bool operator == (json const & rhs) const;

    //friend pfs::byte_ostream & operator << (pfs::byte_ostream & os, json const & v)
    //friend pfs::byte_istream & operator >> (pfs::byte_istream & is, json & v);

    string_type to_string () const;
};

template <PFS_JSON_TEMPLETE_SIGNATURE>
json<PFS_JSON_TEMPLETE_ARGS> &
json<PFS_JSON_TEMPLETE_ARGS>::assign (json const & other)
{
    this->~json();

    switch (other._d.type) {
    case data_type::null:
        _d = rep_type();
        break;

    case data_type::boolean:
        _d = rep_type(other._d.boolean);
        break;

    case data_type::integer:
        _d = rep_type(other._d.integer);
        break;

    case data_type::real:
        _d = rep_type(other._d.real);
        break;

    case data_type::string:
        _d = rep_type(*other._d.string);
        break;

    case data_type::array:
        _d = rep_type(*other._d.array);
        break;

    case data_type::object:
        _d = rep_type(*other._d.object);
        break;
    }

    return *this;
}

template <PFS_JSON_TEMPLETE_SIGNATURE>
typename json<PFS_JSON_TEMPLETE_ARGS>::string_type
json<PFS_JSON_TEMPLETE_ARGS>::to_string () const
{
    switch (type()) {
    case data_type::null:
        return string_type("null");

    case data_type::boolean:
        return pfs::to_string(get<boolean_type>());

    case data_type::integer:
        return pfs::to_string(get<integer_type>());

    case data_type::real:
        return pfs::to_string(get<real_type>());

    case data_type::string: {
        string_type r;
        r.append(1, '"');
        r.append(get<string_type>());
        r.append(1, '"');
        return r;
    }

    case data_type::array: {
        string_type r;
        json::const_iterator it = cbegin();
        json::const_iterator last = cend();

        r.append(1, '[');

        if (it != last) {
            r.append(it->to_string());
            ++it;
        }

        for (; it != last; ++it) {
            r.append(1, ',');
            r.append(it->to_string());
        }

        r.append(1, ']');

        return r;
    }

    case data_type::object: {
        string_type r;
        json::const_iterator it = cbegin();
        json::const_iterator last = cend();

        r.append(1, '{');

        if (it != last) {
            r.append(1, '"');
            r.append(it.key());
            r.append("\":");
            r.append(it->to_string());
            ++it;
        }

        for (; it != last; ++it) {
            r.append(",\"");
            r.append(it.key());
            r.append("\":");
            r.append(it->to_string());
        }

        r.append(1, '}');
        return r;
    }}

    return string_type();
}

template <PFS_JSON_TEMPLETE_SIGNATURE>
bool json<PFS_JSON_TEMPLETE_ARGS>::operator == (json const & rhs) const
{
    if (this == & rhs)
        return true;

    if (this->type() != rhs.type())
        return false;

    switch (this->type()) {
    case data_type::null:
        break;

    case data_type::boolean:
        if (this->_d.boolean != rhs._d.boolean)
            return false;
        break;

    case data_type::integer:
        if (this->_d.integer != rhs._d.integer)
            return false;
        break;

    case data_type::real:
        if (this->_d.real != rhs._d.real)
            return false;
        break;

    case data_type::string: {
        if (*this->_d.string != *rhs._d.string)
            return false;
        break;
    }

    case data_type::array: {
        if (this->_d.array->size() != rhs._d.array->size())
            return false;

        const_iterator itl  = this->cbegin();
        const_iterator last = this->cend();
        const_iterator itr  = rhs.cbegin();

        for (; itl != last; ++itl, ++itr)
            if (!(*itl == *itr))
                return false;

        break;
    }

    case data_type::object: {
        if (this->_d.object->size() != rhs._d.object->size())
            return false;

        const_iterator itl  = this->cbegin();
        const_iterator last = this->cend();
        const_iterator itr  = rhs.cbegin();

        for (; itl != last; ++itl, ++itr) {
            if (itl.key() != itr.key())
                return false;

            if (!(*itl == *itr))
                return false;
        }

        break;
    }}

    return true;
}

template <typename JsonType>
class reference_wrapper_basic
{
protected:
    JsonType * _p;

public:
    typedef typename JsonType::string_type string_type;
    typedef typename JsonType::size_type   size_type;
    typedef typename JsonType::key_type    key_type;

protected:
    reference_wrapper_basic ()
        : _p(0)
    {}

    reference_wrapper_basic (JsonType * p)
        : _p(p)
    {}

public:
    explicit reference_wrapper_basic (JsonType & ref)
        : _p(& ref)
    {}

    reference_wrapper_basic (reference_wrapper_basic<JsonType> const & ref)
        :_p(ref._p)
    {}

    reference_wrapper_basic & operator = (reference_wrapper_basic<JsonType> const & ref)
    {
        _p = ref._p;
        return *this;
    }

    operator bool () const
    {
        return _p != 0;
    }

    template <typename T>
    T get (T const & default_value = T()) const
    {
        return _p ? _p->template get<T>() : default_value;
    }

    string_type get_string (string_type const & default_value = string_type()) const
    {
        return _p ? _p->get_string() : default_value;
    }

};

template <typename JsonType>
class reference_wrapper : public reference_wrapper_basic<JsonType>
{
    typedef reference_wrapper_basic<JsonType> base_class;

public:
    typedef typename base_class::size_type size_type;
    typedef typename base_class::key_type  key_type;

protected:
    reference_wrapper ()
        : base_class()
    {}

 public:
    explicit reference_wrapper (JsonType & ref)
        : base_class(& ref)
    {}

    reference_wrapper (reference_wrapper<JsonType> const & ref)
        :base_class(ref)
    {}

    // For avoid ambiguous overload of operator[] with `0` value
    reference_wrapper operator [] (int index) const
    {
        return operator [] (size_type(index));
    }

    reference_wrapper operator [] (size_type index) const
    {
        if (this->_p) {
            typename JsonType::iterator it = this->_p->find(index);

            if (it != this->_p->end())
                return reference_wrapper(*it);
        }
        return reference_wrapper();
    }

    reference_wrapper operator [] (key_type const & key) const
    {
        if (this->_p) {
            typename JsonType::iterator it = this->_p->find(key);

            if (it != this->_p->end())
                return reference_wrapper(*it);
        }
        return reference_wrapper();
    }

    reference_wrapper operator [] (const char * key) const
    {
        return operator [] (key_type(key));
    }
};

template <typename JsonType>
class reference_wrapper<JsonType const> : public reference_wrapper_basic<JsonType const>
{
    typedef reference_wrapper_basic<JsonType const> base_class;

public:
    typedef typename base_class::size_type size_type;
    typedef typename base_class::key_type  key_type;

protected:
    reference_wrapper ()
        : base_class()
    {}

public:
    explicit reference_wrapper (JsonType const & ref)
        : base_class(& ref)
    {}

    reference_wrapper (reference_wrapper<JsonType> const & ref)
        :base_class(ref)
    {}

    // For avoid ambiguous overload of operator[] with `0` value
    reference_wrapper operator [] (int index) const
    {
        PFS_ASSERT(index >= 0);
        return operator [] (size_type(index));
    }

    reference_wrapper operator [] (size_type index) const
    {
        if (this->_p) {
            typename JsonType::const_iterator it = this->_p->find(index);

            if (it != this->_p->end())
                return reference_wrapper(*it);
        }
        return reference_wrapper();
    }

    reference_wrapper operator [] (key_type const & key) const
    {
        if (this->_p) {
            typename JsonType::const_iterator it = this->_p->find(key);

            if (it != this->_p->end())
                return reference_wrapper(*it);
        }
        return reference_wrapper();
    }

    reference_wrapper operator [] (const char * key) const
    {
        return operator [] (key_type(key));
    }
};

template <typename JsonT, typename T>
JsonT to_json (T const & v, bool plain = false);

template <typename JsonType>
inline reference_wrapper<JsonType> ref (JsonType & j)
{
    return reference_wrapper<JsonType>(j);
}

template <typename JsonType>
inline reference_wrapper<JsonType const> cref (JsonType const & j)
{
    return reference_wrapper<JsonType const>(j);
}

}} // pfs::json

namespace pfs {

template <PFS_JSON_TEMPLETE_SIGNATURE>
inline typename json::json<PFS_JSON_TEMPLETE_ARGS>::string_type to_string (
        json::json<PFS_JSON_TEMPLETE_ARGS> const & v)
{
    return v.to_string();
}

} // pfs
