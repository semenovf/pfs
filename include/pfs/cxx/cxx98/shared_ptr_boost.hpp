#pragma once
#include <pfs/cxxlang.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace pfs {

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr : public boost::shared_ptr<T>
{
    typedef boost::shared_ptr<T> base_class;

public:
    shared_ptr () : base_class(){}

    explicit shared_ptr (T * ptr) : base_class(ptr) {}

    template <typename Deleter>
    shared_ptr (T * ptr, Deleter deleter) : base_class(ptr, deleter) {}

    template <typename U>
    shared_ptr (shared_ptr<U> const & other, T * p) : base_class(other, p) {}

    shared_ptr (shared_ptr const & other) : base_class(other) {}

    template <typename U>
    explicit shared_ptr (weak_ptr<U> const & rhs) : base_class(rhs) {}

    inline shared_ptr & operator = (shared_ptr const & other)
    {
        base_class::operator = (other);
        return *this;
    }
};

template <typename T>
class weak_ptr : public boost::weak_ptr<T>
{
    typedef boost::weak_ptr<T> base_class;

public:
    weak_ptr () : base_class() {}

    weak_ptr (weak_ptr const & rhs) : base_class(rhs) {}

    template <typename U>
    weak_ptr (weak_ptr<U> const & rhs) : base_class(rhs) {}

    template <typename U>
    weak_ptr (shared_ptr<U> const & rhs) : base_class(rhs) {}

    weak_ptr & operator = (weak_ptr const & rhs)
    {
        base_class::operator = (rhs);
        return *this;
    }

    template <typename U>
    weak_ptr & operator = (weak_ptr<U> const & rhs)
    {
        base_class::template operator =<U>(rhs);
        return *this;
    }

    template <typename U>
    weak_ptr & operator = (shared_ptr<U> const & rhs)
    {
        base_class::template operator =<U>(rhs);
        return *this;
    }

    /**
     * @fn long use_count () const noexcept
     * @return The number of @c shared_ptr instances sharing
     *         the ownership of the menaged object at the
     *         instance of the call.
     * @note @c expared() may be faster than @c use_count().
     */

    /**
     * @fn bool expired () const noexcept
     * @return @c true if the managed object has already been
     *         deleted, @c false otherwise.
     */

    /**
     * @brief Creates a new @c shared_ptr that shares ownership
     *        of the menaged object.
     */
    pfs::shared_ptr<T> lock () const noexcept
    {
        return this->expired()
                ? pfs::shared_ptr<T>()
                : pfs::shared_ptr<T>(*this);
    }
};

} // pfs

#include "shared_ptr_common.hpp"
