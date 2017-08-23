#pragma once
#include "BaseCode.h"

class CMemHeap
{
public:
	CMemHeap(const char* pszClassName, int size);
	~CMemHeap(void);

public:
	void* Alloc(size_t size);
	void Free(void* ptr);
    void ClearMemHeap();
    unsigned long long GetSizeInBytesMemHeap() const;
protected:
	int m_nClassSize;
	long m_lMaxAllocsInHeap;
	char m_strName[32];
	std::list<void*> m_vecFree;
};

inline void* AlignedMemory_allocate(size_t size, size_t alignment)
{
#ifdef WIN32
	return _aligned_malloc(size, alignment);
#elif __ANDROID__
	return ::memalign(alignment, size);
#else//slime
    void * aPtr = NULL;
    if (posix_memalign(&aPtr, alignment, size)) {
        aPtr = NULL;
    }
    return aPtr;
#endif
}
inline void AlignedMemory_deallocate(void* ptr)
{
	if (ptr == NULL)
		return;

#ifdef WIN32
	_aligned_free(ptr);
#elif __ANDROID__
	::free(ptr);
#else//slime
    ::free(ptr);
#endif
}



#ifdef WIN32
#define MS_ALIGN(n) __declspec(align(n))
#else
#define MS_ALIGN(n)	__attribute__((aligned(n)))
#endif
