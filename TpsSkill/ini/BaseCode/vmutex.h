#pragma  once

#include "baseType.h"
#include "basehdr.h"

#ifndef WIN32
#include "pthread.h"
#endif

class VMutex
{
public:
	VMutex()
	{
#ifdef WIN32
		#ifdef METRO
		#else
		InitializeCriticalSection( &_critSec );
		#endif
#else
        pthread_mutex_init(&_mutex, NULL);
#endif
	}

	bool trylock() const
	{
#ifdef WIN32
#ifdef METRO
#else
		EnterCriticalSection( &_critSec );
#endif
#else
		int result = pthread_mutex_trylock(&_mutex);
		if (result != 0) {
			LogOut( MAIN, LOG_ERR, "assert: pthread_mutex_lock: %s", strerror(result) );
			//ASSERT(0);
			return false;
		}
#endif
		return true;
	}

	void lock() const
	{
#ifdef WIN32
		#ifdef METRO
		#else
		EnterCriticalSection( &_critSec );
		#endif
#else
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0) {
            LogOut( MAIN, LOG_ERR, "assert: pthread_mutex_lock: %s", strerror(result) );
            //ASSERT(0);
        }
#endif
	}

	void unlock() const
	{
#ifdef WIN32
		LeaveCriticalSection( &_critSec );
#else
        int result = pthread_mutex_unlock(&_mutex);
        if (result != 0) {
            LogOut( MAIN, LOG_ERR, "assert: pthread_mutex_unlock: %s", strerror(result) );
            //ASSERT(0);
        }
#endif
	}
	
	~VMutex()
	{
#ifdef WIN32
		DeleteCriticalSection( &_critSec );
#else
        pthread_mutex_destroy(&_mutex);
#endif
	}

protected:
#ifdef WIN32
	mutable CRITICAL_SECTION _critSec ;
#else
    mutable pthread_mutex_t _mutex;
#endif
};