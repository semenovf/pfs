#pragma once
#include <pfs/compare.hpp>

#if HAVE_BOOST_SMARTPTR

#include <boost/shared_ptr.hpp>

namespace pfs {

template <typename T> class shared_ptr : public boost::shared_ptr<T>
{
    typedef boost::shared_ptr<T> base_class;

public:
    shared_ptr () : base_class()
    {}

    explicit shared_ptr (T * ptr) : base_class(ptr)
    {}

    template <typename Deleter>
    shared_ptr (T * ptr, Deleter deleter) : base_class(ptr, deleter)
    {}

    template <typename U>
    shared_ptr (shared_ptr<U> const & other, T * p) : base_class(other, p)
    {}

    shared_ptr (shared_ptr const & other) : base_class(other)
    {}

    inline shared_ptr & operator = (shared_ptr const & other)
    {
        base_class::operator = (other);
        return *this;
    }
};

} // pfs

#else // !HAVE_BOOST_SMARTPTR

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

#endif // HAVE_BOOST_SMARTPTR

namespace pfs {

template <typename T>
inline shared_ptr<T> make_shared () { return shared_ptr<T>(new T); }

template <class T, class Arg1>
inline shared_ptr<T> make_shared (Arg1 a1) { return shared_ptr<T>(new T(a1)); }

template <class T, class Arg1, class Arg2>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2) { return shared_ptr<T>(new T(a1, a2)); }

template <class T, class Arg1, class Arg2, class Arg3>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3) { return shared_ptr<T>(new T(a1, a2, a3)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) { return shared_ptr<T>(new T(a1, a2, a3, a4)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
inline shared_ptr<T> make_shared (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8)); }

template <class T, class T1>
inline shared_ptr<T> static_pointer_cast (const shared_ptr<T1> & r)
{
	return shared_ptr<T>(r, static_cast<T*>(r.get()));
}

template <class T, class T1>
inline shared_ptr<T> dynamic_pointer_cast (const shared_ptr<T1> & r)
{
	T * p = dynamic_cast<T*>(r.get());
	if (p)
		return shared_ptr<T>(r, p);
	return shared_ptr<T>();
}

template <class T, class T1>
inline shared_ptr<T> const_pointer_cast (const shared_ptr<T1> & r)
{
	return shared_ptr<T>(r, const_cast<T*>(r.get()));
}

template <class T, class T1>
inline shared_ptr<T> reinterpret_pointer_cast (const shared_ptr<T1> & r)
{
	return shared_ptr<T>(r, reinterpret_cast<T*>(r.get()));
}

template <typename T, typename U>
inline bool operator == (shared_ptr<T> const & a, shared_ptr<U> const & b)
{
     return a.get() == b.get();
}

template <typename T, typename U>
inline bool operator < (shared_ptr<T> const & a, shared_ptr<U> const & b)
{
    return a.get() < b.get();
}

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
