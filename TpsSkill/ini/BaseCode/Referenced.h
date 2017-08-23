#ifndef _CREFERENCED_H
#define _CREFERENCED_H 1

#include "BaseMacro.h"
#include "StaticFunc.h"
#include "ref_ptr.h"
// namespace tq
// {
/** @script{interface} */
class  CReferenced
{
public:
	CReferenced():_refCount(0){}
	CReferenced(const CReferenced&):_refCount(0){}

	virtual ~CReferenced(){ 
		if (_refCount>0)
		{
			ASSERT(false);
		}
	}

    inline CReferenced& operator = (const CReferenced&) { return *this; }

    /** Increment the reference count by one, indicating that 
        this object has another pointer which is referencing it.*/
	virtual int ref() const{return ++_refCount;}
    
    /** Decrement the reference count by one, indicating that 
        a pointer to this object is referencing it.  If the
        reference count goes to zero, it is assumed that this object
        is no longer referenced and is automatically deleted.*/
	virtual int unref() const{
		int newRef = --_refCount;
		bool needDelete = (newRef == 0);
		if (needDelete)
		{
			delete this;
		}

		return newRef;
	}
    
	inline int GetRefCount() const{return _refCount;}
protected:
	mutable int	_refCount;
};

//}
#endif
