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

// Created 20-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IRenderTarget.h
This file defines the IRenderTarget interface. The IRenderTarget is the entry point to issue render calls.
*/

#ifndef E3_IRENDER_TARGET_H
#define E3_IRENDER_TARGET_H

#include <Containers/DynamicArray.h>
#include <Graphics/ITexture2D.h>
#include <Graphics/VertexPrimitive.h>
#include <Memory/GarbageCollection.h>

namespace E 
{
namespace Graphics
{
// Forward declarations
class Color;

/*----------------------------------------------------------------------------------------------------------------------
IRenderTarget
----------------------------------------------------------------------------------------------------------------------*/
class IRenderTarget
{
public:
  enum ClearFlag
  {
    eClearFlagColor = 1,
    eClearFlagDepth = 2,
    eClearFlagAll   = eClearFlagColor | eClearFlagDepth
  };

  struct Descriptor
  {
    Containers::DynamicArray<ITexture2DInstance>  colorTargets;
    ITexture2DInstance                            depthTarget;
  };

  virtual                   ~IRenderTarget() {}
  virtual const Descriptor& GetDescriptor() const = 0;

  virtual void              Clear(const Color& clearColor, U8 clearFlags) = 0;
  virtual void              Draw(VertexPrimitive primitive, U32 vertexCount, U32 indexCount, U32 instanceCount, U32 startVertex, U32 startIndex, U32 startInstance) = 0;
  virtual void              DrawIndirect(VertexPrimitive primitive, U32 vertexCount, U32 indexCount, U32 instanceCount, U32 startVertex, U32 startIndex, U32 startInstance) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IRenderTarget types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IRenderTarget> IRenderTargetInstance;
}
}

#endif
