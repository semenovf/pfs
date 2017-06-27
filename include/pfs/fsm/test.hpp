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

template <typename Iterator, typename AtomicInt = size_t>
struct test_valid_entry
{
    test_valid_entry () {}
    
    bool operator () (transition<Iterator, AtomicInt> const * trans_tab
        , void * user_context
        , Iterator const & s);
};

template <typename Iterator, typename AtomicInt = size_t>
struct test_invalid_entry
{
    test_invalid_entry () {}
    
    bool operator () (transition<Iterator, AtomicInt> const * trans_tab
        , void * user_context
        , Iterator const & s
        , ssize_t offset);
};

template <typename Iterator, typename AtomicInt>
bool test_valid_entry<Iterator, AtomicInt>::operator () (
          transition<Iterator, AtomicInt> const * trans_tab
        , void * user_context
        , Iterator const & s)
{
	fsm<Iterator, AtomicInt> f;
	f.set_transition_table(trans_tab);
	f.set_user_context(user_context);

	typename fsm<Iterator, AtomicInt>::result_type result = f.exec(s.cbegin(), s.cend());

	return result.first && result.second == s.cend();
}

template <typename Iterator, typename AtomicInt>
bool test_invalid_entry<Iterator, AtomicInt>::operator () (
          transition<Iterator, AtomicInt> const * trans_tab
        , void * user_context
        , Iterator const & s
        , ssize_t offset)
{
	fsm<Iterator, AtomicInt> f;
	f.set_transition_table(trans_tab);
	f.set_user_context(user_context);

	typename fsm<Iterator, AtomicInt>::result_type result = f.exec(s.cbegin(), s.cend());

	if (offset >= 0) {
		typename Iterator::const_iterator it(s.cbegin());
		std::advance(it, offset);
		return result.first && result.second == it;
	}

	return result.first == false && result.second == s.cend();
}

}} // pfs::fsm

#endif /* __PFS_FSM_TEST_HPP__ */
