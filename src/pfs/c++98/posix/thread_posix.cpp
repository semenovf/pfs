//#include <sched.h>
//#include <errno.h>
//#include <unistd.h>
#include <pthread.h>

#if __COMMENT__

//#include <pfs/utility.hpp>
//
//#include "pfs/thread.hpp"
//#include "pfs/error_code.hpp"
//#include "../thread_p.hpp"

#if defined(PFS_OS_LINUX) && !defined(SCHED_IDLE)
// from linux/sched.h
#	define SCHED_IDLE    5
#endif

#if defined(PFS_OS_DARWIN) || !defined(PFS_OS_OPENBSD) && defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
#	define PFS_HAS_THREAD_PRIORITY_SCHEDULING
#endif

namespace pfs {

static struct main_thread
{
	pthread_t _threadHandle;
	main_thread () { _threadHandle = pthread_self(); }
	bool equals_to (pthread_t otherThreadHandle) const
	{
		return (pthread_equal(_threadHandle, otherThreadHandle) != 0);
	}
} __main_thread;

enum { thread_priority_reset_flag = 0x80000000 };

#if defined(PFS_OS_LINUX) && defined(__GLIBC__) && (defined(PFS_CC_GNU) || defined(PFS_CC_INTEL)) && !defined(PFS_LINUXBASE)
/* LSB doesn't have __thread, https://lsbbugs.linuxfoundation.org/show_bug.cgi?id=993 */
#	define PFS_HAVE_TLS
#endif

#if defined(PFS_CC_XLC) || defined (PFS_CC_SUN)
#	define PFS_HAVE_TLS
#endif

#ifdef PFS_HAVE_TLS
static __thread thread_data * currentThreadData = 0;
#endif

static pthread_once_t current_thread_data_once = PTHREAD_ONCE_INIT;
static pthread_key_t current_thread_data_key;

static void destroy_current_thread_data (void * p)
{
    // POSIX says the value in our key is set to zero before calling
    // this destructor function, so we need to set it back to the
    // right value...
    pthread_setspecific(current_thread_data_key, p);
    thread_data * data = static_cast<thread_data *>(p);
//    if (data->isAdopted) {
//        QThread *thread = data->thread;
//        Q_ASSERT(thread);
//        QThreadPrivate *thread_p = static_cast<QThreadPrivate *>(QObjectPrivate::get(thread));
//        Q_ASSERT(!thread_p->finished);
//        thread_p->finish(thread);
//    }
    data->deref();

    // ... but we must reset it to zero before returning so we aren't
    // called again (POSIX allows implementations to call destructor
    // functions repeatedly until all values are zero)
    pthread_setspecific(current_thread_data_key, 0);
}

static void create_current_thread_data_key ()
{
    pthread_key_create(& current_thread_data_key, destroy_current_thread_data);
}

static void destroy_current_thread_data_key ()
{
    pthread_once(&current_thread_data_once, create_current_thread_data_key);
    pthread_key_delete(current_thread_data_key);

    // Reset current_thread_data_once in case we end up recreating
    // the thread-data in the rare case of QObject construction
    // after destroying the QThreadData.
    pthread_once_t pthread_once_init = PTHREAD_ONCE_INIT;
    current_thread_data_once = pthread_once_init;
}
PFS_AUTO_DTOR_FUNCTION(destroy_current_thread_data_key)


// Utility functions for getting, setting and clearing thread specific data.
static thread_data * get_thread_data ()
{
#ifdef PFS_HAVE_TLS
    return currentThreadData;
#else
    pthread_once(& current_thread_data_once, create_current_thread_data_key);
    return reinterpret_cast<thread_data *>(pthread_getspecific(current_thread_data_key));
#endif
}

static void set_thread_data (thread_data * data)
{
#ifdef PFS_HAVE_TLS
    currentThreadData = data;
#endif
    pthread_once(& current_thread_data_once, create_current_thread_data_key);
    pthread_setspecific(current_thread_data_key, data);
}

void thread_data::clear_current_thread_data()
{
	#ifdef PFS_HAVE_TLS
		currentThreadData = 0;
	#endif
		pthread_setspecific(current_thread_data_key, 0);
}

thread_data * thread_data::current ()
{
	thread_data * data = get_thread_data();
	PFS_ASSERT(data);
    return data;
}

bool thread_impl::is_main_thread () const
{
	return __main_thread.equals_to(_threadHandle);
}

void * thread_impl::start (void * arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_cleanup_push(thread_impl::finish, arg);

    thread * thr = reinterpret_cast<thread *>(arg);
    thread_data * data = thread_data::get2(thr);

    {
    	thread_impl * d = thr->_d;
    	pfs::lock_guard<pfs::mutex> locker(d->_mutex);

        // do we need to reset the thread priority?
        if (int(d->_priority) & thread_priority_reset_flag) {
            d->set_priority(thread::priority_enum(d->_priority & ~thread_priority_reset_flag));
        }

        data->_threadHandle = pthread_self();
        set_thread_data(data);

        data->ref();
        data->_quitNow = d->_exited;
    }

//#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
//    // sets the name of the current thread.
//    QString objectName = thr->objectName();
//
//    if (Q_LIKELY(objectName.isEmpty()))
//        setCurrentThreadName(thr->d_func()->thread_id, thr->metaObject()->className());
//    else
//        setCurrentThreadName(thr->d_func()->thread_id, objectName.toLocal8Bit());
//#endif

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();

    thr->run();

    pthread_cleanup_pop(1);

    return 0;
}

void thread_impl::finish (void * arg)
{
    thread * thr = reinterpret_cast<thread *>(arg);
    thread_impl * d = thr->_d;
    pfs::lock_guard<pfs::mutex> locker(d->_mutex);

    d->_isInFinish = true;
    d->_priority = thread::inherit_priority;
//    void * data = & d->_data->_tls;
//    locker.unlock();
//    QThreadStorageData::finish((void **)data);
//    locker.relock();

    d->_threadHandle = 0;
    d->_running = false;
    d->_finished = true;
    d->_isInFinish = false;
    d->_interruptionRequested = false;

    d->_threadDone.notify_all();
}

//Qt::HANDLE QThread::currentThreadId() Q_DECL_NOTHROW
//{
//    // requires a C cast here otherwise we run into trouble on AIX
//    return (Qt::HANDLE)pthread_self();
//}

int thread::ideal_thread_count ()
{
    int cores = -1;
    cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
    return cores;
}

void thread::yield_current_thread()
{
    sched_yield();
}

static timespec __make_timespec (time_t secs, intmax_t nsecs)
{
    struct timespec ts;
    ts.tv_sec = secs;
    ts.tv_nsec = nsecs;
    return ts;
}

inline void __nanosleep (struct timespec & ts)
{
	int rc;
	while ((rc = nanosleep(& ts, & ts)) < 0 && errno == EINTR)
		;
}

void thread::sleep (uintmax_t secs)
{
	struct timespec ts =  __make_timespec(secs, 0);
    __nanosleep(ts);
}

void thread::msleep (uintmax_t msecs)
{
	struct timespec ts = __make_timespec(msecs / 1000, msecs % 1000 * 1000 * 1000);
    __nanosleep(ts);
}

void thread::usleep (uintmax_t usecs)
{
	struct timespec ts = __make_timespec(usecs / 1000 / 1000, usecs % (1000*1000) * 1000);
    __nanosleep(ts);
}

#ifdef PFS_HAS_THREAD_PRIORITY_SCHEDULING
// Does some magic and calculate the Unix scheduler priorities
// sched_policy is IN/OUT: it must be set to a valid policy before calling this function
// sched_priority is OUT only
static bool calculateUnixPriority (int priority, int *sched_policy, int *sched_priority)
{
#ifdef SCHED_IDLE
    if (priority == thread::idle_priority) {
        *sched_policy = SCHED_IDLE;
        *sched_priority = 0;
        return true;
    }
    const int lowestPriority = thread::lowest_priority;
#else
    const int lowestPriority = thread::idle_priority;
#endif
    const int highestPriority = thread::time_critical_priority;

	int prio_min = sched_get_priority_min(*sched_policy);
	int prio_max = sched_get_priority_max(*sched_policy);

    if (prio_min == -1 || prio_max == -1)
        return false;

    int prio;
    // crudely scale our priority enum values to the prio_min/prio_max
    prio = ((priority - lowestPriority) * (prio_max - prio_min) / highestPriority) + prio_min;
    prio = pfs::max(prio_min, pfs::min(prio_max, prio));

    *sched_priority = prio;
    return true;
}
#endif

void thread::start (priority_enum prior)
{
	//pfs::lock_guard<pfs::mutex> locker(_d->_mutex);
    pfs::unique_lock<pfs::mutex> locker(_d->_mutex);

    if (_d->_isInFinish)
        //_d->_threadDone.wait(_d->_mutex);
        _d->_threadDone.wait(locker);

    if (_d->_running)
        return;

    _d->_running = true;
    _d->_finished = false;
    _d->_returnCode = 0;
    _d->_exited = false;
    _d->_interruptionRequested = false;

    pthread_attr_t attr;
    pthread_attr_init(& attr);
    pthread_attr_setdetachstate(& attr, PTHREAD_CREATE_DETACHED);

    _d->_priority = prior;

#if defined(PFS_HAS_THREAD_PRIORITY_SCHEDULING)
    switch (prior) {
    case inherit_priority:
        {
            pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
            break;
        }

    default:
        {
            int sched_policy;
            if (pthread_attr_getschedpolicy(& attr, & sched_policy) != 0) {
                // failed to get the scheduling policy, don't bother
                // setting the priority
                PFS_DEBUG(std::cerr
                		<< "pfs::thread::start(): cannot determine default scheduler policy\n");
                break;
            }

            int prio;
            
            if (!calculateUnixPriority(prior, & sched_policy, &prio)) {
                // failed to get the scheduling parameters, don't
                // bother setting the priority
            	PFS_DEBUG(std::cerr
            			<< "pfs::thread::start(): cannot determine scheduler priority range\n");
                break;
            }

            sched_param sp;
            sp.sched_priority = prio;

            if (pthread_attr_setinheritsched(& attr, PTHREAD_EXPLICIT_SCHED) != 0
                || pthread_attr_setschedpolicy(& attr, sched_policy) != 0
                || pthread_attr_setschedparam(& attr, & sp) != 0) {
                // could not set scheduling hints, fallback to inheriting them
                // we'll try again from inside the thread
                pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
                _d->_priority = priority_enum(prior | thread_priority_reset_flag);
            }
            break;
        }
    }
#endif // PFS_HAS_THREAD_PRIORITY_SCHEDULING

    if (_d->_stackSize > 0) {
#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && (_POSIX_THREAD_ATTR_STACKSIZE-0 > 0)
        int rc = pthread_attr_setstacksize(& attr, _d->_stackSize);
#else
        int rc = ENOSYS; // stack size not supported, automatically fail
#endif // _POSIX_THREAD_ATTR_STACKSIZE

        if (rc) {
        	PFS_DEBUG(string errstr);
        	PFS_DEBUG(std::cerr
        			<< "pfs::thread::start(): thread stack size error: "
        			<< to_string(pfs::error_code(rc)) << std::endl);

            // we failed to set the stacksize, and as the documentation states,
            // the thread will fail to run...
            _d->_running = false;
            _d->_finished = false;
            return;
        }
    }

    int rc =
        pthread_create(& _d->_threadHandle, & attr, pfs::thread_impl::start, this);

    if (rc == EPERM) {
        // caller does not have permission to set the scheduling
        // parameters/policy
#if defined(PFS_HAS_THREAD_PRIORITY_SCHEDULING)
        pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
#endif
        rc = pthread_create(& _d->_threadHandle, & attr, thread_impl::start, this);
    }

    pthread_attr_destroy(& attr);

    if (rc) {
    	PFS_DEBUG(string errstr);
    	PFS_DEBUG(std::cerr
    			<< "pfs::thread::start(): thread creation error: "
				<< to_string(pfs::error_code(rc)) << std::endl);

        _d->_running = false;
        _d->_finished = false;
        _d->_threadHandle = 0;
    }
}

void thread::terminate ()
{
	pfs::lock_guard<pfs::mutex> locker(_d->_mutex);

    if (!_d->_threadHandle)
        return;

    PFS_ASSERT(pthread_cancel(_d->_threadHandle) == 0);
}

bool thread::wait (uintmax_t timeout)
{
	//pfs::lock_guard<pfs::mutex> locker(_d->_mutex);
    pfs::unique_lock<pfs::mutex> locker(_d->_mutex);

	if (_d->_data->_threadHandle == pthread_self()) {
		PFS_DEBUG(std::cerr << _Tr("Thread attempt to wait on itself\n"));
		return false;
	}

    if (_d->_finished || ! _d->_running)
        return true;

    while (_d->_running) {
        //if (!_d->_threadDone.wait_until(_d->_mutex, timeout))
        if (!_d->_threadDone.wait_until(locker, timeout))
            return false;
    }
    return true;
}

void thread::set_termination_enabled (bool enabled)
{
    PFS_ASSERT_X(current_thread() != 0
    		, _Tr("pfs::thread::setTerminationEnabled(): current thread was not started with pfs::thread."));

    pthread_setcancelstate(enabled ? PTHREAD_CANCEL_ENABLE : PTHREAD_CANCEL_DISABLE, NULL);

    if (enabled)
        pthread_testcancel();
}

// Caller must lock the mutex
void thread_impl::set_priority (thread::priority_enum threadPriority)
{
    _priority = threadPriority;

#ifdef PFS_HAS_THREAD_PRIORITY_SCHEDULING
    int sched_policy;
    sched_param param;

    if (pthread_getschedparam(_threadHandle, & sched_policy, & param) != 0) {
        // failed to get the scheduling policy, don't bother setting
        // the priority
        PFS_DEBUG(std::cerr
        		<< "pfs::thread_impl::setPriority(): cannot get scheduler parameters\n");
        return;
    }

    int prio;
    if (!calculateUnixPriority(_priority, & sched_policy, & prio)) {
        // failed to get the scheduling parameters, don't
        // bother setting the priority
        PFS_DEBUG(std::cerr
        		<< "pfs::thread_impl::setPriority(): cannot determine scheduler priority range\n");
        return;
    }

    param.sched_priority = prio;
    int status = pthread_setschedparam(_threadHandle, sched_policy, & param);

#	ifdef SCHED_IDLE
    // were we trying to set to idle priority and failed?
    if (status == -1 && sched_policy == SCHED_IDLE && errno == EINVAL) {
        // reset to lowest priority possible
        pthread_getschedparam(_threadHandle, & sched_policy, & param);
        param.sched_priority = sched_get_priority_min(sched_policy);
        pthread_setschedparam(_threadHandle, sched_policy, & param);
    }
#	else
    PFS_UNUSED(status);
#	endif // SCHED_IDLE
#endif
}

} // pfs

#endif // __COMMENT__