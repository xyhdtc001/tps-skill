#pragma once
#include <sys/types.h>
#include "singleton.h"
#include "Thread.h"

class Threading : public Singleton<Threading>
{
public:
	static size_t coresCount();
	static double cpuUsage();
	static ThreadId currentThread();
	static ThreadId mainThread() 
	{ return _mainThread; }
	static bool CurrentThreadIsMainThread();

private:
	Threading();
	ET_SINGLETON_COPY_DENY(Threading)

private:
	static ThreadId _mainThread;
};

inline Threading& threading() { return Threading::instance(); }