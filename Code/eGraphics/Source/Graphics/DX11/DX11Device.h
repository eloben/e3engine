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

/** @file DX11Device.h
This file contains the declaration of the DX11Device class.
*/

#ifndef E3_DX11_DEVICE_H
#define E3_DX11_DEVICE_H

#include "DX11BlendState.h"
#include "DX11Buffer.h"
#include "DX11RenderTarget.h"
#include "DX11DepthStencilState.h"
#include "DX11Pipeline.h"
#include "DX11RasterState.h"
#include "DX11Sampler.h"
#include "DX11Shader.h"
#include "DX11Texture2D.h"
#include "DX11VertexLayout.h"
#include "DX11Viewport.h"

namespace E 
{
namespace Graphics
{
// Forward declarations
class DX11Core;

/*----------------------------------------------------------------------------------------------------------------------
DX11Device
----------------------------------------------------------------------------------------------------------------------*/
class DX11Device : public IDevice
{
public:
  DX11Device();
  ~DX11Device();

  bool				    	          Initialize();
  void				    	          Finalize();
  bool                        IsReady() const;

  // Accessors
  const Descriptor&           GetDescriptor() const;
  DeviceType                  GetDeviceType() const;
  IPipeline&                  GetPipeline();

  // Methods
  void                        BindInput(IBuffer* indexBuffer);
  void                        BindInput(IBuffer* vertexBuffer, U32 slot);
  void                        BindInput(IVertexLayout* vertexLayout);
  void                        BindOutput(IRenderTarget* renderTarget);
  void                        BindShader(IShader* shader);
  void	                      BindShaderConstant(IBuffer* constantBuffer, IShader::Stage stage, U32 slot);
  void	                      BindShaderInput(IBuffer* resourceBuffer, IShader::Stage stage, U32 slot);
  void	                      BindShaderInput(IResource* resource, IShader::Stage stage, U32 slot);
  void	                      BindShaderInput(ITexture2D* texture2D, IShader::Stage stage, U32 slot);
  void	                      BindShaderSampler(ISampler* sampler, IShader::Stage stage, U32 slot);
  void                        BindShaderOutput(IBuffer* resourceBuffer, U32 slot);
  void                        BindShaderOutput(ITexture2D* texture2D, U32 slot);
  void                        BindState(IBlendState* blendState);
  void                        BindState(IDepthStencilState* depthStencilState);
  void                        BindState(IRasterState* rasterState);
  IBlendStateInstance         CreateBlendState(const IBlendState::Descriptor& desc);
  IBufferInstance             CreateBuffer(const IBuffer::Descriptor& desc);
  IDepthStencilStateInstance  CreateDepthStencilState(const IDepthStencilState::Descriptor& desc);
  IRenderTargetInstance       CreateRenderTarget(const IRenderTarget::Descriptor& desc);
  IRasterStateInstance        CreateRasterState(const IRasterState::Descriptor& desc);
  ISamplerInstance            CreateSampler(const ISampler::Descriptor& desc);
  IShaderInstance             CreateShader(const IShader::Descriptor& desc);
  ITexture2DInstance          CreateTexture2D(const ITexture2D::Descriptor& desc);
  ITexture2DInstance          CreateTexture2D(const FilePath& filePath);
  ITexture2DInstance          CreateTexture2D(IViewportInstance viewport);
  IVertexLayoutInstance       CreateVertexLayout(const IVertexLayout::Descriptor& desc);
  IViewportInstance           CreateViewport(const IViewport::Descriptor& desc);
  void	                      UnbindShaderInput(IShader::Stage stage, U32 slot);
  void                        UnbindShaderOutput(U32 slot);
  void                        UnbindShaderResources();

private:
  typedef Memory::GCConcreteFactory<DX11BlendState>			    BlendStateFactory;
  typedef Memory::GCConcreteFactory<DX11DepthStencilState>	DepthStencilStateFactory;
  typedef Memory::GCConcreteFactory<DX11RasterState>	      RasterStateFactory;
  typedef Memory::GCConcreteFactory<DX11Viewport>	          ViewportFactory;
  typedef Memory::GCConcreteFactory<DX11RenderTarget>       RenderTargetFactory;
  typedef Memory::GCConcreteFactory<DX11Shader> 	          ShaderFactory;
  typedef Memory::GCConcreteFactory<DX11VertexLayout>	      VertexLayoutFactory;
  typedef Memory::GCConcreteFactory<DX11Sampler>	          SamplerFactory;
  typedef Memory::GCConcreteFactory<DX11Buffer>	            BufferFactory;
  typedef Memory::GCConcreteFactory<DX11Texture2D>	        Texture2DFactory;

  Descriptor                  mDescriptor;
  BlendStateFactory		        mBlendStateFactory;
  DepthStencilStateFactory	  mDepthStencilStateFactory;
  RasterStateFactory	        mRasterStateFactory;
  ViewportFactory             mViewportFactory;
  RenderTargetFactory         mRenderTargetFactory;
  ShaderFactory               mShaderFactory;
  VertexLayoutFactory         mVertexLayoutFactory;
  SamplerFactory              mSamplerFactory;
  BufferFactory               mBufferFactory;
  Texture2DFactory            mTexture2DFactory;
  DX11Pipeline                mPipeline;
  DX11Core&                   mCore;

  E_DISABLE_COPY_AND_ASSSIGNMENT(DX11Device)
}; 
}
}

#endif

