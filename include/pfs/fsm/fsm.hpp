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

template <typename Sequence, typename Atomic>
struct transition
{
    typedef match<Sequence, Atomic>             match_type;
    typedef typename match_type::const_iterator const_iterator;
    
    int        state_next;
    int        state_fail;
    match_type m;
    int        status;
    bool       (* action)(const_iterator begin
                    , const_iterator end
                    , void * context
                    , void * action_args);
    void *     action_args;
};

template <typename Sequence, typename Atomic>
struct context
{
    typedef transition<Sequence, Atomic> transition_type;
    
    transition_type const * trans_tab;
    void * user_context;

    context (transition_type const * tab, void * ctx)
            : trans_tab(tab)
            , user_context(ctx)
    {}
};

template <typename Sequence, typename Atomic = size_t>
class fsm
{
public:
    typedef match<Sequence, Atomic>             match_type;
    typedef typename match_type::sequence_type  sequence_type;
    typedef typename match_type::char_type      char_type;
    typedef typename match_type::size_type      size_type;
    typedef typename match_type::const_iterator const_iterator;
    typedef typename match_type::result_type    result_type;
    typedef typename match_type::func_type      func_type;
    typedef transition<Sequence, Atomic>        transition_type;
    typedef context<Sequence, Atomic>           context_type;
    
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

 	void set_transition_table (transition<Sequence, Atomic> * tb)
	{
		_pcontext->trans_tab = tb;
	}

	void set_user_context (void * user_context)
	{
		_pcontext->user_context = user_context;
	}

    result_type exec (int state_cur, const_iterator begin, const_iterator end);

	result_type exec (const_iterator begin, const_iterator end)
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

    static match_type seq (sequence_type const & seq)
    {
        return match_type::template make<typename match_type::match_seq
                , sequence_type const &>(seq);
    }
    
    static match_type one_of (sequence_type const & seq)
    {
        return match_type::template make<typename match_type::match_one_of
                , sequence_type const &>(seq);
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

    static match_type rpt_one_of (sequence_type const & seq
            , size_type from
            , size_type to)
	{
        return match_type::template make_rpt<typename match_type::match_one_of
                , sequence_type const &>(seq, from, to);
	};

  	static match_type opt_one_of (sequence_type const & seq)
	{
        return rpt_one_of(seq, 0, 1);
	};
    
    static match_type rpt_seq (sequence_type const & seq
            , size_type from
            , size_type to)
	{
        return match_type::template make_rpt<typename match_type::match_seq
                , sequence_type const &>(seq, from, to);
	};

  	static match_type opt_seq (sequence_type const & seq)
	{
        return rpt_seq(seq, 0, 1);
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

template <typename Sequence, typename Atomic>
typename fsm<Sequence, Atomic>::result_type
fsm<Sequence, Atomic>::exec (int state_cur
        , const_iterator begin
        , const_iterator end)
{
	const_iterator ptr = begin;
	const_iterator ptr_accepted = begin;
	
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

template <typename Sequence, typename Atomic>
inline typename match<Sequence, Atomic>::result_type
match<Sequence, Atomic>::match_tr::do_match (context<Sequence, Atomic> * ctx
        , const_iterator begin
        , const_iterator end) const
{
    fsm<Sequence, Atomic> f(_tr, ctx->user_context);
    return f.template exec(fsm<Sequence, Atomic>::normal, begin, end);
}

template <typename Sequence, typename Atomic>
typename match<Sequence, Atomic>::result_type
match<Sequence, Atomic>::match_rpt::do_match (context<Sequence, Atomic> * ctx
        , const_iterator begin
        , const_iterator end) const
{
    int from = 0;
	int to = pfs::numeric_limits<size_type>::max();
	const_iterator ptr(begin);

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
