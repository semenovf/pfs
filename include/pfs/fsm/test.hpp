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

template <typename Sequence, typename Atomic = size_t>
struct test_valid_entry
{
    test_valid_entry () {}
    
    bool operator () (transition<Sequence, Atomic> const * trans_tab
        , void * user_context
        , Sequence const & s);
};

template <typename Sequence, typename Atomic = size_t>
struct test_invalid_entry
{
    test_invalid_entry () {}
    
    bool operator () (transition<Sequence, Atomic> const * trans_tab
        , void * user_context
        , Sequence const & s
        , ssize_t offset);
};

template <typename Sequence, typename Atomic>
bool test_valid_entry<Sequence, Atomic>::operator () (
          transition<Sequence, Atomic> const * trans_tab
        , void * user_context
        , Sequence const & s)
{
	fsm<Sequence, Atomic> f;
	f.set_transition_table(trans_tab);
	f.set_user_context(user_context);

	typename fsm<Sequence, Atomic>::result_type result = f.exec(s.cbegin(), s.cend());

	return result.first && result.second == s.cend();
}

template <typename Sequence, typename Atomic>
bool test_invalid_entry<Sequence, Atomic>::operator () (
          transition<Sequence, Atomic> const * trans_tab
        , void * user_context
        , Sequence const & s
        , ssize_t offset)
{
	fsm<Sequence, Atomic> f;
	f.set_transition_table(trans_tab);
	f.set_user_context(user_context);

	typename fsm<Sequence, Atomic>::result_type result = f.exec(s.cbegin(), s.cend());

	if (offset >= 0) {
		typename Sequence::const_iterator it(s.cbegin());
		std::advance(it, offset);
		return result.first && result.second == it;
	}

	return result.first == false && result.second == s.cend();
}

}} // pfs::fsm

#endif /* __PFS_FSM_TEST_HPP__ */
