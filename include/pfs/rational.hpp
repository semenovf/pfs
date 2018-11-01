#pragma once
#include <pfs/assert.hpp>
#include <pfs/compare.hpp>
#include <pfs/math.hpp>
#include <pfs/ratio.hpp>
#include <pfs/string.hpp>

namespace pfs {

//
// Inspired from Boost::rational.
//

template <typename IntT>
class rational : public compare_op<rational<IntT>, rational<IntT> >
{
public:
    typedef IntT int_type;

private:
    int_type _num;
    int_type _den; // always > 0

private:
    static const intmax_t * poweroften ()
    {
        static const intmax_t __n = 1000000000;

        static const intmax_t __poweroften[] = {
            1 , 10, 100, 1000, 10000, 100000, 1000000
            , 10000000, 100000000
            , 1000000000 // <= __n
#if PFS_HAVE_INT64
            , __n * 10        // INT64_C(10000000000)
            , __n * 100       // INT64_C(100000000000)
            , __n * 1000      // INT64_C(1000000000000)
            , __n * 10000     // INT64_C(10000000000000)
            , __n * 100000    // INT64_C(100000000000000)
            , __n * 1000000   // INT64_C(10000000000000000)
            , __n * 10000000  // INT64_C(100000000000000000)
            , __n * 100000000 // INT64_C(1000000000000000000)
#endif
        };

        return __poweroften;
    }

    static const uint8_t poweroften_size ()
    {
        static const uint8_t __poweroften_size =
#if PFS_HAVE_INT64
        18;
#else
        10;
#endif
        return __poweroften_size;
    }

public:
    rational () : _num(0), _den(1) {}

    rational (int_type const & num, int_type const & den = 1)
        : _num(sign_of(num) * sign_of(den) * pfs::abs(num))
        , _den(pfs::abs(den))
    {
        PFS_ASSERT_X(den != 0, "zero denominator");
    }

    template <intmax_t Num, intmax_t Denom>
    rational (ratio<Num, Denom> ratio)
        : _num(sign_of(ratio.num) * sign_of(ratio.den) * pfs::abs(ratio.num))
        , _den(pfs::abs(ratio.den))
    {
        PFS_ASSERT(Num <= pfs::numeric_limits<int_type>::max());
        PFS_ASSERT(Num > pfs::numeric_limits<int_type>::min());
    }

#if __cplusplus >= 201103L
    rational (rational const & rhs) = default;
    rational (rational && rhs) = default;
    rational & operator = (rational const & rhs) = default;
    rational & operator = (rational && rhs) = default;
    ~rational () = default;
#else
    rational (rational const & rhs) : _num(rhs._num), _den(rhs._den) {}
    rational & operator = (rational const & rhs)
    {
        _num = rhs._num;
        _den = rhs._den;
        return *this;
    }
    ~rational () {}
#endif

    rational & assign (int_type const & n, int_type const & d)
    {
        *this = rational(n, d);
        return *this;
    }

    pfs_constexpr IntT numerator () const
    {
        return _num;
    }

    pfs_constexpr IntT denominator () const
    {
        return _den;
    }

    // Unary minus
    rational operator - () const pfs_noexcept
    {
        rational result(*this);
        result._num = - result._num;
        return result;
    }

    // Unary plus
    rational operator + () const pfs_noexcept
    {
        return *this;
    }

    rational abs () const pfs_noexcept
    {
        if (_num < 0)
            return -*this;
        return *this;
    }

    /**
     * @return An equivalent fraction with all common factors between the
     *         numerator and the denominator removed.
     */
    rational reduce () const pfs_noexcept
    {
        int_type gcd = pfs::gcd(_num, _den);
        return rational(_num / gcd, _den / gcd);
    }

    /**
     * @return Result of fraction invert operation.
     */
    rational invert () const pfs_noexcept
    {
        PFS_ASSERT_ZERO_DIV(_num != int_type(0));
        return rational(sign_of(_num) * _den, pfs::abs(_num));
    }

    /**
     * @return @c true if the denominator is a power of ten, @c false otherwise.
     */
    bool is_decimal() const pfs_noexcept;

    string to_string (int radix = 10, string const & decimal_point = ".") const pfs_noexcept;

    //
    // Compare
    //
    int compare (rational const & rhs) const;

    bool operator == (rational const & rhs) const
    {
        return compare(rhs) == 0;
    }

    bool operator < (rational const & rhs) const
    {
        return compare(rhs) < 0;
    }

    //
    // Arithmetic operators
    //
    rational & operator += (rational const & rhs);
    rational & operator -= (rational const & rhs);
    rational & operator *= (rational const & rhs);
    rational & operator /= (rational const & rhs);

    rational & operator += (int_type const & i) { _num += i * _den; return *this; }
    rational & operator -= (int_type const & i) { _num -= i * _den; return *this; }
    rational & operator *= (int_type const & i);
    rational & operator /= (int_type const & i);
};

template <typename IntT>
bool rational<IntT>::is_decimal () const pfs_noexcept
{
    static const intmax_t * __poweroften = poweroften();
    static const uint8_t    __poweroften_size = poweroften_size();

    for (int i = 0; i < __poweroften_size && _den >= __poweroften[i]; ++i) {
        if (_den == __poweroften[i])
            return true;
        if (_den % __poweroften[i])
            return false;
    }
    return false;
}

template <typename IntT>
rational<IntT> & rational<IntT>::operator += (rational const & rhs)
{
    // This calculation avoids overflow, and minimises the number of expensive
    // calculations. Thanks to Nickolay Mladenov for this algorithm.
    //
    // Proof:
    // We have to compute a/b + c/d, where gcd(a,b)=1 and gcd(b,c)=1.
    // Let g = gcd(b,d), and b = b1*g, d=d1*g. Then gcd(b1,d1)=1
    //
    // The result is (a*d1 + c*b1) / (b1*d1*g).
    // Now we have to normalize this ratio.
    // Let's assume h | gcd((a*d1 + c*b1), (b1*d1*g)), and h > 1
    // If h | b1 then gcd(h,d1)=1 and hence h|(a*d1+c*b1) => h|a.
    // But since gcd(a,b1)=1 we have h=1.
    // Similarly h|d1 leads to h=1.
    // So we have that h | gcd((a*d1 + c*b1) , (b1*d1*g)) => h|g
    // Finally we have gcd((a*d1 + c*b1), (b1*d1*g)) = gcd((a*d1 + c*b1), g)
    // Which proves that instead of normalizing the result, it is better to
    // divide num and den by gcd((a*d1 + c*b1), g)

    // Protect against self-modification
    //int_type r_num = r._num;
    //int_type r_den = r._den;

    int_type r_num = rhs._num;
    int_type r_den = rhs._den;

    int_type g = pfs::gcd(_den, r_den);
    _den /= g;  // = b1 from the calculations above
    _num = _num * (r_den / g) + r_num * _den;
    g = pfs::gcd(_num, g);
    _num /= g;
    _den *= r_den/g;

    return *this;
}

template <typename IntT>
rational<IntT> & rational<IntT>::operator -= (rational const & rhs)
{
    // Protect against self-modification
    int_type r_num = rhs._num;
    int_type r_den = rhs._den;

    // This calculation avoids overflow, and minimises the number of expensive
    // calculations. It corresponds exactly to the += case above
    int_type g = pfs::gcd(_den, r_den);
    _den /= g;
    _num = _num * (r_den / g) - r_num * _den;
    g = pfs::gcd(_num, g);
    _num /= g;
    _den *= r_den/g;

    return *this;
}

template <typename IntT>
rational<IntT> & rational<IntT>::operator *= (rational const & rhs)
{
    // Protect against self-modification
    int_type r_num = rhs._num;
    int_type r_den = rhs._den;

    // Avoid overflow and preserve normalization
    int_type gcd1 = pfs::gcd(_num, r_den);
    int_type gcd2 = pfs::gcd(r_num, _den);
    _num = (_num/gcd1) * (r_num/gcd2);
    _den = (_den/gcd2) * (r_den/gcd1);
    return *this;
}

template <typename IntT>
rational<IntT> & rational<IntT>::operator /= (rational const & rhs)
{
    // Protect against self-modification
    int_type r_num = rhs._num;
    int_type r_den = rhs._den;

    // Avoid repeated construction
    int_type zero(0);

    PFS_ASSERT_ZERO_DIV(r_num != zero);

    if (_num == zero)
        return *this;

    // Avoid overflow and preserve normalization
    int_type gcd1 = pfs::gcd(_num, r_num);
    int_type gcd2 = pfs::gcd(r_den, _den);
    _num = (_num/gcd1) * (r_den/gcd2);
    _den = (_den/gcd2) * (r_num/gcd1);

    if (_den < zero) {
        _num = -_num;
        _den = -_den;
    }
    return *this;
}

template <typename IntT>
inline rational<IntT> & rational<IntT>::operator *= (IntT const & i)
{
    // Avoid overflow and preserve normalization
    int_type gcd = pfs::gcd(i, _den);
    _num *= i / gcd;
    _den /= gcd;

    return *this;
}

template <typename IntT>
rational<IntT> & rational<IntT>::operator /= (IntT const & i)
{
    // Avoid repeated construction
    int_type const zero(0);

    PFS_ASSERT_ZERO_DIV(i != zero);

    if (_num == zero) return *this;

    // Avoid overflow and preserve normalization
    int_type const gcd = pfs::gcd(_num, i);
    _num /= gcd;
    _den *= i / gcd;

    if (_den < zero) {
        _num = -_num;
        _den = -_den;
    }

    return *this;
}

namespace details {

#if PFS_HAVE_INT64
    typedef uint64_t multiply_param_type;
    typedef uint32_t multiply_part_type;
    inline pfs_constexpr uint64_t uint_lo(uint64_t x) { return x & 0xffffffff; }
    inline pfs_constexpr uint64_t uint_hi(uint64_t x) { return x >> 32; }
    static uint64_t const uint_carry = (static_cast<uint64_t>(1) << 32);
#else
    typedef uint32_t multiply_param_type;
    typedef uint16_t multiply_part_type;
    inline pfs_constexpr uint32_t uint_lo(uint32_t x) { return x & 0xffff; }
    inline pfs_constexpr uint32_t uint_hi(uint32_t x) { return x >> 16; }
    static uint32_t const uint_carry = (static_cast<uint32_t>(1) << 16);
#endif

inline void multiply (multiply_param_type a, multiply_param_type b
        , multiply_param_type & hi, multiply_param_type & lo)
{
    multiply_part_type al = uint_lo(a);
    multiply_part_type ah = uint_hi(a);
    multiply_part_type bl = uint_lo(b);
    multiply_part_type bh = uint_hi(b);

    multiply_param_type r0 = static_cast<multiply_param_type>(al) * bl;
    multiply_param_type r1 = static_cast<multiply_param_type>(al) * bh;
    multiply_param_type r2 = static_cast<multiply_param_type>(ah) * bl;
    multiply_param_type r3 = static_cast<multiply_param_type>(ah) * bh;

    r1 += uint_hi(r0);
    r1 += r2;

    if (r1 < r2)
        r3 += uint_carry;

    hi = r3 + uint_hi(r1);
    lo = (uint_lo(r1) << 32) + uint_lo(r0);
}

} // namespace details

template <typename IntT>
int rational<IntT>::compare (rational const & rhs) const
{
    if (_den == rhs._den) {
        return _num < rhs._num
            ? -1
            : _num > rhs._num
                ? 1 : 0;
    }

    int sign1 = sign_of(*this);
    int sign2 = sign_of(rhs);

    if (sign1 < sign2)
        return -1;

    if (sign2 < sign1)
        return 1;

    //
    // Bellow compared rationals has same signs
    //

    // Quotients
    int_type q1 = static_cast<int_type>(_num / _den);
    int_type q2 = static_cast<int_type>(rhs._num / rhs._den);

    // [6]
    if (q1 != q2)
        return q1 < q2 ? -1 : 1;

    if (sizeof(intmax_t) > sizeof(int_type)) {
        intmax_t m1 = static_cast<intmax_t>(_num) * rhs._den;
        intmax_t m2 = static_cast<intmax_t>(rhs._num) * _den;

        return m1 == m2
                ? 0
                : m1 < m2 ? -1 : 1;
    }

    //
    // Emulate comparison of big integers
    //
#if PFS_HAVE_INT64
    uint64_t a1 = pfs::abs(_num);
    uint64_t b1 = rhs._den;
    uint64_t a2 = pfs::abs(rhs._num);
    uint64_t b2 = _den;

    uint64_t lo1, hi1, lo2, hi2;
#else
    uint32_t a1 = pfs::abs(_num);
    uint32_t b1 = rhs._den;
    uint32_t a2 = pfs::abs(rhs._num);
    uint32_t b2 = _den;

    uint32_t lo1, hi1, lo2, hi2;
#endif

    details::multiply(a1, b1, lo1, hi1);
    details::multiply(a2, b2, lo2, hi2);

    if (hi1 == hi2)
        return lo1 == lo2
                ? 0
                : lo1 < lo2
                    ? -1 * sign1
                    : sign1;

    if (hi1 < hi2)
        return -1 * sign1;

    return sign1;
}

template <typename IntT>
string rational<IntT>::to_string (int radix
        , string const & decimal_point) const pfs_noexcept
{
    string result;

    if (_num % _den == 0) {
        result += pfs::to_string(_num / _den, radix);
    } else if (pfs::abs(_num) == _den) {
        result += pfs::to_string(sign_of(_num), radix);
    } else if (radix == 10 && is_decimal()) {
        result += pfs::to_string(_num / _den, radix);
        result += decimal_point;

        int_type x = _den / _num;

        while ((x /= 10) > 0)
            result += '0';

        result += pfs::to_string((_num > 0 ? _num : -_num) % _den, radix);
    } else {
        result += pfs::to_string(_num, radix);
        result += '/';
        result += pfs::to_string(_den, radix);
    }

    return result;
}

template <typename IntT>
inline rational<IntT> abs (rational<IntT> const & x)
{
    return x.abs();
}

template <typename IntT>
inline int sign_of (rational<IntT> const & x)
{
    return sign_of(x.numerator());
}

template <typename IntT>
inline string to_string (rational<IntT> const & r
        , int radix = 10
        , string const & decimal_point = ".")
{
    return r.to_string(radix, decimal_point);
}

template <typename IntT>
inline rational<IntT> operator + (rational<IntT> const & lhs, rational<IntT> const & rhs)
{
    rational<IntT> result(lhs);
    return result += rhs;
}

template <typename IntT>
inline rational<IntT> operator - (rational<IntT> const & lhs, rational<IntT> const & rhs)
{
    rational<IntT> result(lhs);
    return result -= rhs;
}

template <typename IntT>
inline rational<IntT> operator * (rational<IntT> const & lhs, rational<IntT> const & rhs)
{
    rational<IntT> result(lhs);
    return result *= rhs;
}

template <typename IntT>
inline rational<IntT> operator / (rational<IntT> const & lhs, rational<IntT> const & rhs)
{
    rational<IntT> result(lhs);
    return result /= rhs;
}

template <typename IntT>
inline rational<IntT> operator + (rational<IntT> const & lhs, typename rational<IntT>::int_type const & rhs)
{
    rational<IntT> result(lhs);
    return result += rhs;
}

template <typename IntT>
inline rational<IntT> operator - (rational<IntT> const & lhs, typename rational<IntT>::int_type const & rhs)
{
    rational<IntT> result(lhs);
    return result -= rhs;
}

template <typename IntT>
inline rational<IntT> operator * (rational<IntT> const & lhs, typename rational<IntT>::int_type const & rhs)
{
    rational<IntT> result(lhs);
    return result *= rhs;
}

template <typename IntT>
inline rational<IntT> operator / (rational<IntT> const & lhs, typename rational<IntT>::int_type const & rhs)
{
    rational<IntT> result(lhs);
    return result /= rhs;
}

template <typename IntT>
inline rational<IntT> operator + (typename rational<IntT>::int_type const & lhs, rational<IntT> const & rhs)
{
    return rational<IntT>(lhs) + rhs;
}

template <typename IntT>
inline rational<IntT> operator - (typename rational<IntT>::int_type const & lhs, rational<IntT> const & rhs)
{
    return rational<IntT>(lhs) - rhs;
}

template <typename IntT>
inline rational<IntT> operator * (typename rational<IntT>::int_type const & lhs, rational<IntT> const & rhs)
{
    return rational<IntT>(lhs) * rhs;
}

template <typename IntT>
inline rational<IntT> operator / (typename rational<IntT>::int_type const & lhs, rational<IntT> const & rhs)
{
    return rational<IntT>(lhs) / rhs;
}

} // namespace pfs