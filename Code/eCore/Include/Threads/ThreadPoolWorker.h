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

/** @file ThreadPoolWorker.h
This file declares the ThreadPoolWorker helper class and the IThreadPoolWorkerSubscriber interface.
*/

#ifndef E3_THREAD_POOL_THREAD_H
#define E3_THREAD_POOL_THREAD_H

#include "IRunnable.h"
#include "Thread.h"
#include "Mutex.h"
#include "ConditionVariable.h"
#include "Lock.h"
#include <Base.h>

namespace E
{
namespace Threads
{
//Forward declarations
class IThreadPoolWorkerSubscriber;

/*----------------------------------------------------------------------------------------------------------------------
ThreadPoolWorker

A working thread class to host IRunnable objects and execute them through the ThreadPool. This class is also
implements the IRunnable interface. This class is thread-safe.
----------------------------------------------------------------------------------------------------------------------*/	
class ThreadPoolWorker : public IRunnable
{
public:
  ThreadPoolWorker();
  ~ThreadPoolWorker();

  void                          SetSubscriber(IThreadPoolWorkerSubscriber* pSubscriber);
  void									        AssignItem(IRunnable* pItem);	

private:
  Thread                        mThread;
  mutable Mutex							    mMutex;	
  ConditionVariable					    mRunCondition;						
  IThreadPoolWorkerSubscriber* 	mpSubscriber;						
  IRunnable*						        mpPendingItem;		
  bool									        mTerminationFlag;		

  I32										        Run();

  E_DISABLE_COPY_AND_ASSSIGNMENT(ThreadPoolWorker)
};

/*----------------------------------------------------------------------------------------------------------------------
IThreadPoolWorkerSubscriber

This class notifies when a IRunnable task run in a ThreadPoolWorker has finished. IThreadPoolWorkerSubscriber implements
a publisher - subscriber pattern.
----------------------------------------------------------------------------------------------------------------------*/
class IThreadPoolWorkerSubscriber
{
public:
  virtual ~IThreadPoolWorkerSubscriber() {}
  virtual void OnItemCompletion(IRunnable* pItem, ThreadPoolWorker* pCaller) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ThreadPoolWorker initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/	

// Known warning: passing this in the initializer list. In ThreadPoolWorker construction, the Thread 
// member gets a reference to this as a IRunnable object (whose Run method is executed in Thread::Start).
#pragma warning(push)
#pragma warning (disable:4355)
/**
Constructor. Empty at present.
@throw	nothing.
*/
inline ThreadPoolWorker::ThreadPoolWorker()
  : mThread(*this)
  , mpSubscriber(nullptr)
  , mpPendingItem(0)
  , mTerminationFlag(false)
{
  mThread.Start();
}
#pragma warning(pop)

/**
Destructor.
@throw	nothing.
*/
inline ThreadPoolWorker::~ThreadPoolWorker()
{
  // Signal run condition because destruction should follow a WaitForIdle call in ThreadPool which causes
  // main thread to wait for all running task to finish. So after this we signal working thread in order to
  // exit its run

  // [Critical section]
  {
    Lock l(mMutex);
    mTerminationFlag = true;
    mRunCondition.Signal();
  }
  mThread.WaitForTermination();
}

/*----------------------------------------------------------------------------------------------------------------------
ThreadPoolWorker accessors
----------------------------------------------------------------------------------------------------------------------*/

inline void ThreadPoolWorker::SetSubscriber(IThreadPoolWorkerSubscriber* pSubscriber) { mpSubscriber = pSubscriber; }

/*----------------------------------------------------------------------------------------------------------------------
ThreadPoolWorker methods
----------------------------------------------------------------------------------------------------------------------*/

/**
Assigns the task to run
@param task a IRunnable object
@throw nothing (very rare STL exceptions).
*/
inline void ThreadPoolWorker::AssignItem(IRunnable* pItem)
{
  // [Critical section]
  Lock l(mMutex);
  mpPendingItem = pItem;
  mRunCondition.Signal();
}

/*----------------------------------------------------------------------------------------------------------------------
ThreadPoolWorker private methods

Note that Run is made private as it is not intended to be called by the ThreadPool class but by the Thread class through
the IRunnable interface.
----------------------------------------------------------------------------------------------------------------------*/

/**
Working thread run code. This method executes IRunnable task objects. When it has no task to execute it blocks.
@return the thread id.
@throw nothing (very rare STL exceptions).
*/
inline I32 ThreadPoolWorker::Run()
{
  for (;;)
  {
    IRunnable* pRunningItem = 0;
    // [Critical section]
    {	
      // If there is no work to do (no pending task) put the thread to sleep
      Lock l(mMutex);
      while (!mpPendingItem && !mTerminationFlag) mRunCondition.Wait(mMutex);	
      // After being signaled first check termination flag (signaled through destructor)
      if (mTerminationFlag) break;
      // If no termination requested assign pending task to running task
      pRunningItem = mpPendingItem;
      mpPendingItem = 0;
    }
    // Run the item
    pRunningItem->Run();
    //Notify subscriber item completion
    mpSubscriber->OnItemCompletion(pRunningItem, this);
  }
  return 0;
}
}
}

#endif
