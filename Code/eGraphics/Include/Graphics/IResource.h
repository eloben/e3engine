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

// Created 03-Jul-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IResource.h
This file defines the IResource interface.
*/

#ifndef E3_IRESOURCE_H
#define E3_IRESOURCE_H

#include <Memory/GarbageCollection.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IResource
----------------------------------------------------------------------------------------------------------------------*/
class IResource
{
public:
  enum ResourceType
  {
    eResourceTypeBuffer,
    eResourceTypeTexture2D
  };

  enum AccessFlag
  {
    eAccessFlagNone,
    eAccessFlagGpuRead  = 1, 
    eAccessFlagGpuWrite = 2, 
    eAccessFlagCpuRead  = 4, 
    eAccessFlagCpuWrite = 8,
    eAccessFlagAll      = (eAccessFlagGpuRead | eAccessFlagGpuWrite | eAccessFlagGpuRead | eAccessFlagCpuWrite)
  }; 

  virtual                 ~IResource() {}
  virtual U32             GetAccessFlags() const = 0;
  virtual ResourceType    GetResourceType() const = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IResource types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IResource> IResourceInstance;
}
}

#endif
