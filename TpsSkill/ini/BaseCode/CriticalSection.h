#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#else
#include <errno.h>
#include <pthread.h>
#endif

class CriticalSection
{
public:
	CriticalSection();
	~CriticalSection();

	void enter();
	void leave();
	bool tryLock();
private:
#ifdef WIN32
	RTL_CRITICAL_SECTION _cs;
#else
	pthread_mutex_t mutex;
#endif
};

class CriticalSectionScope
{
public:
	CriticalSectionScope(CriticalSection& section) : _cs(section)
	{ _cs.enter(); }

	~CriticalSectionScope()
	{ _cs.leave(); }

private:
	CriticalSectionScope& operator = (const CriticalSectionScope&) 
	{ return *this; }

private:
	CriticalSection& _cs;
};