#pragma once
#include <pfs/algo/split.hpp>
#include <pfs/list.hpp>
#include <pfs/string.hpp>

namespace pfs {
namespace mpl {
template <typename StringT = pfs::string>
class stringlist : public list<StringT>
{
    typedef list<StringT> base_class;

public:
    typedef StringT string_type;
    typedef typename base_class::value_type      value_type;
    typedef typename base_class::difference_type difference_type;
    typedef typename base_class::iterator        iterator;
    typedef typename base_class::const_iterator  const_iterator;

public:
    stringlist () : base_class() {}

    void split (string_type const & s, string_type const & separator
            , bool flag = keep_empty)
    {
        pfs::split(s.begin(), s.end(), separator.begin(), separator.end(), flag, this);
    }
};
} // mpl

typedef mpl::stringlist<pfs::string> stringlist;

} // pfs
