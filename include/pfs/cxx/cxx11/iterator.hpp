#pragma once
#include <pfs/cxx/cxx98/iterator.hpp>

namespace pfs {

template <typename Iterator>
using reverse_iterator = std::reverse_iterator<Iterator>;

template <typename Container>
using back_insert_iterator = std::back_insert_iterator<Container>;

template <typename Container>
inline back_insert_iterator<Container>
back_inserter (Container & c)
{
    return std::back_insert_iterator<Container>(c);
}

} // pfs
