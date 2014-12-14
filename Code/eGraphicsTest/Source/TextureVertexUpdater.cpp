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

// Created 21-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file TextureVertexUpdater.cpp
This file defines the TextureVertexUpdater class.
*/

#include <GraphicsTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
TextureVertexUpdater initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

void TextureVertexUpdater::Initialize(Application::IWindow* pWindow)
{
  SimpleVertexUpdater::Initialize(pWindow);
  CreateTextures();
}

/*----------------------------------------------------------------------------------------------------------------------
TextureVertexUpdater methods
----------------------------------------------------------------------------------------------------------------------*/

void TextureVertexUpdater::Bind()
{
  SimpleVertexUpdater::Bind();
  mDevice->BindShaderInput(mTexture2D.GetPtr(), Graphics::IShader::eStagePixel, 0);
  mDevice->BindShaderSampler(mSampler.GetPtr(), Graphics::IShader::eStagePixel, 0);
}

/*----------------------------------------------------------------------------------------------------------------------
TextureVertexUpdater protected methods
----------------------------------------------------------------------------------------------------------------------*/

void TextureVertexUpdater::CreateShaders()
{
  Graphics::IShader::Descriptor shaderDescriptor;
  shaderDescriptor.modelVersion = 5;
  shaderDescriptor.stages[Graphics::IShader::eStageVertex].filePath  = "texture.hlsl";
  shaderDescriptor.stages[Graphics::IShader::eStageVertex].entryPoint = "VS";
  shaderDescriptor.stages[Graphics::IShader::eStagePixel].filePath  = "texture.hlsl";
  shaderDescriptor.stages[Graphics::IShader::eStagePixel].entryPoint = "PS";
  mShader = mDevice->CreateShader(shaderDescriptor);
  ThrowIf(mShader == nullptr, Exception::eExceptionTypeGraphicsTest);
}

void TextureVertexUpdater::CreateTextures()
{
  mTexture2D = mDevice->CreateTexture2D("..\\..\\..\\Data\\Textures\\test.png");
  ThrowIf(mTexture2D == nullptr, Exception::eExceptionTypeGraphicsTest);

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
  E_ASSERT(samplerDesc.maxLod == ( 3.402823466e+38f ));
  mSampler = mDevice->CreateSampler(samplerDesc);
  ThrowIf(mSampler == nullptr, Exception::eExceptionTypeGraphicsTest);
}

void TextureVertexUpdater::CreateVertexBuffers()
{
  Graphics::IVertexLayout::Descriptor vertexLayoutDescriptor;
  vertexLayoutDescriptor.elements.Reserve(2);
  vertexLayoutDescriptor.elements[0].type = Graphics::IVertexLayout::Element::eTypePosition;
  vertexLayoutDescriptor.elements[0].format = Graphics::IVertexLayout::Element::eFormatFloat3;
  vertexLayoutDescriptor.elements[1].type = Graphics::IVertexLayout::Element::eTypeTexCoord;
  vertexLayoutDescriptor.elements[1].format = Graphics::IVertexLayout::Element::eFormatFloat2;
  mVertexLayout = mDevice->CreateVertexLayout(vertexLayoutDescriptor);
  ThrowIf(mVertexLayout == nullptr, Exception::eExceptionTypeGraphicsTest);

  Graphics::IBuffer::Descriptor vertexBufferDescriptor;
  vertexBufferDescriptor.type = Graphics::IBuffer::eTypeVertex;
  vertexBufferDescriptor.accessFlags = Graphics::IBuffer::eAccessFlagGpuRead;
  vertexBufferDescriptor.elementSize = vertexLayoutDescriptor.GetVertexSize();
  F32 r = 100.0f;
  F32 vertices[15] = 
  { 
    -r ,   -r , 1, 0, 1,
     0,     r,  1, 0.5, 0,
     r ,   -r , 1, 1, 1,
  };
  mVertexBuffer = mDevice->CreateBuffer(vertexBufferDescriptor);
  ThrowIf(mVertexBuffer == nullptr, Exception::eExceptionTypeGraphicsTest);
  mVertexBuffer->Add(&vertices[0], 3);
  mVertexBuffer->Update();
}
