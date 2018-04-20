#pragma once
#include <pfs/assert.hpp>
#include <pfs/exception.hpp>
#include <pfs/memory.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/stdcxx/basic_string.hpp>
#include <pfs/unicode/unicode_iterator.hpp>
#include <pfs/unicode/u8_iterator.hpp>

namespace pfs {

class string : public stdcxx::basic_string<char, string>
{
    typedef stdcxx::basic_string<char, string> base_class;

public:
    typedef typename base_class::value_type       value_type;
    typedef typename base_class::traits_type      traits_type;
    typedef typename base_class::size_type        size_type;
    typedef typename base_class::difference_type  difference_type;
    typedef typename base_class::reference        reference;
    typedef typename base_class::const_reference  const_reference;
    typedef typename base_class::pointer          pointer;
    typedef typename base_class::const_pointer    const_pointer;
    typedef typename base_class::iterator         iterator;
    typedef typename base_class::const_iterator   const_iterator;
    typedef typename base_class::reverse_iterator reverse_iterator;
    typedef typename base_class::const_reverse_iterator const_reverse_iterator;

public:
    string () : base_class () {}

    string (string const & other)
        : base_class(other)
    {}

    string (string const & other
            , size_type pos
            , size_type count = base_class::npos)
        : base_class(other, pos, count)
    {}

    string (const_pointer s)
        : base_class(s)
    {}

    string (const_pointer s, size_type n)
        : base_class(s, n)
    {}

    string (size_type n, value_type c)
        : base_class(n, c)
    {}

    string (std::string const & s)
        : base_class(s)
    {}

    template <typename InputIterator>
    string (InputIterator first, InputIterator last)
        : base_class(first, last)
    {}

    ~string ()
    {}

    std::string utf8 () const
    {
        return std::string(this->c_str(), this->size());
    }
};

namespace unicode {

template <>
struct unicode_iterator_traits<string::iterator>
    :  unicode_iterator_limits<string::iterator, char *>
{
    typedef utf8_iterator<pfs::string::iterator> iterator;
    typedef u8_output_iterator<pfs::back_insert_iterator<pfs::string> > output_iterator;
    typedef u8_input_iterator<pfs::string::iterator> input_iterator;
};

template <>
struct unicode_iterator_traits<std::string::const_iterator>
    :  unicode_iterator_limits<std::string::const_iterator, char const *>
{
    typedef utf8_iterator<std::string::const_iterator> iterator;
    typedef u8_input_iterator<std::string::const_iterator> input_iterator;
};

} // unicode

namespace details {
namespace integral {

//#define BITS_SIZE(T) (sizeof(T) * 8)
template <typename CharT>
CharT * uintmax_to_cstr (uintmax_t num
        , int radix
        , bool uppercase
        , CharT * buf
        , size_t n)
{
    static const CharT digits_lower[] = {
          CharT('0'), CharT('1'), CharT('2'), CharT('3'), CharT('4')
        , CharT('5'), CharT('6'), CharT('7'), CharT('8'), CharT('9')
        , CharT('a'), CharT('b'), CharT('c'), CharT('d'), CharT('e')
        , CharT('f'), CharT('g'), CharT('h'), CharT('i'), CharT('j')
        , CharT('k'), CharT('l'), CharT('m'), CharT('n'), CharT('o')
        , CharT('p'), CharT('q'), CharT('r'), CharT('s'), CharT('t')
        , CharT('u'), CharT('v'), CharT('w'), CharT('x'), CharT('y')
        , CharT('z')
    };

    static const CharT digits_upper[] = {
          CharT('0'), CharT('1'), CharT('2'), CharT('3'), CharT('4')
        , CharT('5'), CharT('6'), CharT('7'), CharT('8'), CharT('9')
        , CharT('A'), CharT('B'), CharT('C'), CharT('D'), CharT('E')
        , CharT('F'), CharT('G'), CharT('H'), CharT('I'), CharT('J')
        , CharT('K'), CharT('L'), CharT('M'), CharT('N'), CharT('O')
        , CharT('P'), CharT('Q'), CharT('R'), CharT('S'), CharT('T')
        , CharT('U'), CharT('V'), CharT('W'), CharT('X'), CharT('Y')
        , CharT('Z')
    };

    CharT * p = & buf[n - 1];

    if (!(radix >= 2 && radix <= 36))
        throw pfs::invalid_argument("uintmax_to_cstr(): bad radix");

    buf[n - 1] = '\0';
    const CharT * digits = uppercase ? digits_upper : digits_lower;

    if (num) {
        while (num > 0) {
            *--p = digits[num % radix];
            num /= radix;
        }
    } else { /* num == 0 */
        *--p = CharT('0');
    }

    return p;
}

template <typename CharT>
CharT * intmax_to_cstr (intmax_t num
        , int radix
        , bool uppercase
        , CharT * buf
        , size_t n)
{
    CharT * p = 0;

    if (num < 0) {
        p = uintmax_to_cstr<CharT>(static_cast<uintmax_t>(num * -1)
                , radix
                , uppercase
                , buf
                , n);
        *--p = '-';
    } else {
        p = uintmax_to_cstr<CharT>(static_cast<uintmax_t>(num)
                , radix
                , uppercase
                , buf
                , n);
    }

    return p;
}

/**
 *
 * @param value
 * @param field_width Specifies the minimum amount of space that a is padded
 *        to and filled with the character fillChar. A positive value produces
 *        right-aligned text; a negative value produces left-aligned text.
 * @param radix
 * @param uppercase
 * @param fill_char
 * @return
 */
template <typename UintType>
string to_string (typename pfs::enable_if<pfs::is_unsigned<UintType>::value, UintType>::type value
        , int radix
        , bool uppercase)
{
    typedef string::value_type char_type;

    char_type buf[sizeof(UintType) * 8 + 1];
    char_type * str = uintmax_to_cstr<char_type>(static_cast<uintmax_t>(value)
            , radix
            , uppercase
            , buf
            , sizeof(buf)/sizeof(buf[0]));

    return string(str);
}

template <typename IntType>
string to_string (typename enable_if<is_signed<IntType>::value, IntType>::type value
        , int radix
        , bool uppercase)
{
    typedef string::value_type char_type;

    char_type buf[sizeof(IntType) * 8 + 1];
    char_type * str = intmax_to_cstr<char_type>(static_cast<intmax_t>(value)
            , radix
            , uppercase
            , buf
            , sizeof(buf)/sizeof(buf[0]));

    return string(str);
}

}} // details::integral

inline string to_string (bool value)
{
    return value ? string("true") : string("false");
}

inline string to_string (char a)
{
    return string(1, string::value_type(a));
}

inline string to_string (signed char a
        , int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<signed char>(a, radix, uppercase);
}

inline string to_string (unsigned char a, int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned char>(a, radix, uppercase);
}

inline string to_string (short a, int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<short>(a, radix, uppercase);
}

inline string to_string (unsigned short a, int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned short>(a, radix, uppercase);
}

inline string to_string (int a, int radix = 10, bool uppercase = false)
{
    return details::integral::to_string<int>(a, radix, uppercase);
}

inline string to_string (unsigned int a, int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned int>(a, radix, uppercase);
}

inline string to_string (long a, int radix = 10, bool uppercase = false)
{
    return details::integral::to_string<long>(a, radix, uppercase);
}

inline string to_string (unsigned long a, int radix = 10, bool uppercase = false)
{
    return details::integral::to_string<unsigned long>(a, radix, uppercase);
}

#if PFS_HAVE_LONGLONG

inline string to_string (long long a, int radix = 10, bool uppercase = false)
{
    return details::integral::to_string<long long>(a, radix, uppercase);
}

inline string to_string (unsigned long long a, int radix = 10
        , bool uppercase = false)
{
    return details::integral::to_string<unsigned long long>(a, radix
            , uppercase);
}
#endif

namespace details {
namespace fp {

char * double_to_cstr (double num
        , char f
        , int prec
        , char * buf
        , size_t * n);

// 1.18973e+4932 with 'f' flag has length 4940
//
template <typename Float>
string to_string (typename pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type value
        , char f
        , int prec)
{
    static const size_t FP_BUFSZ = 128;

    string s;

    size_t sz = FP_BUFSZ;
    char buf[FP_BUFSZ];
    char * pbuf = buf;

    char * str = double_to_cstr(value, f, prec, pbuf, & sz);

    if (!str) {
        ++sz;
        pbuf = pfs::allocator<char>().allocate(sz);
        str = double_to_cstr(value, f, prec, pbuf, & sz);
        PFS_ASSERT(str);
        s = string(str);
        pfs::allocator<char>().destroy(pbuf);
    } else {
        s = string(str);
    }

    return s;
}

}} // details::fp

inline string to_string (float a, char format = 'f', int precision = -1)
{
    return details::fp::to_string<float>(a, format, precision);
}

inline string to_string (double a, char format = 'g', int precision = -1)
{
    return details::fp::to_string<double>(a, format, precision);
}

} // pfs
