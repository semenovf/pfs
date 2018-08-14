#pragma once
#include <pfs/algo/split.hpp>
#include <pfs/algo/join.hpp>
#include <pfs/list.hpp>
#include <pfs/string.hpp>

namespace pfs {

template <typename StringT = pfs::string, template <typename> class Sequence = pfs::list>
class stringlist : public Sequence<StringT>
{
    typedef Sequence<StringT> base_class;

public:
    typedef StringT string_type;
    typedef typename base_class::value_type      value_type;
    typedef typename base_class::size_type       size_type;
    typedef typename base_class::difference_type difference_type;
    typedef typename base_class::iterator        iterator;
    typedef typename base_class::const_iterator  const_iterator;

public:
    stringlist () : base_class() {}

#if __cplusplus >= 201103L
    stringlist (stringlist && rhs)
        : base_class(std::forward<stringlist>(rhs))
    {}

    stringlist (std::initializer_list<StringT> ilist)
        : base_class(ilist.begin(), ilist.end())
    {}
#endif

    void split (string_type const & s, string_type const & separator
            , bool flag = keep_empty)
    {
        pfs::split(s.begin(), s.end(), separator.begin(), separator.end(), flag, this);
    }

    string_type join (string_type const & separator)
    {
        string_type s;
        pfs::join(this->cbegin(), this->cend(), separator, & s);
        return s;
    }

    string_type & operator [] (size_type index)
    {
        iterator it = this->begin();
        pfs::advance(it, index);
        return *it;
    }

    string_type const & operator [] (size_type index) const
    {
        const_iterator it = this->cbegin();
        pfs::advance(it, index);
        return *it;
    }
};

} // pfs
