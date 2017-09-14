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
    
    bool operator () (transition<Iterator, AtomicInt> const * trans_tab
            , Iterator first
            , Iterator last)
    {
        return this->operator ()(trans_tab, 0, first, last);
    }
};

template <typename Grammar, typename AtomicInt = int>
struct test_valid_entry_seq : test_valid_entry<typename Grammar::iterator, AtomicInt>
{
    typedef test_valid_entry<typename Grammar::iterator, AtomicInt> base_class;
    typedef typename Grammar::iterator iterator;
    typedef typename Grammar::string_type string_type;
    
    test_valid_entry_seq () {}
    
    bool operator () (transition<iterator, AtomicInt> const * trans_tab
            , void * user_context
            , string_type const & s)
    {
        return base_class::operator () (trans_tab, user_context, s.begin(), s.end());
    }

    bool operator () (transition<iterator, AtomicInt> const * trans_tab
            , string_type const & s)
    {
        return base_class::operator () (trans_tab, s.begin(), s.end());
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

        if (offset > 0) {
            Iterator it(first);
            pfs::advance(it, offset);
            return result.first && result.second == it;
        }

        return result.first == false && result.second == last;
    }

    bool operator () (transition<Iterator, AtomicInt> const * trans_tab
            , Iterator first
            , Iterator last
            , ptrdiff_t offset)
    {
        return this->operator ()(trans_tab, 0, first, last, offset);
    }
};


template <typename Grammar, typename AtomicInt = int>
struct test_invalid_entry_seq : test_invalid_entry<typename Grammar::iterator, AtomicInt>
{
    typedef test_invalid_entry<typename Grammar::iterator, AtomicInt> base_class;
    typedef typename Grammar::iterator iterator;
    typedef typename Grammar::string_type string_type;

    test_invalid_entry_seq () {}

    bool operator () (transition<iterator, AtomicInt> const * trans_tab
        , void * user_context
        , string_type const & s
        , ptrdiff_t offset)
    {
        return base_class::operator ()(trans_tab, user_context, s.begin(), s.end(), offset);
    }

    bool operator () (transition<iterator, AtomicInt> const * trans_tab
        , string_type const & s
        , ptrdiff_t offset)
    {
        return base_class::operator ()(trans_tab, s.begin(), s.end(), offset);
    }
};

}} // pfs::fsm

#endif /* __PFS_FSM_TEST_HPP__ */
