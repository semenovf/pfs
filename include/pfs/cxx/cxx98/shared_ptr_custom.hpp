#pragma once
#include <pfs/cxxlang.hpp>
#include <pfs/utility.hpp>
#include <pfs/atomic.hpp>
#include <pfs/assert.hpp>

namespace pfs {

template <typename T>
struct default_deleter
{
    void operator () (T * p) const {
        delete p;
    }
};

// Specialization for arrays, default_delete.
//
template <typename T>
struct default_deleter<T[]>
{
public:
    default_deleter() pfs_noexcept {}

    void operator () (T * p) const
    {
        delete [] p;
    }
};


template <typename T>
struct custom_deleter
{
    void operator () (T *) const {}
};

struct ref_count
{
    typedef void (* ref_count_deleter)(ref_count *);

    atomic_int weakref;
    atomic_int strongref;
    ref_count_deleter deleter_fn;

    ref_count () : deleter_fn(0)
    {
        strongref.store(1);
        weakref.store(1);
    }

    ~ref_count ()
    {
        PFS_ASSERT(!weakref.load());
        PFS_ASSERT(strongref.load() <= 0);
    }

     void destroy() { deleter_fn(this); } // need check
};


template <typename T, typename Deleter>
class ref_count_with_deleter: public ref_count
{
private:
    ref_count_with_deleter (const ref_count_with_deleter & );
    ref_count_with_deleter & operator = (const ref_count_with_deleter & );

public:
    ref_count_with_deleter(T * ptr, Deleter d) : ref_count(), _p(ptr), _deleter(d) { deleter_fn = deleter; }
    ~ref_count_with_deleter() { this->~ref_count(); }

    static void deleter(ref_count * self)
    {
        reinterpret_cast<ref_count_with_deleter *>(self)->_deleter(
                reinterpret_cast<ref_count_with_deleter *>(self)->_p);
    }

private:
    T *     _p;
    Deleter _deleter;
};

template <class T>
class shared_ptr
{
protected:
    template <typename T1> friend  class shared_ptr;

    T * _value;
    ref_count * _d;

protected:
    template <typename Deleter>
    void construct (T * ptr, Deleter deleter)
    {
        if (!ptr) {
            _d = 0;
            return;
        }
        _d = new ref_count_with_deleter<T, Deleter>(ptr, deleter);
    }

public:
    shared_ptr () : _value(0), _d(0)
    {}

    explicit shared_ptr (T * ptr) : _value(ptr), _d(0)
    {
        if (ptr) {
            default_deleter<T> deleter;
            construct<default_deleter<T> >(ptr, deleter);
        }
    }

    template <typename Deleter>
    shared_ptr (T * ptr, Deleter deleter) : _value(ptr), _d(0)
    {
        construct<Deleter>(ptr, deleter);
    }

    template <typename T1>
    shared_ptr (const shared_ptr<T1> & other, T * p) : _value(p), _d(other._d)
    {
        if (_d)
            ref();
    }

    shared_ptr (const shared_ptr<T> & other) : _value(other._value), _d(other._d)
    {
        if (_d)
            ref();
    }

    inline shared_ptr<T> & operator = (const shared_ptr<T> & other)
    {
        shared_ptr copy(other);
        swap(copy);
        return *this;
    }

    ~shared_ptr ()
    {
        deref();
    }

    bool is_null () const
    {
    	return _value == 0;
    }

    T & operator * () const
    {
        PFS_ASSERT(_value != 0);
        return *_value;
    }

    T * operator -> () const
    {
        PFS_ASSERT(_value != 0);
        return _value;
    }

    inline operator bool () const throw()
    {
        return (0 < use_count());
    }

    T * get () const
    {
        return _value;
    }

    void swap (shared_ptr & other)
    {
        std::swap<ref_count *>(_d, other._d);
        std::swap(this->_value, other._value);
    }

    inline void reset ()
    {
        shared_ptr copy;
        swap(copy);
    }

    inline void reset (T * ptr)
    {
        shared_ptr copy(ptr);
        swap(copy);
    }

    template <typename Deleter>
    inline void reset (T * ptr, Deleter deleter)
    {
        shared_ptr copy(ptr, deleter);
        swap(copy);
    }

    int use_count () const
    {
        if (_value != 0)
            return _d->strongref.load();
        return 0;
    }

    /**
     * @brief Checks if *this is the only shared_ptr instance managing the current object, i.e. whether use_count() == 1.
     *
     * @return
     */
    bool unique () const
    {
        return use_count() == 1 ? true : false;
    }

private:
    static void deref (ref_count * d)
    {
        if (! d)
            return;

        if (! --d->strongref) {
            d->destroy();
        }

        if (! --d->weakref)
            delete d;
    }

    void deref ()
    {
        deref (_d);
    }

    void ref() const
    {
        ++_d->weakref;
        ++_d->strongref;
    }
};

} // namespace pfs

namespace pfs {

// Avoid direct comaparison with raw pointers
//
// template <typename T>
// inline bool operator == (shared_ptr<T> const & a, void const * b)
// {
//      return a.get() == b;
// }
//
// template <typename T>
// inline bool operator < (shared_ptr<T> const & a, void const * b)
// {
//     return a.get() < b;
// }
//
// template <typename T>
// inline bool operator == (void const * a, shared_ptr<T> const & b)
// {
//      return a == b.get();
// }
//
// template <typename T>
// inline bool operator < (void const * a, shared_ptr<T> const & b)
// {
//     return a < b.get();
// }

} // pfs

#include "shared_ptr_common.hpp"
