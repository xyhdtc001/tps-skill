#pragma once
#include "Referenced.h"
#include "stream.h"

#pragma pack(1)
class MemStream : public CReferenced, public Stream
{
public:
	MemStream( uint32 size );
	uint32 getStreamSize();

	bool read( const uint32 size,  void* buff );
	bool write( const uint32 size, const void* buff );
	uint32 readEx( uint32 size, void *buf );
	bool eof();
	bool hasCapability( const Capability caps ) const;
	uint32 getWritePos() const;
	bool setWritePos( const uint32 pos );
	bool setReadPos( const uint32 pos );

	virtual void* getPoint();

	void* operator new( size_t size, size_t len );
	void operator delete( void * p, size_t size );
	void operator delete( void * p);

	ref_ptr<MemStream> clone();

	 uint32 _inStart;
	 uint32 _outStart;
	 uint32 _size;
	 uint8  _data[1];				///< Start of memory data
};
#pragma pack()

/*
class FileMemStream : public MemStream
{
public:
	FileMemStream( uint32 size ) : MemStream( size )
	{}

	~FileMemStream()
	{}

 	void* operator new( size_t size, uint32 len );
 	void operator delete( void * p, size_t size );


	class PoolFree
	{
	public:
		~PoolFree()
		{
			for ( std::multimap<uint32,void*>::iterator iter=FileMemStream::_memPool.begin(); iter!=FileMemStream::_memPool.end(); iter++ )
			{
				free( iter->second );
			}
			FileMemStream::_memPool.clear();
		}
	};
	static PoolFree poolFree;

protected:
	static std::multimap<uint32,void*> _memPool;
	static VMutex _mutex;
	
#ifdef _DEBUG
	static std::multimap< uint32,uint32 > _sizePool;
#endif
};
*/
