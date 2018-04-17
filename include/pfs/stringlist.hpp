#pragma once
#include <pfs/algo/split.hpp>
#include <pfs/list.hpp>
#include <pfs/string.hpp>

namespace pfs {

template <typename StringList = string>
class stringlist : public list<StringList>
{
    typedef list<string> base_class;

public:
    typedef typename base_class::value_type      value_type;
    typedef typename base_class::difference_type difference_type;
    typedef typename base_class::iterator        iterator;
    typedef typename base_class::const_iterator  const_iterator;
    typedef string string_type;

public:
    stringlist () : base_class() {}

    void split (string_type const & s, string_type const & separator
            , bool flag = keep_empty)
    {
        pfs::split(s.begin(), s.end(), separator.begin(), separator.end(), flag, this);
    }
};

} // pfs
