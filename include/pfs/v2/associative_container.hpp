#pragma once

namespace pfs {
namespace associative_container {

template <typename Key
        , typename T
        , typename AssociativeContainerTag>
struct type_trait;

template <typename Key
        , typename T
        , typename AssociativeContainerTag>
struct iterators;

template <typename Key
        , typename T
        , typename AssociativeContainerTag>
struct inserter;

template <typename Key
        , typename T
        , typename AssociativeContainerTag>
struct finder;

}} // pfs::associative_container
