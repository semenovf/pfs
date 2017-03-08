/*
 * traits.hpp
 *
 *  Created on: Mar 086, 2017
 *      Author: wladt
 */

#ifndef __PFS_FSM_TRAITS_HPP__
#define __PFS_FSM_TRAITS_HPP__

#include <pfs/utility.hpp>   // pair
//#include <pfs/limits.hpp>

namespace pfs { namespace fsm {

template <typename Sequence>
struct match_traits
{
    typedef Sequence                                 sequence_type;
    typedef typename sequence_type::value_type       char_type;
    typedef typename sequence_type::size_type        size_type;
    typedef typename sequence_type::const_iterator   const_iterator;
    typedef typename pfs::pair<bool, const_iterator> result_type;
    
    static result_type xmatch_length (
              const_iterator begin
            , const_iterator end
            , size_type n);
    
    static result_type xmatch_one_of (
              const_iterator begin
            , const_iterator end
            , sequence_type const & seq);

    static result_type xmatch_subseq (
              const_iterator begin
            , const_iterator end
            , sequence_type const & seq);
    
    static result_type xmatch_range (const_iterator begin
            , const_iterator end
            , char_type from
            , char_type to);
};

template <typename Sequence>
inline typename match_traits<Sequence>::result_type
match_traits<Sequence>::xmatch_length (const_iterator begin
            , const_iterator end
            , size_type n)
{
    while (n > 0 && begin != end) {
        ++begin;
        --n;
    }

    return n > 0
            ? result_type(false, end)
            : result_type(true, begin);
}

template <typename Sequence>
typename match_traits<Sequence>::result_type
match_traits<Sequence>::xmatch_one_of (const_iterator begin
            , const_iterator end
            , sequence_type const & seq)
{
    const_iterator it = seq.cbegin();
    const_iterator it_end = seq.cend();

    if (begin == end)
        return seq.size() > 0
                    ? result_type(false, end)
                    : result_type(true, begin);
    
    for (; it != it_end; ++it) {
        if (*it == *begin)
            return result_type(true, ++begin);
    }
    
    return result_type(false, end);
}

template <typename Sequence>
typename match_traits<Sequence>::result_type
match_traits<Sequence>::xmatch_subseq (const_iterator begin
            , const_iterator end
            , sequence_type const & seq)
{
    const_iterator it = seq.cbegin();
    const_iterator it_end = seq.cend();
    
    if (begin == end) {
        return it == it_end
                ? result_type(true, end)
                : result_type(false, end);
    }
    
	while (begin != end && it != it_end) {
		if (*begin != *it)
			break;
		++begin;
		++it;
	}

	return it == it_end
			? result_type(true, begin)
			: result_type(false, end);
}

template <typename Sequence>
inline typename match_traits<Sequence>::result_type
match_traits<Sequence>::xmatch_range (const_iterator begin
            , const_iterator end
            , char_type from
            , char_type to)
{
    return begin != end && *begin >= from && *begin <= to 
            ? result_type(true, ++begin)
            : result_type(false, end);
}
#if __COMMENT__

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

#endif

}} // pfs::fsm

#endif /* __PFS_FSM_TRAITS_HPP__ */
