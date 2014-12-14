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

// Created 19-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IBuffer.h
This file defines the IBuffer interface.
*/

#ifndef E3_IBUFFER_H
#define E3_IBUFFER_H

#include <Graphics/IResource.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IBuffer

Please note that this interface has the following usage contract:

1. Element data initializer immediately allocates Gpu data.
2. No element data initializer delays the Gpu allocation until binding. Note that buffer capacity is unknown until
the buffer is bound.
3. Add is used to add element data to the buffer. This data must be Gpu allocated before binding the buffer.
----------------------------------------------------------------------------------------------------------------------*/
class IBuffer : public IResource
{
public:
  enum Type
  {
    eTypeConstant,
    eTypeVertex,
    eTypeIndex,
    eTypeResource
  };

  enum GpuWriteAccess
  {
    eGraphicsWriteAccessNone,
    eGraphicsWriteAccessRaw,
    eGraphicsWriteAccessStructured,
    eGraphicsWriteAccessAppendConsume
  };

  struct Descriptor 
  { 
    Type            type; 
    size_t          elementSize;
    U32             accessFlags;
    GpuWriteAccess  gpuWriteAccess;

    Descriptor() 
      : type(eTypeConstant) 
      , elementSize(0)
      , accessFlags(eAccessFlagGpuRead) 
      , gpuWriteAccess(eGraphicsWriteAccessNone)
    {} 
  }; 

  virtual const Descriptor& GetDescriptor() const = 0;
  virtual U32               GetCount() const = 0;

  virtual void              Add(const void* pElements, U32 elementCount) = 0;
  virtual void              Clear() = 0;
  virtual void              Remove(U32 elementCount, U32 startIndex) = 0;
  virtual void              Set(const void* pElements, U32 elementCount, U32 startIndex) = 0;
  virtual bool              Update() = 0;
};
    
/*----------------------------------------------------------------------------------------------------------------------
IBuffer types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IBuffer> IBufferInstance;
}
}

#endif
