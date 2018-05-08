#pragma once
#include <pfs/cxxlang.hpp>
#include <boost/function.hpp>

namespace pfs {

template <typename F>
struct function : public ::boost::function<F>
{
    function () : ::boost::function<F>() {}
    function (F f) : ::boost::function<F>(f) {}
    function (::boost::function<F> const x) : ::boost::function<F>(x) {}

    template <typename Functor>
    function (Functor f) : ::boost::function<F>(f) {}

    using ::boost::function<F>::operator();
};

} // namespace pfs
