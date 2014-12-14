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

// Created 06-Jul-2014 by Elías Lozada-Benavente
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
RenderDepthToTextureUpdater methods
----------------------------------------------------------------------------------------------------------------------*/

void RenderDepthToTextureUpdater::Bind()
{
  // Render simple vertex to the auxiliary context
  mSimpleVertexUpdater.Bind();
  mDevice->BindOutput(mAuxiliaryContext.GetPtr());
  mAuxiliaryContext->Clear(Graphics::Color::eWhite, Graphics::IRenderTarget::eClearFlagDepth);
  mAuxiliaryContext->Draw(Graphics::eVertexPrimitiveTriangleList, 3, 0, 0, 0, 0, 0);

  IndexedVertexUpdater::Bind();
}

/*----------------------------------------------------------------------------------------------------------------------
RenderDepthToTextureUpdater protected methods
----------------------------------------------------------------------------------------------------------------------*/

void RenderDepthToTextureUpdater::CreateContexts()
{
  IndexedVertexUpdater::CreateContexts();

  Graphics::ITexture2D::Descriptor depthTargetDescriptor;
  depthTargetDescriptor.type = Graphics::ITexture2D::eTypeDepthTarget;
  depthTargetDescriptor.format = Graphics::ITexture2D::eFormatDepth24S8;
  depthTargetDescriptor.width = mViewportDescriptor.width;
  depthTargetDescriptor.height = mViewportDescriptor.height;
  depthTargetDescriptor.unitCount = 1;
  depthTargetDescriptor.accessFlags = Graphics::ITexture2D::eAccessFlagGpuRead;
  mTexture2D = mDevice->CreateTexture2D(depthTargetDescriptor);
  ThrowIf(mTexture2D == nullptr, Exception::eExceptionTypeGraphicsTest);

  Graphics::IRenderTarget::Descriptor auxiliaryContextDescriptor;
  auxiliaryContextDescriptor.depthTarget = mTexture2D;
  
  // Uncomment but the Pixel shader will complain
  auxiliaryContextDescriptor.colorTargets.Reserve(1);
  Graphics::ITexture2D::Descriptor colorTargetDescriptor;
  colorTargetDescriptor.type = Graphics::ITexture2D::eTypeColorTarget;
  colorTargetDescriptor.format = Graphics::ITexture2D::eFormatRGBA32;
  colorTargetDescriptor.width = mViewportDescriptor.width;
  colorTargetDescriptor.height = mViewportDescriptor.height;
  colorTargetDescriptor.unitCount = 1;
  colorTargetDescriptor.accessFlags = Graphics::ITexture2D::eAccessFlagGpuRead;
  auxiliaryContextDescriptor.colorTargets[0] = mDevice->CreateTexture2D(colorTargetDescriptor);
  ThrowIf(auxiliaryContextDescriptor.colorTargets[0] == nullptr, Exception::eExceptionTypeGraphicsTest);
  
  mAuxiliaryContext = mDevice->CreateRenderTarget(auxiliaryContextDescriptor);
  ThrowIf(mAuxiliaryContext == nullptr, Exception::eExceptionTypeGraphicsTest);
}

void RenderDepthToTextureUpdater::CreateShaders()
{
  Graphics::IShader::Descriptor shaderDescriptor;
  shaderDescriptor.modelVersion = 5;
  shaderDescriptor.stages[Graphics::IShader::eStageVertex].filePath  = "depth.hlsl";
  shaderDescriptor.stages[Graphics::IShader::eStageVertex].entryPoint = "VS";
  shaderDescriptor.stages[Graphics::IShader::eStagePixel].filePath  = "depth.hlsl";
  shaderDescriptor.stages[Graphics::IShader::eStagePixel].entryPoint = "PS";
  mShader = mDevice->CreateShader(shaderDescriptor);
}
