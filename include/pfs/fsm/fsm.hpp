/**
 * @file   fsm.hpp
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Header file for Finite State Machine definitions.
 */

#ifndef __PFS_FSM_HPP__
#define __PFS_FSM_HPP__

#include <pfs/memory.hpp>    // unique_ptr
#include <pfs/fsm/match.hpp>
#include <pfs/limits.hpp>
#include <pfs/exception.hpp>

namespace pfs { namespace fsm {

template <typename Iterator, typename AtomicInt>
struct transition
{
    typedef match<Iterator, AtomicInt>    match_type;
    typedef typename match_type::iterator iterator;
    
    int        state_next;
    int        state_fail;
    match_type m;
    int        status;
    bool       (* action)(iterator begin
                    , iterator end
                    , void * context
                    , void * action_args);
    void *     action_args;
};

template <typename Iterator, typename AtomicInt>
struct context
{
    typedef transition<Iterator, AtomicInt> transition_type;
    
    transition_type const * trans_tab;
    void * user_context;

    context (transition_type const * tab, void * ctx)
            : trans_tab(tab)
            , user_context(ctx)
    {}
};

template <typename Iterator, typename AtomicInt = int>
class fsm
{
public:
    typedef match<Iterator, AtomicInt>       match_type;
    typedef typename match_type::iterator    iterator;
    typedef typename match_type::char_type   char_type;
    typedef typename match_type::size_type   size_type;
    typedef typename match_type::result_type result_type;
    typedef typename match_type::func_type   func_type;
    typedef transition<Iterator, AtomicInt>  transition_type;
    typedef context<Iterator, AtomicInt>     context_type;
    
    enum status_enum {
          normal = 0
        , reject
        , accept
    };
private:
    unique_ptr<context_type> _pcontext;

public:
  	fsm ()
        : _pcontext(new context_type(0, 0))
    {}

  	fsm (transition_type const * initial, void * parse_context)
        : _pcontext(new context_type(initial, parse_context))
    {}

   	fsm (transition_type const * initial)
        : _pcontext(new context_type(initial, 0))
    {}

 	void set_transition_table (transition_type const * tb)
	{
		_pcontext->trans_tab = tb;
	}

	void set_user_context (void * user_context)
	{
		_pcontext->user_context = user_context;
	}

    result_type exec (int state_cur, iterator begin, iterator end);

	result_type exec (iterator begin, iterator end)
	{
		return exec(0, begin, end);
	}
    
    static match_type nothing ()
    {
        return match_type::template make<typename match_type::match_nothing>();
    }

    static match_type length (size_type n)
    {
        return match_type::template make<typename match_type::match_length
                , size_type>(n);
    }

    static match_type seq (iterator first, iterator last)
    {
        return match_type::template make<typename match_type::match_seq
                , iterator, iterator>(first, last);
    }
    
    static match_type one_of (iterator first, iterator last)
    {
        return match_type::template make<typename match_type::match_one_of
                , iterator, iterator>(first, last);
    }

    static match_type range (char_type min, char_type max)
    {
        return match_type::template make<typename match_type::match_range
                , char_type, char_type>(min, max);
    }

    static match_type func (func_type fn, void * fn_context)
    {
        return match_type::template make<typename match_type::match_func
                , func_type, void *>(fn, fn_context);
    }

    static match_type tr (transition_type const * t)
    {
        return match_type::template make<typename match_type::match_tr
                , transition_type const *>(t);
    }

    static match_type rpt_one_of (iterator first, iterator last
            , size_type from
            , size_type to)
	{
        return match_type::template make_rpt<typename match_type::match_one_of
                , iterator, iterator>(first, last, from, to);
	};

  	static match_type opt_one_of (iterator first, iterator last)
	{
        return rpt_one_of(first, last, 0, 1);
	};
    
    static match_type rpt_seq (iterator first, iterator last
            , size_type from
            , size_type to)
	{
        return match_type::template make_rpt<typename match_type::match_seq
                , iterator, iterator>(first, last, from, to);
	};

  	static match_type opt_seq (iterator first, iterator last)
	{
        return rpt_seq(first, last, 0, 1);
	};

 	static match_type rpt_range (
              char_type min
            , char_type max
            , size_type from
            , size_type to)
	{
        
        return match_type::template make<typename match_type::match_range
                , char_type, char_type>(min, max, from, to);
	};

	static match_type opt_range (
              char_type min
            , char_type max)
	{
        return rpt_range(min, max, 0, 1);
	};

    static match_type rpt_func (
              func_type fn
            , void * fn_context
            , size_type from
            , size_type to)
	{
        return match_type::template make_rpt<typename match_type::match_func
                , func_type, void *>(fn, fn_context, from, to);
	};

	static match_type opt_func (func_type fn, void * fn_context)
	{
        return rpt_func(fn, fn_context, 0, 1);
	};
    
   	static match_type rpt_tr (
              transition_type const * trn
            , size_type from
            , size_type to)
	{
        return match_type::template make_rpt<typename match_type::match_tr
                , transition_type const *>(trn, from, to);
	};
    
	static match_type opt_tr (transition_type const * trn)
	{
        return rpt_tr(trn, 0, 1);
	};

};

template <typename Iterator, typename AtomicInt>
typename fsm<Iterator, AtomicInt>::result_type
fsm<Iterator, AtomicInt>::exec (int state_cur
        , iterator begin
        , iterator end)
{
	iterator ptr = begin;
	iterator ptr_accepted = begin;
	
	bool accepted = false;

	// FIXME no need after _contex
//	PFS_ASSERT(_context);
//	PFS_ASSERT(_context->_trans_tab);

    // FIXME need to check state_cur value (must be in specified bounds)
    transition_type const * trans = & _pcontext->trans_tab[state_cur];

	do {
		result_type r = trans->m(_pcontext.get(), ptr, end);

		if (r.first) {
			if (trans->action) {
				if (!trans->action(ptr
                        , r.second
                        , _pcontext->user_context
                        , trans->action_args)) {

					// Let's support this situation
					//
					//static FSM_TRANSITION record_fsm[] = {
					//	  {-1,  1, FSM_MATCH_SEQ(10) , FSM_ACCEPT, on_record1, 0 }
					//	, {-1,  2, FSM_MATCH_SEQ(11) , FSM_ACCEPT, on_record2, 0 }
					//	, {-1, -1, FSM_MATCH_SEQ(12) , FSM_ACCEPT, on_record3, 0 }
					//};
					if (trans->status == accept) {
						if (trans->state_fail >= 0) {
							trans = & _pcontext->trans_tab[trans->state_fail];
							continue;
						}
					}

					return result_type(false, end);
				}
			}

			if (trans->status == accept) {
				accepted = true;
			}

			ptr = r.second;

			if (trans->status == accept) {
				ptr_accepted = ptr;
			}

			if (trans->status == reject) {
				state_cur = -1;
				accepted = false;
			} else {
				state_cur = trans->state_next;
			}
		} else {
			state_cur = trans->state_fail;

			if (trans->status != accept) {
				accepted = false;
			}

			ptr = ptr_accepted;
		}

		if (state_cur < 0)
			break;

		trans = & _pcontext->trans_tab[state_cur];

	} while (true);

	return accepted
			? result_type(true, ptr_accepted)
			: result_type(false, end);
}

template <typename Iterator, typename AtomicInt>
inline typename match<Iterator, AtomicInt>::result_type
match<Iterator, AtomicInt>::match_tr::do_match (context<Iterator, AtomicInt> * ctx
        , iterator begin
        , iterator end) const
{
    fsm<Iterator, AtomicInt> f(_tr, ctx->user_context);
    return f.template exec(fsm<Iterator, AtomicInt>::normal, begin, end);
}

template <typename Iterator, typename AtomicInt>
typename match<Iterator, AtomicInt>::result_type
match<Iterator, AtomicInt>::match_rpt::do_match (context<Iterator, AtomicInt> * ctx
        , iterator begin
        , iterator end) const
{
    int from = 0;
	int to = pfs::numeric_limits<int>::max();
	iterator ptr(begin);

	if (_from >= 0)
		from = _from;

	if (_to >= 0)
		to = _to;

    if (from > to)
        throw out_of_range("match_rpt::do_match()");

	int i = 0;

	for (i = 0; i < to && ptr < end; i++) {
		result_type r = _match(ctx, ptr, end);

		if (!r.first) {
			break;
		}

		ptr = r.second;
	}

	if (i < from) {
		return result_type(false, end);
	}

	return result_type(true, ptr);
}

}} // pfs::fsm

#endif /* __PFS_FSM_HPP__ */
