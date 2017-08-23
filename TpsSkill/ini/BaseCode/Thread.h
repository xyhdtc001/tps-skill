#pragma once
#include <string>

typedef unsigned long ThreadResult;
typedef unsigned long ThreadId;

class ThreadPrivate;
class Thread
{
public:
	static void sleep(unsigned long millisecond);

public:
	Thread(const char* pszName);
	virtual ~Thread();

	void run();
    void stop();
	
	bool running() const;
	bool suspended() const;

	ThreadId id() const;
	virtual ThreadResult main() = 0;

protected:
	void suspend();
	void resume();
    void setName(const char* pszName);

private:
    void terminate(int result = 0);

private:
	Thread(const Thread&)
	{ }

	Thread& operator = (const Thread&)
	{ return *this; }

private:
	friend class ThreadPrivate;
	ThreadPrivate* _private;
    std::string m_strName;
};


// Memory barrier.
//
// Necessary to call this when using volatile to order writes/reads in multiple threads.
inline void UnityMemoryBarrier()
{
// #ifdef WIN32
// 	#ifdef MemoryBarrier
// 	MemoryBarrier();
// 	#else
// 	long temp;
// 	__asm xchg temp,eax;
// 	#endif
// 	
// #elif __APPLE__
// 	OSMemoryBarrier();
// #endif
}
