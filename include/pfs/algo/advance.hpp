////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Vladislav Trifochkin
//
// This file is part of [pfs](https://github.com/semenovf/pfs) library.
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace pfs {

/**
 *
 */
template <typename ForwardIterator1, typename ForwardIterator2>
inline void advance_sequence (ForwardIterator1 & pos, ForwardIterator1 last
        , ForwardIterator2 first2, ForwardIterator2 last2)
{
    ForwardIterator1 p = pos;

    while (p != last && first2 != last2 && *p++ == *first2++)
        ;

    if (first2 == last2)
        pos = p;
}

/**
 */
template <typename ForwardIterator1
        , typename ForwardIterator2
        , typename EndSeq2>
inline void advance_sequence_until (ForwardIterator1 & pos, ForwardIterator1 last
        , ForwardIterator2 first2, EndSeq2 eos2)
{
    ForwardIterator1 p = pos;

    while (p != last && !eos2(first2) && *p++ == *first2++)
        ;

    if (eos2(first2))
        pos = p;
}

/**
 */
template <typename ForwardIterator1
        , typename ForwardIterator2
        , typename EndSeq1
        , typename EndSeq2>
inline void advance_sequence_until (ForwardIterator1 & pos, EndSeq1 eos1
        , ForwardIterator2 first2, EndSeq2 eos2)
{
    ForwardIterator1 p = pos;

    while (!eos1(p) && !eos2(first2) && *p++ == *first2++)
        ;

    if (eos2(first2))
        pos = p;
}

} // pfs

