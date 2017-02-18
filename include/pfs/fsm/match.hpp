/*
 * fsm_match.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: wladt
 */

#ifndef __PFS_FSM_MATCH_HPP__
#define __PFS_FSM_MATCH_HPP__

#include <pfs/limits.hpp>

#include "fsm.hpp"

namespace pfs { namespace fsm {

//
// Matches sequence of characters
//
template <typename Sequence>
class match_len : public match_base<Sequence>
{
	typedef match_base<Sequence>                base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	size_t _len;

public:
	match_len (size_t len)
		: _len(len)
	{}

	virtual result_type do_match (context<Sequence> *
			, const_iterator begin
			, const_iterator end) const
	{
		size_t n = _len;
		while (n > 0 && begin != end) {
			++begin;
			--n;
		}
		return n > 0
				? result_type(false, end)
				: result_type(true, begin);
	}
};

//
// Matches string of characters
//
template <typename Sequence>
class match_substr : public match_base<Sequence>
{
	typedef match_base<Sequence>                base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	Sequence _p;

public:
	match_substr (const Sequence & p)
		: _p(p) {}

	virtual result_type do_match (context<Sequence> *
			, const_iterator begin
			, const_iterator end) const
	{
		if (begin == end)
			return result_type(false, end);
		return fsm<Sequence>::contains_chars(_p.begin(), _p.end(), begin, end);
	}
};

//
// Matches one character from characters
//
template <typename Sequence>
class match_char : public match_base<Sequence>
{
	typedef match_base<Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	Sequence _p;

public:
	match_char (const Sequence & p)
		: _p(p)
	{}

	virtual ~match_char ()
	{}

	virtual result_type do_match (context<Sequence> *
			, const_iterator begin
			, const_iterator end) const
	{
		if (begin == end)
			return result_type(false, end);
		return fsm<Sequence>::belongs_char(*begin, _p.begin(), _p.end())
			? result_type(true, ++begin)
			: result_type(false, end);
	}
};

template <typename Sequence>
class match_range : public match_base<Sequence>
{
	typedef match_base<Sequence>                base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	char_type _from;
	char_type _to;

public:
	match_range (char_type from,  char_type to)
		: _from(from)
		, _to(to)
	{}

	virtual ~match_range ()
	{}

	virtual result_type do_match (context<Sequence> *
            , const_iterator begin
            , const_iterator end) const
	{
		return fsm<Sequence>::range_char(*begin, _from, _to)
			? result_type(true, ++begin)
			: result_type(false, end);
	}
};

template <typename Sequence>
class match_fsm : public match_base<Sequence>
{
	typedef match_base<Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	const transition<Sequence> * _trans;

public:
	match_fsm (const transition<Sequence> * trans)
		: _trans(trans)
	{}

	virtual ~match_fsm ()
	{}

	virtual result_type do_match(context<Sequence> * ctx
			, const_iterator begin
			, const_iterator end) const
	{
		fsm<Sequence> fsm(_trans, ctx->parse_context);
		return fsm.exec(normal_status, begin, end);
	}
};

template <typename Sequence>
class match_func : public match_base<Sequence>
{
	typedef match_base<Sequence>                base_class;
	typedef typename base_class::char_type       char_type;
	typedef typename base_class::const_iterator  const_iterator;
	typedef typename base_class::result_type     result_type;
	typedef typename match<Sequence>::func_type func_type;

private:
	func_type _fn;
	void *    _fnContext;

public:
	match_func (func_type fn, void * fnContext)
		: _fn(fn)
		, _fnContext(fnContext)
	{}

	virtual ~match_func ()
	{}

	virtual result_type do_match (context<Sequence> * ctx
			, const_iterator begin
			, const_iterator end) const
	{
		return _fn(ctx->parse_context, _fnContext, begin, end);
	}
};

template <typename Sequence>
class match_rpt : public match_base<Sequence>
{
	typedef match_base<Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	match<Sequence> _match;
	rpt_bounds       _bounds;

public:
	match_rpt (const match<Sequence> & m, int from, int to)
		: _match(m)
	{
		_bounds.from = from;
		_bounds.to = to;
	}

	virtual ~match_rpt()
	{}

	virtual result_type do_match (context<Sequence> * ctx
			, const_iterator begin
			, const_iterator end) const;
};

template <typename Sequence>
typename match_rpt<Sequence>::result_type match_rpt<Sequence>::do_match (context<Sequence> * ctx
		, const_iterator begin
		, const_iterator end) const
{
	int from = 0;
	int to = pfs::numeric_limits<int>::max();
	const_iterator ptr(begin);

	if (_bounds.from >= 0)
		from = _bounds.from;

	if (_bounds.to >= 0)
		to = _bounds.to;

	PFS_ASSERT(from <= to);

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

template <typename Sequence>
class match_nothing : public match_base<Sequence>
{
	typedef match_base<Sequence>                base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

public:
	match_nothing ()
	{}

	virtual ~match_nothing ()
	{}

	virtual result_type do_match (context<Sequence> *
            , const_iterator begin
            , const_iterator) const
	{
		return result_type(true, begin);
	}
};

}} // pfs::fsm

#endif /* __PFS_FSM_MATCH_HPP__ */
