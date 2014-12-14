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

/** @file IndexedVertexUpdater.cpp
This file defines the IndexedVertexUpdater class.
*/

#include <GraphicsTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
IndexedVertexUpdater methods
----------------------------------------------------------------------------------------------------------------------*/

void IndexedVertexUpdater::Bind()
{
  TextureVertexUpdater::Bind();
  mDevice->BindInput(indexBuffer.GetPtr());
}

void IndexedVertexUpdater::Draw()
{
  mRenderTarget->Clear(Graphics::Color::eDarkestGrey, Graphics::IRenderTarget::eClearFlagAll);
  mRenderTarget->Draw(Graphics::eVertexPrimitiveTriangleList, 0, 6, 0, 0, 0, 0);
}

/*----------------------------------------------------------------------------------------------------------------------
IndexedVertexUpdater protected methods
----------------------------------------------------------------------------------------------------------------------*/

void IndexedVertexUpdater::CreateVertexBuffers()
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
  F32 vertices[20] = 
  { 
    -r ,   -r , 1, 0, 1,
    -r,     r,  1, 0, 0,
     r ,    r , 1, 1, 0,
     r ,   -r , 1, 1, 1
  };
  mVertexBuffer = mDevice->CreateBuffer(vertexBufferDescriptor);
  ThrowIf(mVertexBuffer == nullptr, Exception::eExceptionTypeGraphicsTest);
  mVertexBuffer->Add(&vertices[0], 4);
  mVertexBuffer->Update();
    
  Graphics::IBuffer::Descriptor indexBufferDescriptor;
  indexBufferDescriptor.type = Graphics::IBuffer::eTypeIndex;
  indexBufferDescriptor.accessFlags = Graphics::IBuffer::eAccessFlagGpuRead;
  indexBufferDescriptor.elementSize = sizeof(U16);
  U16 indices[6] = { 0, 1, 2, 2, 3, 0};
  indexBuffer = mDevice->CreateBuffer(indexBufferDescriptor);
  ThrowIf(indexBuffer == nullptr, Exception::eExceptionTypeGraphicsTest);
  indexBuffer->Add(&indices[0], 6);
  indexBuffer->Update();
  
}
