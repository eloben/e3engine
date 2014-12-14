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

/** @file SimpleVertexUpdater.cpp
This file defines the SimpleVertexUpdater class.
*/

#include <GraphicsTestPch.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
SimpleVertexUpdater initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

SimpleVertexUpdater::SimpleVertexUpdater()
  : mDevice(Graphics::Global::GetDevice()) {}

SimpleVertexUpdater::~SimpleVertexUpdater()
{
  mDevice->Finalize();
}

void SimpleVertexUpdater::Initialize(Application::IWindow* pWindow)
{
  mpWindow = pWindow;
  pWindow->GetUpdateEventCallback() += this;
  pWindow->GetKeyDownEventCallback() += this;

  InitializeDevice();
  CreateContexts();
  CreateConstantBuffers();
  CreateShaders();
  CreateVertexBuffers();
}

/*----------------------------------------------------------------------------------------------------------------------
SimpleVertexUpdater accessors
----------------------------------------------------------------------------------------------------------------------*/

/*
// const Application::IWindow::Descriptor& SimpleVertexUpdater::GetDescriptor() const
// {
//   return mWindowDescriptor;
// }
*/

/*----------------------------------------------------------------------------------------------------------------------
SimpleVertexUpdater methods
----------------------------------------------------------------------------------------------------------------------*/

void SimpleVertexUpdater::Bind()
{
  mDevice->BindOutput(mRenderTarget.GetPtr());
  mDevice->BindState(mRasterState.GetPtr());
  mDevice->BindState(mDepthStencilState.GetPtr());
  mDevice->BindState(mBlendState.GetPtr());
  mDevice->BindInput(mVertexLayout.GetPtr());
  mDevice->BindInput(mVertexBuffer.GetPtr(), 0);
  mDevice->BindShader(mShader.GetPtr());
  mDevice->BindShaderConstant(mConstantBuffer.GetPtr(), Graphics::IShader::eStageVertex, 0);
}

void SimpleVertexUpdater::Draw()
{
  mRenderTarget->Clear(Graphics::Color::eBlack, Graphics::IRenderTarget::eClearFlagAll);
  mRenderTarget->Draw(Graphics::eVertexPrimitiveTriangleList, 3, 0, 0, 0, 0, 0);
}

void SimpleVertexUpdater::OnEvent(const Application::UpdateEvent&)
{
  Bind();
  Draw();
  ThrowIfFalse(mViewport->Update(), Exception::eExceptionTypeGraphicsTest);
}

void SimpleVertexUpdater::OnEvent(const Application::KeyDownEvent& event)
{
  if (event.key == Application::Input::eKeyEscape)
  {
    Application::Global::GetApplication().DestroyWindow(mpWindow);
    mpWindow = nullptr;
  }
}

/*----------------------------------------------------------------------------------------------------------------------
SimpleVertexUpdater protected methods
----------------------------------------------------------------------------------------------------------------------*/

void SimpleVertexUpdater::CreateConstantBuffers()
{
  //mProjectionMatrix = Math::BuildPerspectiveLH(90, static_cast<float>(mViewportDescriptor.width) / mViewportDescriptor.height, 1.0f, 1000.0f);
  mProjectionMatrix = Math::BuildOrthographicLH(mViewportDescriptor.width, mViewportDescriptor.height, 1.0f, 100.0f);

	// Retrieve position from local matrix
	Vector3f worldPosition(0, 0, 0);

	// Compute axis vectors
	Vector3f right( 1.0f, 0.0f, 0.0f);
	Vector3f up(    0.0f, 1.0f, 0.0f);
	Vector3f look(  0.0f, 0.0f, 1.0f);
	
	// Build the view matrix
	mViewMatrix[ 0] = right.x;
	mViewMatrix[ 1] = right.y;
	mViewMatrix[ 2] = right.z;
	mViewMatrix[ 4] = up.x;
	mViewMatrix[ 5] = up.y;
	mViewMatrix[ 6] = up.z;
	mViewMatrix[ 8] = look.x;
	mViewMatrix[ 9] = look.y;
	mViewMatrix[10] = look.z;
	mViewMatrix[ 3] = 0.0f;
	mViewMatrix[ 7] = 0.0f;
	mViewMatrix[11] = 0.0f;
	mViewMatrix[12] = -Vector3f::Dot(worldPosition, right);
	mViewMatrix[13] = -Vector3f::Dot(worldPosition, up);
	mViewMatrix[14] = -Vector3f::Dot(worldPosition, look);
	mViewMatrix[15] = 1.0f;
  
  //Matrix4f wvp = mViewMatrix * mProjectionMatrix;
  //Matrix4f wvp = mProjectionMatrix * mViewMatrix;
  Graphics::IBuffer::Descriptor constantBufferDescriptor;
  constantBufferDescriptor.type = Graphics::IBuffer::eTypeConstant;
  constantBufferDescriptor.accessFlags = (Graphics::IBuffer::eAccessFlagGpuRead | Graphics::IBuffer::eAccessFlagCpuWrite);
  constantBufferDescriptor.elementSize = sizeof(F32);
  mConstantBuffer = mDevice->CreateBuffer(constantBufferDescriptor);
  ThrowIf(mConstantBuffer == nullptr, Exception::eExceptionTypeGraphicsTest);
  mConstantBuffer->Add(&Matrix4f::Identity()[0], 16);
  mConstantBuffer->Add(&Matrix4f::Identity()[0], 16);
  mConstantBuffer->Add(&mProjectionMatrix[0], 16);
  mConstantBuffer->Update();
}

void SimpleVertexUpdater::CreateContexts()
{
  Graphics::ITexture2DInstance colorTarget = mDevice->CreateTexture2D(mViewport);
  ThrowIf(colorTarget == nullptr, Exception::eExceptionTypeGraphicsTest);

  Graphics::ITexture2D::Descriptor depthTargetDescriptor;
  depthTargetDescriptor.type = Graphics::ITexture2D::eTypeDepthTarget;
  depthTargetDescriptor.format = Graphics::ITexture2D::eFormatDepth24S8;
  depthTargetDescriptor.width = mViewportDescriptor.width;
  depthTargetDescriptor.height = mViewportDescriptor.height;
  depthTargetDescriptor.unitCount = 1;
  depthTargetDescriptor.accessFlags = Graphics::ITexture2D::eAccessFlagNone;
  Graphics::ITexture2DInstance depthTarget = mDevice->CreateTexture2D(depthTargetDescriptor);
  ThrowIf(depthTarget == nullptr, Exception::eExceptionTypeGraphicsTest);
  
  Graphics::IRenderTarget::Descriptor mainContextDescriptor;
  mainContextDescriptor.colorTargets.Reserve(1);
  mainContextDescriptor.colorTargets[0] = colorTarget;
  mainContextDescriptor.depthTarget = depthTarget;
  mRenderTarget = mDevice->CreateRenderTarget(mainContextDescriptor);
  ThrowIf(mRenderTarget == nullptr, Exception::eExceptionTypeGraphicsTest);

}

void SimpleVertexUpdater::CreateShaders()
{
  Graphics::IShader::Descriptor shaderDescriptor;
  shaderDescriptor.modelVersion = 5;
  shaderDescriptor.stages[Graphics::IShader::eStageVertex].filePath  = "color.hlsl";
  shaderDescriptor.stages[Graphics::IShader::eStageVertex].entryPoint = "VS";
  shaderDescriptor.stages[Graphics::IShader::eStagePixel].filePath  = "color.hlsl";
  shaderDescriptor.stages[Graphics::IShader::eStagePixel].entryPoint = "PS";
  mShader = mDevice->CreateShader(shaderDescriptor);
}

void SimpleVertexUpdater::CreateVertexBuffers()
{
  Graphics::IVertexLayout::Descriptor vertexLayoutDescriptor;
  vertexLayoutDescriptor.elements.Reserve(1);
  vertexLayoutDescriptor.elements[0].type = Graphics::IVertexLayout::Element::eTypePosition;
  vertexLayoutDescriptor.elements[0].format = Graphics::IVertexLayout::Element::eFormatFloat3;
  mVertexLayout = mDevice->CreateVertexLayout(vertexLayoutDescriptor);
  ThrowIf(mVertexLayout == nullptr, Exception::eExceptionTypeGraphicsTest);
  
  Graphics::IBuffer::Descriptor vertexBufferDescriptor;
  vertexBufferDescriptor.type = Graphics::IBuffer::eTypeVertex;
  vertexBufferDescriptor.accessFlags = Graphics::IBuffer::eAccessFlagGpuRead;
  vertexBufferDescriptor.elementSize = vertexLayoutDescriptor.GetVertexSize();
  F32 r = 100.0f;
  F32 vertices[9] = 
  { 
    -r ,   -r , 1,
     0,     r,  1,
     r ,   -r , 1
  };
  mVertexBuffer = mDevice->CreateBuffer(vertexBufferDescriptor);
  ThrowIf(mVertexBuffer == nullptr, Exception::eExceptionTypeGraphicsTest);
  mVertexBuffer->Add(&vertices[0], 3);
  mVertexBuffer->Update();
}

/*----------------------------------------------------------------------------------------------------------------------
SimpleVertexUpdater private initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

void SimpleVertexUpdater::InitializeDevice()
{
  // Device
  if (!mDevice->IsReady()) mDevice->Initialize();
  ThrowIfFalse(mDevice->IsReady(), Exception::eExceptionTypeGraphicsTest);

  // Viewport
  const Application::IWindow::Descriptor& windowDesc = mpWindow->GetDescriptor();
  mViewportDescriptor.windowHandle = windowDesc.handle;
  mViewportDescriptor.width = windowDesc.width;
  mViewportDescriptor.height = windowDesc.height;
  mViewportDescriptor.flags = static_cast<U8>(windowDesc.mode == Application::IWindow::eModeFullScreen ? Graphics::IViewport::eViewportFlagEnableFullScreen : Graphics::IViewport::eViewportFlagEnableVerticalSync);
  mViewport = mDevice->CreateViewport(mViewportDescriptor);
  ThrowIf(mViewport == nullptr, Exception::eExceptionTypeGraphicsTest);

  // Device states
  Graphics::IRasterState::Descriptor rasterStateDescriptor;
  rasterStateDescriptor.fillMode = Graphics::IRasterState::eFillModeSolid;
  rasterStateDescriptor.cullMode = Graphics::IRasterState::eCullModeBack;
  mRasterState = mDevice->CreateRasterState(rasterStateDescriptor);
  ThrowIf(mRasterState == nullptr, Exception::eExceptionTypeGraphicsTest);

  Graphics::IDepthStencilState::Descriptor depthStateDescriptor;
  depthStateDescriptor.flags = Graphics::IDepthStencilState::eDepthStencilFlagEnableAll;
  mDepthStencilState = mDevice->CreateDepthStencilState(depthStateDescriptor);
  ThrowIf(mDepthStencilState == nullptr, Exception::eExceptionTypeGraphicsTest);

  Graphics::IBlendState::Descriptor blendStateDescriptor;
  mBlendState = mDevice->CreateBlendState(blendStateDescriptor);
  ThrowIf(mBlendState == nullptr, Exception::eExceptionTypeGraphicsTest);
}
