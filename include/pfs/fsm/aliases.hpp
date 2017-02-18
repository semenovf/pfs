/*
 * alias.hpp
 *
 *  Created on: Jan 6, 2016
 *      Author: wladt
 */

#ifndef __PFS_FSM_ALIASES_HPP__
#define __PFS_FSM_ALIASES_HPP__

namespace pfs { namespace fsm {

template <typename Sequence>
struct m
{
	typedef Sequence                           sequence_type;
	typedef typename sequence_type::value_type char_type;
	typedef transition<sequence_type>          transition_type;
	typedef match<sequence_type>               match_type;
	typedef typename match_type::func_type     func_type;

	struct nothing
	{
		match_type m;
		nothing()
			: m(new match_nothing<sequence_type>())
		{}
	};

	struct len
	{
		match_type m;
		len (size_t n)
			: m(new match_len<sequence_type>(n))
		{}
	};

	struct chr
	{
		match_type m;
		chr (sequence_type const & s)
			:	m(new match_char<sequence_type>(s))
		{}
	};

	struct sub
	{
		match_type m;
		sub (sequence_type const & s)
			: m(new match_substr<sequence_type>(s))
		{}
	};

	struct func
	{
		match_type m;
		func (func_type f, void * func_context)
			: m(new match_func<sequence_type>(f, func_context))
		{}
	};

	struct tr
	{
		match_type m;
		tr (transition_type * t)
			: m(new match_fsm<sequence_type>(t))
		{}
	};

	struct range
	{
		match_type m;
		range (char_type min, char_type max)
			: m (new match_range<sequence_type>(min, max))
		{}
	};

	struct rpt_range
	{
		match_type m;
		rpt_range (char_type min, char_type max, int from, int to)
			: m(new match_rpt<sequence_type>(range(min, max).m, from, to))
		{}
	};

	struct opt_range
	{
		match_type m;
		opt_range (char_type min, char_type max)
			: m(new match_rpt<sequence_type>(range(min,max).m, 0, 1))
		{}
	};

	struct rpt_tr
	{
		match_type m;
		rpt_tr (transition_type * t, int from, int to)
			: m(new match_rpt<sequence_type>(tr(t).m, from, to))
		{}
	};

	struct opt_tr
	{
		match_type m;
		opt_tr (transition_type * t)
			: m(new match_rpt<sequence_type>(tr(t).m, 0, 1))
		{}
	};

	struct rpt_func
	{
		match_type m;
		rpt_func (func_type f, void * func_context, int from, int to)
			: m(new match_rpt<sequence_type>(func(f, func_context).m, from, to))
		{}
	};

	struct opt_func
	{
		match_type m;
		opt_func (func_type f, void * func_context)
			: m(new match_rpt<sequence_type>(func(f, func_context).m, 0, 1))
		{}
	};

	struct rpt_sub
	{
		match_type m;
		rpt_sub (const sequence_type & s, int from, int to)
			: m(new match_rpt<sequence_type>(sub(s).m, from, to))
		{}
	};

	struct opt_sub
	{
		match_type m;
		opt_sub (const sequence_type & s)
			: m(new match_rpt<sequence_type>(sub(s).m, 0, 1))
		{}
	};

	struct rpt_chr
	{
		match_type m;
		rpt_chr (const sequence_type & s, int from, int to)
			: m(new match_rpt<sequence_type>(chr(s).m, from, to))
		{}
	};

	struct opt_chr
	{
		match_type m;
		opt_chr (const sequence_type & s)
			: m(new match_rpt<sequence_type>(chr(s).m, 0, 1))
		{}
	};
};

}} // pfs::fsm

#endif /* __PFS_FSM_ALIASES_HPP__ */
