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

template <typename Iterator, typename AtomicInt = int>
struct match_traits
{
    typedef Iterator                      iterator;
    typedef AtomicInt                     atomic_type;
    typedef size_t                        size_type;
    typedef typename iterator::value_type char_type;
    
    typedef typename pfs::pair<bool, iterator> result_type;
    
    typedef result_type (* func_type) (
                          iterator begin
                        , iterator end
                        , void * parse_context
                        , void * fn_context);
    
    static result_type xmatch_length (
              iterator begin
            , iterator end
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
    
    static result_type xmatch_one_of (
              iterator begin
            , iterator end
            , iterator seq_begin
            , iterator seq_end)
    {
        if (begin == end)
            return seq_begin != seq_end // not empty
                        ? result_type(false, end)
                        : result_type(true, begin);

        for (iterator it = seq_begin; it != seq_end; ++it) {
            if (*it == *begin)
                return result_type(true, ++begin);
        }

        return result_type(false, end);
    }
    

    static result_type xmatch_seq (
              iterator begin
            , iterator end
            , iterator seq_begin
            , iterator seq_end)
    {
        if (begin == end) {
            return seq_begin == seq_end
                    ? result_type(true, end)
                    : result_type(false, end);
        }

        iterator it = seq_begin;
        
        while (begin != end && it != seq_end) {
            if (*begin != *it)
                break;
            ++begin;
            ++it;
        }

        return it == seq_end
                ? result_type(true, begin)
                : result_type(false, end);
    }
    
    static result_type xmatch_range (iterator begin
            , iterator end
            , char_type from
            , char_type to)
    {
        return begin != end && *begin >= from && *begin <= to 
                ? result_type(true, ++begin)
                : result_type(false, end);
    }
    
};

}} // pfs::fsm

#endif /* __PFS_FSM_TRAITS_HPP__ */
