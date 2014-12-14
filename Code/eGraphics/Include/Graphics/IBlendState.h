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

/** @file IBlendState.h
This file defines the IBlendState interface and the Descriptor struct.
*/

#ifndef E3_IBLEND_STATE_H
#define E3_IBLEND_STATE_H

#include <Base.h>
#include <Memory/GarbageCollection.h>

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IBlendState
----------------------------------------------------------------------------------------------------------------------*/
class IBlendState
{
public:
  enum BlendFactor
  {
    eBlendFactorZero,
    eBlendFactorOne,
    eBlendFactorSourceColor,
    eBlendFactorInverseSourceColor,
    eBlendFactorSourceAlpha,
    eBlendFactorInverseSourceAlpha,
    eBlendFactorDestinationAlpha,
    eBlendFactorInverseDestinationAlpha,
    eBlendFactorDestinationColor,
    eBlendFactorInverseDestinationColor
  };

  enum BlendFunction
  {
    eBlendFunctionAdd,
    eBlendFunctionSubstract,
    eBlendFunctionReverseSubstract,
    eBlendFunctionMin,
    eBlendFunctionMax
  };

  enum WriteMask
  {
    eWriteMaskRed    = 1,
    eWriteMaskGreen  = 2,
    eWriteMaskBlue   = 4,
    eWriteMaskAlpha  = 8,
    eWriteMaskAll    = eWriteMaskRed | eWriteMaskGreen | eWriteMaskBlue | eWriteMaskAlpha
  };

  enum BlendFlag
  {
    eBlendFlagNone                   = 0,
    eBlendFlagEnableBlend            = 1,
    eBlendFlagEnableAlphaToCoverage  = 2
  };

  struct Descriptor
  {
    BlendFactor     sourceFactor;
    BlendFactor     destinationFactor;
    BlendFunction   blendFunction;
    BlendFactor     alphaSourceFactor;
    BlendFactor     alphaDestinationFactor;
    BlendFunction   alphaBlendFunction;
    U8              writeMask;
    U8              flags;

    Descriptor::Descriptor()
      : sourceFactor(eBlendFactorOne)
      , destinationFactor(eBlendFactorZero)
      , blendFunction(eBlendFunctionAdd)
      , alphaSourceFactor(eBlendFactorOne)
      , alphaDestinationFactor(eBlendFactorZero)
      , alphaBlendFunction(eBlendFunctionAdd)
      , writeMask(eWriteMaskAll)
      , flags(eBlendFlagNone)
    {}

    static Descriptor& Default() { static Descriptor sInstance; return sInstance; }
  };

  virtual                   ~IBlendState() {}
  virtual const Descriptor& GetDescriptor() const = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IBlendState types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IBlendState> IBlendStateInstance;
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Graphics::IBlendState::Descriptor)

#endif
