/* 
 * File:   active_queue.hpp
 * Author: wladt
 *
 * Created on September 22, 2016
 * Moved to pfs-mpl and re-implemented on June 14, 2017
 */

#ifndef __PFS_ACTIVE_QUEUE_HPP__
#define __PFS_ACTIVE_QUEUE_HPP__

#include <pfs/mutex.hpp>
#include <pfs/cxx/cxx98/binder.hpp>
#include <pfs/traits/sequence_container.hpp>

namespace pfs {

template <template <typename> class SequenceContainer
    , typename BasicLockable // see [C++ concepts: BasicLockable](http://en.cppreference.com/w/cpp/concept/BasicLockable)>
    , int GcThreshold = 256> 
class active_queue
{
private:
    enum state_enum
    {
          FREE      // Candidate for memory deallocation
        , BUSY
        , PROCESSING
    };
    
    struct value 
    {
        int8_t state;
        binder_base<void> * p;
        
        value (binder_base<void> * ptr) 
            : state(BUSY)
            , p(ptr) 
        {}
        
        ~value ()
        {
            if (p)
                delete p;
        }
    };

    typedef value value_type;
    typedef BasicLockable mutex_type;
    typedef traits::sequence_container<value_type, SequenceContainer> sequence_container_type;
    typedef typename sequence_container_type::size_type size_type;
    typedef typename sequence_container_type::iterator  iterator;

    static size_type const GC_THRESHOLD = GcThreshold;
    
private:
    sequence_container_type _sequence;
    size_type               _count;
    mutable mutex_type      _mutex;

private:
    /* @brief Creates (using default constructor) and inserts element of type T at the end.
     */
    void push_helper (binder_base<void> * ptr)
    {
        unique_lock<mutex_type> locker(_mutex);
        
        if (_sequence.size() > GC_THRESHOLD 
                && _sequence.begin()->state == FREE) {
            gc();
        }
        
        value_type v(ptr);
        _sequence.push_back(v);
        v.p = 0; // Do not destroy 

        ++_count;
    }
    
    iterator front_busy ()
    {
        iterator pos = _sequence.begin();
        iterator end = _sequence.end();
        
        // TODO Need an optimization to access to the first BUSY element
        while (pos != end) {
            if (pos->state == BUSY) {
                pos->state = PROCESSING;
                break;
            }
            ++pos;
        }
            
        return pos;
    }
    
    // Garbage collector
    void gc ()
    {
        iterator pos = _sequence.begin();
        iterator end = _sequence.end();
        
        while (pos != end && pos->state == FREE)
            ++pos;

        _sequence.erase(_sequence.begin(), pos);
    }
    
public:
	active_queue ()
        : _count(0)
	{}

	virtual ~active_queue ()
	{
		call_all();
        gc();
	}
    
	bool empty () const
	{
        unique_lock<mutex_type> locker(_mutex);
		return _count == 0;
	}

    /**
     * @return Number of elements ready to call or in process (already calling).
     */
  	size_type count () const
	{
        unique_lock<mutex_type> locker(_mutex);
		return _count;
	}

	size_type size () const
	{
        unique_lock<mutex_type> locker(_mutex);
		return _sequence.size();
	}

//#if __cplusplus >= 201103L
//#   error Implement using variadic templates    
//#else    
	void push_function (void (* f) ())
	{
        push_helper(new binder_function0<void>(f));
    }

   	template <typename Arg1>
	void push_function (void (* f) (Arg1), Arg1 a1)
	{
        push_helper(new binder_function1<void, Arg1>(f, a1));
	}
    
	template <typename Arg1, typename Arg2>
	void push_function (void (* f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
        push_helper(new binder_function2<void, Arg1, Arg2>(f, a1, a2));
	}

	template <typename Arg1, typename Arg2, typename Arg3>
	void push_function (void (* f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
        push_helper(new binder_function3<void, Arg1, Arg2, Arg3>(f, a1, a2, a3));
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void push_function (void (* f) (Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
        push_helper(new binder_function4<void, Arg1, Arg2, Arg3, Arg4>(f, a1, a2, a3, a4));
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	void push_function (void (* f) (Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
        push_helper(new binder_function5<void, Arg1, Arg2, Arg3, Arg4, Arg5>(f, a1, a2, a3, a4, a5));
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	void push_function (void (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
        push_helper(new binder_function6<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>(f, a1, a2, a3, a4, a5, a6));
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	void push_function (void (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
        push_helper(new binder_function7<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>(f, a1, a2, a3, a4, a5, a6, a7));
	}

  	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	void push_function (void (* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
        push_helper(new binder_function8<void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>(f, a1, a2, a3, a4, a5, a6, a7, a8));
	}
    
    template <typename C>
	void push_method (void (C::* f) (), C * c)
	{
        push_helper(new binder_method0<C, void>(f, c));
	}

	template <typename C, typename Arg1>
	void push_method (void (C::* f) (Arg1), C * c, Arg1 a1)
	{
        push_helper(new binder_method1<C, void, Arg1>(f, c, a1));
	}
    
	template <typename C, typename Arg1, typename Arg2>
	void push_method (void (C::* f) (Arg1, Arg2), C * c, Arg1 a1, Arg2 a2)
	{
        push_helper(new binder_method2<C, void, Arg1, Arg2>(f, c, a1, a2));
	}

	template <typename C, typename Arg1, typename Arg2, typename Arg3>
	void push_method (void (C::* f) (Arg1, Arg2, Arg3), C * c, Arg1 a1, Arg2 a2, Arg3 a3)
	{
        push_helper(new binder_method3<C, void, Arg1, Arg2, Arg3>(f, c, a1, a2, a3));
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
        push_helper(new binder_method4<C, void, Arg1, Arg2, Arg3, Arg4>(f, c, a1, a2, a3, a4));
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
        push_helper(new binder_method5<C, void, Arg1, Arg2, Arg3, Arg4, Arg5>(f, c, a1, a2, a3, a4, a5));
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
        push_helper(new binder_method6<C, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>(f, c, a1, a2, a3, a4, a5, a6));
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
        push_helper(new binder_method7<C, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>(f, c, a1, a2, a3, a4, a5, a6, a7));
	}

  	template <typename C, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	void push_method (void (C::* f) (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), C * c, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
        push_helper(new binder_method8<C, void, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>(f, c, a1, a2, a3, a4, a5, a6, a7, a8));
	}
//#endif
    
  	void call ()
    {
        unique_lock<mutex_type> locker(_mutex);
        
        iterator pos = front_busy();
        
        if (pos != _sequence.end()) {
            --_count;
            locker.unlock();

            (*(pos->p))();
            
            locker.lock();
            
            pos->state = FREE;
        }
    }
    
	void call_all ()
    {
        while (!this->empty())
            call();
    }
};

} // pfs

#endif /* __PFS_ACTIVE_QUEUE_HPP__ */