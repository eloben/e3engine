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

/** @file DX11DepthStencilState.cpp
This file defines the DX11DepthStencilState class.
*/

#include <GraphicsPch.h>
#include "DX11DepthStencilState.h"
#include "DX11Core.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

static const D3D11_COMPARISON_FUNC kDX11DepthStencilStateComparisonFunctionTable[] = 
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

static const D3D11_STENCIL_OP kDX11DepthStencilStateFunctionTable[] = 
{
  D3D11_STENCIL_OP_KEEP,
  D3D11_STENCIL_OP_ZERO,
  D3D11_STENCIL_OP_REPLACE,
  D3D11_STENCIL_OP_INCR_SAT,
  D3D11_STENCIL_OP_DECR_SAT,
  D3D11_STENCIL_OP_INVERT,
  D3D11_STENCIL_OP_INCR,
  D3D11_STENCIL_OP_DECR
};

/*----------------------------------------------------------------------------------------------------------------------
DX11DepthStencilState initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11DepthStencilState::DX11DepthStencilState()
  : mpDXDepthStencilState(nullptr)
{
}

Graphics::DX11DepthStencilState::~DX11DepthStencilState()
{
	Finalize();
}

bool Graphics::DX11DepthStencilState::Initialize(const Descriptor& desc)
{
  mDescriptor = desc;

  D3D11_DEPTH_STENCIL_DESC dxDescriptor;
  ZeroMemory(&dxDescriptor, sizeof(dxDescriptor));

  // Depth settings
  dxDescriptor.DepthEnable = (mDescriptor.flags & eDepthStencilFlagEnableDepth) ? true : false;
  dxDescriptor.DepthWriteMask = (mDescriptor.flags & eDepthStencilFlagEnableDepthWriting) ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
  dxDescriptor.DepthFunc = kDX11DepthStencilStateComparisonFunctionTable[mDescriptor.depthFunction];
  
  // Stencil settings
  dxDescriptor.StencilEnable = (mDescriptor.flags & eDepthStencilFlagEnableStencil) ? true : false;
  dxDescriptor.StencilReadMask = mDescriptor.stencilReadMask;
  dxDescriptor.StencilWriteMask = mDescriptor.stencilWriteMask;

  // Front face
  dxDescriptor.FrontFace.StencilFailOp = kDX11DepthStencilStateFunctionTable[mDescriptor.frontFaceFailStencilFunction];
  dxDescriptor.FrontFace.StencilDepthFailOp = kDX11DepthStencilStateFunctionTable[mDescriptor.frontFaceDepthFailStencilFunction];
  dxDescriptor.FrontFace.StencilPassOp = kDX11DepthStencilStateFunctionTable[mDescriptor.frontFacePassStencilFunction];
  dxDescriptor.FrontFace.StencilFunc = kDX11DepthStencilStateComparisonFunctionTable[mDescriptor.frontFaceStencilComparisonFunction];

  // Back face
  dxDescriptor.BackFace.StencilFailOp = kDX11DepthStencilStateFunctionTable[mDescriptor.backFaceFailStencilFunction];
  dxDescriptor.BackFace.StencilDepthFailOp = kDX11DepthStencilStateFunctionTable[mDescriptor.backFaceDepthFailStencilFunction];
  dxDescriptor.BackFace.StencilPassOp = kDX11DepthStencilStateFunctionTable[mDescriptor.backFacePassStencilFunction];
  dxDescriptor.BackFace.StencilFunc = kDX11DepthStencilStateComparisonFunctionTable[mDescriptor.backFaceStencilComparisonFunction];

  if (GDXDevice->CreateDepthStencilState(&dxDescriptor, &mpDXDepthStencilState) < 0)
    return false;

  return true;
}

void Graphics::DX11DepthStencilState::Finalize()
{
  Win32::ReleaseCom(mpDXDepthStencilState);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11DepthStencilState accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IDepthStencilState::Descriptor& Graphics::DX11DepthStencilState::GetDescriptor() const
{
  return mDescriptor;
}

ID3D11DepthStencilState* Graphics::DX11DepthStencilState::GetDXDepthStencilState() const
{
  return mpDXDepthStencilState;
}

