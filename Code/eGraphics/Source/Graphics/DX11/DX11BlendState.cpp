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

/** @file DX11BlendState.cpp
This file defines the DX11BlendState class.
*/

#include <GraphicsPch.h>
#include "DX11BlendState.h"
#include "DX11Core.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

static const D3D11_BLEND kDX11BlendFactorTable[] = 
{
  D3D11_BLEND_ZERO,
  D3D11_BLEND_ONE,
  D3D11_BLEND_SRC_COLOR,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_SRC_ALPHA,
  D3D11_BLEND_INV_SRC_ALPHA,
  D3D11_BLEND_DEST_ALPHA,
  D3D11_BLEND_INV_DEST_ALPHA,
  D3D11_BLEND_DEST_COLOR,
  D3D11_BLEND_INV_DEST_COLOR
};

static const D3D11_BLEND_OP kDX11BlendFunctionTable[] = 
{
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_SUBTRACT,
  D3D11_BLEND_OP_REV_SUBTRACT,
  D3D11_BLEND_OP_MIN,
  D3D11_BLEND_OP_MAX
};

static const F32 kDefaultBlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

/*----------------------------------------------------------------------------------------------------------------------
DX11BlendState initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11BlendState::DX11BlendState()
  : mpDXBlendState(nullptr)
{
}

Graphics::DX11BlendState::~DX11BlendState()
{
	Finalize();
}

bool Graphics::DX11BlendState::Initialize(const Descriptor& desc)
{
  mDescriptor = desc;

  D3D11_BLEND_DESC dxDescriptor;
  ZeroMemory(&dxDescriptor, sizeof(dxDescriptor));

  // dxDescriptor.AlphaToCoverageEnable = true;
  dxDescriptor.RenderTarget[0].BlendEnable = (mDescriptor.flags & eBlendFlagEnableBlend) ? true : false;
  dxDescriptor.RenderTarget[0].SrcBlend = kDX11BlendFactorTable[mDescriptor.sourceFactor];
  dxDescriptor.RenderTarget[0].DestBlend = kDX11BlendFactorTable[mDescriptor.destinationFactor];
  dxDescriptor.RenderTarget[0].BlendOp = kDX11BlendFunctionTable[mDescriptor.blendFunction];
  dxDescriptor.RenderTarget[0].SrcBlendAlpha = kDX11BlendFactorTable[mDescriptor.alphaSourceFactor];
  dxDescriptor.RenderTarget[0].DestBlendAlpha = kDX11BlendFactorTable[mDescriptor.alphaDestinationFactor];
  dxDescriptor.RenderTarget[0].BlendOpAlpha = kDX11BlendFunctionTable[mDescriptor.alphaBlendFunction];
  dxDescriptor.RenderTarget[0].RenderTargetWriteMask = mDescriptor.writeMask;
  dxDescriptor.AlphaToCoverageEnable = (mDescriptor.flags & eBlendFlagEnableAlphaToCoverage) ? true : false;

  if (GDXDevice->CreateBlendState(&dxDescriptor, &mpDXBlendState) < 0)
    return false;

  return true;
}

void Graphics::DX11BlendState::Finalize()
{
  Win32::ReleaseCom(mpDXBlendState);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11BlendState accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IBlendState::Descriptor& Graphics::DX11BlendState::GetDescriptor() const
{
  return mDescriptor;
}

ID3D11BlendState* Graphics::DX11BlendState::GetDXBlendState() const
{
  return mpDXBlendState;
}
