#pragma once
#include <boost/move/unique_ptr.hpp>

namespace pfs {

template <typename T>
class shared_ptr : public boost::unique_ptr<T>
{
    typedef boost::unique_ptr<T> base_class;

public:
    unique_ptr () : base_class()
    {}

    explicit unique_ptr (T * ptr) : base_class(ptr)
    {}

    template <typename Deleter>
    unique_ptr (T * ptr, Deleter deleter) : base_class(ptr, deleter)
    {}
};

} // pfs

