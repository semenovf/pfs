#pragma once
#include <list>
#include <pfs/v2/sequence_container.hpp>

namespace pfs {

namespace stdcxx { struct list {}; } //  stdcxx

namespace sequence_container {

template <typename T>
struct type_traits<T, stdcxx::list>
{
    typedef std::list<T> type;
    typedef T value_type;
};

}} // pfs::associative_container

