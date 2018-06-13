#pragma once
#include <pfs/utility.hpp>
#include <pfs/string.hpp>
#include <pfs/lexical_cast.hpp>
#include <pfs/algo/split.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>

namespace pfs {
namespace net {

/**
 * @brief Satisfies Concepts:
 *        @li CopyConstructible
 *        @li CopyAssignable
 */
class inet4_addr
{
public:
    static const uint32_t invalid_addr_value = 0xFFFFFFFF;
    static const uint32_t any_addr_value     = 0x00000000;

private:
    uint32_t _addr;

public:
    /**
     * @brief Constructs invalid @c inet4_addr instance.
     */
    inet4_addr ()
        : _addr(invalid_addr_value)
    {}

    /**
     * @brief Copy constructor.
     */
    inet4_addr (inet4_addr const & x)
        : _addr(x._addr)
    {}

    /**
     * @brief Copy assignment operator.
     */
    inet4_addr & operator = (inet4_addr const & x)
    {
        _addr = x._addr;
        return *this;
    }

    /**
     * @brief Constructs inet4_addr from four numeric parts.
     *
     * @details Each of the four numeric parts specifies a byte of the address;
     *          the bytes are assigned in left-to-right order to produce the binary address.
     *
     * @param a First numeric part.
     * @param b Second numeric part.
     * @param c Third numeric part.
     * @param d Fourth numeric part.
     */
    inet4_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d)
        : _addr(0)
    {
        _addr |= (static_cast<uint32_t>(a) << 24);
        _addr |= (static_cast<uint32_t>(b) << 16);
        _addr |= (static_cast<uint32_t>(c) << 8);
        _addr |= static_cast<uint32_t>(d);
    }


    /**
     * @brief Constructs inet4_addr from three numeric parts.
     *
     * @details Parts @a a and @a b specify the first two bytes of the binary address.
     *          Part @a c is interpreted as a 16-bit value that defines the rightmost
     *          two bytes of the binary address. This notation is suitable
     *          for specifying (outmoded) Class B network addresses.
     *
     * @param a First numeric part
     * @param b Second numeric part
     * @param c Third numeric parts.
     */
    inet4_addr (uint8_t a, uint8_t b, uint16_t c)
        : _addr(0)
    {
        _addr |= (static_cast<uint32_t>(a) << 24);
        _addr |= (static_cast<uint32_t>(b) << 16);
        _addr |= static_cast<uint32_t>(c);
    }

    /**
     * @brief Constructs inet4_addr from two numeric parts.
     *
     * @details Part @a a specifies the first byte of the binary address.
     *          Part @a b is interpreted as a 24-bit value that defines
     *          the rightmost three bytes of the binary address.
     *          This notation is suitable for specifying (outmoded)
     *          Class C network addresses.
     *
     * @param a First numeric part
     * @param b Second numeric part
     */
    inet4_addr (uint8_t a, uint32_t b)
        : _addr(0)
    {
        _addr |= (static_cast<uint32_t>(a) << 24);
        _addr |= b;
    }


    /**
     * @brief Constructs inet4_addr from one numeric part.
     *
     * @param a Numeric part.
     */
    inet4_addr (uint32_t a)
        : _addr(a)
    {}

    /**
     * @brief Constructs inet4_addr from string representing the IPv4 address.
     *
     * @details @a s must be in one of the format:
     *             (1) a.b.c.d
     *             (2) a.b.C
     *             (3) a.B
     *             (4) A
     *
     * @param s String representing the IPv4 address.
     */
    inet4_addr (string const & s)
        : _addr(invalid_addr_value)
    {
        stringlist<string> sl;
        string separator(".");

        if (s.empty())
            return;

        sl.split(s, separator, true);

        if (sl.size() > 4)
            return;

        switch (sl.size()) {
        case 1: {
            uint32_t A = 0;

           typename stringlist<string>::const_iterator it0 = sl.cbegin();

            if (parse_part(A, 0xFFFFFFFF, it0->cbegin(), it0->cend())) {
                inet4_addr other(A);
                this->swap(other);
            }

            break;
        }

        case 2: {
            uint32_t a = 0;
            uint32_t B = 0;

            typename stringlist<string>::const_iterator it0 = sl.cbegin();
            typename stringlist<string>::const_iterator it1 = it0;

            ++it1;

            if (parse_part(a, 0xFF, it0->cbegin(), it0->cend())
                    && parse_part(B, 0x00FFFFFF, it1->cbegin(), it1->cend())) {

                inet4_addr other(static_cast<uint8_t>(a), B);
                this->swap(other);
            }

            break;
        }

        case 3: {
            uint32_t a = 0;
            uint32_t b = 0;
            uint32_t C = 0;

            typename stringlist<string>::const_iterator it0 = sl.cbegin();
            typename stringlist<string>::const_iterator it1 = it0;
            typename stringlist<string>::const_iterator it2 = it0;

            ++it1;
            ++(++it2);

            if (parse_part(a, 0xFF, it0->cbegin(), it0->cend())
                    && parse_part(b, 0xFF, it1->cbegin(), it1->cend())
                    && parse_part(C, 0x0000FFFF, it2->cbegin(), it2->cend())) {

                inet4_addr other(static_cast<uint8_t>(a)
                        , static_cast<uint8_t>(b)
                        , static_cast<uint16_t>(C));
                this->swap(other);
            }

            break;
        }

        case 4: {
            uint32_t a = 0;
            uint32_t b = 0;
            uint32_t c = 0;
            uint32_t d = 0;

            typename stringlist<string>::const_iterator it0 = sl.cbegin();
            typename stringlist<string>::const_iterator it1 = it0;
            typename stringlist<string>::const_iterator it2 = it0;
            typename stringlist<string>::const_iterator it3 = it0;

            ++it1;
            ++(++it2);
            ++(++(++it3));

            if (parse_part(a, 0xFF, it0->cbegin(), it0->cend())
                    && parse_part(b, 0xFF, it1->cbegin(), it1->cend())
                    && parse_part(c, 0xFF, it2->cbegin(), it2->cend())
                    && parse_part(d, 0xFF, it3->cbegin(), it3->cend())) {

                inet4_addr other(static_cast<uint8_t>(a)
                        , static_cast<uint8_t>(b)
                        , static_cast<uint8_t>(c)
                        , static_cast<uint8_t>(d));

                this->swap(other);
            }

            break;
        }

        default:
            break;
        }
    }

    /**
     * @brief Returns @c true if inet4_addr represents a valid IPv4 address.
     */
    operator bool () const
    {
        return _addr != invalid_addr_value;
    }

    uint32_t native () const
    {
        return _addr;
    }

    void swap (inet4_addr & other)
    {
        ::pfs::swap(_addr, other._addr);
    }

    /**
    * @brief Converts IPv4 address to string.
    *
    * @details The format specification is a null-terminated string and may
    *      contain special character sequences called conversion specifications,
    *      each of which is introduced by a '%' character and terminated by
    *      some other character known as a conversion specifier character.
    *      All other character sequences are ordinary character sequences.
    *
    *      The characters of ordinary character sequences (including the null byte)
    *      are copied verbatim from format to resulting string. However,
    *      the characters of conversion specifications are replaced as follows:
    *
    *      @li %a First byte of IPv4 address.
    *      @li %A The full 32-bit representation of IPv4 address.
    *      @li %b Second byte of IPv4 address.
    *      @li %B 24-bit representation of last part of IPv4 address
    *      @li %c Third byte of IPv4 address.
    *      @li %C 16-bit representation of last part of IPv4 address
    *      @li %d Fourth byte of IPv4 address.
    *      @li %% '%' character.
    *
    * @param addr Source address for conversion.
    * @param format Conversion format string. If @a format is empty the default format
    *      will be used: "%a.%b.%c.%d".
    * @param base Specifies the radix (8, 10 or 16) for output address parts.
    * @return String representation of IPv4 address. If @a addr is not valid
    *      result will be an empty string.
    *
    * @note 192.0.2.235           decimal
    *       0xC0.0x00.0x02.0xEB   hex
    *       0300.0000.0002.0353   octal
    *       0xC00002EB            decimal
    *       3221226219            hex
    *       030000001353          octal
    *
    *
    */
    string to_string (string const & format, int base) const
    {
        typedef string::value_type char_type;
        static const string __default_format("%a.%b.%c.%d");

        if (!_addr == invalid_addr_value)
            return string();

        string r;
        string const * f = 0;

        if (format.empty()) {
            f = & __default_format;
        } else {
            f = & format;
        }

        if (not (base == 10 || base == 8 || base == 16)) {
            base = 10;
        }

        string::const_iterator it = f->cbegin();
        string::const_iterator it_end = f->cend();

        bool uppercase = true;

        string A = pfs::to_string(_addr, base, uppercase);
        string B = pfs::to_string(0x00FFFFFF & _addr, base, uppercase);
        string C = pfs::to_string(0x0000FFFF & _addr, base, uppercase);
        string a = pfs::to_string(static_cast<uint8_t>(0x000000FF & (_addr >> 24)), base, uppercase);
        string b = pfs::to_string(static_cast<uint8_t>(0x000000FF & (_addr >> 16)), base, uppercase);
        string c = pfs::to_string(static_cast<uint8_t>(0x000000FF & (_addr >> 8)), base, uppercase);
        string d = pfs::to_string(static_cast<uint8_t>(0x000000FF & _addr), base, uppercase);

        while (it != it_end) {
            if (*it == char_type('%')) {
                ++it;
                if (it == it_end) {
                    r.push_back(char_type('%'));
                    break;
                }

                if (*it == char_type('a')) {
                    append_number_prefix(r, a, base);
                    r.append(a);
                } else if (*it == char_type('b')) {
                    append_number_prefix(r, b, base);
                    r.append(b);
                } else if (*it == char_type('c')) {
                    append_number_prefix(r, c, base);
                    r.append(c);
                } else if (*it == char_type('d')) {
                    append_number_prefix(r, d, base);
                    r.append(d);
                } else if (*it == char_type('A')) {
                    append_number_prefix(r, base);
                    r.append(A);
                } else if (*it == char_type('B')) {
                    append_number_prefix(r, base);
                    r.append(B);
                } else if (*it == char_type('C')) {
                    append_number_prefix(r, base);
                    r.append(C);
                } else {
                    r.push_back(*it);
                }
            } else {
                r.push_back(*it);
            }

            ++it;
        }

        return r;
    }

public:
    // TODO DEPRECATED (use net::uri class instead)
    /**
     * @brief Parses string representing TCP/UDP address in format @c proto://inet4_addr:port.
     *
     * @param s String in format @c proto://inet4_addr:port, where proto must be equal to 'tcp' or 'udp'.
     * @param proto Reference to store resulting proto.
     * @param ip Reference to store resulting inet4 address.
     * @param port Reference to store resulting port.
     * @return @c true on successful parsing, @c false otherwise.
     */
    static bool parse (string const & s
            , string * proto
            , inet4_addr * ip
            , uint16_t * port)
    {
        stringlist<string> sl;
        sl.split(s, string("://"), true);

        if (sl.size() != 2)
            return false;

//        if (! (sl[0] == "tcp" || sl[0] == "udp"))
//            return false;

        typename stringlist<string>::const_iterator it = sl.cbegin();

        if (proto)
            *proto = *it;

        ++it;
        string tail = *it;
        sl.clear();

        sl.split(tail, string(":"), true);

        if (sl.size() != 2)
            return false;

        it = sl.cbegin();

        if (ip) {
            *ip = pfs::net::inet4_addr(*it);

            if (! *ip)
                return false;
        }

        ++it;

        if (port) {
            try {
                *port = ::pfs::lexical_cast<uint16_t>(*it, 0);
            } catch (bad_lexical_cast) {
                return false;
            }
        }

        return true;
    }

private:
    static bool parse_part (uint32_t & result
            , uint32_t maxvalue
            , string::const_iterator begin
            , string::const_iterator end)
    {
        uint32_t r = 0;

        try {
            r = lexical_cast<uint32_t>(string(begin, end), 0);
        } catch (bad_lexical_cast) {
            return false;
        }

        if (!(r <= maxvalue))
            return false;

        result = r;
        return true;
    }

    void append_number_prefix (string & r, string const & a, int base) const
    {
        if (base == 16) {
            size_t len = a.length();
            r.append("0x");

            if (len == 1)
                r.push_back('0');
        } else if (base == 8) {
            size_t len = a.length();
            r.append("0");

            if (len < 3)
                r.push_back('0');

            if (len < 2)
                r.push_back('0');
        }
    }

    void append_number_prefix (string & r, int base) const
    {
        if (base == 16)
            r.append("0x");
        else if (base == 8)
            r.append("0");
    }
};

} // net

inline string to_string (net::inet4_addr const & addr
        , string const & format
        , int base)
{
    return addr.to_string(format, base);
}

/**
 * @brief Converts IPv4 address to string with base 10.
 * @param addr Source address for conversion.
 * @param format Conversion format string. If @a format is empty the default format
 *      will be used: "%a.%b.%c.%d".
 * @return String representation of IPv4 address. If @a addr is not valid
 *      result will be an empty string.
 */
inline string to_string (net::inet4_addr const & addr
        , string const & format)
{
    return addr.to_string(format, 10);
}

/**
 * @brief Converts IPv4 address to string with format "%a.%b.%c.%d" and base 10.
 *
 * @param addr Source address for conversion.
 * @return String representation of IPv4 address. If @a addr is not valid
 *         result will be an empty string.
 */
inline string to_string (net::inet4_addr const & addr)
{
    return addr.to_string(string(), 10);
}

} // pfs
