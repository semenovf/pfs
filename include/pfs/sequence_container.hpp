#pragma once
#include "container.hpp"

namespace pfs {
namespace sequence_container {

template <typename T
        , typename SequenceContainerTag>
struct type_traits;

template <typename T
        , typename SequenceContainer>
struct iterators : container::details::iterators<T, SequenceContainer>
{
    typedef container::details::iterators<T, SequenceContainer> base_class;
    iterators (typename base_class::container_type & ac) : base_class(ac) {}
};

}} // pfs::sequence_container

