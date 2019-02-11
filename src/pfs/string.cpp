#include <cctype>
#include "pfs/string.hpp"
#include "pfs/unicode/char.hpp"
#include "pfs/unicode/unicode_iterator.hpp"

namespace pfs {

string to_lower (string::const_iterator first, string::const_iterator last)
{
    typedef pfs::unicode::unicode_iterator_traits<
            string::const_iterator>::iterator unicode_iterator;

    typedef pfs::unicode::unicode_iterator_traits<
            string::iterator>::output_iterator output_iterator;

    string result;

    output_iterator out(pfs::back_inserter(result));

    unicode_iterator f(first);
    unicode_iterator l(last);

    while (f != l)
        *out++ = unicode::to_lower(*f++);

    return result;
}

string to_upper (string::const_iterator first, string::const_iterator last)
{
    typedef pfs::unicode::unicode_iterator_traits<
            string::const_iterator>::iterator unicode_iterator;

    typedef pfs::unicode::unicode_iterator_traits<
            string::iterator>::output_iterator output_iterator;

    string result;

    output_iterator out(pfs::back_inserter(result));

    unicode_iterator f(first);
    unicode_iterator l(last);

    while (f != l)
        *out++ = unicode::to_upper(*f++);

    return result;
}

} // pfs
