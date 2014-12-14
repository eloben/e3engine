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

/** @file DX11RenderTarget.cpp
This file defines the DX11RenderTarget class.
*/

#include <GraphicsPch.h>
#include "DX11RenderTarget.h"
#include "DX11Texture2D.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
DX11RenderTarget assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_DX11_DRAW_CONTEXT_COLOR_TARGET_COUNT_VALUE      "DX11 color target count cannot exceed %d"
#define E_ASSERT_MSG_DX11_DRAW_CONTEXT_COLOR_TARGET_UNIT_COUNT_VALUE "Texture unit count value must be one when combining different textures"
#define E_ASSERT_MSG_DX11_DRAW_CONTEXT_COLOR_TARGET_TEXTURE_TYPE     "ITexture2D must be of type eTypeColorTarget"
#define E_ASSERT_MSG_DX11_DRAW_CONTEXT_DEPTH_TARGET_TEXTURE_TYPE     "ITexture2D must be of type eTypeDepthTarget"
#define E_ASSERT_MSG_DX11_DRAW_CONTEXT_VALID_CONTEXT                 "Draw context must include minimum one color target or a depth target"

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

static const D3D_PRIMITIVE_TOPOLOGY kDX11PrimitiveTable[] =
{
  D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
  D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
  D3D_PRIMITIVE_TOPOLOGY_LINELIST,
  D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
  D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
  D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

/*----------------------------------------------------------------------------------------------------------------------
DX11RenderTarget initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11RenderTarget::DX11RenderTarget()
  : mpDXDepthStencilView(nullptr)
  , mpDXRenderTargetViews()
{
}

Graphics::DX11RenderTarget::~DX11RenderTarget()
{
  Finalize();
}

bool Graphics::DX11RenderTarget::Initialize(const Descriptor& desc)
{
  E_ASSERT_MSG(desc.colorTargets.GetSize() && desc.colorTargets[0] != nullptr || desc.depthTarget != nullptr, E_ASSERT_MSG_DX11_DRAW_CONTEXT_VALID_CONTEXT);
  if (desc.depthTarget != nullptr && !CreateDepthStencilView(desc.depthTarget))  return false;
  if (desc.colorTargets.GetSize())
  {
    E_ASSERT_MSG(
      desc.colorTargets.GetSize() <= D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT,    
      E_ASSERT_MSG_DX11_DRAW_CONTEXT_COLOR_TARGET_COUNT_VALUE, 
      D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);

    for (U32 i = 0; i < desc.colorTargets.GetSize(); ++i)
    {
      E_ASSERT_MSG(desc.colorTargets[i]->GetDescriptor().unitCount == 1, E_ASSERT_MSG_DX11_DRAW_CONTEXT_COLOR_TARGET_UNIT_COUNT_VALUE);
      if (!CreateRenderTargetView(i, desc.colorTargets[i]))
      {
        Finalize();
        return false;
      }
    }
  }
  
  mDescriptor = desc;
  return true;
}

void Graphics::DX11RenderTarget::Finalize()
{
  for (U32 i = 0; i < mDescriptor.colorTargets.GetSize(); ++i) Win32::ReleaseCom(mpDXRenderTargetViews[i]);
  Win32::ReleaseCom(mpDXDepthStencilView);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11RenderTarget accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IRenderTarget::Descriptor& Graphics::DX11RenderTarget::GetDescriptor() const
{
  return mDescriptor;
}

ID3D11DepthStencilView* Graphics::DX11RenderTarget::GetDXDepthTarget() const
{
  return mpDXDepthStencilView;
}

ID3D11RenderTargetView*const * Graphics::DX11RenderTarget::GetDXColorTargets() const
{
  return mpDXRenderTargetViews;
}

/*----------------------------------------------------------------------------------------------------------------------
DX11RenderTarget methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::DX11RenderTarget::Clear(const Color& clearColor, U8 clearFlags)
{
  ID3D11DeviceContext* pDXDeviceContext = GDXDeviceContext;
  if (clearFlags & eClearFlagColor && mpDXRenderTargetViews[0])
  {
    // Only when using D3D_FEATURE_LEVEL_9_x, ClearRenderTargetView only clears the first array slice in the render target view
    pDXDeviceContext->ClearRenderTargetView(mpDXRenderTargetViews[0], clearColor.Get());
  }
  if (clearFlags & eClearFlagDepth && mpDXDepthStencilView)
  {
    pDXDeviceContext->ClearDepthStencilView(
      mpDXDepthStencilView,
      D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
      1.0f,
      0);
  }
}

void Graphics::DX11RenderTarget::Draw(VertexPrimitive vertexPrimitive, U32 vertexCount, U32 indexCount, U32 instanceCount, U32 startVertex, U32 startIndex, U32 startInstance)
{
  GDXDeviceContext->IASetPrimitiveTopology(kDX11PrimitiveTable[vertexPrimitive]);
  if (indexCount)
  {
    if (instanceCount)
    {
      GDXDeviceContext->DrawIndexedInstanced(indexCount, instanceCount, startIndex, startVertex, startInstance);
    }
    else
    {
      GDXDeviceContext->DrawIndexed(indexCount, startIndex, startVertex);
    }
  }
  else
  {
    if (instanceCount)
    {
      GDXDeviceContext->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
    }
    else
    {
      GDXDeviceContext->Draw(vertexCount, startVertex);
    }
  }
}

void Graphics::DX11RenderTarget::DrawIndirect(VertexPrimitive vertexPrimitive, U32 /*vertexCount*/, U32 indexCount, U32 /*instanceCount*/, U32 /*startVertex*/, U32 /*startIndex*/, U32 /*startInstance*/)
{
  //E_ASSERT_MSG(instanceCount > 0, E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE);
  GDXDeviceContext->IASetPrimitiveTopology(kDX11PrimitiveTable[vertexPrimitive]);
  if (indexCount)
  {
    //GDXDeviceContext->DrawIndexedInstancedIndirect(indexCount, instanceCount, startIndex, startVertex, startInstance);
    E_ASSERT_ALWAYS(E_ASSERT_MSG_TODO);
  }
  else
  {
    //GDXDeviceContext->DrawInstancedIndirect(vertexCount, instanceCount, startVertex, startInstance);
    E_ASSERT_ALWAYS(E_ASSERT_MSG_TODO);
  }
}

/*----------------------------------------------------------------------------------------------------------------------
DX11RenderTarget private methods
----------------------------------------------------------------------------------------------------------------------*/

bool Graphics::DX11RenderTarget::CreateDepthStencilView(const ITexture2DInstance& texture2D)
{
  E_ASSERT_MSG(texture2D->GetDescriptor().type == ITexture2D::eTypeDepthTarget, E_ASSERT_MSG_DX11_DRAW_CONTEXT_DEPTH_TARGET_TEXTURE_TYPE);
  ID3D11Texture2D* pDXTexture = DX11Texture2DInstance(texture2D)->GetDXTexture();
  D3D11_TEXTURE2D_DESC dxTextureDesc;
  pDXTexture->GetDesc(&dxTextureDesc);

  D3D11_DEPTH_STENCIL_VIEW_DESC	dxDepthStencilViewDesc;
  ZeroMemory(&dxDepthStencilViewDesc, sizeof(dxDepthStencilViewDesc));
  switch (texture2D->GetDescriptor().format)
  {
  case ITexture2D::eFormatDepth16:
    dxDepthStencilViewDesc.Format = DXGI_FORMAT_D16_UNORM;
    break;
  case ITexture2D::eFormatDepth24S8:
    dxDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    break;
  case ITexture2D::eFormatDepth32:
    dxDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    break;
  case ITexture2D::eFormatDepth32S8:
    dxDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    break;
  }
  dxDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dxDepthStencilViewDesc.Texture2D.MipSlice = 0;
  
  //TODO: create a version read only for being able to depth test and read depth => dxDepthStencilViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
  return (GDXDevice->CreateDepthStencilView(pDXTexture, &dxDepthStencilViewDesc, &mpDXDepthStencilView) == S_OK);
}

bool Graphics::DX11RenderTarget::CreateRenderTargetView(U32 renderTargetIndex, const ITexture2DInstance& texture2D, U32 sliceIndex /* = 0 */)
{
  E_ASSERT_MSG(texture2D->GetDescriptor().type == ITexture2D::eTypeColorTarget, E_ASSERT_MSG_DX11_DRAW_CONTEXT_COLOR_TARGET_TEXTURE_TYPE);
  ID3D11Texture2D* pDXTexture = DX11Texture2DInstance(texture2D)->GetDXTexture();
  D3D11_TEXTURE2D_DESC dxTextureDesc;
  pDXTexture->GetDesc(&dxTextureDesc);

  D3D11_RENDER_TARGET_VIEW_DESC dxRenderTargetViewDesc;
  dxRenderTargetViewDesc.Format = dxTextureDesc.Format;
  dxRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  dxRenderTargetViewDesc.Texture2D.MipSlice = D3D11CalcSubresource(0, sliceIndex, dxTextureDesc.MipLevels);

  return (GDXDevice->CreateRenderTargetView(pDXTexture, &dxRenderTargetViewDesc, &mpDXRenderTargetViews[renderTargetIndex]) == S_OK);
}
