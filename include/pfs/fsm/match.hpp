/* 
 * File:   match.hpp
 * Author: wladt
 *
 * Created on March 8, 2017, 12:13 PM
 */

#ifndef __PFS_FSM_MATCH_HPP__
#define __PFS_FSM_MATCH_HPP__

#include <pfs/fsm/traits.hpp>

namespace pfs {
namespace fsm {

template <typename Sequence, typename Atomic>
struct transition;

template <typename Sequence, typename Atomic>
struct context;

template <typename Sequence, typename Atomic>
class match
{
public:
    typedef match_traits<Sequence, Atomic>             match_traits_type;
    typedef typename match_traits_type::atomic_type    atomic_type;
    typedef typename match_traits_type::sequence_type  sequence_type;
    typedef typename match_traits_type::char_type      char_type;
    typedef typename match_traits_type::size_type      size_type;
    typedef typename match_traits_type::const_iterator const_iterator;
    typedef typename match_traits_type::result_type    result_type;
    typedef typename match_traits_type::func_type      func_type;

protected:    
    struct match_base
    {
        atomic_type ref;
        
        match_base ()
            : ref(1)
        {}
        
        virtual ~match_base () {}
        virtual result_type do_match (context<Sequence, Atomic> * ctx
                , const_iterator begin
                , const_iterator end) const = 0;
    };

    match_base * _p;

protected:
    match (match_base * p)
        : _p(p)
    {}
        
public:
    ~match ()
    {
        if(!--_p->ref) {
            delete _p;
        }
    }

    match (match const & m)
        : _p(m._p)
    {
        ++_p->ref;
    }

    match operator = (match const & m)
    {
        _p = m._p;
        ++_p.ref;
    }

    result_type operator () (context<Sequence, Atomic> * ctx
            , const_iterator begin
            , const_iterator end) const
    {
        return _p->do_match(ctx, begin, end);
    }

    class match_nothing : public match_base
    {
    public:
        match_nothing ()
        {}

        virtual ~match_nothing ()
        {}

        virtual result_type do_match (context<Sequence, Atomic> *
                , const_iterator begin
                , const_iterator) const
        {
            return result_type(true, begin);
        }
    };

    class match_length : public match_base
    {
        size_type _len;

        virtual result_type do_match (context<Sequence, Atomic> * ctx
                , const_iterator begin
                , const_iterator end) const
        {
            return match_traits_type::xmatch_length(begin, end, _len);
        }

    public:
        match_length (size_type len)
                : _len(len)
        {}
    };

    class match_subseq : public match_base
    {
        sequence_type _seq;

        virtual result_type do_match (context<Sequence, Atomic> * ctx
                , const_iterator begin
                , const_iterator end) const
        {
            return match_traits_type::xmatch_subseq(begin, end, _seq);
        }

    public:
        match_subseq (sequence_type const & seq)
                : _seq(seq)
        {}
    };

    class match_one_of : public match_base
    {
        sequence_type _seq;

        virtual result_type do_match (context<Sequence, Atomic> * ctx
                , const_iterator begin
                , const_iterator end) const
        {
            return match_traits_type::xmatch_one_of(begin, end, _seq);
        }

    public:
        match_one_of (sequence_type const & seq)
                : _seq(seq)
        {}
    };

    class match_range : public match_base
    {
        char_type _min;
        char_type _max;

        virtual result_type do_match (context<Sequence, Atomic> * ctx
                , const_iterator begin
                , const_iterator end) const
        {
            return match_traits_type::xmatch_range(begin, end, _min, _max);
        }

    public:
        match_range (char_type min, char_type max)
                : _min(min)
                , _max(max)
        {}
    };
    
    class match_func : public match_base
    {
        func_type _fn;
        void *    _fn_context;

        virtual result_type do_match (context<Sequence, Atomic> * ctx
                , const_iterator begin
                , const_iterator end) const
        {
            return _fn(begin, end, ctx->user_context, _fn_context);
        }

    public:
        match_func (func_type fn, void * fn_context)
                : _fn(fn)
                , _fn_context(fn_context)
         {}
    };

    class match_tr : public match_base
    {
        transition<Sequence, Atomic> const * _tr;
        
        virtual result_type do_match (context<Sequence, Atomic> * ctx
                , const_iterator begin
                , const_iterator end) const;

    public:
        match_tr (transition<Sequence, Atomic> const * tr)
                : _tr(tr)
        {}
    };

    class match_rpt : public match_base
    {
        match<Sequence, Atomic> _match;
        size_type       _from;
        size_type       _to;
        
        virtual result_type do_match (context<Sequence, Atomic> * ctx
                , const_iterator begin
                , const_iterator end) const;

    public:
        match_rpt (match<Sequence, Atomic> const & m, size_type from, size_type to)
                : _match(m)
                , _from(from)
                , _to(to)
        {}
    };
    
    template <typename MatchT>
    static match make ()
    {
        return match(new MatchT);
    }

    template <typename MatchT, typename Arg1>
    static match make (Arg1 a1)
    {
        return match(new MatchT(a1));
    }

    template <typename MatchT, typename Arg1, typename Arg2>
    static match make (Arg1 a1, Arg2 a2)
    {
        return match(new MatchT(a1, a2));
    }

    template <typename MatchT, typename Arg1, typename Arg2, typename Arg3>
    static match make (Arg1 a1, Arg2 a2, Arg3 a3)
    {
        return match(new MatchT(a1, a2, a3));
    }

    template <typename MatchT>
    static match make_rpt (size_type from, size_type to)
    {
        return match(new match_rpt(make<MatchT>()), from, to);
    }

    template <typename MatchT, typename Arg1>
    static match make_rpt (Arg1 a1, size_type from, size_type to)
    {
        return match(new match_rpt(make<MatchT>(a1), from, to));
    }

    template <typename MatchT, typename Arg1, typename Arg2>
    static match make_rpt (Arg1 a1, Arg2 a2, size_type from, size_type to)
    {
        return match(new match_rpt(make<MatchT>(a1, a2), from, to));
    }

    template <typename MatchT, typename Arg1, typename Arg2, typename Arg3>
    static match make_rpt (Arg1 a1, Arg2 a2, Arg3 a3, size_type from, size_type to)
    {
        return match(new match_rpt(make<MatchT>(a1, a2, a3), from, to));
    }
};

}} // pfs::fsm

#endif /* __PFS_FSM_MATCH_HPP__ */

