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

/** @file IDevice.h
This file defines the IDevice interface. IDevice is intended to abstract Cpu - Gpu communication implementations.
*/

#ifndef E3_IDEVICE_H
#define E3_IDEVICE_H

#include <Graphics/IBlendState.h>
#include <Graphics/IBuffer.h>
#include <Graphics/IRenderTarget.h>
#include <Graphics/IDepthStencilState.h>
#include <Graphics/IPipeline.h>
#include <Graphics/IRasterState.h>
#include <Graphics/ISampler.h>
#include <Graphics/IShader.h>
#include <Graphics/ITexture2D.h>
#include <Graphics/IVertexLayout.h>
#include <Graphics/IViewport.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IDevice

Please note that this interface has the following usage contract: 

1. IDevice methods require IDevice to be initialized.
2. Descriptor memorySize indicates the graphics device memory size in MB.
----------------------------------------------------------------------------------------------------------------------*/
class IDevice : public IPipeline
{
public:
  enum DeviceType
  {
    eDeviceTypeDX11,
    eDeviceTypeCount
  };

  struct Descriptor 
  {   
    String	name;
    size_t	      memorySize;  

    Descriptor::Descriptor() 
      : memorySize(0) 
    {} 
  }; 
  
  virtual bool				                Initialize() = 0;
  virtual void				                Finalize() = 0;
  virtual bool                        IsReady() const = 0;

  // Accessors
  virtual const Descriptor&           GetDescriptor() const = 0;                  
  virtual DeviceType                  GetDeviceType() const = 0;
  
  // Methods
  virtual IBlendStateInstance         CreateBlendState(const IBlendState::Descriptor& desc) = 0;
  virtual IBufferInstance             CreateBuffer(const IBuffer::Descriptor& desc) = 0;
  virtual IDepthStencilStateInstance  CreateDepthStencilState(const IDepthStencilState::Descriptor& desc) = 0;
  virtual IRenderTargetInstance       CreateRenderTarget(const IRenderTarget::Descriptor& desc) = 0;
  virtual IRasterStateInstance        CreateRasterState(const IRasterState::Descriptor& desc) = 0;
  virtual ISamplerInstance            CreateSampler(const ISampler::Descriptor& desc) = 0;
  virtual IShaderInstance             CreateShader(const IShader::Descriptor& desc) = 0;
  virtual ITexture2DInstance          CreateTexture2D(const ITexture2D::Descriptor& desc) = 0;
  virtual ITexture2DInstance          CreateTexture2D(const FilePath& filePath) = 0;
  virtual ITexture2DInstance          CreateTexture2D(IViewportInstance viewport) = 0;
  virtual IVertexLayoutInstance       CreateVertexLayout(const IVertexLayout::Descriptor& desc) = 0;
  virtual IViewportInstance           CreateViewport(const IViewport::Descriptor& desc) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IDevice types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IDevice> IDeviceInstance;
} 
}

#endif

