#ifndef _LOCK_HELPER_H_
#define _LOCK_HELPER_H_

#include "vmutex.h"

class LockHelper
{
public:
	LockHelper( const VMutex* mutex )
	{
		_mutex = mutex;
		mutex->lock();
	}
	~LockHelper()
	{
		_mutex->unlock();
	}

protected:
	const VMutex* _mutex;
};

#endif
