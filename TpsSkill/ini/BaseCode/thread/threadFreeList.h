#ifndef _THREADSAFEFREELIST_H_
#define _THREADSAFEFREELIST_H_


#include "threadRef.h"
#include "intrinsics.h"

/// @file
/// Lock-free freelists for concurrent access.


/// Freelist for re-using allocations in a concurrent setting.
///
/// @note Make sure that there are no more allocations in use
///   when the free list is destructed.
/// @note Allocated instances come with a reference already counted
///   on the instance.
///
/// @param T Type of elements to allocate; must be derived from
///   ThreadSafeRefCount and have at least define one additional
///   pointer-sized field.
template< class T >
class ThreadSafeFreeList
{
   protected:

      T* mFreeList;

      #ifdef _DEBUG
      int32 mNumNodesTotal;
      int32 mNumNodesFree;
      #endif

      T*& getNext( T* ptr )
      {
         return *( ( T** ) &( ( uint8* ) ptr )[ sizeof( T ) - sizeof( T* ) ] );
      }

   public:

      /// Create the freelist.
      ///
      /// @param numPreAlloc Number of instances to pre-allocate.
      ThreadSafeFreeList( uint32 numPreAlloc = 0 )
         : mFreeList( 0 )
      {
         #ifdef _DEBUG
         mNumNodesTotal = 0;
         mNumNodesFree = 0;
         #endif

         for( uint32 i = 0; i < numPreAlloc; ++ i )
            _free( alloc() );
      }

      ~ThreadSafeFreeList()
      {
         #ifdef _DEBUG
         ASSERT_MESSAGE( mNumNodesTotal == mNumNodesFree,
            "ThreadSafeFreeList::~ThreadSafeFreeList() - still got live instances" );
         #endif

         // Destroy remaining nodes.  Not synchronized.  We assume all
         // concurrent processing to have finished.

         while( mFreeList )
         {
            T* next = getNext( mFreeList );
            free( mFreeList );
            mFreeList = next;
         }
      }

      /// Return memory for a new instance.
      void* alloc()
      {
         T* ptr;
         while( 1 )
         {
            ptr = ThreadSafeRef< T >::safeRead( mFreeList );
            if( !ptr )
            {
               ptr = ( T* ) malloc( sizeof( T ) );
               memset( ptr, 0, sizeof( T ) );

               #ifdef _DEBUG
               dFetchAndAdd( mNumNodesTotal, 1 );
               #endif

               ptr->addRef();
               break;
            }
            else if( dCompareAndSwap( mFreeList, ptr, getNext( ptr ) ) )
            {
               #ifdef _DEBUG
               dFetchAndAdd( mNumNodesFree, -1 );
               #endif

               ptr->clearLowestBit();
               break;
            }
            else
               ptr->release();
         }

         return ptr;
      }

      /// Return the memory allocated to the given instance to the freelist.
      void _free( void* ptr )
      {
         ASSERT_MESSAGE( ptr!=NULL, "ThreadSafeFreeList::free() - got a NULL pointer" );
         T* node = ( T* ) ptr;

         while( 1 )
         {
            T* list = mFreeList;
            getNext( node ) = list;
            if( dCompareAndSwap( mFreeList, list, node ) )
               break;
         }

         #ifdef _DEBUG
         dFetchAndAdd( mNumNodesFree, 1 );
         #endif
      }

      void dumpDebug()
      {
         #ifdef _DEBUG
         log_out(MAIN, LOG_DEBUG, "[ThreadSafeFreeList] total=%i, free=%i",
            mNumNodesTotal, mNumNodesFree );
         #endif
      }
};

/// Baseclass for objects allocated from ThreadSafeFreeLists.
template< class T, class DeletePolicy = DeleteSingle >
class ThreadSafeFreeListNode : public ThreadSafeRefCount< T, DeletePolicy >
{
   public:

      typedef ThreadSafeRefCount< T, DeletePolicy > Parent;

      ThreadSafeFreeListNode()
         : Parent( false ) {}

      static void* operator new( size_t size, ThreadSafeFreeList< T >& freeList )
      {
         ASSERT_MESSAGE( size <= sizeof( T ),
            "ThreadSafeFreeListNode::new() - size exceeds limit of freelist" );
         return freeList.alloc();
      }
      static void operator delete( void* ptr, ThreadSafeFreeList< T >& freeList )
      {
         freeList._free( ptr );
      }
};

#endif // _THREADSAFEFREELIST_H_
