/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

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

// Created 04-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file ConditionVariable.cpp
This file defines ConditionVariable test functions.
*/

#include <CoreTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

class Task
{
public:
  Task() : mTypeID(-1) {}
  explicit Task(I32 typeID) : mTypeID(typeID) {}

  I32 GetType() const
  {
    return mTypeID;
  }

private:
  I32 mTypeID;
};
/*
class TaskProducer
{
public:
  Task Produce(I32 taskTypeID)
  { 
    E::Threads::Thread::Sleep(kTimeFactor * (taskTypeID + 1));
    return Task(taskTypeID);
  }

private:
  static const TimeValue kTimeFactor;
};
const TimeValue TaskProducer::kTimeFactor = TimeValue::kOneMillisecond * 50;

class TaskConsumer : public E::Threads::IRunnable
{
public:
  TaskConsumer() : mExitFlag(false) {}

  size_t GetPendingTaskCount() const
  {
    E::Threads::Lock l(mTaskQueueMutex);
    return mTaskQueue.GetCount();
  }

  void AddTask(const Task& task)
  {
    E::Threads::Lock l(mTaskQueueMutex);
    mTaskQueue.Push(task);
    if (mTaskQueue.GetCount() == 1) mRunCondition.Signal();
  }

  void Consume(const Task& task)
  {
    E::Threads::Thread::Sleep(kConsumptionTimeFactor * (task.GetType() + 1));
  }

  I32 Run()
  {
    Task nextTask;
    for (;;)
    {
      {
        E::Threads::Lock l(mTaskQueueMutex);
        while (mTaskQueue.IsEmpty())
        {
          if (mExitFlag) return 0;
          mRunCondition.Wait(mTaskQueueMutex);
        }

        nextTask = mTaskQueue.GetFront();
        mTaskQueue.Pop();
      }
      Consume(nextTask);
    }
  }

  void TerminateWhenDone()
  { 
    mExitFlag = true;
    E::Threads::Lock l(mTaskQueueMutex);
    mRunCondition.Signal();
  }

private:
  using TaskQueue = Containers::Queue<Task>;
 
  static const TimeValue        kConsumptionTimeFactor;
  TaskQueue                     mTaskQueue;
  mutable E::Threads::Mutex     mTaskQueueMutex;
  E::Threads::ConditionVariable mRunCondition;
  bool                          mExitFlag;
};
const TimeValue TaskConsumer::kConsumptionTimeFactor = TimeValue::kOneMillisecond * 100;
*/
/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

class TaskProducer
{
public:
  Task Produce(I32 taskTypeID)
  {
    E::Threads::Thread::Sleep(kProductionTimeFactor * (taskTypeID + 1));
    return Task(taskTypeID);
  }

private:
  static const TimeValue kProductionTimeFactor;
};

const TimeValue TaskProducer::kProductionTimeFactor = TimeValue::kOneMillisecond * 20;

class TaskConsumer : public E::Threads::IRunnable
{
public:
  TaskConsumer() : mExitFlag(false) {}

  size_t GetPendingTaskCount() const
  {
    E::Threads::Lock l(mTaskQueueMutex);
    return mTaskQueue.GetCount();
  }

  void AddTask(const Task& task)
  {
    E::Threads::Lock l(mTaskQueueMutex);
    mTaskQueue.Push(task);
    if (mTaskQueue.GetCount() == 1) mRunCondition.Signal();
  }

  I32 Run()
  {
    Task nextTask;
    for (;;)
    {
      {
        E::Threads::Lock l(mTaskQueueMutex);
        while (mTaskQueue.IsEmpty())
        {
          if (mExitFlag) return 0;
          mRunCondition.Wait(mTaskQueueMutex);
        }

        nextTask = mTaskQueue.GetFront();
        mTaskQueue.Pop();
      }
      Consume(nextTask);
    }
  }

  void TerminateWhenDone()
  {
    mExitFlag = true;
    E::Threads::Lock l(mTaskQueueMutex);
    mRunCondition.Signal();
  }

private:
  using TaskQueue = Containers::Queue < Task > ;
  static const TimeValue        kConsumptionTimeFactor;
  TaskQueue                     mTaskQueue;
  mutable E::Threads::Mutex     mTaskQueueMutex;
  E::Threads::ConditionVariable mRunCondition;
  bool                          mExitFlag;

  void Consume(const Task& task)
  {
    E::Threads::Thread::Sleep(kConsumptionTimeFactor * (task.GetType() + 1));
  }
};

const TimeValue TaskConsumer::kConsumptionTimeFactor = TimeValue::kOneMillisecond * 30;

bool Test::ConditionVariable::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::ConditionVariable::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::ConditionVariable::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::ConditionVariable::RunFunctionalityTest()
{
  /* // First approac
  TaskProducer producer;
  TaskConsumer consumer;

  //Math::Global::GetRandom().RandomizeSeed(); // We want always the same sequence
  E::Timer::Timer timer;
  for (I32 i = 0; i < 10; ++i)
  {
    consumer.Consume(producer.Produce(Math::Global::GetRandom().GetI32(0, 9)));
  }
  std::cout << "Elapsed time: " << timer.GetElapsed().GetMilliseconds() << std::endl;*/
  TaskProducer producer;
  TaskConsumer consumer;
  E::Threads::Thread consumerThread(consumer);

  //Math::Global::GetRandom().RandomizeSeed(); // Disabled on purpose
  consumerThread.Start();
  E::Timer::Timer timer;
  for (I32 i = 0; i < 10; ++i)
  {
    consumer.AddTask(producer.Produce(Math::Global::GetRandom().GetI32(0, 9)));
  }

  std::cout << "Consumer work done. Waiting 10 secs" << std::endl;
  //Threads::Thread::Sleep(TimeValue::kOneSecond * 10);
  std::cout << "Calling TerminateWhenDone... (we're gonna wait long here)" << std::endl;
  consumer.TerminateWhenDone();
  consumerThread.WaitForTermination();
  E_ASSERT(!consumer.GetPendingTaskCount());
  std::cout << "Elapsed time: " << timer.GetElapsed().GetMilliseconds() << std::endl;

  return true;
}

bool Test::ConditionVariable::RunPerformanceTest()
{
  std::cout << "[Test::ConditionVariable::RunPerformanceTest]" << std::endl;

  return true;
}
