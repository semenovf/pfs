#ifndef __PFS_FUNCTION_MAP_HPP__
#define __PFS_FUNCTION_MAP_HPP__

#include <pfs/type_traits.hpp>
#include <pfs/memory.hpp>
#include <pfs/mutex.hpp>
#include <pfs/cxx/cxx98/binder.hpp>
#include <pfs/traits/associative_container.hpp>

namespace pfs {

template <typename Key
    , typename R
    , template <typename> class AssociativeContainerImplType
    , typename BasicLockable = pfs::mutex> // see [C++ concepts: BasicLockable](http://en.cppreference.com/w/cpp/concept/BasicLockable)
class active_map_base
{
public:
  	typedef Key key_type;
    typedef R   result_type;
    
protected:
    typedef pfs::shared_ptr<binder_base<void> >  value_type;
    typedef BasicLockable                        mutex_type;
    typedef pfs::traits::associative_container<
              pfs::traits::kv<key_type, value_type>
            , AssociativeContainerImplType>      map_type;
    typedef typename map_type::size_type         size_type;
    typedef typename map_type::iterator          iterator;

protected:
    map_type           _map;
    mutable mutex_type _mutex;
    
protected:
    void insert_helper (key_type const & key, shared_ptr<binder_base<void> > ptr)
    {
        unique_lock<mutex_type> locker(_mutex);
        _map.insert(key, ptr);
    }

public:
    active_map_base ()
	{}

	virtual ~active_map_base ()
	{
		clear();
	}
    
	bool empty () const
	{
		return size() == 0;
	}

	size_type size () const
	{
        unique_lock<mutex_type> locker(_mutex);
		return _map.size();
	}
    
	void insert_function (key_type const & key, result_type (* f) ())
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_function0<result_type>(f)));
	}

  	template <typename Arg1>
	void insert_function (key_type const & key, result_type (*f) (Arg1), Arg1 a1)
	{
        insert_helper(key, shared_ptr<binder_base<void> >(new binder_function1<result_type, Arg1>(f, a1)));
	}

    template <typename Arg1, typename Arg2>
	void insert_function (key_type const & key, result_type (*f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
        insert_helper(key, shared_ptr<binder_base<void> >(new binder_function2<result_type, Arg1, Arg2>(f, a1, a2)));
	}

   	template <typename Arg1, typename Arg2, typename Arg3>
	void insert_function (key_type const & key, result_type (*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_function3<result_type, Arg1, Arg2, Arg3>(f, a1, a2, a3)));
	}

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void insert_function (key_type const & key, result_type (*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_function4<result_type, Arg1, Arg2, Arg3, Arg4>(f, a1, a2, a3, a4)));
	}

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	void insert_function (key_type const & key, result_type (*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_function5<result_type, Arg1, Arg2, Arg3, Arg4, Arg5>(f, a1, a2, a3, a4, a5)));
	}

	template <typename Class>
	void insert_method (key_type const & key, result_type (Class::* f) (), Class * p)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_method0<Class, result_type>(f, p)));
	}

	template <typename Class, typename Arg1>
	void insert_method (key_type const & key, result_type (Class::*f) (Arg1), Class * p, Arg1 a1)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_method1<Class, result_type, Arg1>(f, p, a1)));
	}

	template <typename Class, typename Arg1, typename Arg2>
	void insert_method (key_type const & key, result_type (Class::*f) (Arg1, Arg2), Class * p, Arg1 a1, Arg2 a2)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_method2<Class, result_type, Arg1, Arg2>(f, p, a1, a2)));
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3>
	void insert_method (key_type const & key, result_type (Class::*f) (Arg1, Arg2, Arg3), Class * p, Arg1 a1, Arg2 a2, Arg3 a3)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_method3<Class, result_type, Arg1, Arg2, Arg3>(f, p, a1, a2, a3)));
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void insert_method (key_type const & key, result_type (Class::*f) (Arg1, Arg2, Arg3), Class * p, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_method4<Class, result_type, Arg1, Arg2, Arg3, Arg4>(f, p, a1, a2, a3, a4)));
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	void insert_method (key_type const & key, result_type (Class::*f) (Arg1, Arg2, Arg3), Class * p, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		insert_helper(key, shared_ptr<binder_base<void> >(new binder_method5<Class, result_type, Arg1, Arg2, Arg3, Arg4, Arg5>(f, p, a1, a2, a3, a4, a5)));
	}
    
    void clear ()
	{
        unique_lock<mutex_type> locker(_mutex);
        _map.clear();
	}

	void erase (key_type const & key)
	{
		unique_lock<mutex_type> locker(_mutex);
        _map.erase(key);
	}
};

template <typename Key
    , typename R
    , template <typename> class AssociativeContainerImplType
    , typename BasicLockable = pfs::mutex>
class active_map : public active_map_base<Key, R, AssociativeContainerImplType, BasicLockable>
{
    typedef active_map_base<Key, R, AssociativeContainerImplType, BasicLockable> base_class;
    typedef typename base_class::map_type    map_type;
    typedef typename base_class::mutex_type  mutex_type;
    typedef typename base_class::key_type    key_type;
    typedef typename base_class::result_type result_type;
    typedef typename base_class::iterator    iterator;
    
public:
    active_map () : base_class() {}
    
    virtual ~active_map () {}
    
    result_type call (key_type const & key, bool * called = 0)
    {
        unique_lock<mutex_type> locker(this->_mutex);
        iterator pos = this->_map.find(key);
        result_type result;
     
        if (pos != this->_map.end()) {
            locker.unlock();

            result = (*map_type::mapped_reference(pos).p)();
           
            if (called)
                *called = true;
            locker.lock();
        } else {
            locker.unlock();
            if (called)
               *called = false;
        }
        
        return result;
    }
    
    result_type call_and_erase (key_type const & key, bool * called = 0)
	{
		result_type r = call(key, called);
		this->erase(key);
        return r;
	}
};

template <typename Key
    , template <typename> class AssociativeContainerImplType
    , typename BasicLockable>
class active_map<Key, void, AssociativeContainerImplType, BasicLockable>
        : public active_map_base<Key, void, AssociativeContainerImplType, BasicLockable>
{
    typedef active_map_base<Key, void, AssociativeContainerImplType, BasicLockable> base_class;
    typedef typename base_class::map_type    map_type;
    typedef typename base_class::mutex_type  mutex_type;
    typedef typename base_class::key_type    key_type;
    typedef typename base_class::iterator    iterator;

public:
    active_map () : base_class() {}
    
    virtual ~active_map () {}
    
    void call (key_type const & key, bool * called = 0)
    {
        unique_lock<mutex_type> locker(this->_mutex);
        iterator pos = this->_map.find(key);
     
        if (pos != this->_map.end()) {
            locker.unlock();

            (*map_type::mapped_reference(pos))();
           
            if (called)
                *called = true;
            locker.lock();
        } else {
            locker.unlock();
            if (called)
               *called = false;
        }
    }
    
    void call_and_erase (key_type const & key, bool * called = 0)
	{
		call(key, called);
		this->erase(key);
	}
};

} // pfs

#endif /* __PFS_FUNCTION_MAP_HPP__ */

