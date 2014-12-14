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

/** @file ISampler.h
This file defines the ISampler interface and the Descriptor struct.
*/

#ifndef E3_ITEXTURE_SAMPLER_H
#define E3_ITEXTURE_SAMPLER_H

#include <Graphics/Color.h>
#include <Math/Comparison.h>
#include <Memory/GarbageCollection.h>

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
ISampler
----------------------------------------------------------------------------------------------------------------------*/
class ISampler
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

  enum AddressMode
  {
    eAddressModeWrap,
    eAddressModeMirror,
    eAddressModeClamp,
    eAddressModeBorder,
    eAddressModeMirrorOnce
  };

  enum Filter
  {
    eFilterMinMagMipPoint,
    eFilterMinMagPointMipLinear,
    eFilterMinPointMagLinearMipPoint,
    eFilterMinPointMagMipLinear,
    eFilterMinLinearMagMipPoint,
    eFilterMinLinearMagPointMipLinear,
    eFilterMinMagLinearMipPoint,
    eFilterMinMagMipLinear,
    eFilterAnisotropic,

    eFilterComparisonMinMagMipPoint,
    eFilterComparisonMinMagPointMipLinear,
    eFilterComparisonMinPointMagLinearMipPoint,
    eFilterComparisonMinPointMagMipLinear,
    eFilterComparisonMinLinearMagMipPoint,
    eFilterComparisonMinLinearMagPointMipLinear,
    eFilterComparisonMinMagLinearMipPoint,
    eFilterComparisonMinMagMipLinear,
    eFilterComparisonAnisotropic
  };

  struct Descriptor
  {
    Filter              filter;
    AddressMode         addressU;
    AddressMode         addressV;
    AddressMode         addressW;
    ComparisonFunction  comparisonFunction;
    Color               borderColor;
    F32                 maxLod;
    F32                 minLod;
    F32                 mipLodBias;
    U32                 maxAnisotropy;

    Descriptor::Descriptor()
      : filter(eFilterMinMagMipLinear)
      , addressU(eAddressModeClamp)
      , addressV(eAddressModeClamp)
      , addressW(eAddressModeClamp)
      , comparisonFunction(eComparisonFunctionNever)
      , borderColor(Color::eBlack)
      , maxLod(Math::NumericLimits<F32>::Max())
      , minLod(0.0f)
      , mipLodBias(0.0f)
      , maxAnisotropy(1)
    {}

    static Descriptor& Default() { static Descriptor sInstance; return sInstance; }
  };

  virtual                   ~ISampler() {}
  virtual const Descriptor& GetDescriptor() const = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ISampler types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<ISampler> ISamplerInstance;
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Graphics::ISampler::Descriptor)

#endif
