/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Engine

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 05-Jul-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file MultitargetVertexUpdater.cpp
This file defines the MultitargetVertexUpdater class.
*/

#include <GraphicsTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
RenderToTextureUpdater initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

void RenderToTextureUpdater::Initialize(Application::IWindow* pWindow)
{
  IndexedVertexUpdater::Initialize(pWindow);
  mSimpleVertexUpdater.Initialize(pWindow);
  pWindow->GetUpdateEventCallback() -= &mSimpleVertexUpdater;
}

/*----------------------------------------------------------------------------------------------------------------------
RenderToTextureUpdater methods
----------------------------------------------------------------------------------------------------------------------*/

void RenderToTextureUpdater::Bind()
{
  // Render simple vertex to the auxiliary context
  mSimpleVertexUpdater.Bind();
  mDevice->BindOutput(mAuxiliaryContext.GetPtr());
  mAuxiliaryContext->Clear(Graphics::Color::eBlack, Graphics::IRenderTarget::eClearFlagColor);
  mAuxiliaryContext->Draw(Graphics::eVertexPrimitiveTriangleList, 3, 0, 0, 0, 0, 0);
  IndexedVertexUpdater::Bind();
}

void RenderToTextureUpdater::Draw()
{
  IndexedVertexUpdater::Draw();
  mDevice->UnbindShaderInput(Graphics::IShader::eStagePixel, 0);
}

/*----------------------------------------------------------------------------------------------------------------------
RenderToTextureUpdater protected methods
----------------------------------------------------------------------------------------------------------------------*/

void RenderToTextureUpdater::CreateContexts()
{
  IndexedVertexUpdater::CreateContexts();
  
  Graphics::ITexture2D::Descriptor renderTargetDescriptor;
  renderTargetDescriptor.type = Graphics::ITexture2D::eTypeColorTarget;
  renderTargetDescriptor.format = Graphics::ITexture2D::eFormatRGBA32;
  renderTargetDescriptor.width = mViewportDescriptor.width;
  renderTargetDescriptor.height = mViewportDescriptor.height;
  renderTargetDescriptor.unitCount = 1;
  renderTargetDescriptor.accessFlags = Graphics::ITexture2D::eAccessFlagGpuRead;
  mTexture2D = mDevice->CreateTexture2D(renderTargetDescriptor);
  ThrowIf(mTexture2D == nullptr, Exception::eExceptionTypeGraphicsTest);

  Graphics::IRenderTarget::Descriptor auxiliaryContextDescriptor;
  auxiliaryContextDescriptor.colorTargets.Reserve(1);
  auxiliaryContextDescriptor.colorTargets[0] = mTexture2D;
  mAuxiliaryContext = mDevice->CreateRenderTarget(auxiliaryContextDescriptor);
  ThrowIf(mAuxiliaryContext == nullptr, Exception::eExceptionTypeGraphicsTest);
}

void RenderToTextureUpdater::CreateTextures()
{
  Graphics::ISampler::Descriptor samplerDesc;
  samplerDesc.addressU = Graphics::ISampler::eAddressModeWrap;
  samplerDesc.addressV = Graphics::ISampler::eAddressModeWrap;
  samplerDesc.addressW = Graphics::ISampler::eAddressModeWrap;
  samplerDesc.filter = Graphics::ISampler::eFilterMinMagMipLinear; 
  samplerDesc.mipLodBias = 0.0f;
  samplerDesc.maxAnisotropy = 1;
  samplerDesc.comparisonFunction = Graphics::ISampler::eComparisonFunctionAlways;
  samplerDesc.borderColor = Graphics::Color::eWhite;
  samplerDesc.minLod = 0;
  mSampler = mDevice->CreateSampler(samplerDesc);
  ThrowIf(mSampler == nullptr, Exception::eExceptionTypeGraphicsTest);
}
