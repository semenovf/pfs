/**
 * @file   fsm_test.h
 * @author wladt
 * @date   Oct 31, 2012 4:48:26 PM
 *
 * @brief Used for testing FSM rules. Simply include this file
 *        in test source file.
 */

#ifndef __PFS_FSM_TEST_HPP__
#define __PFS_FSM_TEST_HPP__

#include <pfs/test/test.hpp>
#include <pfs/fsm/fsm.hpp>

namespace pfs { namespace fsm {

template <typename Iterator, typename AtomicInt = int>
struct test_valid_entry
{
    test_valid_entry () {}
    
    bool operator () (transition<Iterator, AtomicInt> const * trans_tab
        , void * user_context
        , Iterator first
        , Iterator last)
    {
        fsm<Iterator, AtomicInt> f;
        f.set_transition_table(trans_tab);
        f.set_user_context(user_context);

        typename fsm<Iterator, AtomicInt>::result_type result = f.exec(first, last);

        return result.first && result.second == last;
    }
};

template <typename Iterator, typename AtomicInt = int>
struct test_invalid_entry
{
    test_invalid_entry () {}
    
    bool operator () (transition<Iterator, AtomicInt> const * trans_tab
        , void * user_context
        , Iterator first
        , Iterator last
        , ptrdiff_t offset)
    {
        fsm<Iterator, AtomicInt> f;
        f.set_transition_table(trans_tab);
        f.set_user_context(user_context);

        typename fsm<Iterator, AtomicInt>::result_type result = f.exec(first, last);

        if (offset >= 0) {
            typename Iterator::iterator it(first);
            std::advance(it, offset);
            return result.first && result.second == it;
        }

        return result.first == false && result.second == last;
    }
};

}} // pfs::fsm

#endif /* __PFS_FSM_TEST_HPP__ */
