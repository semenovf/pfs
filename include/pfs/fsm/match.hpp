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

//template <typename Sequence>
//struct transition;

template <typename Sequence>
struct context;

template <typename Sequence>
class match
{
public:
    typedef match_traits<Sequence>                     match_traits_type;
    typedef typename match_traits_type::sequence_type  sequence_type;
    typedef typename match_traits_type::char_type      char_type;
    typedef typename match_traits_type::size_type      size_type;
    typedef typename match_traits_type::const_iterator const_iterator;
    typedef typename match_traits_type::result_type    result_type;

protected:    
    struct match_base
    {
        virtual ~match_base () {}
        virtual result_type do_match (context<Sequence> * ctx
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
        delete _p;
    }

    result_type operator () (context<Sequence> * ctx
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

        virtual result_type do_match (context<Sequence> *
                , const_iterator begin
                , const_iterator) const
        {
            return result_type(true, begin);
        }
    };

    class match_length : public match_base
    {
        size_type _len;

        virtual result_type do_match (context<Sequence> * ctx
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

        virtual result_type do_match (context<Sequence> * ctx
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

        virtual result_type do_match (context<Sequence> * ctx
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
        char_type _from;
        char_type _to;

        virtual result_type do_match (context<Sequence> * ctx
                , const_iterator begin
                , const_iterator end) const
        {
            return match_traits_type::xmatch_range(begin, end, _from, _to);
        }

    public:
        match_range (char_type from, char_type to)
                : _from(from)
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
};


}} // pfs::fsm

#endif /* __PFS_FSM_MATCH_HPP__ */

