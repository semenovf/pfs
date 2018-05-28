#pragma once
#include <pfs/exception.hpp>
#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(iterator)

namespace pfs {

template <typename Category, typename Iter>
struct basic_safe_iterator
{
    typedef typename Iter::value_type value_type;
    typedef typename Iter::pointer    pointer;

    Iter pos;
    Iter const first;
    Iter const last;

    basic_safe_iterator () {}

    basic_safe_iterator (Iter const & p, Iter const & f, Iter const & l)
            : pos(p), first(f), last(l)
    {}
};


template <typename Iter>
struct safe_input_iterator : basic_safe_iterator<input_iterator_tag, Iter>
{
    typedef basic_safe_iterator<input_iterator_tag, Iter> base_class;
    typedef typename base_class::value_type value_type;
    typedef typename base_class::pointer    pointer;

    safe_input_iterator () : base_class() {}

    safe_input_iterator (Iter const & p, Iter const & l)
            : base_class(p, p, l)
    {}

    value_type operator * () const
    {
        return *base_class::pos;
    }

    pointer operator -> () const
    {
        return base_class::pos.operator -> ();
    }

    safe_input_iterator & operator ++ () // prefix increment
    {
        if (base_class::pos != base_class::last)
            ++base_class::pos;
        else
            PFS_THROW(pfs::range_error("safe_input_iterator::operator++()"));
        return *this;
    }

    safe_input_iterator operator ++ (int) // postfix increment
    {
        safe_input_iterator r(*this);
        ++(*this);
        return r;
    }
};

} // pfs
