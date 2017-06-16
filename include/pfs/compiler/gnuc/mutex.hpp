namespace pfs {

inline mutex::mutex ()
{
    pthread_mutex_init(& _mutex, 0);
}

inline mutex::~mutex ()
{
    pthread_mutex_destroy(& _mutex); 
}

inline void mutex::lock ()
{
    pthread_mutex_lock(& _mutex);
}

inline bool mutex::try_lock ()
{
    return ! pthread_mutex_trylock(& _mutex);
}

inline void mutex::unlock ()
{
    pthread_mutex_unlock(& _mutex);
}


inline recursive_mutex::recursive_mutex ()
{
	pthread_mutexattr_t mutex_attr;
	pthread_mutexattr_init(& mutex_attr);
	pthread_mutexattr_settype(& mutex_attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(& _mutex, & mutex_attr);
}

inline recursive_mutex::~recursive_mutex ()
{
    pthread_mutex_destroy(& _mutex);
}

inline void recursive_mutex::lock ()
{
    pthread_mutex_lock(& _mutex);
}

inline bool recursive_mutex::try_lock ()
{
    return ! pthread_mutex_trylock(& _mutex);
}
    
inline void recursive_mutex::unlock ()
{
    pthread_mutex_unlock(& _mutex);
}

}