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

// Created 03-Jul-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file DX11Pipeline.cpp
This file defines the DX11Pipeline class.
*/

#include <GraphicsPch.h>
#include "DX11Pipeline.h"
#include "DX11BlendState.h"
#include "DX11DepthStencilState.h"
#include "DX11RasterState.h"
#include "DX11Viewport.h"
#include "DX11RenderTarget.h"
#include "DX11Shader.h"
#include "DX11VertexLayout.h"
#include "DX11Buffer.h"
#include "DX11Texture2D.h"
#include "DX11Sampler.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
DX11Pipeline messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_DX11_PIPELINE_BUFFER_TYPE                              "Expected parameter buffer type is (%d) but provided is (%d)"
#define E_ASSERT_MSG_DX11_PIPELINE_CONSTANT_BUFFER_SLOT_INDEX_VALUE         "Constant buffer slot index value cannot exceed %d"
#define E_ASSERT_MSG_DX11_PIPELINE_SHADER_INPUT_RESOURCE                    "Attempting to bind a non Gpu readable resource"
#define E_ASSERT_MSG_DX11_PIPELINE_SHADER_INPUT_RESOURCE_SLOT_INDEX_VALUE   "Shader input resource slot index value cannot exceed %d"
#define E_ASSERT_MSG_DX11_PIPELINE_SHADER_OUTPUT_RESOURCE                   "Attempting to bind a non Gpu writable resource"
#define E_ASSERT_MSG_DX11_PIPELINE_SHADER_OUTPUT_RESOURCE_SLOT_INDEX_VALUE  "Shader outpur resource slot index value cannot exceed %d"
#define E_ASSERT_MSG_DX11_PIPELINE_SHADER_SAMPLER_SLOT_INDEX_VALUE          "Shader sampler slot index value cannot exceed %d"
#define E_ASSERT_MSG_DX11_PIPELINE_UNBOUND_SHADER_INPUT_RESOURCE            "Attempting to unbind a readable resource which is not bound to this shader stage"
#define E_ASSERT_MSG_DX11_PIPELINE_UNBOUND_SHADER_OUTPUT_RESOURCE           "Attempting to unbind a writable resource which is not bound to this shader stage"
#define E_ASSERT_MSG_DX11_PIPELINE_VERTEX_BUFFER_SLOT_INDEX_VALUE           "Vertex buffer slot index value cannot exceed %d"

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

namespace E
{
  namespace Graphics
  {
    void BindDXShaderResource(ID3D11ShaderResourceView* pDXShaderResourceView, IShader::Stage stage, U32 slot);
  }
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Pipeline initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11Pipeline::DX11Pipeline()
  : mpDXDeviceContext(nullptr) {}

Graphics::DX11Pipeline::~DX11Pipeline() {}

/*----------------------------------------------------------------------------------------------------------------------
DX11Pipeline methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::DX11Pipeline::BindInput(IBuffer* indexBuffer)
{
  E_ASSERT_PTR(indexBuffer);
  E_ASSERT_MSG(
    indexBuffer->GetDescriptor().type == IBuffer::eTypeIndex,
    E_ASSERT_MSG_DX11_PIPELINE_BUFFER_TYPE,
    IBuffer::eTypeIndex,
    indexBuffer->GetDescriptor().type);

  GDXDeviceContext->IASetIndexBuffer(
    static_cast<DX11Buffer*>(indexBuffer)->GetDXBuffer(), 
    indexBuffer->GetDescriptor().elementSize == sizeof(U32) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
    0);
}

void Graphics::DX11Pipeline::BindInput(IBuffer* vertexBuffer, U32 slot)
{
  E_ASSERT_PTR(vertexBuffer);
  E_ASSERT_MSG(
    vertexBuffer->GetDescriptor().type == IBuffer::eTypeVertex,
    E_ASSERT_MSG_DX11_PIPELINE_BUFFER_TYPE,
    IBuffer::eTypeVertex,
    vertexBuffer->GetDescriptor().type);
  E_ASSERT_MSG(
    slot < eMaxVertexInputCount,
    E_ASSERT_MSG_DX11_PIPELINE_VERTEX_BUFFER_SLOT_INDEX_VALUE,
    eMaxVertexInputCount);

  ID3D11Buffer* pDXBuffer = static_cast<DX11Buffer*>(vertexBuffer)->GetDXBuffer();
  U32 stride = static_cast<U32>(vertexBuffer->GetDescriptor().elementSize);
  U32 offset = 0;
  GDXDeviceContext->IASetVertexBuffers(slot, 1, &pDXBuffer, &stride, &offset);
}

void Graphics::DX11Pipeline::BindInput(IVertexLayout* vertexLayout)
{
  GDXDeviceContext->IASetInputLayout(static_cast<DX11VertexLayout*>(vertexLayout)->GetDXVertexLayout());
}

void Graphics::DX11Pipeline::BindOutput(IRenderTarget* renderTarget)
{
  E_ASSERT_PTR(renderTarget);
  DX11RenderTarget* dxRenderTarget = static_cast<DX11RenderTarget*>(renderTarget);
  GDXDeviceContext->OMSetRenderTargets(
    static_cast<U32>(renderTarget->GetDescriptor().colorTargets.GetSize()),
    dxRenderTarget->GetDXColorTargets(),
    dxRenderTarget->GetDXDepthTarget());
}

void Graphics::DX11Pipeline::BindShader(IShader* shader)
{
  E_ASSERT_PTR(shader);
  DX11Shader* dxShader = static_cast<DX11Shader*>(shader);
  GDXDeviceContext->VSSetShader(dxShader->GetDXVertexShader(), nullptr, 0);
  GDXDeviceContext->HSSetShader(dxShader->GetDXHullShader(), nullptr, 0);
  GDXDeviceContext->DSSetShader(dxShader->GetDXDomainShader(), nullptr, 0);
  GDXDeviceContext->GSSetShader(dxShader->GetDXGeometryShader(), nullptr, 0);
  GDXDeviceContext->PSSetShader(dxShader->GetDXPixelShader(), nullptr, 0);
  GDXDeviceContext->CSSetShader(dxShader->GetDXComputeShader(), nullptr, 0);
}

void Graphics::DX11Pipeline::BindShaderConstant(IBuffer* constantBuffer, IShader::Stage stage, U32 slot)
{
  E_ASSERT_PTR(constantBuffer);
  E_ASSERT_MSG(
    constantBuffer->GetDescriptor().type == IBuffer::eTypeConstant,
    E_ASSERT_MSG_DX11_PIPELINE_BUFFER_TYPE,
    IBuffer::eTypeConstant,
    constantBuffer->GetDescriptor().type);
  E_ASSERT_MSG(
    slot < eMaxShaderConstantCount,
    E_ASSERT_MSG_DX11_PIPELINE_CONSTANT_BUFFER_SLOT_INDEX_VALUE,
    eMaxShaderConstantCount);

  ID3D11Buffer* pDXBuffer = static_cast<DX11Buffer*>(constantBuffer)->GetDXBuffer();
  switch (stage)
  {
  case IShader::eStageVertex:
    GDXDeviceContext->VSSetConstantBuffers(slot, 1, &pDXBuffer);
    break;
  case IShader::eStageHull:
    GDXDeviceContext->HSSetConstantBuffers(slot, 1, &pDXBuffer);
    break;
  case IShader::eStageDomain:
    GDXDeviceContext->DSSetConstantBuffers(slot, 1, &pDXBuffer);
    break;
  case IShader::eStageGeometry:
    GDXDeviceContext->GSSetConstantBuffers(slot, 1, &pDXBuffer);
    break;
  case IShader::eStagePixel:
    GDXDeviceContext->PSSetConstantBuffers(slot, 1, &pDXBuffer);
    break;
  case IShader::eStageCompute:
    GDXDeviceContext->CSSetConstantBuffers(slot, 1, &pDXBuffer);
    break;
  }
}

void Graphics::DX11Pipeline::BindShaderInput(IBuffer* resourceBuffer, IShader::Stage stage, U32 slot)
{
  E_ASSERT_PTR(resourceBuffer);
  E_ASSERT_MSG(
    resourceBuffer->GetDescriptor().type == IBuffer::eTypeResource,
    E_ASSERT_MSG_DX11_PIPELINE_BUFFER_TYPE,
    IBuffer::eTypeResource,
    resourceBuffer->GetDescriptor().type);
  E_ASSERT_MSG(
    slot < eMaxShaderInputResourceCount,
    E_ASSERT_MSG_DX11_PIPELINE_SHADER_INPUT_RESOURCE_SLOT_INDEX_VALUE,
    eMaxShaderInputResourceCount);
  E_ASSERT_MSG(resourceBuffer->GetAccessFlags() & IResource::eAccessFlagGpuRead, E_ASSERT_MSG_DX11_PIPELINE_SHADER_INPUT_RESOURCE);
  ID3D11ShaderResourceView* pDXShaderResourceView = static_cast<DX11Buffer*>(resourceBuffer)->GetDXShaderResourceView();
  BindDXShaderResource(pDXShaderResourceView, stage, slot);
}

void Graphics::DX11Pipeline::BindShaderInput(IResource* resource, IShader::Stage stage, U32 slot)
{
  E_ASSERT_PTR(resource);
  (resource->GetResourceType() == IResource::eResourceTypeBuffer) ? 
    BindShaderInput(static_cast<IBuffer*>(resource), stage, slot) :
    BindShaderInput(static_cast<ITexture2D*>(resource), stage, slot);
}

void Graphics::DX11Pipeline::BindShaderInput(ITexture2D* texture2D, IShader::Stage stage, U32 slot)
{
  E_ASSERT_PTR(texture2D);
  E_ASSERT_MSG(
    slot < eMaxShaderInputResourceCount,
    E_ASSERT_MSG_DX11_PIPELINE_SHADER_INPUT_RESOURCE_SLOT_INDEX_VALUE,
    eMaxShaderInputResourceCount);
  E_ASSERT_MSG(texture2D->GetAccessFlags() & IResource::eAccessFlagGpuRead, E_ASSERT_MSG_DX11_PIPELINE_SHADER_INPUT_RESOURCE);
  ID3D11ShaderResourceView* pDXShaderResourceView = static_cast<DX11Texture2D*>(texture2D)->GetDXShaderResourceView();
  BindDXShaderResource(pDXShaderResourceView, stage, slot);
}

void Graphics::DX11Pipeline::BindShaderSampler(ISampler* sampler, IShader::Stage stage, U32 slot)
{
  E_ASSERT_PTR(sampler);
  E_ASSERT_MSG(
    slot < eMaxShaderSamplerCount,
    E_ASSERT_MSG_DX11_PIPELINE_SHADER_SAMPLER_SLOT_INDEX_VALUE,
    eMaxShaderSamplerCount);
  ID3D11SamplerState* pDXSamplerState = static_cast<DX11Sampler*>(sampler)->GetDXSamplerState();
  switch (stage)
  {
  case IShader::eStageVertex:
    GDXDeviceContext->VSSetSamplers(slot, 1, &pDXSamplerState);
    return;
  case IShader::eStageHull:
    GDXDeviceContext->HSSetSamplers(slot, 1, &pDXSamplerState);
    return;
  case IShader::eStageDomain:
    GDXDeviceContext->DSSetSamplers(slot, 1, &pDXSamplerState);
    return;
  case IShader::eStageGeometry:
    GDXDeviceContext->GSSetSamplers(slot, 1, &pDXSamplerState);
    return;
  case IShader::eStagePixel:
    GDXDeviceContext->PSSetSamplers(slot, 1, &pDXSamplerState);
    return;
  case IShader::eStageCompute:
    GDXDeviceContext->CSSetSamplers(slot, 1, &pDXSamplerState);
    return;
  }
}

void Graphics::DX11Pipeline::BindShaderOutput(IBuffer* resourceBuffer, U32 slot)
{
  E_ASSERT_PTR(resourceBuffer);
  E_ASSERT_MSG(
    slot < eMaxShaderOutputResourceCount,
    E_ASSERT_MSG_DX11_PIPELINE_SHADER_OUTPUT_RESOURCE_SLOT_INDEX_VALUE,
    eMaxShaderOutputResourceCount);
  E_ASSERT_MSG(resourceBuffer->GetAccessFlags() & IResource::eAccessFlagGpuWrite, E_ASSERT_MSG_DX11_PIPELINE_SHADER_OUTPUT_RESOURCE);
  ID3D11UnorderedAccessView* pDXUnorderedAccessView = static_cast<DX11Buffer*>(resourceBuffer)->GetDXUnorderedAccessView();
  GDXDeviceContext->CSSetUnorderedAccessViews(slot, 1, &pDXUnorderedAccessView, nullptr);
}

void Graphics::DX11Pipeline::BindShaderOutput(ITexture2D* texture2D, U32 slot)
{
  E_ASSERT_PTR(texture2D);
  E_ASSERT_MSG(
    slot < eMaxShaderOutputResourceCount,
    E_ASSERT_MSG_DX11_PIPELINE_SHADER_OUTPUT_RESOURCE_SLOT_INDEX_VALUE,
    eMaxShaderOutputResourceCount);
  E_ASSERT_MSG(texture2D->GetAccessFlags() & IResource::eAccessFlagGpuWrite, E_ASSERT_MSG_DX11_PIPELINE_SHADER_OUTPUT_RESOURCE);
  ID3D11UnorderedAccessView* pDXUnorderedAccessView = static_cast<DX11Texture2D*>(texture2D)->GetDXUnorderedAccessView();
  GDXDeviceContext->CSSetUnorderedAccessViews(slot, 1, &pDXUnorderedAccessView, nullptr);
}

void Graphics::DX11Pipeline::BindState(IBlendState* blendState)
{
  E_ASSERT_PTR(blendState);
  GDXDeviceContext->OMSetBlendState(static_cast<DX11BlendState*>(blendState)->GetDXBlendState(), Color::ZeroColor().Get(), 0xffffffff);
}

void Graphics::DX11Pipeline::BindState(IDepthStencilState* depthStencilState)
{
  E_ASSERT_PTR(depthStencilState);
  GDXDeviceContext->OMSetDepthStencilState(static_cast<DX11DepthStencilState*>(depthStencilState)->GetDXDepthStencilState(), 1);
}

void Graphics::DX11Pipeline::BindState(IRasterState* rasterState)
{
  E_ASSERT_PTR(rasterState);
  GDXDeviceContext->RSSetState(static_cast<DX11RasterState*>(rasterState)->GetDXRasterState());
}

void Graphics::DX11Pipeline::UnbindShaderInput(IShader::Stage stage, U32 slot)
{
  BindDXShaderResource(nullptr, stage, slot);
}

void Graphics::DX11Pipeline::UnbindShaderOutput(U32 slot)
{
  ID3D11UnorderedAccessView* pDXUnorderedAccessView = nullptr;
  GDXDeviceContext->CSSetUnorderedAccessViews(slot, 1, &pDXUnorderedAccessView, nullptr);
}

void Graphics::DX11Pipeline::UnbindShaderResources()
{
  ID3D11ShaderResourceView* pDXNullSRVs[eMaxShaderInputResourceCount] = { 0 };
  ID3D11UnorderedAccessView* pDXNullUAVs[eMaxShaderOutputResourceCount] = { 0 };

  // Clean up shader inputs
  GDXDeviceContext->VSSetShaderResources(0, eMaxShaderInputResourceCount, pDXNullSRVs);
  GDXDeviceContext->HSSetShaderResources(0, eMaxShaderInputResourceCount, pDXNullSRVs);
  GDXDeviceContext->DSSetShaderResources(0, eMaxShaderInputResourceCount, pDXNullSRVs);
  GDXDeviceContext->GSSetShaderResources(0, eMaxShaderInputResourceCount, pDXNullSRVs);
  GDXDeviceContext->PSSetShaderResources(0, eMaxShaderInputResourceCount, pDXNullSRVs);
  GDXDeviceContext->CSSetShaderResources(0, eMaxShaderInputResourceCount, pDXNullSRVs);

  // Clean up shader outputs
  GDXDeviceContext->CSSetUnorderedAccessViews(0, eMaxShaderOutputResourceCount, pDXNullUAVs, nullptr);
}

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::BindDXShaderResource(ID3D11ShaderResourceView* pDXShaderResourceView, Graphics::IShader::Stage stage, U32 slot)
{
  switch (stage)
  {
  case IShader::eStageVertex:
    GDXDeviceContext->VSSetShaderResources(slot, 1, &pDXShaderResourceView);
    break;
  case IShader::eStageHull:
    GDXDeviceContext->HSSetShaderResources(slot, 1, &pDXShaderResourceView);
    break;
  case IShader::eStageDomain:
    GDXDeviceContext->DSSetShaderResources(slot, 1, &pDXShaderResourceView);
    break;
  case IShader::eStageGeometry:
    GDXDeviceContext->GSSetShaderResources(slot, 1, &pDXShaderResourceView);
    break;
  case IShader::eStagePixel:
    GDXDeviceContext->PSSetShaderResources(slot, 1, &pDXShaderResourceView);
    break;
  case IShader::eStageCompute:
    GDXDeviceContext->CSSetShaderResources(slot, 1, &pDXShaderResourceView);
    break;
  }
}
