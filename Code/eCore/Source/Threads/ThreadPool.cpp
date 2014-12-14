/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2012 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 09-Feb-2010 by Elías Lozada-Benavente
// Based on original by Ángel Riesgo
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Thread.cpp
This file defines the the Thread class.
*/

#include <CorePch.h>

namespace E
{
namespace Threads
{
/*----------------------------------------------------------------------------------------------------------------------
ThreadPool constants
----------------------------------------------------------------------------------------------------------------------*/	
const U32 Threads::ThreadPool::kDefaultMaxPendingItemCount = 8172; // A max pending task count is required to avoid memory failures.


/*----------------------------------------------------------------------------------------------------------------------
Threads::Global methods
----------------------------------------------------------------------------------------------------------------------*/
 
Threads::ThreadPool& Threads::Global::GetThreadPool() { return Singleton<Threads::ThreadPool>::GetInstance(); }

/*----------------------------------------------------------------------------------------------------------------------
ThreadPool initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/	

Threads::ThreadPool::ThreadPool()
  : mpAllocator(Memory::Global::GetAllocator())
  , mMaxActiveThreadCount(Threads::Thread::GetProcessorCount() * 2)
  , mMaxPendingItemCount(kDefaultMaxPendingItemCount)
{
}

Threads::ThreadPool::~ThreadPool()
{
  CleanUp(true);
}

/*----------------------------------------------------------------------------------------------------------------------
ThreadPool accessors
----------------------------------------------------------------------------------------------------------------------*/

const Memory::IAllocator* Threads::ThreadPool::GetAllocator() const
{
  // [Critical section]
  Lock l(mMutex);
  return mpAllocator;
}

U32 Threads::ThreadPool::GetActiveThreadCount() const
{
  // [Critical section]
  Lock l(mMutex);
  return static_cast<U32>(mActiveThreadList.GetCount());
}

U32 Threads::ThreadPool::GetMaxActiveThreadCount() const
{
  // [Critical section]
  Lock l(mMutex);
  return mMaxActiveThreadCount;
}

U32 Threads::ThreadPool::GetMaxPendingItemCount() const
{
  // [Critical section]
  Lock l(mMutex);
  return mMaxPendingItemCount;
}

U32 Threads::ThreadPool::GetPendingItemCount() const
{
  // [Critical section]
  Lock l(mMutex);
  return static_cast<U32>(mPendingItemQueue.GetCount());
}

bool Threads::ThreadPool::HasActiveThreads() const
{
  // [Critical section]
  Lock l(mMutex);
  return !mActiveThreadList.IsEmpty();
}

bool Threads::ThreadPool::HasPendingItems() const
{
  // [Critical section]
  Lock l(mMutex);
  return !mPendingItemQueue.IsEmpty();
}

void Threads::ThreadPool::SetAllocator(Memory::IAllocator* p)
{
  // [Critical section]
  Lock l(mMutex);
  mpAllocator = p;
  mItemWaiterMap.SetAllocator(p);
  mActiveThreadList.SetAllocator(p);
  mIdleThreadList.SetAllocator(p);
  mPendingItemQueue.SetAllocator(p);
}

void Threads::ThreadPool::SetMaxActiveThreadCount(U32 v)
{
  // [Critical section]
  Lock l(mMutex);

  mMaxActiveThreadCount = v;
  if (mActiveThreadList.GetCount() < mMaxActiveThreadCount)
  {
    U32 maxIdleThreadCount = mMaxActiveThreadCount - static_cast<U32>(mActiveThreadList.GetCount());
    while (mIdleThreadList.GetCount() > maxIdleThreadCount) 
    {
      E_DELETE(*mIdleThreadList.GetBack(), 1, mpAllocator);
      mIdleThreadList.PopBack();
    }
  }
}

void Threads::ThreadPool::SetMaxPendingItemCount(U32 v)
{
  // [Critical section]
  Lock l(mMutex);
  mMaxPendingItemCount = v;
}

/*----------------------------------------------------------------------------------------------------------------------
ThreadPool methods
----------------------------------------------------------------------------------------------------------------------*/

/**
Executes the given thread pItem if possible (no maximum concurrent thread pItem number reached). Otherwise it queues the 
task in pending tasks in order to be launched later (when some current running pItem finishes).
@param p the specified thread pItem.
@return true or false depending on the task can be processed by ThreadPool
@throw nothing (very rare STL exceptions).
*/

bool Threads::ThreadPool::AddItem(IRunnable* pItem)
{
  // [Critical section]
  Lock l(mMutex);

  // We check if current WorkingThread number is less than the maximum allowed. If so start a new thread and add the new 
  // task, otherwise queue the pItem in order to wait for a free  WorkingThread
  if (mActiveThreadList.GetCount() < mMaxActiveThreadCount)
  {
    ThreadPoolWorker* pWorkerThread = nullptr;
    if (mIdleThreadList.IsEmpty())
    {
      pWorkerThread = E_NEW(ThreadPoolWorker, 1, mpAllocator);
      pWorkerThread->SetSubscriber(this);
    }
    else
    {
      pWorkerThread = *mIdleThreadList.GetBack();
      mIdleThreadList.PopBack();
    }

    pWorkerThread->AssignItem(pItem);
    mActiveThreadList.PushBack(pWorkerThread);
  }
  else
  {
    //Return if max pending tasks limit has been reached
    if (mPendingItemQueue.GetCount() >= mMaxPendingItemCount) return false;
    mPendingItemQueue.Push(pItem);
  }
  // Set the pItem waiters to false
  mItemWaiterMap.Insert(pItem, false);

  return true;
}

/**
Terminates current tasks in progress and kills all working threads
@throw nothing (very rare STL exceptions).
*/

void Threads::ThreadPool::CleanUp(bool terminate /* = false */)
{
  {
    // [Critical section]
    Lock l(mMutex);
    // Remove all idling threads
    for (auto it = begin(mIdleThreadList); it != end(mIdleThreadList); ++it)
    {
      E_DELETE(*it, 1, mpAllocator);
    }
    mIdleThreadList.Clear();
  }
  if (terminate)
  {
    // Remove current pending tasks.
    RemovePendingItems();
    // Wait for active threads to finish.
    WaitForIdle();
    // Remove all active threads (note tha ThreadPoolWorker object destruction calls WaitForTermination()).
    for (auto it = begin(mActiveThreadList); it != end(mActiveThreadList); ++it)
    {
      E_DELETE(*it, 1, mpAllocator);
    } 
    mActiveThreadList.Clear();
  }
}

/**
Discards pending tasks by removing them from the queue. 
@throw nothing.
*/

void Threads::ThreadPool::RemovePendingItems()
{
  // [Critical section]
  {
    Lock l(mMutex);
    while (!mPendingItemQueue.IsEmpty())
    {
      // Remove pItem from pItem waiter map
      mItemWaiterMap.Remove(mItemWaiterMap.Find(mPendingItemQueue.GetFront()));
      // Remove task from pending task queue
      mPendingItemQueue.Pop();
    }
  }
}

/**
Makes the client thread wait for all pending tasks to be finished before continue
@throw nothing (very rare STL exceptions).
*/
void Threads::ThreadPool::WaitForIdle()
{
  // [Critical section]
  Lock l(mMutex);
  while (!mActiveThreadList.IsEmpty()) mWaitCondition.Wait(mMutex);
}

/**
Makes the owner thread to wait for a concrete pending pItem completion
@throw nothing (very rare STL exceptions).
*/

void Threads::ThreadPool::WaitForItem(IRunnable* pItem)
{
  // [Critical section]
  Lock l(mMutex);
  if (!mActiveThreadList.IsEmpty())
  {
    IRunnableBoolMap::Pair* pPair = mItemWaiterMap.FindPair(pItem);
    while(pPair)
    {
      (*pPair).second = true;
      mWaitCondition.Wait(mMutex);
      pPair = mItemWaiterMap.FindPair(pItem);
    }
  }
}

/*----------------------------------------------------------------------------------------------------------------------
ThreadPool IThreadPoolWorkerSubscriber methods
----------------------------------------------------------------------------------------------------------------------*/

/**
Releases a pItem when it has finished.
@throw nothing (very rare STL exceptions).
*/

void Threads::ThreadPool::OnItemCompletion(IRunnable* pItem,  ThreadPoolWorker* pCallerThread)
{
  // [Critical section]
  {
    Lock l(mMutex);
    // If maximum working thread count has not been reached assign another task to this thread
    if (mActiveThreadList.GetCount() <= mMaxActiveThreadCount)
    {
      if (mPendingItemQueue.IsEmpty())
      {
        // If no working thread has a pItem in progress and no pending tasks pending signal the wait for idle condition
        if (mActiveThreadList.GetCount() == 1)
        {	
          E_ASSERT(*mActiveThreadList.GetBegin() == pCallerThread);
          mWaitCondition.Broadcast();
        }
        // Remove caller from the active thread list and add it to the idle thread list. Why no delete the thread instead 
        // of putting it into an idle thread list?
        // This method is called by a ThreadPoolWorker object itself, thus it can not delete himself because it will
        // cause a run condition signaling followed by WaitForTermination() call. Next instruction would cause a dead
        // lock by calling the wait condition. Also using an idle thread list enforce re-usage of memory in favor of
        // constant allocation / deallocation of memory.
        mActiveThreadList.RemoveIfFast(pCallerThread);
        mIdleThreadList.PushBack(pCallerThread);
      }
      else
      {
        IRunnable* pItem = mPendingItemQueue.GetFront();
        mPendingItemQueue.Pop();
        pCallerThread->AssignItem(pItem);
      }
    }
    // Signal waiters about task completion
    IRunnableBoolMap::Pair* pPair = mItemWaiterMap.FindPair(pItem);
    E_ASSERT_PTR(pPair);
    if ((*pPair).second) mWaitCondition.Broadcast();    
    // Remove task waiter map entry
    mItemWaiterMap.RemovePair(pPair);
  }
}
}
}
