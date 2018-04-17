#pragma once
#include <pfs/iterator.hpp>

namespace pfs {

template <typename InputIterator>
class ring_iterator
{
private:
    typedef typename pfs::iterator_traits<InputIterator>::reference reference;

public:
    ring_iterator () {}

    /**
     * Construct iterator for iterate over range [@a begin, @ end).
     * @a begin and @a end must not be equals.
     *
     * @param begin Range start.
     * @param end   Range end (excluding).
     */
    ring_iterator (InputIterator begin, InputIterator end)
        : _begin(begin)
        , _end(end)
        , _pos(begin)
    {}

    ~ring_iterator () {}

    reference operator * () const
    {
        return *_pos;
    }

    // Iterator never points to the end
    ring_iterator & operator ++ ()
    {
        if (_pos != _end)
            ++_pos;

        if (_pos == _end)
            _pos = _begin;

        return *this;
    }

    ring_iterator operator ++ (int)
    {
        ring_iterator r(*this);
        ++(*this);
        return r;
    }

    ring_iterator & operator -- ()
    {
        if (_pos == _begin)
            _pos = _end;
        --_pos;

        return *this;
    }

    ring_iterator operator -- (int)
    {
        ring_iterator r(*this);
        --(*this);
        return r;
    }

    friend bool operator == (ring_iterator const & lhs, ring_iterator const & rhs)
    {
        return lhs._pos == rhs._pos;
    }

    friend bool operator != (ring_iterator const & lhs, ring_iterator const & rhs)
    {
        return !(lhs == rhs);
    }

private:
    InputIterator _begin;
    InputIterator _end;
    InputIterator _pos;
};

//template <typename InputIterator>
//void ring_buffer<InputIterator>::push (const value_type & value)
//{
//	PFS_ASSERT(can_push(1));
//
//	if (_tail == _end)
//		_tail = _begin;
//
//	*_tail = value;
//	++_tail;
//}
//
//template <typename InputIterator>
//void ring_buffer<InputIterator>::pop ()
//{
//	if (empty())
//		return;
//
//	_head->~value_type();
//	++_head;
//}
//
} // pfs
