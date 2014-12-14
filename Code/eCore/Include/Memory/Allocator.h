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

// Created 04-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Allocator.h
This file defines a series of basic allocation template interfaces and classes and the global allocator class GAllocator.
The allocation classes defined are:

- IAllocator: a basic template interface for memory allocation
- DefaultAllocator: a basic IAlllocator based allocation class
- AlignedAllocator: an aligned allocator IAllocator class
- ProxyAllocated: base class to allow automatic allocation through the GAllocator 

GAllocator is a singleton type class and it is used by default by all Utility library classes requiring heap allocation.
The allocation class used by GAllocator can be user defined (See below).
*/

#ifndef E3_ALLOCATOR_H
#define E3_ALLOCATOR_H

#include "Heap.h"

namespace E 
{
namespace Memory
{
//Forward declarations
class IAllocator;
class AllocatorProvider;

/*----------------------------------------------------------------------------------------------------------------------
Memory::Global methods

Please note that this namespace methods have the following usage contract:

1. E_API methods get access to the library global allocator defining a unified memory model across executables / dlls.
2. Custom allocators can be defined ensuring a consistent used through this and other libraries by using the 
Memory::GlobalAllocatorProvider().SetAllocator method.
----------------------------------------------------------------------------------------------------------------------*/
namespace Global
{
E_API IAllocator* GetAllocator();
E_API void        SetAllocator(Memory::IAllocator* pAllocator);
E_API void        SetDefaultAllocator();
}

/*----------------------------------------------------------------------------------------------------------------------
IAllocator

Please note that this class has the following usage contract:

1. IAllocator allows tagging allocation types using the extensible enum type Tag. In order to extend it adding
custom Tag types the setting macro E_SETTING_CUSTOM_MEMORY_TAGS must be defined. E_SETTING_CUSTOM_MEMORY_TAGS
must define the additional Tag types desired. For this purpose the helper macro E_ALLOCATOR_TAG is supplied e.g:

#define E_SETTING_CUSTOM_MEMORY_TAGS \
E_ALLOCATOR_TAG(CustomNew) \
E_ALLOCATOR_TAG(CustomDelete)

2. Custom Tag types must be defined in pairs (xNew, xDelete) as described in the example.
3. A default Tag is included for general purpose however it is not used within the library.
4. A Tag type counter eMemoryTagCount is included.
----------------------------------------------------------------------------------------------------------------------*/	
class IAllocator
{
public:
  enum Tag
  {
    #define E_ALLOCATOR_TAG(name) eTag##name,
    E_ALLOCATOR_TAG(New)
    E_ALLOCATOR_TAG(Delete)
    E_ALLOCATOR_TAG(ArrayNew)
    E_ALLOCATOR_TAG(ArrayDelete)
    E_ALLOCATOR_TAG(FactoryNew)
    E_ALLOCATOR_TAG(FactoryDelete)
    E_ALLOCATOR_TAG(ProxyNew)
    E_ALLOCATOR_TAG(ProxyDelete)
    #ifdef E_SETTING_CUSTOM_MEMORY_TAGS
      E_SETTING_CUSTOM_MEMORY_TAGS
    #endif
    E_ALLOCATOR_TAG(Count)
    #undef E_ALLOCATOR_TAG
  };

  virtual ~IAllocator() {}

  virtual void* Allocate(size_t size, const Tag tag) = 0;
  virtual void  Deallocate(void* p, const Tag tag) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
DefaultAllocator
----------------------------------------------------------------------------------------------------------------------*/	
class DefaultAllocator : public IAllocator
{
public:
  DefaultAllocator() {}

  void*  Allocate(size_t size, const Tag = IAllocator::eTagNew) { return Heap::Allocate(size); }
  void   Deallocate(void* p, const Tag = IAllocator::eTagDelete) { Heap::Deallocate(p); }

private:
  E_DISABLE_COPY_AND_ASSSIGNMENT(DefaultAllocator)    
};

/*----------------------------------------------------------------------------------------------------------------------
ProxyAllocated

Please note that this class has the following usage contract:

1. ProxyAllocated allows child classes to delegate allocation / deallocation operations to the Memory global allocator
whenever using operators new & delete.
----------------------------------------------------------------------------------------------------------------------*/
struct ProxyAllocated
{
  void* operator new(size_t s)  { return Global::GetAllocator()->Allocate(s, IAllocator::eTagProxyNew); }
  void operator delete(void* p) { Global::GetAllocator()->Deallocate(p, IAllocator::eTagProxyDelete); }
};
}
}

#endif
