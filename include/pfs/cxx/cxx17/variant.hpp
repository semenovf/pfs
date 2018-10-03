#pragma once
#include <variant>

namespace pfs {

template <typename... Types>
using variant = std::variant<Types...>;

} // pfs

