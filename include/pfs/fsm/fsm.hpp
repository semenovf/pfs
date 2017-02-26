/**
 * @file   fsm.hpp
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Header file for Finite State Machine definitions.
 */

#ifndef __PFS_FSM_HPP__
#define __PFS_FSM_HPP__

#include <pfs/memory.hpp>  // unique_ptr
#include <pfs/utility.hpp> // pair
//#include <pfs/exception.hpp>

namespace pfs { namespace fsm {

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

	result_type operator () (context<Sequence> * fsm
            , const_iterator begin
            , const_iterator end) const
	{
		return _match->do_match(fsm, begin, end);
	}
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
	fsm ()
        : _context(new context<Sequence>)
    {
        _context->trans_tab     = 0;
        _context->parse_context = 0;
    }
        
	fsm (transition<Sequence> const * initial_tb, void * parse_context)
        : _context(new context<Sequence>)
    {
        _context->trans_tab     = initial_tb;
        _context->parse_context = parse_context;
    }

	void set_transition_table (transition<Sequence> * tb)
	{
		_context->trans_tab = tb;
	}

	void set_user_context (void * parse_context)
	{
		_context->parse_context = parse_context;
	}

	result_type exec (int state_cur, const_iterator begin, const_iterator end);

	result_type exec (const_iterator begin, const_iterator end)
	{
		return exec(0, begin, end);
	}

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

template <typename Sequence>
inline bool fsm<Sequence>::range_char (char_type ch, char_type from, char_type to)
{
	return ch >= from && ch <= to ? true : false;
}

}} // pfs::fsm

#include <pfs/fsm/match.hpp>
#include <pfs/fsm/exec.hpp>

#endif /* __PFS_FSM_HPP__ */
