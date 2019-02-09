#include <cctype>
#include "pfs/unicode/char.hpp"

namespace pfs {
namespace unicode {

// TODO Need to support full unicode table
char_t to_lower (char_t const & c)
{
    return c.value <= 127 ? std::tolower(static_cast<unsigned char>(c.value)) : c;
}

// TODO Need to support full unicode table
char_t to_upper (char_t const & c)
{
    return c.value <= 127 ? std::toupper(static_cast<unsigned char>(c.value)) : c;
}

}} // namespace pfs::unicode
