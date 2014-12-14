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

/** @file DX11Device.cpp
This file contains the DX11Device class methods definition.
*/

#include <GraphicsPch.h>
#include "DX11Device.h"
#include "DX11Core.h"

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11Device initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

DX11Device::DX11Device()
: mCore(GDX11Core::GetInstance())
{
}

DX11Device::~DX11Device()
{
	Finalize();
}

bool DX11Device::Initialize()
{
  E_ASSERT(!IsReady());
  // Identify the Gpu device
  IDXGIFactory*	pDXAdapterFactory;

  // Create a DXGI factory to create an adapter for the primary Gpu device
  if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pDXAdapterFactory) == S_OK)
  {    
    // Create a DXGI adapter
    IDXGIAdapter*	pDXAdapter;
    if (pDXAdapterFactory->EnumAdapters(0, &pDXAdapter) == S_OK)
    { 
      // Extract the adapter information (Gpu device description and memory)
      DXGI_ADAPTER_DESC	dxAdapterDesc;
      if (pDXAdapter->GetDesc(&dxAdapterDesc) == S_OK)
      {
        Text::WideToUtf8(mDescriptor.name, WString(dxAdapterDesc.Description));
        mDescriptor.memorySize = dxAdapterDesc.DedicatedVideoMemory / (1024 * 1024); // MB
      }
      Win32::ReleaseCom(pDXAdapter);
    }
    Win32::ReleaseCom(pDXAdapterFactory);
  }

  if (mDescriptor.memorySize == 0)
  {
    return false;
  }

  // Create DX device resources
  D3D_FEATURE_LEVEL dxFeatureLevel = D3D_FEATURE_LEVEL_11_0;
  if (D3D11CreateDevice(
    nullptr,                       // Default IDXGIAdapter
    D3D_DRIVER_TYPE_HARDWARE,   // Attempt to create always a hardware device
    nullptr,                       // No software device is desired
    #ifdef E_DEBUG
      D3D11_CREATE_DEVICE_DEBUG,
    #else
      0,                        // No device creation flags
    #endif
    &dxFeatureLevel,            // DX feature level (DX11)
    1,                          // Only one (DX11) feature level
    D3D11_SDK_VERSION,          // SDK version &swapChainDesc, &mCore.mpDXSwapChain, 
    &mCore.mpDXDevice,          // DX device
    nullptr,                       // Successful DX feature level (can only be DX11)
    &mCore.mpDXDeviceContext)   // DX device context
    == S_OK)
  {
    return true;
  }
  return false;
}

void DX11Device::Finalize()
{
  mBlendStateFactory.CleanUp();
  mDepthStencilStateFactory.CleanUp();
  mRasterStateFactory.CleanUp();
  mViewportFactory.CleanUp();
  mRenderTargetFactory.CleanUp();
  mShaderFactory.CleanUp();
  mVertexLayoutFactory.CleanUp();
  mSamplerFactory.CleanUp();
  mBufferFactory.CleanUp();
  mTexture2DFactory.CleanUp();
  
  // Release DX device resources
  Win32::ReleaseCom(mCore.mpDXDeviceContext);
  /*
#ifdef E_DEBUG
  if (mCore.mpDXDevice)
  {
    E_WARNING(" ------------------------------ DX11 Device report live objects begin ------------------------------");
    ID3D11Debug* pDXDebug;
    mCore.mpDXDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDXDebug));
    pDXDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    Win32::ReleaseCom(pDXDebug);
    E_WARNING(" ------------------------------ DX11 Device report live objects end --------------------------------");
  }
#endif
*/
  Win32::ReleaseCom(mCore.mpDXDevice);
}

bool DX11Device::IsReady() const
{
  return (mCore.mpDXDevice != nullptr && mCore.mpDXDeviceContext);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Device accessors
----------------------------------------------------------------------------------------------------------------------*/

const IDevice::Descriptor& DX11Device::GetDescriptor() const
{
  return mDescriptor;
}

IDevice::DeviceType DX11Device::GetDeviceType() const
{
  return eDeviceTypeDX11;
}

IPipeline& DX11Device::GetPipeline()
{
  return mPipeline;
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Device methods
----------------------------------------------------------------------------------------------------------------------*/

void DX11Device::BindInput(IBuffer* indexBuffer)
{ 
  mPipeline.BindInput(indexBuffer);
}

void DX11Device::BindInput(IBuffer* vertexBuffer, U32 slot)
{
  mPipeline.BindInput(vertexBuffer, slot);
}

void DX11Device::BindInput(IVertexLayout* vertexLayout)
{
  mPipeline.BindInput(vertexLayout);
}

void DX11Device::BindOutput(IRenderTarget* renderTarget)
{
  mPipeline.BindOutput(renderTarget);
}

void DX11Device::BindShader(IShader* shader)
{
  mPipeline.BindShader(shader);
}

void DX11Device::BindShaderConstant(IBuffer* constantBuffer, IShader::Stage stage, U32 slot)
{
  mPipeline.BindShaderConstant(constantBuffer, stage, slot);
}

void DX11Device::BindShaderInput(IBuffer* resourceBuffer, IShader::Stage stage, U32 slot)
{
  mPipeline.BindShaderInput(resourceBuffer, stage, slot);
}

void DX11Device::BindShaderInput(IResource* resource, IShader::Stage stage, U32 slot)
{
  mPipeline.BindShaderInput(resource, stage, slot);
}

void DX11Device::BindShaderInput(ITexture2D* texture2D, IShader::Stage stage, U32 slot)
{
  mPipeline.BindShaderInput(texture2D, stage, slot);
}

void DX11Device::BindShaderSampler(ISampler* sampler, IShader::Stage stage, U32 slot)
{
  mPipeline.BindShaderSampler(sampler, stage, slot);
}

void DX11Device::BindShaderOutput(IBuffer* resourceBuffer, U32 slot)
{
  mPipeline.BindShaderOutput(resourceBuffer, slot);
}

void DX11Device::BindShaderOutput(ITexture2D* texture2D, U32 slot)
{
  mPipeline.BindShaderOutput(texture2D, slot);
}

void DX11Device::BindState(IBlendState* blendState)
{
  mPipeline.BindState(blendState);
}

void DX11Device::BindState(IDepthStencilState* depthStencilState)
{
  mPipeline.BindState(depthStencilState);
}

void DX11Device::BindState(IRasterState* rasterState)
{
  mPipeline.BindState(rasterState);
}

IBlendStateInstance DX11Device::CreateBlendState(const IBlendState::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11BlendStateInstance: [%d]", mBlendStateFactory.GetLiveCount() + 1);
  DX11BlendStateInstance blendState = mBlendStateFactory.Create();
  if (!blendState->Initialize(desc)) blendState = nullptr;
  return blendState;
}

IBufferInstance DX11Device::CreateBuffer(const IBuffer::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11BufferInstance: [%d]", mBufferFactory.GetLiveCount() + 1);
  DX11BufferInstance buffer = mBufferFactory.Create();
  buffer->Initialize(desc);
  return buffer;
}

IDepthStencilStateInstance DX11Device::CreateDepthStencilState(const IDepthStencilState::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11DepthStencilStateInstance: [%d]", mDepthStencilStateFactory.GetLiveCount() + 1);
  DX11DepthStencilStateInstance depthStencilState = mDepthStencilStateFactory.Create();
  if (!depthStencilState->Initialize(desc)) depthStencilState = nullptr;
  return depthStencilState;
}

IRenderTargetInstance DX11Device::CreateRenderTarget(const IRenderTarget::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11RenderTargetInstance: [%d]", mRenderTargetFactory.GetLiveCount() + 1);
  DX11RenderTargetInstance context = mRenderTargetFactory.Create();
  if (!context->Initialize(desc)) context = nullptr;
  return context;
}

IRasterStateInstance DX11Device::CreateRasterState(const IRasterState::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11RasterStateInstance: [%d]", mRasterStateFactory.GetLiveCount() + 1);
  DX11RasterStateInstance rasterState = mRasterStateFactory.Create();
  if (!rasterState->Initialize(desc)) rasterState = nullptr;
  return rasterState;
}

ISamplerInstance DX11Device::CreateSampler(const ISampler::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11SamplerInstance: [%d]", mSamplerFactory.GetLiveCount() + 1);
  DX11SamplerInstance sampler = mSamplerFactory.Create();
  if (!sampler->Initialize(desc)) sampler = nullptr;
  return sampler;
}

IShaderInstance DX11Device::CreateShader(const IShader::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11ShaderInstance: [%d]", mShaderFactory.GetLiveCount() + 1);
  DX11ShaderInstance shader = mShaderFactory.Create();
  if (!shader->Initialize(desc)) shader = nullptr;
  return shader;
}

ITexture2DInstance DX11Device::CreateTexture2D(const ITexture2D::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11Texture2DInstance: [%d]", mTexture2DFactory.GetLiveCount() + 1);
  DX11Texture2DInstance texture2D = mTexture2DFactory.Create();
  if (!texture2D->Initialize(desc)) texture2D = nullptr;
  return texture2D;
}

ITexture2DInstance DX11Device::CreateTexture2D(const FilePath& filePath)
{
  E_DEBUG_MSG("[DX11Device] DX11Texture2DInstance: [%d]", mTexture2DFactory.GetLiveCount() + 1);
  DX11Texture2DInstance texture2D = mTexture2DFactory.Create();
  if (!texture2D->Initialize(filePath)) texture2D = nullptr;
  return texture2D;
}

ITexture2DInstance DX11Device::CreateTexture2D(IViewportInstance viewport)
{
  E_DEBUG_MSG("[DX11Device] DX11Texture2DInstance: [%d]", mTexture2DFactory.GetLiveCount() + 1);
  DX11Texture2DInstance texture2D = mTexture2DFactory.Create();
  if (!texture2D->Initialize(viewport)) texture2D = nullptr;
  return texture2D;
}

IVertexLayoutInstance DX11Device::CreateVertexLayout(const IVertexLayout::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11VertexLayoutInstance: [%d]", mVertexLayoutFactory.GetLiveCount() + 1);
  DX11VertexLayoutInstance vertexLayout = mVertexLayoutFactory.Create();
  if (!vertexLayout->Initialize(desc)) vertexLayout = nullptr;
  return vertexLayout;
}

IViewportInstance DX11Device::CreateViewport(const IViewport::Descriptor& desc)
{
  E_DEBUG_MSG("[DX11Device] DX11ViewportInstance: [%d]", mViewportFactory.GetLiveCount() + 1);
  DX11ViewportInstance viewport = mViewportFactory.Create();
  if (!viewport->Initialize(desc)) viewport = nullptr;
  return viewport;
}

void DX11Device::UnbindShaderInput(IShader::Stage stage, U32 slot)
{
  mPipeline.UnbindShaderInput(stage, slot);
}

void DX11Device::UnbindShaderOutput(U32 slot)
{
  mPipeline.UnbindShaderOutput(slot);
}

void DX11Device::UnbindShaderResources()
{
  mPipeline.UnbindShaderResources();
}
}
}
