////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2019 Vladislav Trifochkin
//
// This file is part of [pfs](https://github.com/semenovf/pfs) library.
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace pfs {

/**
 * Compare sequence [@a first1, @a last1) with sequence @a [@a first2, @a last2).
 *
 * @return -1 if [@a first1, @a last1) less than @a [@a first2, @a last2),
 *          0 if [@a first1, @a last1) equals to [@a first2, @a last2),
 *          1 if [@a first1, @a last1) greater than [@a first2, @a last2).
 */
template <typename ForwardIterator1, typename ForwardIterator2, typename Compare>
int compare (ForwardIterator1 first1, ForwardIterator1 last1
        , ForwardIterator2 first2, ForwardIterator2 last2
        , Compare comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2))
            return -1;

        if (comp(*first2, *first1))
            return 1;

        ++first1;
        ++first2;
    }

    if (first1 == last1 && first2 != last2)
        return -1;

    if (first1 != last1 && first2 == last2)
        return 1;

    return 0;
}

/**
 */
template <typename ForwardIterator1
        , typename ForwardIterator2
        , typename EndSeq2
        , typename Compare>
int compare_until (ForwardIterator1 first1, ForwardIterator1 last1
        , ForwardIterator2 first2, EndSeq2 eos2
        , Compare comp)
{
    while (first1 != last1 && !eos2(first2)) {
        if (comp(*first1, *first2))
            return -1;

        if (comp(*first2, *first1))
            return 1;

        ++first1;
        ++first2;
    }

    if (first1 == last1 && !eos2(first2))
        return -1;

    if (first1 != last1 && eos2(first2))
        return 1;

    return 0;
}

/**
 */
template <typename ForwardIterator1
        , typename ForwardIterator2
        , typename EndSeq1
        , typename EndSeq2
        , typename Compare>
int compare_until (ForwardIterator1 first1, EndSeq1 eos1
        , ForwardIterator2 first2, EndSeq2 eos2
        , Compare comp)
{
    while (!eos1(first1) && !eos2(first2)) {
        if (comp(*first1, *first2))
            return -1;

        if (comp(*first2, *first1))
            return 1;

        ++first1;
        ++first2;
    }

    if (eos1(first1) && !eos2(first2))
        return -1;

    if (!eos1(first1) && eos2(first2))
        return 1;

    return 0;
}

} // pfs
