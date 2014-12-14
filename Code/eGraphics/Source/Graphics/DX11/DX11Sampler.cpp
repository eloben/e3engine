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

/** @file DX11Sampler.cpp
This file defines the DX11Sampler class.
*/

#include <GraphicsPch.h>
#include "DX11Sampler.h"
#include "DX11Core.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

static const D3D11_COMPARISON_FUNC kDX11SamplerComparisonFunctionTable[] = 
{
  D3D11_COMPARISON_NEVER,
  D3D11_COMPARISON_LESS,
  D3D11_COMPARISON_EQUAL,
  D3D11_COMPARISON_LESS_EQUAL,
  D3D11_COMPARISON_GREATER,
  D3D11_COMPARISON_NOT_EQUAL,
  D3D11_COMPARISON_GREATER_EQUAL,
  D3D11_COMPARISON_ALWAYS
};

static const D3D11_TEXTURE_ADDRESS_MODE kDX11SamplerAddressModeTable[] = 
{
  D3D11_TEXTURE_ADDRESS_WRAP,
  D3D11_TEXTURE_ADDRESS_MIRROR,
  D3D11_TEXTURE_ADDRESS_CLAMP,
  D3D11_TEXTURE_ADDRESS_BORDER,
  D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};

static const D3D11_FILTER kDX11SamplerFilterTable[] = 
{
  D3D11_FILTER_MIN_MAG_MIP_POINT,
  D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
  D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
  D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
  D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
  D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
  D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
  D3D11_FILTER_MIN_MAG_MIP_LINEAR,
  D3D11_FILTER_ANISOTROPIC,
  D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
  D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
  D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
  D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
  D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
  D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
  D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
  D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
  D3D11_FILTER_COMPARISON_ANISOTROPIC
};

/*----------------------------------------------------------------------------------------------------------------------
DX11Sampler initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11Sampler::DX11Sampler()
  : mpDXSamplerState(nullptr)
{
}

Graphics::DX11Sampler::~DX11Sampler()
{
	Finalize();
}

bool Graphics::DX11Sampler::Initialize(const Descriptor& desc)
{
  mDescriptor = desc;

  D3D11_SAMPLER_DESC dxDescriptor;
  ZeroMemory(&dxDescriptor, sizeof(dxDescriptor));

  dxDescriptor.Filter = kDX11SamplerFilterTable[mDescriptor.filter];
  dxDescriptor.AddressU = kDX11SamplerAddressModeTable[mDescriptor.addressU];
  dxDescriptor.AddressV = kDX11SamplerAddressModeTable[mDescriptor.addressV];
  dxDescriptor.AddressW = kDX11SamplerAddressModeTable[mDescriptor.addressW];
  dxDescriptor.MipLODBias = mDescriptor.mipLodBias;
  dxDescriptor.MaxAnisotropy = mDescriptor.maxAnisotropy;
  dxDescriptor.ComparisonFunc = kDX11SamplerComparisonFunctionTable[mDescriptor.comparisonFunction];
  dxDescriptor.BorderColor[0] = mDescriptor.borderColor.Get()[0];
  dxDescriptor.BorderColor[1] = mDescriptor.borderColor.Get()[1];
  dxDescriptor.BorderColor[2] = mDescriptor.borderColor.Get()[2];
  dxDescriptor.BorderColor[3] = mDescriptor.borderColor.Get()[3];
  dxDescriptor.MinLOD = mDescriptor.minLod;
  dxDescriptor.MaxLOD = mDescriptor.maxLod;

  return (GDXDevice->CreateSamplerState(&dxDescriptor, &mpDXSamplerState) == S_OK);
}

void Graphics::DX11Sampler::Finalize()
{
  Win32::ReleaseCom(mpDXSamplerState);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Sampler accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::ISampler::Descriptor& Graphics::DX11Sampler::GetDescriptor() const
{
  return mDescriptor;
}

ID3D11SamplerState* Graphics::DX11Sampler::GetDXSamplerState() const
{
  return mpDXSamplerState;
}
