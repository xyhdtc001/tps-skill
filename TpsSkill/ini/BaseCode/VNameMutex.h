#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 
#include "windows.h"
#endif

#include "basehdr.h"

#if !defined INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

class VNameMutex
{
public:
	enum WAIT_RESULT
	{
		WAIT_RES_SIGNAL = 0,
		WAIT_RES_TIMEOUT,
		WAIT_RES_FAIL,
	};
	
public:
	VNameMutex();
	void create( const char* name );
	~VNameMutex();
	WAIT_RESULT wait( uint32 timeout );
	void release();
	inline bool isInited()
	{
		#if defined METRO
		return false;
		#else
		return _hMutex != NULL;
		#endif
	}

protected:
	HANDLE _hMutex;
};


class VNameMutexLockHelper
{
public:
	VNameMutexLockHelper( VNameMutex* mutex ) : _mutex(NULL)
	{
		if ( !mutex->isInited() )
			return;
		_mutex = mutex;
		mutex->wait( INFINITE );
	}
	~VNameMutexLockHelper()
	{
		if ( _mutex )
			_mutex->release();
	}
protected:
	VNameMutex* _mutex;
};
