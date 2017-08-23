#ifndef _THREADSAFEPRIORITYQUEUE_H_
#define _THREADSAFEPRIORITYQUEUE_H_


#include "threadRef.h"
#include "..\basefunc.h"





template< typename T, typename K = float32, bool SORT_MIN_TO_MAX = false, uint32 MAX_LEVEL = 4, uint32 PROBABILISTIC_BIAS = 50 >
struct ThreadSafePriorityQueue
{
   typedef T ValueType;
   typedef K KeyType;

   enum { MAX_LEVEL_CONST = MAX_LEVEL };

   ThreadSafePriorityQueue();

   bool              isEmpty();
   void              insert( KeyType priority, const T& value );
   bool              takeNext( T& outValue, KeyType upToPriority = ( SORT_MIN_TO_MAX ? TypeTraits< KeyType >::MAX : TypeTraits< KeyType >::MIN ) );

protected:
   struct Node;
   typedef ThreadSafeRef< Node > NodePtr;
   friend class ThreadSafeRefCount< Node >;
   friend struct DeleteSingle;


   struct Node : public ThreadSafeRefCount< Node >
   {
      typedef ThreadSafeRefCount< Node > Parent;

      Node( KeyType priority, const ValueType& value );
      ~Node();

      KeyType        getPriority()                 { return mPriority; }
      ValueType&     getValue()                    { return mValue; }
      uint32            getLevel();
      NodePtr&       getNext( uint32 level );

      bool           isMarkedForDeletion();
      bool           tryMarkForDeletion();
      
      void           clearValue()                  { mValue = ValueType(); }

      static uint32     randomLevel();

      void*          operator new( size_t size, int32 level = -1 );
      void           operator delete( void* ptr );

   private:
      KeyType        mPriority;     ///< Priority key.
      uint32            mLevel;        ///< Level count and deletion bit (highest).
      ValueType      mValue;
      Node*          mNext[ 1 ];    ///< Variable-sized array of next pointers.

      struct FreeList
      {
         bool        mDestroyed;
         Node*       mNodes;

         ~FreeList();
      };

      static FreeList smFreeLists[ MAX_LEVEL ];
   };

   NodePtr           mHead;         ///< Artificial head node.
   NodePtr           mTail;         ///< Artificial tail node.

   void              readNext( NodePtr& refPrev, NodePtr& refNext, uint32 level );
   void              scan( NodePtr& refPrev, NodePtr& refNext, uint32 level, KeyType priority );
   void              scanFromHead( NodePtr& refPrev, NodePtr& refNext, uint32 level, KeyType priority );
   void              insert( KeyType priority, const T& value, NodePtr& outResult );
   void              helpDelete();
};

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
typename ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::FreeList ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::smFreeLists[ MAX_LEVEL ];

/// Construct an empty queue.
///
/// Internally, this creates a head node with maximal priority and a tail node with minimal priority,
/// both at maximum level.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::ThreadSafePriorityQueue()
{
   NodePtr::unsafeWrite( mHead, new ( MAX_LEVEL - 1 )
      Node( SORT_MIN_TO_MAX ? TypeTraits< KeyType >::MIN : TypeTraits< KeyType >::MAX, ValueType() ) );
   NodePtr::unsafeWrite( mTail, new ( MAX_LEVEL - 1 )
      Node( SORT_MIN_TO_MAX ? TypeTraits< KeyType >::MAX : TypeTraits< KeyType >::MIN, ValueType() ) );

   for( uint32 level = 0; level < MAX_LEVEL; level ++ )
      mHead->getNext( level ) = mTail;
}

/// Return true if the queue does not currently contain an item.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
bool ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::isEmpty()
{
   return ( mHead->getNext( 0 ) == mTail );
}

/// Insert the given value into the queue at the place determined by the given priority.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
inline void ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::insert( KeyType priority, const ValueType& value )
{
   NodePtr result;
   insert( priority, value, result );
}

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::insert( KeyType priority, const ValueType& value, NodePtr& outResult )
{
   // Create a new node at a random level.

   outResult = NULL;
   NodePtr::unsafeWrite( outResult, new Node( priority, value ) );
   uint32 resultNodeLevel = outResult->getLevel();

   // Link up all the levels.  Do this bottom-up instead of
   // top-down (as would be the right way for a skiplist) so
   // that our list state always remains valid.  If going top-down,
   // we'll insert nodes with NULL pointers at their lower levels.

   uint32 currentLevel = 0;
   do 
   {
      while( 1 )
      {
         NodePtr nextNode;
         NodePtr prevNode;
         
         scanFromHead( prevNode, nextNode, currentLevel, priority );        

         outResult->getNext( currentLevel ) = nextNode;
         if( prevNode->getNext( currentLevel ).trySetFromTo( nextNode, outResult, NodePtr::TAG_FailIfSet ) )
            break;
         else
            outResult->getNext( currentLevel ) = 0;
      }

      currentLevel ++;
   }
   while(    currentLevel <= resultNodeLevel
          && !outResult->isMarkedForDeletion() ); // No point linking up remaining levels if another thread already took this node.
}

/// Take the item with the highest priority from the queue.
///
/// @param outValue Reference to where the resulting value should be stored.
/// @param upToPriority Priority limit (inclusive) up to which items are taken from the queue.
/// @return true if there was a matching item in the queue.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
bool ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::takeNext( T& outValue, KeyType upToPriority )
{
   // Iterate through to the first unmarked node.

   NodePtr prevNode = mHead;
   while( 1 )
   {
      NodePtr node;
      readNext( prevNode, node, 0 );

      if( node == mTail )
         return false; // End reached.

      bool priorityThresholdReached = SORT_MIN_TO_MAX
         ? ( upToPriority >= node->getPriority() )
         : ( upToPriority <= node->getPriority() );

      if( !priorityThresholdReached )
         return false;
      else
      {
         // Try to mark the node for deletion.  Only if that succeeds, taking the
         // node was a success and we can return.  If it fails, spin and try again.

         if( node->tryMarkForDeletion() )
         {
            helpDelete();

            // Node is now off the list and will disappear as soon as
            // all references held by threads (including this one)
            // go out of scope.

            outValue = node->getValue();
            node->clearValue();

            return true;
         }
      }
   }
}

/// Update the given references to the next non-deleted node at the given level.
/// refPrev will be updated to reference the immediate predecessor of the next
/// node returned.  Note that this can be a node in deleted state.
///
/// @param refPrev Reference to a node of which the successor node should be
///    returned.  Updated to immediate predecessor of refNext on return.
/// @param refNext Reference to update to refer to next non-deleted node on
///    the given level.
/// @param level Skiplist level to operate on.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
inline void ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::readNext( NodePtr& refPrev, NodePtr& refNext, uint32 level )
{
   while( 1 )
   {
      refNext = refPrev->getNext( level );
      ASSERT_MESSAGE( refNext != NULL, "ThreadSafePriorityQueue::readNext() - next is NULL" );
      if( !refNext->isMarkedForDeletion() || refNext == mTail )
         break;

      refPrev = refNext;
   }
}

/// Scan for the position at which to insert a node of the given priority.
/// Upon return, the position between refPrev and refNext is the one to insert at.
///
/// @param refPrev position at which to start scanning; updated to match insert position.
/// @param refNext

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::scan( NodePtr& refPrev, NodePtr& refNext, uint32 level, KeyType priority )
{
   while( 1 )
   {
      readNext( refPrev, refNext, level );
      if( refNext == mTail
         || ( SORT_MIN_TO_MAX
            ? ( refNext->getPriority() > priority )
            : ( refNext->getPriority() < priority ) ) )
         break;

      refPrev = refNext;
   }
}

///

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::scanFromHead( NodePtr& refPrev, NodePtr& refNext, uint32 level, KeyType priority )
{
   // Purge dead nodes at left end of queue so
   // we don't get stuck hitting the same node
   // in deletable state over and over again.
   helpDelete();

   int32 currentLevel = MAX_LEVEL - 1;
   refPrev = mHead;
   do
   {
      scan( refPrev, refNext, currentLevel, priority );
      currentLevel --;
   }
   while( currentLevel >= int32( level ) );
}

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::helpDelete()
{
   // Clean out all the references from head.
   // Spin over a given reference on each level until head
   // clearly refers to a node in non-deletable state.  This
   // makes this code work cooperatively with other threads
   // doing takeNexts on prior or later nodes while also
   // guaranteeing that all next pointers to us will eventually
   // disappear.
   //
   // Note that this is *the only place* where we will be cleaning
   // out our lists.

   int32 level = MAX_LEVEL - 1;
   do
   {
      while( 1 )
      {
         NodePtr ptr = mHead->getNext( level );
         if( !ptr->isMarkedForDeletion() )
            break;
         else
         {
            NodePtr& next = ptr->getNext( level );
            next.setTag();
            mHead->getNext( level ).trySetFromTo( ptr, next, NodePtr::TAG_Unset );
            ASSERT_MESSAGE( next->getRefCount() >= 2, "ThreadSafePriorityQueue::helpDelete() - invalid refcount" );
         }
      }

      level --;
   }
   while( level >= 0 );
}

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
inline ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::Node( KeyType priority, const ValueType& value )
   : Parent( false ),
     mPriority( priority ),
     mValue( value )
{
   memset( mNext, 0, sizeof( Node* ) * ( getLevel() + 1 ) );

   // Level is already set by the allocation routines.
}

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::~Node()
{
   for( uint32 level = 0; level < ( getLevel() + 1 ); level ++ )
      getNext( level ) = NULL;
}

/// Return the skip list level the node is at.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
inline uint32 ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::getLevel()
{
   // Mask out the deletion request bit.

   return ( mLevel & 0x7FFFFFFF );
}

/// Return the successor node at the given level.
/// @param level The level of the desired successor node; must be within the node's level bounds.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
inline typename ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::NodePtr& ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::getNext( uint32 level )
{
   return *reinterpret_cast< NodePtr* >( &mNext[ level ] );
}

/// Return true if the node is marked to be deleted.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
inline bool ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::isMarkedForDeletion()
{
   return ( mLevel & 0x80000000 )!=0;
}

/// Attempt to mark the node for deletion.  If the mark bit has not yet been set
/// and setting it on the current thread succeeds, returns true.
///
/// @return true, if the marking succeeded.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
inline bool ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::tryMarkForDeletion()
{
	uint32 oldVal = mLevel & 0x7FFFFFFF;
	uint32 newVal = oldVal | 0x80000000;

	return ( dCompareAndSwap( mLevel, oldVal, newVal ) );
}

/// Choose a random level.
///
/// The chosen level depends on the given PROBABILISTIC_BIAS and MAX_LEVEL,
/// but is not affected by the actual number of nodes in a queue.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
uint32 ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::randomLevel()
{
   uint32 level = 0;
   while( Randomizer::frand() < ( ( ( float32 ) PROBABILISTIC_BIAS ) / 100 ) && level < ( MAX_LEVEL - 1 ) )
      level ++;
   return level;
}

/// Allocate a new node.
/// The node comes with a reference count of one and its level already set.
///
/// @param level The level to allocate the node at.  If this is -1, a random level is chosen.
/// @return a new node.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void* ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::operator new( size_t size, int32 level )
{
   if( level == -1 )
      level = randomLevel();

   Node* node = 0;
   while( 1 )
   {
      // Try to take a node from the freelist.  If there's none,
      // allocate a new one.

      if( !smFreeLists[ level ].mDestroyed )
         node = Node::safeRead( smFreeLists[ level ].mNodes );

      if( !node )
      {
         node = ( Node* ) malloc( sizeof( Node ) + sizeof( Node* ) * level );
         memset( node, 0, sizeof( Node ) );
         node->mLevel = level;
         node->addRef();
         break;
      }
      else if( dCompareAndSwap( smFreeLists[ level ].mNodes, node, node->mNext[ 0 ] ) )
      {
         node->clearLowestBit();
         break;
      }
      else
         node->release(); // Other thread was quicker than us; release.
   }

   ASSERT_MESSAGE( node->getRefCount() != 0, "ThreadSafePriorityQueue::new Node() - invalid refcount" );
   ASSERT_MESSAGE( ( node->getRefCount() % 2 ) == 0, "ThreadSafePriorityQueue::new Node() - invalid refcount" );
   return node;
}

/// Reclaim a node.
///
/// @param node The node to reclaim.  Must refer to a Node instance.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::operator delete( void* ptr )
{
   //TODO: limit number of nodes kept

   Node* node = ( Node* ) ptr;
   uint32 level = node->getLevel();
   node->mLevel = level; // Reset the node's deletion bit.

   while( !smFreeLists[ level ].mDestroyed )
   {
      // Put the node on the freelist.

      Node* freeList = smFreeLists[ level ].mNodes;
      node->mNext[ 0 ] = freeList;
      
      if( dCompareAndSwap( smFreeLists[ level ].mNodes, freeList, node ) )
      {
         node = NULL;
         break;
      }
   }
   
   if( node )
      free( node );
}

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::Node::FreeList::~FreeList()
{
   mDestroyed = true;
   while( mNodes )
   {
      //FIXME: could leak some bytes under unfortunate circumstances (this in
      //   combination with mDestroyed is a dependent write)

      Node* next = mNodes;
      if( dCompareAndSwap( mNodes, next, next->mNext[ 0 ] ) )
         free( next );
   }
}

//--------------------------------------------------------------------------
//    ThreadSafePriorityQueueWithUpdate.
//--------------------------------------------------------------------------

/// Fast, lock-free priority queue implementation for concurrent access that
/// performs dynamic re-prioritization of items.
///
/// Within the bounds of a set update interval UPDATE_INTERVAL, the takeNext
/// method is guaranteed to always return the item that has the highest priority
/// at the time the method is called rather than at the time items were inserted
/// into the queue.
///
/// Values placed on the queue must implement the following interface:
///
/// @code
/// template&lt; typename K >
/// struct IThreadSafePriorityQueueItem
/// {
///    // Default constructor.
///    IThreadSafePriorityQueueItem();
///
///    // Return the current priority.
///    // This must run normally even if the item is already dead.
///    K getPriority();
///
///    // Return true if the item is still meant to be waiting in the queue.
///    bool isAlive();
/// };
/// @endcode

template< typename T, typename K, bool SORT_MIN_TO_MAX = false, uint32 MAX_LEVEL = 4, uint32 PROBABILISTIC_BIAS = 50 >
struct ThreadSafePriorityQueueWithUpdate : public ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >
{

   typedef T ValueType;
   typedef K KeyType;

   enum { DEFAULT_UPDATE_INTERVAL = 256 };

   ThreadSafePriorityQueueWithUpdate( uint32 updateInterval = DEFAULT_UPDATE_INTERVAL );

   void              insert( KeyType priority, const T& value );
   bool              takeNext( T& outValue, KeyType upToPriority = ( SORT_MIN_TO_MAX ? TypeTraits< KeyType >::MAX : TypeTraits< KeyType >::MIN ) );

   uint32               getUpdateInterval() const;
   void              setUpdateInterval( uint32 value );

   KeyType           getTimeBasedPriorityBoost() const;
   void              setTimeBasedPriorityBoost( KeyType value );

   void              updatePriorities();

protected:
   typedef ThreadSafePriorityQueue< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS > Parent;
   typedef uint32 TickType;
   typedef typename Parent::NodePtr NodePtr;

   uint32               mUpdateInterval;
   KeyType           mPriorityBoost;      ///< If this is non-zero, priorities will be boosted by this amount each update.  This can be used to prevent constant high-priority inserts to starve low-priority items already in the queue.

   /// Work queue for node updates.
   ThreadSafePriorityQueue< NodePtr, TickType, true, MAX_LEVEL, PROBABILISTIC_BIAS > mUpdateQueue;

   TickType          getTick()            { return getTimeMillsec(); }
};

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
ThreadSafePriorityQueueWithUpdate< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::ThreadSafePriorityQueueWithUpdate( uint32 updateInterval )
   : mUpdateInterval( updateInterval ),
     mPriorityBoost( TypeTraits< KeyType >::ZERO )
{
}

/// Return the current update interval in milliseconds.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
uint32 ThreadSafePriorityQueueWithUpdate< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::getUpdateInterval() const
{
   return mUpdateInterval;
}

/// Set update interval of queue to given value.
///
/// <em>Call this method on the main thread only.</em>
///
/// @param value Time between priority updates in milliseconds.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueueWithUpdate< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::setUpdateInterval( uint32 value )
{
   mUpdateInterval = value;
}

/// Return the delta to apply to priorities on each update.
/// Set to zero to deactivate time-based priority adjustments.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
K ThreadSafePriorityQueueWithUpdate< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::getTimeBasedPriorityBoost() const
{
   return mPriorityBoost;
}

/// Set the delta for time-based priority adjustments to the given value.
///
/// <em>Call this method on the main thread only.</em>
///
/// @param value The new priority adjustment value.

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueueWithUpdate< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::setTimeBasedPriorityBoost( KeyType value )
{
   mPriorityBoost = value;
}

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueueWithUpdate< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::insert( KeyType priority, const ValueType& value )
{
   NodePtr node;
   Parent::insert( priority, value, node );
   mUpdateQueue.insert( getTick() + getUpdateInterval(), node );
}

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
bool ThreadSafePriorityQueueWithUpdate< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::takeNext( T& outValue, KeyType upToPriority )
{
   updatePriorities();

   bool result = false;
   do
   {
      result = Parent::takeNext( outValue, upToPriority );
   }
   while( result && !outValue.isAlive() );
   
   return result;
}

///

template< typename T, typename K, bool SORT_MIN_TO_MAX, uint32 MAX_LEVEL, uint32 PROBABILISTIC_BIAS >
void ThreadSafePriorityQueueWithUpdate< T, K, SORT_MIN_TO_MAX, MAX_LEVEL, PROBABILISTIC_BIAS >::updatePriorities()
{
   TickType currentTime       = getTick();
   uint32      numNodesUpdated   = 0;
   uint32      numNodesDead      = 0;
   uint32      numNodesChanged   = 0;

   NodePtr node;
   while( mUpdateQueue.takeNext( node, currentTime ) )
   {
      numNodesUpdated ++;

      // Since we're updating nodes on the update queue only periodically,
      // their associated values or main queue nodes may have died in the
      // meantime.  If so, we just discard them here.

      if( node->getValue().isAlive()
          && !node->isMarkedForDeletion() )
      {
         KeyType newPriority = node->getValue().getPriority() + getTimeBasedPriorityBoost();
         if( newPriority != node->getPriority() )
         {
            // Node is outdated.  Reinsert with new priority and mark the
            // old node for deletion.

            insert( newPriority, node->getValue() );
            node->tryMarkForDeletion();
            numNodesChanged ++;
         }
         else
         {
            // Node is still current.  Just move to end.

            mUpdateQueue.insert( currentTime + getUpdateInterval(), node );
         }
      }
      else
         numNodesDead ++;
   }
}

#endif // !_THREADSAFEPRIORITYQUEUE_H_
