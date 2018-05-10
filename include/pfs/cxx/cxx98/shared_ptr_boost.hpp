#pragma once
#include <boost/shared_ptr.hpp>

namespace pfs {

template <typename T>
class shared_ptr : public boost::shared_ptr<T>
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
