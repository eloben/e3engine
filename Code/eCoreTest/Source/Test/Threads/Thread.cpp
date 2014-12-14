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

/** @file Thread.cpp
This file defines Thread test functions.
*/

#include <CoreTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

struct Task : public E::Threads::IRunnable
{
  explicit Task(const StringBuffer& s) : mString(s) {}

  I32 Run()
  {
    //Thread::Sleep(TimeValue::kOneMillisecond * 100);
    for (U32 i = 0; i < 1000; ++i)
    {
      std::cout << mString.GetPtr();
    }

    return 0;
  }

  StringBuffer mString;

};

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Thread::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Thread::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Thread::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::Thread::RunFunctionalityTest()
{
  std::cout << "[Test::Thread::RunFunctionalityTest]" << std::endl;

  /*-----------------------------------------------------------------
  Thread
  -----------------------------------------------------------------*/
  U32 taskCount = 20;
  E::Containers::List<E::Threads::Thread*> threadList;
  E::Containers::List<Task*> taskList;
  
  // Load and start thread tasks
  for (U32 i = 0; i < taskCount; ++i)
  {
    E::StringBuffer sb;
    sb << (char)('a' + i);
    Task* pTask = new Task(sb.GetPtr());
    E::Threads::Thread* pT = new E::Threads::Thread(*pTask);
    pT->Start();
    threadList.PushBack(pT);
    taskList.PushBack(pTask);
  }

  // Wait for termination
  for (E::Containers::List<E::Threads::Thread*>::ConstIterator cit = threadList.GetBegin(); cit != threadList.GetEnd(); ++cit)
  {
    (*cit)->WaitForTermination();
  }
  
  std::cout << std::endl;;

  // Sequential thread task
  for (E::Containers::List<E::Threads::Thread*>::ConstIterator cit = threadList.GetBegin(); cit != threadList.GetEnd(); ++cit)
  {
    (*cit)->Start();
    (*cit)->WaitForTermination();
  }

  // Delete threads
  for (E::Containers::List<E::Threads::Thread*>::ConstIterator cit = threadList.GetBegin(); cit != threadList.GetEnd(); ++cit) delete (*cit);

  std::cout << std::endl;

  /*-----------------------------------------------------------------
  ThreadPool
  -----------------------------------------------------------------*/

  std::cout << "ThreadPool Test" << std::endl;
  E::Time::Timer t;
  E::Threads::ThreadPool pool;
  
  pool.SetMaxActiveThreadCount(2);
  for (E::Containers::List<Task*>::ConstIterator cit = taskList.GetBegin(); cit != taskList.GetEnd(); ++cit) pool.AddItem(*cit);

  t.Reset();
  pool.WaitForIdle();
  std::cout << "\nWaited for Idle: " << t.GetElapsed().GetSeconds() << std::endl;
  std::cout << std::endl;
  std::cout << "ThreadPool - Wait for task Test " << std::endl;

  for (E::Containers::List<Task*>::ConstIterator cit = taskList.GetBegin(); cit != taskList.GetEnd(); ++cit) pool.AddItem(*cit);

  Task* pTaskToWait = taskList[Math::Global::GetRandom().GetU32(taskCount)];
  std::cout << "\nWaiting for task: " << pTaskToWait->mString.GetPtr() << std::endl;
 
  t.Reset();
  pool.WaitForItem(pTaskToWait);
  std::cout << "\nWaited for task " << pTaskToWait->mString.GetPtr()  << ": " << t.GetElapsed().GetSeconds() << std::endl;
  std::cout << std::endl;
  
  t.Reset();
  pool.WaitForIdle();
  std::cout << "\nWaited for Idle: " << t.GetElapsed().GetSeconds() << std::endl;
  std::cout << std::endl;

  for (E::Containers::List<Task*>::ConstIterator cit = taskList.GetBegin(); cit != taskList.GetEnd(); ++cit) delete (*cit);

  E::Threads::Atomic<U32> au;
  au.Get();
  ++au;
  au++;
  std::cout << std::endl;

  /*-----------------------------------------------------------------
  ThreadPool Wrong exit test
  -----------------------------------------------------------------*/
  Task someTask("*");
  Threads::Global::GetThreadPool().AddItem(&someTask);
  Threads::Global::GetThreadPool().WaitForIdle();
  Threads::Global::GetThreadPool().CleanUp(); // comment to trigger wrong exit (thread termination must be called from the process using the threads)
  /*
  Threads::ThreadPool somePool;
  somePool.AddItem(&someTask);
  somePool.WaitForIdle();*/
  return true;
}

bool Test::Thread::RunPerformanceTest()
{
  std::cout << "[Test::Thread::RunPerformanceTest]" << std::endl;

  return true;
}
