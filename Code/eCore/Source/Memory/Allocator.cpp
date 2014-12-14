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
------------------------------------------------------------------------------------------------------------------------*/

// Created 03-Nov-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Allocator.cpp
This file defines the library global allocator accessor methods.
*/

#include <CorePch.h>

namespace E
{
namespace Memory
{
/*----------------------------------------------------------------------------------------------------------------------
AllocatorProvider

Please note that this class has the following usage contract: 

1. AllocatorProvider implements a Service Locator pattern to provide a configurable global allocator.
2. The global allocator MUST implement the IAllocator interface.
3. AllocatorProvider is intended to be used only by Memory::GlobalAllocator / Memory::GlobalAllocatorProviderAPI calls 
to provide access to the library global allocator. Client code MUST NOT manually access this class through
Singleton <AllocatorProvider>::GetInstance() as it would retrieve its own copy in its executable / dll. The above
Memory calls must be used for that purpose in order to have a unified and consistent memory model.
----------------------------------------------------------------------------------------------------------------------*/	
class AllocatorProvider
{
public:
  IAllocator*       GetAllocator() const                  { return mpAllocator; }
  void              SetAllocator(IAllocator* pAllocator)  { mpAllocator = pAllocator; }
  void              ResetDefaultAllocator()               { mpAllocator = &mDefaultAllocator; }

  DefaultAllocator  mDefaultAllocator;
  IAllocator*       mpAllocator;

private:
  AllocatorProvider() : mpAllocator(&mDefaultAllocator) {}

  E_DECLARE_SINGLETON_FRIENDLY(AllocatorProvider)
};
/*----------------------------------------------------------------------------------------------------------------------
Memory::Global methods

Please note that these methods must be defined in a source file instead of: 

inline IAllocator* Global::GetAllocator()                         { return Singleton<AllocatorProvider>::GetInstance().GetAllocator(); }
inline void Global::SetAllocator(Memory::IAllocator* pAllocator)  { return Singleton<AllocatorProvider>::GetInstance()..SetAllocator(pAllocator); }

We need to guarantee a unique global allocator across DLLs as static variables are local to the compilation unit.
----------------------------------------------------------------------------------------------------------------------*/
IAllocator* Global::GetAllocator()
{
  return Singleton<AllocatorProvider>::GetInstance().GetAllocator();
}

void Global::SetAllocator(IAllocator* pAllocator)
{
  return Singleton<AllocatorProvider>::GetInstance().SetAllocator(pAllocator);
}

void Global::SetDefaultAllocator()
{
  return Singleton<AllocatorProvider>::GetInstance().ResetDefaultAllocator();
}
}
}
