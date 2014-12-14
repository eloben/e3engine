/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 17-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ThreadPool.h
This file declares the ThreadPool class. ThreadPool implements a thread pItem manager. 
*/

#ifndef E3_THREAD_POOL_H
#define E3_THREAD_POOL_H

#include "ThreadPoolWorker.h"
#include "Mutex.h"
#include "ConditionVariable.h"
#include "Lock.h"
#include <Containers/List.h>
#include <Containers/Queue.h>
#include <Containers/Map.h>
#include <Memory/Factory.h>

namespace E
{
namespace Threads
{
//Forward declarations
class IRunnable;
class ThreadPool;

/*----------------------------------------------------------------------------------------------------------------------
Threads API methods

Please note that this namespace methods have the following usage contract:

1. E_API methods get access to the library global allocator defining a unified memory model across executables / dlls.
2. Custom allocators can be defined ensuring a consistent used through this and other libraries by using the 
Memory::GlobalAllocatorProvider().SetAllocator method.
3. The process (executable / DLL) using the global thread pool should ensure all created threads are destroyed before 
finalization in order to have a clean exit. This can be easily achieved by combining Threads::Global::GetThreadPool().
WaitForItem() and Threads::Global::GetThreadPool().CleanUp(). Threads::Global::GetThreadPool().CleanUp(true) is 
discouraged due to the fact that other processes may have active threads also.

Note that there is a common practice with DLLs to not freeing up resources due to the fact that the OS clears all the
address space on DLL unload. Feel free to do so. However if you decide to manually cleanup your resources ensure not 
to call Threads::Global::GetThreadPool().WaitForItem() or Threads::Global::GetThreadPool().WaitForIdle() without
calling Threads::Global::GetThreadPool().CleanUp(ture / false) afterwards. Otherwise you will get a 
E_ASSERT_MSG_THREAD_RUNNING assertion on the thread pool destruction. The reason behind this is that when the main
executable unloads the DLL using thread pool, the call to the ThreadPool destructor will NOT trigger
Thread::WaitForTermination properly as it is not the DLL calling the destructor but the main executable.
----------------------------------------------------------------------------------------------------------------------*/
namespace Global
{
  E_API ThreadPool&  GetThreadPool();
}

/*----------------------------------------------------------------------------------------------------------------------
ThreadPool

This class is thread-safe.

1. CleanUp just clears idling threads by default. Enabling terminate CleanUp also clears all pending tasks and kills 
all working threads.
2. CleanUp(true) is called upon destruction.
3. ThreadPool does not create any threads on construction but it keeps them when a task is completed.
----------------------------------------------------------------------------------------------------------------------*/
class ThreadPool : public IThreadPoolWorkerSubscriber
{
public:
	E_API ThreadPool();	
	E_API ~ThreadPool();

	// Accessors
  E_API const Memory::IAllocator* GetAllocator() const;
  E_API U32						            GetActiveThreadCount() const;	  // Gets the current working thread with pItem in progress count
  E_API U32						            GetMaxActiveThreadCount() const;// Gets the maximum number of concurrent running thread tasks
  E_API U32						            GetMaxPendingItemCount() const;	// Gets the maximum number of pending thread tasks
  E_API U32						            GetPendingItemCount() const;		// Gets the current pending thread count
  E_API bool									    HasActiveThreads() const;		    // Returns true or false depending on whether it has working thread with pItem in progress 
  E_API bool									    HasPendingItems() const;				// Returns true or false depending on whether it has pending tasks
  E_API void                      SetAllocator(Memory::IAllocator* p);
  E_API void									    SetMaxActiveThreadCount(U32 v); // Sets the maximum number of concurrent running thread
  E_API void									    SetMaxPendingItemCount(U32 v);	// Sets the maximum number of pending thread tasks

  // Methods
  E_API bool									    AddItem(IRunnable* pItem);	    // Adds a IRunnable object to schedule it for execution if max pending pItem number is not reached
  E_API void									    CleanUp(bool terminate = false);// Kills idling threads; terminate = true also clears all pending tasks and kills all working threads.
  E_API void									    RemovePendingItems();					  // Removes current pending tasks, removing them from the pending task queue
  E_API void									    WaitForIdle();							    // Makes the owner thread to wait till all pending tasks finish
  E_API void									    WaitForItem(IRunnable* pItem);	// Makes the owner thread to wait for a concrete pending pItem completion

  // IThreadPoolWorkerSubscriber methods
  E_API void									    OnItemCompletion(IRunnable* pItem, ThreadPoolWorker* pCallerThread);	

private:
  typedef Containers::List<ThreadPoolWorker*> ThreadPoolWorkerList;
  typedef Containers::Queue<IRunnable*>       IRunnableQueue;
  typedef Containers::Map<IRunnable*, bool>   IRunnableBoolMap;
  
  static const U32                kDefaultMaxActiveThreadCount;   // A maximum number of active working threads.
  static const U32                kDefaultMaxPendingItemCount;    // A max pending task count is required to avoid memory failures.

  mutable Mutex							      mMutex;
  ConditionVariable					      mWaitCondition;
  IRunnableBoolMap                mItemWaiterMap;         // Map to mark if some pItem running in a concrete thread has a waiter for its completion
  ThreadPoolWorkerList            mActiveThreadList;      // Vector of working threads
  ThreadPoolWorkerList            mIdleThreadList;        // Queue of dead working threads (due to a maximum working thread count decrease)
  IRunnableQueue                  mPendingItemQueue;      // Queue of pending thread tasks
  Memory::IAllocator*             mpAllocator;
  U32								              mMaxActiveThreadCount;	// maximum allowed concurrent thread pItem number
  U32								              mMaxPendingItemCount;		// maximum allowed pending thread pItem number
	
  void                            CreateThredPoolWorker();
  void                            DestroyThreadPoolWorker();

	E_DISABLE_COPY_AND_ASSSIGNMENT(ThreadPool)
};
}
}

#endif
