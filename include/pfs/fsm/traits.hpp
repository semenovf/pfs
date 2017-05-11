/*
 * traits.hpp
 *
 *  Created on: Mar 086, 2017
 *      Author: wladt
 */

#ifndef __PFS_FSM_TRAITS_HPP__
#define __PFS_FSM_TRAITS_HPP__

#include <pfs/utility.hpp>   // pair

namespace pfs { namespace fsm {

template <typename Sequence, typename Atomic = size_t>
struct match_traits
{
    typedef Sequence                                 sequence_type;
    typedef Atomic                                   atomic_type;
    typedef typename sequence_type::value_type       char_type;
    typedef typename sequence_type::size_type        size_type;
    typedef typename sequence_type::const_iterator   const_iterator;
    typedef typename pfs::pair<bool, const_iterator> result_type;
    
    typedef result_type (* func_type) (
                          const_iterator begin
                        , const_iterator end
                        , void * parse_context
                        , void * fn_context);
    
    static result_type xmatch_length (
              const_iterator begin
            , const_iterator end
            , size_type n);
    
    static result_type xmatch_one_of (
              const_iterator begin
            , const_iterator end
            , sequence_type const & seq);

    static result_type xmatch_seq (
              const_iterator begin
            , const_iterator end
            , sequence_type const & seq);
    
    static result_type xmatch_range (const_iterator begin
            , const_iterator end
            , char_type from
            , char_type to);
};

template <typename Sequence, typename Atomic>
inline typename match_traits<Sequence, Atomic>::result_type
match_traits<Sequence, Atomic>::xmatch_length (const_iterator begin
            , const_iterator end
            , size_type n)
{
    while (n > 0 && begin != end) {
        ++begin;
        --n;
    }

    return n > 0
            ? result_type(false, end)
            : result_type(true, begin);
}

template <typename Sequence, typename Atomic>
typename match_traits<Sequence, Atomic>::result_type
match_traits<Sequence, Atomic>::xmatch_one_of (const_iterator begin
            , const_iterator end
            , sequence_type const & seq)
{
    const_iterator it = seq.begin();
    const_iterator it_end = seq.end();

    if (begin == end)
        return seq.size() > 0
                    ? result_type(false, end)
                    : result_type(true, begin);
    
    for (; it != it_end; ++it) {
        if (*it == *begin)
            return result_type(true, ++begin);
    }
    
    return result_type(false, end);
}

template <typename Sequence, typename Atomic>
typename match_traits<Sequence, Atomic>::result_type
match_traits<Sequence, Atomic>::xmatch_seq (const_iterator begin
            , const_iterator end
            , sequence_type const & seq)
{
    const_iterator it = seq.begin();
    const_iterator it_end = seq.end();
    
    if (begin == end) {
        return it == it_end
                ? result_type(true, end)
                : result_type(false, end);
    }
    
	while (begin != end && it != it_end) {
		if (*begin != *it)
			break;
		++begin;
		++it;
	}

	return it == it_end
			? result_type(true, begin)
			: result_type(false, end);
}

template <typename Sequence, typename Atomic>
inline typename match_traits<Sequence, Atomic>::result_type
match_traits<Sequence, Atomic>::xmatch_range (const_iterator begin
            , const_iterator end
            , char_type from
            , char_type to)
{
    return begin != end && *begin >= from && *begin <= to 
            ? result_type(true, ++begin)
            : result_type(false, end);
}

}} // pfs::fsm

#endif /* __PFS_FSM_TRAITS_HPP__ */
