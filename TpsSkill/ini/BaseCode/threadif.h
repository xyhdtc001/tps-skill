#ifndef THREADIF_HXX
#define THREADIF_HXX

#include "baseType.h"
#include "basehdr.h"

#ifndef WIN32
#include "pthread.h"


typedef pthread_t vthread_t;
typedef pthread_mutex_t vmutex_t;
typedef pthread_attr_t vthread_attr_t;
typedef pthread_cond_t vcondition_t;

const int VTHREAD_PRIORITY_DEFAULT = 0;

const int VTHREAD_STACK_SIZE_DEFAULT = 131072; // in bytes -- 2 ^ 17

const unsigned long VTHREAD_SCHED_MASK = 0x0000000f;
const unsigned long VTHREAD_SCHED_DEFAULT = 0x00000001;
const unsigned long VTHREAD_SCHED_FIFO = 0x00000002;
const unsigned long VTHREAD_SCHED_RR = 0x00000004;

class VThread
{
public:
	VThread( );
	virtual ~VThread( );
	
	bool spawn( void *(*startFunc)(void *),
			   void *startArgs = 0,
			   unsigned long flags = 0,
			   unsigned long priority = VTHREAD_PRIORITY_DEFAULT,
			   int stack_size = VTHREAD_STACK_SIZE_DEFAULT );
	
	void DisableSelfId();
	int join( void **status = 0 );
	int getPriority() const;
	const vthread_t getId() const;
	const vthread_attr_t* getAttributes() const;
	void exit();
	void shutdown();
	bool isShutdown();
	void setparam( int param );
	int getparam();
	static const vthread_t selfId ();
	
private:
	vthread_t myId;        
	pthread_attr_t myAttributes;
	bool bThreadCreated;
	bool _bShutdown;
	int _param;
};
#endif

class ThreadIf
{
public:
	
	ThreadIf();
	
	virtual ~ThreadIf();
	
	void run( unsigned long priority=0 );
	
	void join();
	
	#if defined WIN32
	inline const unsigned long getId()
	{
		return _threadId;
	}
	#endif
	
	virtual void shutdown()
	{
		_shutdown = true;
	}
	
	inline bool isShutdown() const
	{
		return _shutdown;
	}

	inline bool isRunning()
	{
		return _bRun;
	}
	
	virtual void thread() = 0;

public:
	volatile bool _bRun;

private:
	bool _shutdown;

	#if defined WIN32
	DWORD _threadId;
	HANDLE _hThread;
	#else
	#if defined METRO
	#else
	#endif
	#endif
	
#ifndef WIN32
protected:
	VThread itsThread;
#endif
};

#endif
