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

/** @file IDepthStencilState.h
This file defines the IDepthStencilState interface and the Descriptor struct.
*/

#ifndef E3_IDEPTH_STENCIL_STATE_H
#define E3_IDEPTH_STENCIL_STATE_H

#include <Memory/GarbageCollection.h>

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IDepthStencilState
----------------------------------------------------------------------------------------------------------------------*/
class IDepthStencilState
{
public:
  enum ComparisonFunction
  {
    eComparisonFunctionNever,
    eComparisonFunctionLess,
    eComparisonFunctionEqual,
    eComparisonFunctionLessOrEqual,
    eComparisonFunctionGreater,
    eComparisonFunctionNotEqual,
    eComparisonFunctionGreaterOrEqual,
    eComparisonFunctionAlways          
  };

  enum StencilFunction
  {
    eStencilFunctionKeep,
    eStencilFunctionZero,
    eStencilFunctionReplace,
    eStencilFunctionIncrementSaturate,
    eStencilFunctionDecrementSaturate,
    eStencilFunctionInvert,
    eStencilFunctionIncrement,
    eStencilFunctionDecrement
  };

  enum DepthStencilFlag
  {
    eDepthStencilFlagNone               = 0,
    eDepthStencilFlagEnableDepth        = 1,
    eDepthStencilFlagEnableDepthWriting = 2,
    eDepthStencilFlagEnableStencil      = 4, 
    eDepthStencilFlagEnableAll          = eDepthStencilFlagEnableDepth | eDepthStencilFlagEnableDepthWriting | eDepthStencilFlagEnableStencil
  };

  struct Descriptor
  {
    ComparisonFunction  depthFunction;
    StencilFunction     backFaceFailStencilFunction;
    StencilFunction     backFaceDepthFailStencilFunction;
    StencilFunction     backFacePassStencilFunction;
    ComparisonFunction  backFaceStencilComparisonFunction;
    StencilFunction     frontFaceFailStencilFunction;
    StencilFunction     frontFaceDepthFailStencilFunction;
    StencilFunction     frontFacePassStencilFunction;
    ComparisonFunction  frontFaceStencilComparisonFunction;
    U8                  stencilReadMask;
    U8                  stencilWriteMask;
    U8                  flags;    

    Descriptor::Descriptor()
      : depthFunction(eComparisonFunctionLess)
      , backFaceFailStencilFunction(eStencilFunctionKeep)
      , backFaceDepthFailStencilFunction(eStencilFunctionDecrement)
      , backFacePassStencilFunction(eStencilFunctionKeep)
      , backFaceStencilComparisonFunction(eComparisonFunctionAlways)
      , frontFaceFailStencilFunction(eStencilFunctionKeep)
      , frontFaceDepthFailStencilFunction(eStencilFunctionIncrement)
      , frontFacePassStencilFunction(eStencilFunctionKeep)
      , frontFaceStencilComparisonFunction(eComparisonFunctionAlways)
      , stencilReadMask(0xFF)
      , stencilWriteMask(0xFF)
      , flags(eDepthStencilFlagEnableDepth | eDepthStencilFlagEnableDepthWriting)
    {}

    static Descriptor& Default() { static Descriptor sInstance; return sInstance; }
  };

  virtual                   ~IDepthStencilState() {}
  virtual const Descriptor& GetDescriptor() const = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IDepthStencilState types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IDepthStencilState> IDepthStencilStateInstance;
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Graphics::IDepthStencilState::Descriptor)

#endif

