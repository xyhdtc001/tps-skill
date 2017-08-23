#pragma once
#include "NonCopyable.h"


class SemaphorePrivate;

class Semaphore : public NonCopyable
{
public:
	Semaphore(const char* pName);
	~Semaphore();

	void Reset();
	void WaitForSignal(); 
	void Signal();

private:
	SemaphorePrivate* _private; 
};

class SuspendableSemaphore : public NonCopyable
{
public:
	explicit SuspendableSemaphore(bool suspended = true) : m_Semaphore(""), m_Suspended(suspended), m_SuspendedIndefinitely(false) { }
	bool IsSuspended() const { return m_Suspended; }
	void Reset() { m_Semaphore.Reset(); }
	void WaitForSignal() { if (!m_Suspended) m_Semaphore.WaitForSignal(); };
	void Signal() { if (!m_Suspended) m_Semaphore.Signal(); }
	void Resume(bool reset = true);
	void Suspend(bool indefinitely = false);

private:
	volatile bool m_SuspendedIndefinitely;
	volatile bool m_Suspended;
	Semaphore m_Semaphore;
};

inline void SuspendableSemaphore::Resume(bool reset)
{
	if (reset)
		Reset();

	if (!m_SuspendedIndefinitely)
		m_Suspended = false;
}

inline void SuspendableSemaphore::Suspend(bool indefinitely)
{
	m_Suspended = true;
	if (indefinitely)
		m_SuspendedIndefinitely = indefinitely;

	m_Semaphore.Signal(); // release any waiting thread
}
