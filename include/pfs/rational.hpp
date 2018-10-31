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
        int_type den = pfs::abs(_num);
        _num = sign_of(_num) * _den;
        _den = den;
    }

    /**
     * @return @c true if the denominator is a power of ten, @c false otherwise.
     */
    bool is_decimal() const pfs_noexcept;

    string to_string (int radix = 10, string const & decimal_point = ".") const pfs_noexcept;

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

    int compare (rational const & s) const;
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

//
// see [Euclidian division](https://wikipedia.org/wiki/Euclidian_division)
// see [Division algorithm](https://wikipedia.org/wiki/Division_algorithm)
//
// Euclidian theorem:
//
// [1] n = d * q + r;
// [2] 0 <= r < |d|
//
// [3]<=[1] n / d = q + r / d
// [4]<=[3] n1 / d1 ? n2 / d2 => (q1 + r1 / d1) ? (q2 + r2 / d2)
// [5]<=[2] |r / d| < 1
//
// For positive numbers:
//
// [6] if q1 != q2
//     then n1 / d1 < n2 / d2 if q1 < q2
//     or   n1 / d1 > n2 / d2 if q1 > q2
//
// [7] if q1 == q2
//     then n1 / d1 < n2 / d2  if (r1 / d1) <  (r2 / d2)
//     or   n1 / d1 > n2 / d2  if (r1 / d1) >  (r2 / d2)
//     or   n1 / d1 == n2 / d2 if (r1 / d1) == (r2 / d2)
//
// [7'] if q1 == q2
//     then n1 / d1 < n2 / d2  if (r1 * d2) <  (r2 * d1)
//     or   n1 / d1 > n2 / d2  if (r1 * d2) >  (r2 * d1)
//     or   n1 / d1 == n2 / d2 if (r1 * d2) == (r2 * d1)

template <typename IntT>
int rational<IntT>::compare (rational const & rhs) const
{
    if (_den == rhs._den) {
        return _num < rhs._num
            ? -1
            : _num > rhs._num
                ? 1 : 0;
    }

    int a_sign = sign_of(*this);
    int b_sign = sign_of(rhs);

    if (a_sign < b_sign)
        return -1;

    if (b_sign < a_sign)
        return 1;

    //
    // Signs are same further
    //

    // Quotients
    int_type q1 = static_cast<int_type>(_num / _den);
    int_type q2 = static_cast<int_type>(rhs._num / rhs._den);

    // [6]
    if (q1 != q2)
        return q1 < q2 ? -1 : 1;

    // Remainders
    int_type r1 = static_cast<int_type>(_num % _den);
    int_type r2 = static_cast<int_type>(rhs._num % rhs._den);

    //
    // Now compare r / d
    //

    bool over1 = numeric_limits<int_type>::max() / b._den < a._num;
    bool over2 = numeric_limits<int_type>::max() / a._den < b._num;

//    unsigned reverse = 0u;
//
//     rational a(*this);
//     rational b(rhs);
//
//     a.reduce();
//     b.reduce();
//
//     if (a._num == b._num && a._den == b._den)
//         return 0;
//
//     //
//     // Now only < or >
//     //
//
//     if (a._den == b._den)
//         return a._num < b._num ? -1 : 1;
//
//     int a_sign = sign_of(a);
//     int b_sign = sign_of(b);
//
//     if (a_sign < b_sign)
//         return -1;
//
//     if (b_sign < a_sign)
//         return 1;
//
//     a = a.abs();
//     b = b.abs();
//
//     bool aover = numeric_limits<int_type>::max() / b._den < a._num;
//     bool bover = numeric_limits<int_type>::max() / a._den < b._num;
//
//     // Will no overflow
//     if (!aover && !bover)
//         return a_sign * a._num * b._den < b_sign * b._num * a._den ? -1 : 1;
//
//     // One part is overflow (left)
//     if (aover && !bover)
//         return a_sign < b_sign ? -1 : 1;
//
//     // One part is overflow (right)
//     if (!aover && bover)
//         return a_sign < b_sign ? 1 : -1;
//
//     // All indirect comparisons are exhausted.
//     // Now emulate comparison of big integers (128)
//     PFS_ASSERT_T(sizeof(int_type) <= 8);


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

} // namespace pfs
