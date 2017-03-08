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

namespace pfs { namespace fsm {

template <typename Sequence>
struct transition
{
    typedef match<Sequence>                     match_type;
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

template <typename Sequence>
struct context
{
    typedef transition<Sequence> transition_type;
    
    transition_type const * trans_tab;
    void * parse_context;

    context (transition_type const * tab, void * ctx)
            : trans_tab(tab)
            , parse_context(ctx)
    {}
};

template <typename Sequence>
class fsm
{
public:
    typedef match<Sequence>                     match_type;
    typedef typename match_type::sequence_type  sequence_type;
    typedef typename match_type::char_type      char_type;
    typedef typename match_type::size_type      size_type;
    typedef typename match_type::const_iterator const_iterator;
    typedef typename match_type::result_type    result_type;
    typedef transition<Sequence>                transition_type;
    typedef context<Sequence>                   context_type;
    
    enum status_enum {
          normal = 0
        , reject
        , accept
    };
private:
    unique_ptr<context_type> _pcontext;

public:
        
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

    static match_type subseq (sequence_type const & seq)
    {
        return match_type::template make<typename match_type::match_subseq
                , sequence_type const &>(seq);
    }
    
    static match_type one_of (sequence_type const & seq)
    {
        return match_type::template make<typename match_type::match_one_of
                , sequence_type const &>(seq);
    }

    static match_type range (char_type from, char_type to)
    {
        return match_type::template make<typename match_type::match_range
                , char_type, char_type>(from, to);
    }
};

//
// Below is the old implementation
//
#if __COMMENT__
enum {
      normal_status = 0
    , reject_status
    , accept_status
};

template <typename Sequence>
struct result
{
	typedef typename pfs::pair<bool, typename Sequence::const_iterator> type;
};

template <typename Sequence>
struct context;

template <typename Sequence>
struct transition;

struct rpt_bounds
{
	int from, to;
};

template <typename Sequence>
class match_base
{
public:
	typedef typename Sequence::value_type     char_type;
	typedef typename Sequence::const_iterator const_iterator;
	typedef typename result<Sequence>::type   result_type;

	match_base() : _ref(1) {}
	virtual ~match_base() {}

	virtual result_type do_match (context<Sequence> * fsm
            , const_iterator begin
            , const_iterator end) const = 0;

	int _ref;
};

template <typename Sequence>
class match
{
	match_base<Sequence> * _match;

public:
	typedef typename Sequence::value_type     char_type;
	typedef typename Sequence::const_iterator const_iterator;
	typedef typename result<Sequence>::type   result_type;

	typedef result_type (* func_type)(void * parse_context
			, void * fn_context
			, const_iterator begin
			, const_iterator end);

private:
	match() : _match(0) {}
	void deref () {
		if (_match) {
			PFS_ASSERT(_match->_ref > 0);
			--_match->_ref;
			if (_match->_ref == 0)
				delete _match;
			_match = 0;
		}
	}

public:
	match (match_base<Sequence> * match)
        : _match(match) 
    {
        PFS_ASSERT(_match); 
    }
        
	match (match const & other)
	{
		PFS_ASSERT(other._match);
		_match = other._match;
		++_match->_ref;
	}
    
    match & operator = (match const & other)
    {
        if (& other != this) {
            PFS_ASSERT(other._match);
            _match = other._match;
            ++_match->_ref;
        }
        return *this;
    }

	~match () 
    {
        deref(); 
    }

//	result_type operator () (context<Sequence> * fsm
//            , const_iterator begin
//            , const_iterator end) const
//	{
//		return _match->do_match(fsm, begin, end);
//	}
};

template <typename Sequence>
struct transition
{
	typedef typename Sequence::value_type char_type;
	typedef typename Sequence::const_iterator const_iterator;

	int             state_next;
	int             state_fail;
	match<Sequence> m;
	int             status;
	bool            (* action)(const_iterator begin
                            , const_iterator end
                            , void * context
                            , void * action_args);
	void *          action_args;
};

template <typename Sequence>
struct context
{
	transition<Sequence> const * trans_tab;
	void * parse_context;
};


template <typename Sequence>
class fsm
{
    unique_ptr<context<Sequence> > _context;
	//context<Sequence> * _context;

public:
	typedef Sequence                               sequence_type;
	typedef typename sequence_type::value_type     char_type;
	typedef typename sequence_type::const_iterator const_iterator;
	typedef typename result<sequence_type>::type   result_type;

public:
//	fsm ()
//        : _context(new context<Sequence>)
//    {
//        _context->trans_tab     = 0;
//        _context->parse_context = 0;
//    }
        
//	fsm (transition<Sequence> const * initial_tb, void * parse_context)
//        : _context(new context<Sequence>)
//    {
//        _context->trans_tab     = initial_tb;
//        _context->parse_context = parse_context;
//    }

	void set_transition_table (transition<Sequence> * tb)
	{
		_context->trans_tab = tb;
	}

	void set_user_context (void * parse_context)
	{
		_context->parse_context = parse_context;
	}

//	result_type exec (int state_cur, const_iterator begin, const_iterator end);
//
//	result_type exec (const_iterator begin, const_iterator end)
//	{
//		return exec(0, begin, end);
//	}

public:
	/** @brief Checks if character @c ch belongs to the subset of characters
	 *         specified by begin and end iterators.
	 */
	static bool belongs_char (char_type ch, const_iterator begin, const_iterator end);

	/** @brief Checks if haystack specified by begin and end iterators
	 *         (@a haystack_begin and @a haystack_begin respectively)
	 *         contains the needle also specified by begin and end iterators
	 *         (@a needle_begin and @a needle_end respectively).
	 *
	 *  @return pair
	 */
	static result_type contains_chars (const_iterator needle_begin, const_iterator needle_end
			, const_iterator haystack_begin, const_iterator haystack_end);

	/** @brief Checks if character 'ch' belongs to the specified range
	  */
	static bool range_char (char_type ch, char_type from, char_type to);
};


template <typename Sequence>
bool fsm<Sequence>::belongs_char (char_type ch
		, const_iterator begin
		, const_iterator end)
{
	while (begin < end) {
		if (*begin == ch)
			return true;
		++begin;
	}
	return false;
}

template <typename Sequence>
typename fsm<Sequence>::result_type fsm<Sequence>::contains_chars (const_iterator needle_begin
		, const_iterator needle_end
		, const_iterator haystack_begin
		, const_iterator haystack_end)
{
	const_iterator itn(needle_begin);
	const_iterator ith(haystack_begin);

	if (needle_begin == needle_end)
		return result_type(false, haystack_end);

	while(itn < needle_end && ith < haystack_end) {
		if (*itn != *ith)
			break;
		++itn;
		++ith;
	}

	return itn == needle_end
			? result_type(true, ith)
			: result_type(false, haystack_end);
}
#endif

template <typename Sequence>
typename fsm<Sequence>::result_type
fsm<Sequence>::exec (int state_cur
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
                        , _pcontext->parse_context
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

}} // pfs::fsm

#endif /* __PFS_FSM_HPP__ */
