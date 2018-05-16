#pragma once
#include <boost/move/unique_ptr.hpp>

namespace pfs {

template <typename T, typename Deleter = boost::movelib::default_delete<T> >
class unique_ptr : public boost::movelib::unique_ptr<T, Deleter>
{
    typedef boost::movelib::unique_ptr<T> base_class;
    typedef typename base_class::deleter_type deleter_type;

public:
    struct rval {
        T *     _value;
        Deleter _deleter;
        rval (T * v) : _value(v), _deleter(deleter_type()) {}
    };

public:
    unique_ptr () : base_class()
    {}

    explicit unique_ptr (T * ptr) : base_class(ptr)
    {}

    unique_ptr (T * ptr, Deleter deleter) : base_class(ptr, deleter)
    {}

    unique_ptr (rval const & rhs) pfs_noexcept
        : base_class(rhs._value, rhs._deleter)
    {}

    unique_ptr & operator = (rval const & rhs) pfs_noexcept
    {
        unique_ptr p(rhs);
        this->swap(p);
        return *this;
    }
};

} // pfs

#include "unique_ptr_common.hpp"
