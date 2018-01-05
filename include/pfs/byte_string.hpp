#ifndef __PFS_BYTE_STRING_HPP__
#define __PFS_BYTE_STRING_HPP__

#include <string>
#include <pfs/limits.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/endian.hpp>
#include <pfs/memory.hpp>
#include <pfs/algorithm.hpp>
#include <pfs/iterator.hpp>
#include <pfs/utility.hpp>
#include <pfs/io/exception.hpp>
#include <pfs/traits/binary_stream.hpp>
#include <pfs/compiler.hpp>

#define NOT_ENOUGH_DATA_EXCEPTION_STR "not enough data"

namespace pfs {

class byte_string
{
    typedef std::basic_string<uint8_t> rep_type;

public:
    typedef uint8_t value_type;
    typedef typename rep_type::traits_type traits_type;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;

    typedef pfs::shared_ptr<byte_string> shared_type;

    static const size_type npos = rep_type::npos;
private:
    rep_type _d;

    explicit byte_string (const rep_type & d) : _d (d)
    {
    }

    byte_string & assign (const rep_type & d)
    {
        _d.assign(d);
        return *this;
    }

public:

    byte_string () : _d ()
    {
    }

    byte_string (byte_string const & other)
    : _d (other._d)
    {
    }

    byte_string (byte_string const & other,
            size_type pos,
            size_type count = npos)
    : _d (other._d, pos, count)
    {
    }

    explicit byte_string (const_pointer s)
    : _d (s)
    {
    }

    byte_string (const_pointer s, size_type n)
    : _d (s, n)
    {
    }

    explicit byte_string (const char * s)
    : _d (reinterpret_cast<const uint8_t *> (s))
    {
    }

    byte_string (const char * s, size_type n)
    : _d (reinterpret_cast<const uint8_t *> (s), n)
    {
    }

    byte_string (size_type n, uint8_t c)
    : _d (n, c)
    {
    }

    byte_string (size_type n, char c)
    : _d (n, static_cast<uint8_t> (c))
    {
    }

    template <typename InputIterator>
    byte_string (InputIterator first, InputIterator last)
    : _d (first, last)
    {
    }

    ~byte_string ()
    {
    }

    byte_string & operator= (byte_string const & s)
    {
        _d = s._d;
        return *this;
    }

    byte_string & operator= (const_pointer s)
    {
        _d = s;
        return *this;
    }

    byte_string & operator= (value_type ch)
    {
        _d = ch;
        return *this;
    }

    byte_string & assign (size_type count, value_type ch)
    {
        _d.assign(count, ch);
        return *this;
    }

    byte_string & assign (byte_string const & s
            , size_type pos
            , size_type count)
    {
        _d.assign(s._d, pos, count);
        return *this;
    }

    byte_string & assign (byte_string const & s)
    {
        return assign(s, 0, npos);
    }

    byte_string & assign (byte_string const & s, size_type pos)
    {
        return assign(s, pos, npos);
    }

    byte_string & assign (const_pointer s, size_type count)
    {
        _d.assign(s, count);
        return *this;
    }

    byte_string & assign (const_pointer s)
    {
        _d.assign(s);
        return *this;
    }

    template <typename InputIterator>
    byte_string & assign (InputIterator first, InputIterator last)
    {
        _d.assign<InputIterator>(first, last);
        return *this;
    }

    reference at (size_type pos)
    {
        return _d.at(pos);
    }

    const_reference at (size_type pos) const
    {
        return _d.at(pos);
    }

    reference operator[] (size_type pos)
    {
        return _d.operator[] (pos);
    }

    const_reference operator[] (size_type pos) const
    {
        return _d.operator[] (pos);
    }

    reference front ()
    {
        PFS_ASSERT(!empty());
#if __cplusplus < 201103L
        return at(0);
#else
        return _d.front();
#endif
    }

    const_reference front () const
    {
        PFS_ASSERT(!empty());
#if __cplusplus < 201103L
        return at(0);
#else
        return _d.front();
#endif
    }

    reference back ()
    {
        PFS_ASSERT(!empty());
#if __cplusplus < 201103L
        return at(size() - 1);
#else
        return _d.back();
#endif
    }

    const_reference back () const
    {
        PFS_ASSERT(!empty());
#if __cplusplus < 201103L
        return at(size() - 1);
#else
        return _d.back();
#endif
    }

    const_pointer data () const
    {
        return _d.data();
    }

    const char * c_str () const
    {
        return reinterpret_cast<const char *> (_d.c_str());
    }

    iterator begin ()
    {
        return _d.begin();
    }

    const_iterator begin () const
    {
        return _d.begin();
    }

    const_iterator cbegin () const
    {
        return begin();
    }

    iterator end ()
    {
        return _d.end();
    }

    const_iterator end () const
    {
        return _d.end();
    }

    const_iterator cend () const
    {
        return end();
    }

    reverse_iterator rbegin ()
    {
        return _d.rbegin();
    }

    const_reverse_iterator rbegin () const
    {
        return _d.rbegin();
    }

    const_reverse_iterator crbegin () const
    {
#if __cplusplus < 201103L
        return _d.rbegin();
#else
        return _d.crbegin();
#endif
    }

    reverse_iterator rend ()
    {
        return _d.rend();
    }

    const_reverse_iterator rend () const
    {
        return _d.rend();
    }

    const_reverse_iterator crend () const
    {
#if __cplusplus < 201103L
        return _d.rend();
#else
        return _d.crend();
#endif
    }

    /**
     * @brief Checks if byte string is empty.
     *
     * @return @c true if byte string is empty (size() == 0) or @c false otherwise.
     */
    bool empty () const
    {
        return _d.empty();
    }

    /**
     * @brief Return size of string in code units.
     *
     * @return String in code units.
     */
    size_type size () const
    {
        return _d.size();
    }

    /**
     * @brief Return size of byte string.
     *
     * @return Size of byte string.
     * @see size()
     */
    size_type length () const
    {
        return _d.length();
    }

    size_type max_size () const
    {
        return _d.max_size();
    }

    void reserve (size_type new_cap = 0)
    {
        _d.reserve(new_cap);
    }

    size_type capacity () const
    {
        return _d.capacity();
    }

    void shrink_to_fit ()
    {
#if __cplusplus < 201103L
        if (capacity() > size()) {
            reserve(0);
        }
#else
        _d.shrink_to_fit();
#endif
    }

    void clear ()
    {
        _d.clear();
    }

    /**
     * @brief Inserts @c count copies of character @c ch at the position @c index.
     * @param index Position at which the content will be inserted.
     * @param count Number of characters to insert.
     * @param ch Character to insert.
     * @return @code *this @endcode
     */
    byte_string & insert (size_type index, size_type count, value_type ch)
    {
        PFS_ASSERT(index <= size());
        PFS_ASSERT(max_size() - count >= size());
        _d.insert(index, count, ch);
        return *this;
    }

    /**
     * @brief Inserts @c count copies of character @c ch at the position @c index.
     * @param index Position at which the content will be inserted.
     * @param count Number of characters to insert.
     * @param ch Character to insert.
     * @return @code *this @endcode
     */
    byte_string & insert (size_type index, size_type count, char ch)
    {
        return insert(index, count, static_cast<value_type> (ch));
    }

    /**
     * @brief Inserts null-terminated character string pointed to by @c s at the position @c index.
     *        The length of the string is determined by the first null character.
     * @param index Position at which the content will be inserted.
     * @param s Pointer to the character string to insert.
     * @return @code *this @endcode
     */
    byte_string & insert (size_type index, const_pointer s)
    {
        PFS_ASSERT(index <= size());
        PFS_ASSERT(max_size() - traits_type::length(s) >= size());
        _d.insert(index, s);
        return *this;
    }

    /**
     * @brief Inserts null-terminated character string pointed to by @c s at the position @c index.
     *        The length of the string is determined by the first null character.
     * @param index Position at which the content will be inserted.
     * @param s Pointer to the character string to insert.
     * @return @code *this @endcode
     */
    byte_string & insert (size_type index, const char * s)
    {
        return insert(index, reinterpret_cast<const_pointer> (s));
    }

    byte_string & insert (size_type index, const_pointer s, size_type count)
    {
        _d.insert(index, s, count);
        return *this;
    }

    byte_string & insert (size_type index, byte_string const & s)
    {
        _d.insert(index, s._d);
        return *this;
    }

    byte_string & insert (size_type index, byte_string const & s
            , size_type index_str, size_type count = npos)
    {
        _d.insert(index, s._d, index_str, count);
        return *this;
    }

    // FIXME
    //	iterator insert (const_iterator pos, value_type ch)
    //	{
    //#if __cplusplus < 201103L
    //		size_type index = pos - cbegin();
    //		insert(index, ch);
    //		return iterator(begin() + index);
    //#else
    //		return _d.insert(pos, ch);
    //#endif
    //	}

    // FIXME
    //	iterator insert (const_iterator pos, size_type count, value_type ch)
    //	{
    //#if __cplusplus < 201103L
    //		iterator it(begin());
    //		std::advance(it, std::distance<const_iterator>(begin(), pos));
    //		_d.insert(it, count, ch);
    //		it = begin();
    //		std::advance(it, std::distance<const_iterator>(begin(), pos));
    //		return it;
    //#else
    //		return _d.insert(pos, count, ch);
    //#endif
    //	}

    template <typename InputIterator>
    iterator insert (const_iterator pos, InputIterator first, InputIterator last)
    {
        return _d.insert<InputIterator>(pos, first, last);
    }

    byte_string & erase (size_type index = 0, size_type count = npos)
    {
        PFS_ASSERT(index <= size());
        _d.erase(index, count);
        return *this;
    }

    iterator erase (const_iterator position)
    {
#if __cplusplus < 201103L
// || PFS_CC_GCC_VERSION <= 40800 // TODO Check for valid version
        size_type index = position - cbegin();
        if (index < size()) {
            erase(index, 1);
            return begin() + index;
        }
        return end();
#else
        return _d.erase(position);
#endif
    }

    iterator erase (const_iterator first, const_iterator last)
    {
#if __cplusplus < 201103L
// || PFS_CC_GCC_VERSION <= 40800 // TODO Check for valid version
        size_type pos1 = first - cbegin();
        size_type pos2 = last - cbegin();
        if (pos1 < pos2) {
            erase(pos1, pos2 - pos1);
            return begin() + pos1;
        }
        return end();
#else
        return _d.erase(first, last);
#endif
    }

    void push_back (value_type ch)
    {
        _d.push_back(ch);
    }

    void pop_back ()
    {
#if __cplusplus < 201103L
        if (!empty())
            erase(size() - 1, 1);
#else
        _d.pop_back();
#endif
    }

    byte_string & append (size_type count, value_type ch)
    {
        _d.append(count, ch);
        return *this;
    }

    byte_string & append (size_type count, char ch)
    {
        return append(count, static_cast<value_type> (ch));
    }

    byte_string & append (byte_string const & s)
    {
        _d.append(s._d);
        return *this;
    }

    byte_string & append (byte_string const & s
            , size_type pos
            , size_type count = npos)
    {
        _d.append(s._d, pos, count);
        return *this;
    }

    byte_string & append (const_pointer s, size_type count)
    {
        _d.append(s, count);
        return *this;
    }

    byte_string & append (const char * s, size_type count)
    {
        return append(reinterpret_cast<const_pointer> (s), count);
    }

    byte_string & append (const_pointer s)
    {
        _d.append(s);
        return *this;
    }

    byte_string & append (const char * s)
    {
        return append(reinterpret_cast<const_pointer> (s));
    }

    template <typename InputIterator>
    byte_string & append (InputIterator first, InputIterator last)
    {
        _d.append<InputIterator>(first, last);
        return *this;
    }

    byte_string & operator+= (byte_string const & s)
    {
        _d.operator+= (s._d);
        return *this;
    }

    byte_string & operator+= (value_type ch)
    {
        _d.operator+= (ch);
        return *this;
    }

    byte_string & operator+= (char ch)
    {
        return operator+= (static_cast<value_type> (ch));
    }

    byte_string & operator+= (const_pointer s)
    {
        _d.operator+= (s);
        return *this;
    }

    byte_string & operator+= (const char * s)
    {
        return operator+= (reinterpret_cast<const_pointer> (s));
    }

    int compare (byte_string const & s) const
    {
        return _d.compare(s._d);
    }

    int compare (size_type pos1, size_type count1, byte_string const & s) const
    {
        return _d.compare(pos1, count1, s._d);
    }

    int compare (size_type pos1, size_type count1
            , byte_string const & s
            , size_type pos2, size_type count2 = npos) const
    {
        return _d.compare(pos1, count1, s._d, pos2, count2);
    }

    int compare (const_pointer s) const
    {
        return _d.compare(s);
    }

    int compare (const char * s) const
    {
        return compare(reinterpret_cast<const_pointer> (s));
    }

    int compare (size_type pos1, size_type count1, const_pointer s) const
    {
        return _d.compare(pos1, count1, s);
    }

    int compare (size_type pos1, size_type count1, const char * s) const
    {
        return compare(pos1, count1, reinterpret_cast<const_pointer> (s));
    }

    int compare (size_type pos1, size_type count1, const_pointer s, size_type count2) const
    {
        return _d.compare(pos1, count1, s, count2);
    }

    int compare (size_type pos1, size_type count1, const char * s, size_type count2) const
    {
        return compare(pos1, count1, reinterpret_cast<const_pointer> (s), count2);
    }

    byte_string & replace (size_type pos, size_type count, byte_string const & s)
    {
        _d.replace(pos, count, s._d);
        return *this;
    }

    //	byte_string & replace (const_iterator first, const_iterator last, byte_string const & s)
    //	{
    //		_d.replace(first, last, s._d);
    //		return *this;
    //	}

    byte_string & replace (size_type pos, size_type count
            , byte_string const & s
            , size_type pos2, size_type count2 = npos)
    {
        _d.replace(pos, count, s._d, pos2, count2);
        return *this;
    }

    template <typename InputIterator>
    byte_string & replace (const_iterator first, const_iterator last
            , InputIterator first2, InputIterator last2)
    {
        _d.replace<InputIterator>(first, last, first2, last2);
        return *this;
    }

    byte_string & replace (size_type pos, size_type count, const_pointer s, size_type count2)
    {
        _d.replace(pos, count, s, count2);
        return *this;
    }

    //	byte_string & replace (const_iterator first, const_iterator last, const_pointer s, size_type count2)
    //	{
    //		_d.replace(first, last, s, count2);
    //		return *this;
    //	}

    byte_string & replace (size_type pos, size_type count, const_pointer s)
    {
        _d.replace(pos, count, s);
        return *this;
    }

    // FIXME
    //	byte_string & replace (const_iterator first, const_iterator last, const_pointer s)
    //	{
    //		_d.replace(first, last, s);
    //		return *this;
    //	}

    byte_string & replace (size_type pos, size_type count, size_type count2, value_type ch)
    {
        _d.replace(pos, count, count2, ch);
        return *this;
    }

    // FIXME
    //	byte_string & replace (const_iterator first, const_iterator last, size_type count2, value_type ch)
    //	{
    //		_d.replace(first, last, count2, ch);
    //		return *this;
    //	}

    byte_string substr (size_type pos = 0, size_type count = npos) const
    {
        PFS_ASSERT(pos <= size());
        return byte_string(_d.substr(pos, count));
    }

    byte_string & substr (byte_string & result, size_type pos = 0, size_type count = npos) const
    {
        PFS_ASSERT(pos <= size());
        return result.assign(_d.substr(pos, count));
    }

    size_type copy (pointer dest, size_type count, size_type pos = 0) const
    {
        return _d.copy(dest, count, pos);
    }

    size_type copy (char * dest, size_type count, size_type pos = 0) const
    {
        return copy(reinterpret_cast<pointer> (dest), count, pos);
    }

    void resize (size_type count)
    {
        _d.resize(count);
    }

    void resize (size_type count, value_type ch)
    {
        _d.resize(count, ch);
    }

    void swap (byte_string & other)
    {
        _d.swap(other._d);
    }
};

template <int N>
struct byte_string_ref_n
{
    typedef typename size_type_n<N>::type size_type;
    byte_string * p;

    byte_string_ref_n (byte_string * ptr)
    : p (ptr)
    {
    }

    size_type max_size () const
    {
        return numeric_limits<size_type>::max();
    }
};

struct byte_string_ref
{
    byte_string * p;
    byte_string::size_type max_size;

    byte_string_ref (byte_string * ptr)
    : p (ptr)
    , max_size (ptr->size ())
    {
    }

    byte_string_ref (byte_string * ptr, byte_string::size_type sz)
    : p (ptr)
    , max_size (sz)
    {
    }
};

template <typename T>
struct buffer_wrapper
{
    T * p;
    size_t max_size;

    buffer_wrapper (T * ptr, byte_string::size_type n)
    : p (ptr)
    , max_size (n)
    {
    }
};

class byte_ostream
{
    byte_string _buffer;
    endian _o;

public:

    byte_ostream (endian const & order = endian::native_order ())
    : _o (order)
    {
    }

    endian const & order () const
    {
        return _o;
    }

    byte_string const & data () const
    {
        return _buffer;
    }

    template <typename Integral>
    void write_integral (Integral v)
    {
        Integral a = _o.convert(v);

        union
        {
            Integral v;
            byte_string::value_type b[sizeof (Integral)];
        } u;
        u.v = a;
        _buffer.append(byte_string(u.b, sizeof (Integral)));
    }

    template <typename Real>
    void write_real (Real v)
    {
#ifdef PFS_HAVE_INT64
        if (sizeof (Real) == 8) {

            union
            {
                uint64_t i;
                real64_t f;
            } u;
            u.f = v;
            return write_integral(u.i);
        } else
#endif
            if (sizeof (Real) == 4) {

            union
            {
                uint32_t i;
                real32_t f;
            } u;
            u.f = v;
            return write_integral(u.i);
        } else {

            union
            {
                Real v;
                byte_string::value_type b[sizeof (Real)];
            } d;

            if (_o != endian::native_order()) {
                byte_string::value_type b[sizeof (Real)];

                for (int i = 0, j = sizeof (Real) - 1; j >= 0; ++i, --j) {
                    b[i] = d.b[j];
                }

                _buffer.append(byte_string(b, sizeof (Real)));
            } else {
                _buffer.append(byte_string(d.b, sizeof (Real)));
            }
        }
    }

    void write (byte_string const & v, byte_string::size_type n = byte_string::npos)
    {
        _buffer.append(v, 0, n);
    }

    void write (byte_string::const_pointer v, byte_string::size_type n = byte_string::npos)
    {
        _buffer.append(v, n);
    }

    void write (char const * v, byte_string::size_type n = byte_string::npos)
    {
        _buffer.append(v, n);
    }
};

class byte_istream
{
    byte_string::const_iterator b;
    byte_string::const_iterator e;
    endian o;

public:

    byte_istream (byte_string::const_iterator begin
            , byte_string::const_iterator end
            , endian const & order = endian::native_order ())
    : b (begin)
    , e (end)
    , o (order)
    {
    }

    byte_string::const_iterator begin () const
    {
        return b;
    }

    byte_string::const_iterator end () const
    {
        return e;
    }

    byte_string::const_iterator cbegin () const
    {
        return b;
    }

    byte_string::const_iterator cend () const
    {
        return e;
    }

    endian const & order () const
    {
        return o;
    }

    byte_string::difference_type available () const
    {
        return pfs::distance(b, e);
    }

    void skip (size_t n)
    {
        n = pfs::min(n, integral_cast_check<size_t>(available()));
        pfs::advance(b, n);
    }

    template <typename Integral>
    void read_integral (Integral & v)
    {

        union u
        {
            Integral const v;
            byte_string::value_type const b[sizeof (Integral)];
        };

        byte_string::const_iterator pos(b);
        pfs::advance(pos, sizeof (Integral));

        if (pos <= e) {
            u const * d = reinterpret_cast<u const *> (b.base());
            v = o.convert(d->v);
            b = pos;
        } else {
            throw io_exception(make_error_code(io_errc::stream), NOT_ENOUGH_DATA_EXCEPTION_STR);
        }
    }

    template <typename Real>
    void read_real (Real & v)
    {

        union u
        {
            Real const v;
            byte_string::value_type const b[sizeof (Real)];
        };

#ifdef PFS_HAVE_INT64
        if (sizeof (Real) == 8) {
            uint64_t d = 0;
            read_integral(d);
            v = *reinterpret_cast<Real *> (& d);
        } else
#endif
            if (sizeof (Real) == 4) {
            uint32_t d = 0;
            read_integral(d);
            v = *reinterpret_cast<Real *> (& d);
        } else if (sizeof (Real) == 2) {
            uint16_t d = 0;
            read_integral(d);
            v = *reinterpret_cast<Real *> (& d);
        } else {
            // FIXME
            //        byte_string::const_iterator pos(ctx.b);
            //        std::advance(pos, sizeof(Float));
            //
            //        if (pos <= ctx.e) {
            //            u * b = reinterpret_cast<u *>(ctx.b.base());
            //
            //            if (order != endian::native_order()) {
            //                byte_string::value_type b[sizeof(Float)];
            //
            //            for (int i = 0, j = sizeof(Float) - 1; j >= 0; ++i, --j) {
            //                b[i] = d.b[j];
            //            }
            //
            //            appender.append(byte_string(b, sizeof(Float)));
            //        } else {
            //            appender.append(byte_string(d.b, sizeof(Float)));
            //        }
            //
            //            v = ctx.o.convert(d->v);
            //            ctx.b = pos;
            //            ctx.fail = false;
            //        } else {
            //            ctx.fail = true;
            //        }
        }
    }

    void read (byte_string & v, byte_string::size_type sz)
    {
        if (static_cast<byte_string::size_type> (pfs::distance(b, e)) < sz)
            throw io_exception(make_error_code(io_errc::stream), NOT_ENOUGH_DATA_EXCEPTION_STR);

        byte_string::const_iterator last(b);
        pfs::advance(last, sz);
        v.append(b, last);
        b = last;
    }

    void read (byte_string::pointer v, byte_string::size_type sz)
    {
        if (static_cast<byte_string::size_type> (pfs::distance(b, e)) < sz)
            throw io_exception(make_error_code(io_errc::stream), NOT_ENOUGH_DATA_EXCEPTION_STR);
        pfs::copy(b.base(), b.base() + sz, v);
        pfs::advance(b, sz);
    }

    void read (char * v, byte_string::size_type sz)
    {
        read(reinterpret_cast<byte_string::pointer> (v), sz);
    }
};

template <int N>
inline byte_ostream & operator<< (byte_ostream & os, byte_string_ref_n<N> const & v)
{
    typename byte_string_ref_n<N>::size_type n = pfs::min(v.max_size()
            , static_cast<typename byte_string_ref_n<N>::size_type> (v.p->size()));
    os.write_integral(n);
    os.write(*v.p, n);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, byte_string_ref const & v)
{
    os.write(*v.p, v.max_size);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, buffer_wrapper<byte_string::value_type const> const & v)
{
    os.write(v.p, v.max_size);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, buffer_wrapper<char const> const & v)
{
    os.write(v.p, v.max_size);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, byte_string const & v)
{
    os.write(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, bool v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, char v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, signed char v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, unsigned char v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, wchar_t v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, short v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, unsigned short v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, int v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, unsigned int v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, long v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, unsigned long v)
{
    os.write_integral(v);
    return os;
}

#if PFS_HAVE_LONGLONG

inline byte_ostream & operator<< (byte_ostream & os, long long v)
{
    os.write_integral(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, unsigned long long v)
{
    os.write_integral(v);
    return os;
}

#endif

inline byte_ostream & operator<< (byte_ostream & os, float v)
{
    os.write_real(v);
    return os;
}

inline byte_ostream & operator<< (byte_ostream & os, double v)
{
    os.write_real(v);
    return os;
}

template <int N>
byte_istream & operator>> (byte_istream & is, byte_string_ref_n<N> const & v)
{
    typename byte_string_ref_n<N>::size_type sz = 0;
    is >> sz;
    is.read(*v.p, sz);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, byte_string_ref const & v)
{
    is.read(*v.p, v.max_size);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, buffer_wrapper<byte_string::value_type> const & v)
{
    is.read(v.p, v.max_size);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, buffer_wrapper<char> const & v)
{
    is.read(v.p, v.max_size);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, bool & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, char & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, signed char & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, unsigned char & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, wchar_t & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, short & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, unsigned short & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, int & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, unsigned int & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, long & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, unsigned long & v)
{
    is.read_integral(v);
    return is;
}

#if PFS_HAVE_LONGLONG

inline byte_istream & operator>> (byte_istream & is, long long & v)
{
    is.read_integral(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, unsigned long long & v)
{
    is.read_integral(v);
    return is;
}

#endif

inline byte_istream & operator>> (byte_istream & is, float & v)
{
    is.read_real(v);
    return is;
}

inline byte_istream & operator>> (byte_istream & is, double & v)
{
    is.read_real(v);
    return is;
}

inline byte_string operator+ (byte_string const & lhs, byte_string const & rhs)
{
    byte_string r(lhs);
    return r += rhs;
}

inline byte_string operator+ (byte_string const & lhs, byte_string::value_type ch)
{
    byte_string r(lhs);
    return r += ch;
}

inline byte_string operator+ (byte_string::value_type ch, byte_string const & rhs)
{
    byte_string r(1, ch);
    return r += rhs;
}

inline byte_string operator+ (char ch, byte_string const & rhs)
{
    byte_string r(1, ch);
    return r += rhs;
}

inline byte_string operator+ (byte_string const & lhs, byte_string::const_pointer rhs)
{
    byte_string r(lhs);
    return r += rhs;
}

inline byte_string operator+ (byte_string::const_pointer lhs, byte_string const & rhs)
{
    byte_string r(lhs);
    return r += rhs;
}

inline byte_string operator+ (byte_string const & lhs, char const * rhs)
{
    byte_string r(lhs);
    return r += rhs;
}

inline byte_string operator+ (char const * lhs, byte_string const & rhs)
{
    byte_string r(lhs);
    return r += rhs;
}

#if __DEPRECATED__

class unpack_committer
{
    unpack_context & _ctx;
    byte_string::const_iterator _b;
    bool _committed;

public:

    unpack_committer (unpack_context & ctx)
    : _ctx (ctx)
    , _b (ctx.b)
    , _committed (false)
    {
    }

    ~unpack_committer ()
    {
        if (not _committed)
            _ctx.b = _b;
    }

    void commit ()
    {
        _b = _ctx.b;
        _committed = true;
    }
};

#endif // __DEPRECATED__

//// TODO Obsolete
//template <>
//bool unpack (unpack_context & ctx, string & v);
//
//// TODO Obsolete
//// UTF8 Specialization
////
//template <>
//inline void pack (pack_context & ctx, string const & v)
//{
//    pack(ctx, v.size());
//    ctx.buffer.append(v.c_str(), v.size());
//}

///**
// * @brief Represents (convert) @c byte_string data as @c string.
// *
// * @param v Value for string representation.
// * @param base Radix.
// * @param zero_padded @c true if value should be zero padded.
// * @param uppercase @c true if digit representaion characters should be in upper case.
// * @param prefix Prefix prepended for converted value.
// * @param separator String inserted bitween numbers.
// * @return
// */
//string to_string (byte_string const & v, int base = 10
//        , bool zero_padded = false
//        , bool uppercase = false
//        , string const & prefix = string()
//        , string const & separator = string());

byte_string & base64_encode (byte_string const & src, byte_string & result);
byte_string & base64_decode (byte_string const & src, byte_string & result);

inline bool operator== (byte_string const & lhs, byte_string const & rhs)
{
    return lhs.compare(rhs) == 0;
}

inline bool operator!= (byte_string const & lhs, byte_string const & rhs)
{
    return lhs.compare(rhs) != 0;
}

inline bool operator< (byte_string const & lhs, byte_string const & rhs)
{
    return lhs.compare(rhs) < 0;
}

inline bool operator<= (byte_string const & lhs, byte_string const & rhs)
{
    return lhs.compare(rhs) <= 0;
}

inline bool operator> (byte_string const & lhs, byte_string const & rhs)
{
    return lhs.compare(rhs) > 0;
}

inline bool operator>= (byte_string const & lhs, byte_string const & rhs)
{
    return lhs.compare(rhs) >= 0;
}

inline bool operator== (byte_string const & lhs, const char * rhs)
{
    return lhs.compare(rhs) == 0;
}

inline bool operator!= (byte_string const & lhs, const char * rhs)
{
    return lhs.compare(rhs) != 0;
}

inline bool operator< (byte_string const & lhs, const char * rhs)
{
    return lhs.compare(rhs) < 0;
}

inline bool operator<= (byte_string const & lhs, const char * rhs)
{
    return lhs.compare(rhs) <= 0;
}

inline bool operator> (byte_string const & lhs, const char * rhs)
{
    return lhs.compare(rhs) > 0;
}

inline bool operator>= (byte_string const & lhs, const char * rhs)
{
    return lhs.compare(rhs) >= 0;
}

inline bool operator== (const char * lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) == 0;
}

inline bool operator!= (const char * lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) != 0;
}

inline bool operator< (const char * lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) > 0;
}

inline bool operator<= (const char * lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) >= 0;
}

inline bool operator> (const char * lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) < 0;
}

inline bool operator>= (const char * lhs, byte_string const & rhs)
{
    return rhs.compare(lhs) <= 0;
}

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, byte_string const & v)
{
    return details::sequence::write_binary<Device>(dev, order, v.c_str(), v.size());
}

template <typename Device>
ssize_t read_binary (Device & dev, endian order, byte_string & v)
{
    char * buffer = 0;
    size_t size = 0;

    ssize_t result = details::sequence::read_binary<Device>(dev, order, & buffer, & size);

    if (buffer) {
        v.append(buffer, size);
        delete buffer;
    }

    return result;
}

// TODO Must be deprecated

template <typename StringType>
inline StringType to_string (byte_string const & s)
{
    return StringType(s.c_str());
}

} // pfs

#endif /* __PFS_BYTE_STRING_HPP__ */
