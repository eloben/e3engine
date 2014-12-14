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

// Created 28-May-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IndexedInstanceVertexUpdater.cpp
This file defines the IndexedInstanceVertexUpdater class.
*/

#include <GraphicsTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
IndexedInstanceVertexUpdater methods
----------------------------------------------------------------------------------------------------------------------*/

void IndexedInstanceVertexUpdater::Draw()
{
  mRenderTarget->Clear(Graphics::Color::eDarkGrey, Graphics::IRenderTarget::eClearFlagAll);
  mRenderTarget->Draw(Graphics::eVertexPrimitiveTriangleList, 0, 6, 4, 0, 0, 0);
}

/*----------------------------------------------------------------------------------------------------------------------
IndexedInstanceVertexUpdater protected methods
----------------------------------------------------------------------------------------------------------------------*/

void IndexedInstanceVertexUpdater::CreateConstantBuffers()
{
  SimpleVertexUpdater::CreateConstantBuffers();

  Graphics::IBuffer::Descriptor constantBufferDescriptor;
  constantBufferDescriptor.type = Graphics::IBuffer::eTypeConstant;
  constantBufferDescriptor.accessFlags = Graphics::IBuffer::eAccessFlagGpuRead;
  constantBufferDescriptor.elementSize = sizeof(F32);
  mInstanceBuffer = mDevice->CreateBuffer(constantBufferDescriptor);
  ThrowIf(mInstanceBuffer == nullptr, Exception::eExceptionTypeGraphicsTest);
}

void IndexedInstanceVertexUpdater::CreateShaders()
{
  Graphics::IShader::Descriptor shaderDescriptor;
  shaderDescriptor.modelVersion = 5;
  shaderDescriptor.stages[Graphics::IShader::eStageVertex].filePath  = "instance.hlsl";
  shaderDescriptor.stages[Graphics::IShader::eStageVertex].entryPoint = "VS";
  shaderDescriptor.stages[Graphics::IShader::eStagePixel].filePath  = "instance.hlsl";
  shaderDescriptor.stages[Graphics::IShader::eStagePixel].entryPoint = "PS";
  mShader = mDevice->CreateShader(shaderDescriptor);
}