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

/** @file IRasterState.h
This file defines the IRasterState interface and the Descriptor struct.
*/

#ifndef E3_IRASTER_STATE_H
#define E3_IRASTER_STATE_H

#include <Memory/GarbageCollection.h>

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IRasterState
----------------------------------------------------------------------------------------------------------------------*/
class IRasterState
{
public:
  enum CullMode
  {
    eCullModeNone,
    eCullModeFront,
    eCullModeBack
  };

  enum FillMode
  {
    eFillModeWireframe,
    eFillModeSolid
  };

  enum RasterFlag
  {
    eRasterFlagNone                   = 0,
    eRasterFlagFrontCounterClockWise  = 1,
    eRasterFlagEnableDepthClip        = 2,
    eRasterFlagEnableScissor          = 4,
    eRasterFlagEnableMultisample      = 8,
    eRasterFlagEnableAntialisedLine   = 16
  };
  struct Descriptor
  {
    CullMode    cullMode;
    FillMode    fillMode;
    I32         depthBias;
    F32         depthBiasClamp;
    F32         slopeScaledDepthBias;
    U8          flags;

    Descriptor::Descriptor()
      : cullMode(eCullModeBack)
      , fillMode(eFillModeSolid)
      , depthBias(0)
      , depthBiasClamp(0.0f)
      , slopeScaledDepthBias(0.0f)
      , flags(eRasterFlagEnableDepthClip)
    {}

    static Descriptor& Default() { static Descriptor sInstance; return sInstance; }
  };

  virtual                   ~IRasterState() {}
  virtual const Descriptor& GetDescriptor() const = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IRasterState types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IRasterState> IRasterStateInstance;
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Graphics::IRasterState::Descriptor)

#endif
