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

/** @file DX11Sampler.h
This file declares the DX11Sampler class.
*/

#ifndef E3_DX11_TEXTURE_SAMPLER_H
#define E3_DX11_TEXTURE_SAMPLER_H

// Forward declarations
struct ID3D10Blob;
struct ID3D11SamplerState;

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11Sampler
----------------------------------------------------------------------------------------------------------------------*/
class DX11Sampler : public ISampler
{
public:
  DX11Sampler();
  ~DX11Sampler();

  bool                                Initialize(const Descriptor& desc);
  void                                Finalize();

  const Descriptor&                   GetDescriptor() const;
  ID3D11SamplerState*                 GetDXSamplerState() const;

private:
  Descriptor                          mDescriptor;
  ID3D11SamplerState*                 mpDXSamplerState;
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(DX11Sampler)
};

/*----------------------------------------------------------------------------------------------------------------------
DX11Sampler types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<DX11Sampler> DX11SamplerInstance;
}
}

#endif
