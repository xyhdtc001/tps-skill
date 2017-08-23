#pragma once
#ifdef WIN32
typedef long ReferenceCounterType;
#else
typedef int ReferenceCounterType;
#endif

class ReferenceCounter
{
public:
	ReferenceCounter(void);
	virtual ~ReferenceCounter(void){}

	ReferenceCounterType retain();
	ReferenceCounterType release();

	ReferenceCounterType referenceCount()
	{ return _referenceCount; }

private:
	volatile ReferenceCounterType _referenceCount;
};
