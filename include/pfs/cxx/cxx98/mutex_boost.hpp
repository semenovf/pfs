#pragma once
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

namespace pfs {

typedef boost::mutex mutex;
typedef boost::defer_lock_t defer_lock_t;
typedef boost::try_to_lock_t try_to_lock_t;
typedef boost::adopt_lock_t adopt_lock_t;

template <typename Mutex>
class lock_guard : public boost::lock_guard<Mutex>
{ 
    typedef boost::lock_guard<Mutex> base_class;

public:
    typedef Mutex mutex_type;
    
public:
    explicit lock_guard (mutex_type & m) : base_class(m) {}
    lock_guard (mutex_type & m, adopt_lock_t t) : base_class(m, t) {}
};

template <typename Mutex>
class unique_lock : public boost::unique_lock<Mutex>
{
    typedef boost::unique_lock<Mutex> base_class;

public:
    typedef Mutex mutex_type;
    
public:
    unique_lock() : base_class() {}
    explicit unique_lock (mutex_type & m) : base_class(m) {}
    unique_lock (mutex_type & m, defer_lock_t t) : base_class(m, t) {}
    unique_lock (mutex_type & m, try_to_lock_t t) : base_class(m, t) {}
    unique_lock (mutex_type & m, adopt_lock_t t) : base_class(m, t) {}
};
    
} // pfs

