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

/** @file DX11RasterState.cpp
This file defines the DX11RasterState class.
*/

#include <GraphicsPch.h>
#include "DX11RasterState.h"
#include "DX11Core.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

static const D3D11_CULL_MODE kDX11CullModeTable[] = 
{
	D3D11_CULL_NONE,
  D3D11_CULL_FRONT,
  D3D11_CULL_BACK
};

static const D3D11_FILL_MODE kDX11FillModeTable[] = 
{
	D3D11_FILL_WIREFRAME,
  D3D11_FILL_SOLID
};

/*----------------------------------------------------------------------------------------------------------------------
DX11RasterState initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11RasterState::DX11RasterState()
  : mpDXRasterState(nullptr)
{
}

Graphics::DX11RasterState::~DX11RasterState()
{
	Finalize();
}

bool Graphics::DX11RasterState::Initialize(const Descriptor& desc)
{
  mDescriptor = desc;

  D3D11_RASTERIZER_DESC dxDescriptor;
  ZeroMemory(&dxDescriptor, sizeof(dxDescriptor));

  // settings
  dxDescriptor.FillMode = kDX11FillModeTable[mDescriptor.fillMode];
  dxDescriptor.CullMode = kDX11CullModeTable[mDescriptor.cullMode];
  dxDescriptor.DepthBias = mDescriptor.depthBias;
  dxDescriptor.DepthBiasClamp = mDescriptor.depthBiasClamp;

  // Flags
  dxDescriptor.DepthClipEnable = (mDescriptor.flags & eRasterFlagEnableDepthClip) ? true : false;
  dxDescriptor.AntialiasedLineEnable = (mDescriptor.flags & eRasterFlagEnableAntialisedLine) ? true : false;
  dxDescriptor.FrontCounterClockwise = (mDescriptor.flags & eRasterFlagFrontCounterClockWise) ? true : false;
  dxDescriptor.MultisampleEnable = (mDescriptor.flags & eRasterFlagEnableMultisample) ? true : false;
  dxDescriptor.ScissorEnable = (mDescriptor.flags & eRasterFlagEnableScissor) ? true : false;
  dxDescriptor.SlopeScaledDepthBias = mDescriptor.slopeScaledDepthBias;

  if (GDXDevice->CreateRasterizerState(&dxDescriptor, &mpDXRasterState) < 0)
    return false;

  return true;
}

void Graphics::DX11RasterState::Finalize()
{
  Win32::ReleaseCom(mpDXRasterState);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11RasterState accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IRasterState::Descriptor& Graphics::DX11RasterState::GetDescriptor() const
{
  return mDescriptor;
}

ID3D11RasterizerState* Graphics::DX11RasterState::GetDXRasterState() const
{
  return mpDXRasterState;
}
