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

/** @file DX11Buffer.cpp
This file defines the DX11Buffer class.
*/

#include <GraphicsPch.h>
#include "DX11Buffer.h"
#include "DX11Core.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions

Please note that this class has the following usage contract: 

1. kDX11MaxConstantBufferElementCount corresponds to the number of registers (four 32-bit values) in a constant buffer.
2. kDX11MaxBufferSize uses the maximum size the DX11 debug layer checks. However the real limit is:
    min(max(128,0.25f * (amount of dedicated VRAM)), 2048) MB
    
    Source:
    Resource Limits (Direct3D 11) http://msdn.microsoft.com/en-us/library/windows/desktop/ff819065%28v=vs.85%29.aspx
----------------------------------------------------------------------------------------------------------------------*/
static const U32 kDX11MaxConstantBufferElementCount = D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * 4; // 4096 registers * 4
static const U32 kDX11MaxBufferSize = 128 * 1024 * 1024; // 128 MB

/*----------------------------------------------------------------------------------------------------------------------
DX11Buffer assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_DX11_BUFFER_COUNT_VALUE                            "Element count value must be equal or smaller than the buffer capacity"
#define E_ASSERT_MSG_DX11_BUFFER_WRITABLE                               "Buffer must be writable in order to use Write"
#define E_ASSERT_MSG_DX11_BUFFER_BUFFER_ACCESS_FLAG_COMBINATION_INVALID "The buffer access flag combination is not supported"
#define E_ASSERT_MSG_DX11_BUFFER_GPU_WRITE_ACCESS_TYPE_INVALID          "Buffer must be GPU writable for GPU write access (%d)"
#define E_ASSERT_MSG_DX11_BUFFER_CONSTANT_BUFFER_MAX_ELEMENT_COUNT      "Constant buffer element count (%d) exceeds maximum of (%d)"
#define E_ASSERT_MSG_DX11_BUFFER_MAX_SIZE                               "Buffer size (%d) exceeds maximum of 128 MB"

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

// GpuBuffer access translation to DX11 types
static const D3D11_BIND_FLAG kDX11TypeTable[] =
{
  D3D11_BIND_CONSTANT_BUFFER,
  D3D11_BIND_VERTEX_BUFFER,
  D3D11_BIND_INDEX_BUFFER, 
  D3D11_BIND_SHADER_RESOURCE,
};

/*----------------------------------------------------------------------------------------------------------------------
DX11Buffer initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11Buffer::DX11Buffer()
  : mpDXBuffer(nullptr)
  , mpDXShaderResourceView(nullptr)
  , mpDXUnorderedAccessView(nullptr)
  , mElementCount(0)
  , mMaxElementCount(0)
  , mAllocatedElementCount(0)
  , mPendingDataFlag(false)
{
}

Graphics::DX11Buffer::~DX11Buffer()
{
  Finalize();
}

void Graphics::DX11Buffer::Initialize(const Descriptor& desc)
{
  mDescriptor = desc;
}

void Graphics::DX11Buffer::Finalize()
{
  // Release vertex buffer
  Win32::ReleaseCom(mpDXBuffer);
  Win32::ReleaseCom(mpDXShaderResourceView);
  Win32::ReleaseCom(mpDXUnorderedAccessView);
  
  // Zero variables
  mData.Clear();
  mElementCount = 0;
  mMaxElementCount = 0;
  mAllocatedElementCount = 0;
  mPendingDataFlag = false;
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Buffer accessors
----------------------------------------------------------------------------------------------------------------------*/

U32 Graphics::DX11Buffer::GetAccessFlags() const
{
  return mDescriptor.accessFlags;
}

const Graphics::IBuffer::Descriptor& Graphics::DX11Buffer::GetDescriptor() const
{
  return mDescriptor;
}

U32 Graphics::DX11Buffer::GetCapacity() const
{
  return mMaxElementCount;
}

U32	Graphics::DX11Buffer::GetCount() const
{
  return mElementCount;
}

ID3D11Buffer* Graphics::DX11Buffer::GetDXBuffer() const
{
  return mpDXBuffer;
}

ID3D11ShaderResourceView* Graphics::DX11Buffer::GetDXShaderResourceView() const
{
  return mpDXShaderResourceView;
}
ID3D11UnorderedAccessView* Graphics::DX11Buffer::GetDXUnorderedAccessView() const
{
  return mpDXUnorderedAccessView;
}

Graphics::IResource::ResourceType Graphics::DX11Buffer::GetResourceType() const
{
  return eResourceTypeBuffer;
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Buffer methods
----------------------------------------------------------------------------------------------------------------------*/

void Graphics::DX11Buffer::Add(const void* pElements, U32 elementCount)
{
  E_ASSERT_PTR(pElements);
  E_ASSERT_MSG(elementCount > 0, E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE);
  E_ASSERT_MSG(
    mDescriptor.type != IBuffer::eTypeConstant || (mElementCount + elementCount) <= kDX11MaxConstantBufferElementCount,
    E_ASSERT_MSG_DX11_BUFFER_CONSTANT_BUFFER_MAX_ELEMENT_COUNT, 
    mElementCount + elementCount,
    kDX11MaxConstantBufferElementCount);
  E_ASSERT_MSG(
    (mElementCount + elementCount) * mDescriptor.elementSize <= kDX11MaxBufferSize,
    E_ASSERT_MSG_DX11_BUFFER_MAX_SIZE, 
    (mElementCount + elementCount) * mDescriptor.elementSize,
    kDX11MaxBufferSize);
  mData.Copy(static_cast<const Byte*>(pElements), elementCount * mDescriptor.elementSize, mData.GetCount());
  mElementCount += elementCount;
  mPendingDataFlag = true;
}

void Graphics::DX11Buffer::Clear()
{
  mData.Clear();
  mElementCount = 0;
  mPendingDataFlag = false;
}

void Graphics::DX11Buffer::Remove(U32 elementCount, U32 startIndex /* = 0 */)
{
  E_ASSERT_MSG(elementCount > 0, E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE);
  E_ASSERT_MSG(startIndex < elementCount, E_ASSERT_MSG_MATH_A_SMALLER_B, E_TO_STR(startIndex), startIndex, E_TO_STR(elementCount), elementCount);
  E_ASSERT_MSG(elementCount + startIndex <= mElementCount, E_ASSERT_MSG_MATH_A_SMALLER_EQUAL_B, E_TO_STR(elementCount + startIndex), elementCount + startIndex, E_TO_STR(mElementCount), mElementCount);
  mData.Remove(mData.GetBegin() + (startIndex * mDescriptor.elementSize), (elementCount * mDescriptor.elementSize));
  mElementCount -= elementCount;
  mPendingDataFlag = true;
}

void Graphics::DX11Buffer::Set(const void* pElements, U32 elementCount, U32 startIndex /* = 0 */)
{
  E_ASSERT_MSG(elementCount > 0, E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE);
  // Prepare required data size
  mData.Copy(
    static_cast<const Byte*>(pElements), 
    elementCount * mDescriptor.elementSize, 
    startIndex * mDescriptor.elementSize);
  mElementCount = Math::Max(mElementCount, startIndex + elementCount);
  mPendingDataFlag = true;
}

bool Graphics::DX11Buffer::Update()
{
  if (mPendingDataFlag) return (mDescriptor.accessFlags & eAccessFlagCpuWrite && mElementCount <= mMaxElementCount) ? Map() : Allocate(); 
  return false;
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Buffer private methods
----------------------------------------------------------------------------------------------------------------------*/

bool Graphics::DX11Buffer::Allocate()
{
  E_ASSERT_MSG(mDescriptor.elementSize > 0, E_ASSERT_MSG_MATH_GREATER_THAN_ZERO_VALUE);

  // Set buffer capacity
  mMaxElementCount = mElementCount;

  // Create a default buffer description
  D3D11_BUFFER_DESC dxBufferDesc;
  dxBufferDesc.ByteWidth = static_cast<U32>(mDescriptor.elementSize) * mMaxElementCount;
  dxBufferDesc.BindFlags = kDX11TypeTable[mDescriptor.type];
  dxBufferDesc.CPUAccessFlags = 0;
  dxBufferDesc.StructureByteStride = 0;
  dxBufferDesc.MiscFlags = 0;

  // Configure buffer access flags
  if (mDescriptor.accessFlags == eAccessFlagAll)
  {
    dxBufferDesc.Usage = D3D11_USAGE_STAGING;
    dxBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
  }
  else
  {
    E_ASSERT_MSG(
      mDescriptor.accessFlags & eAccessFlagGpuRead && 
      !(mDescriptor.accessFlags & eAccessFlagCpuRead),
      E_ASSERT_MSG_DX11_BUFFER_BUFFER_ACCESS_FLAG_COMBINATION_INVALID);
    
    if (mDescriptor.accessFlags & eAccessFlagCpuWrite)
    {
      E_ASSERT_MSG(!(mDescriptor.accessFlags & eAccessFlagGpuWrite), E_ASSERT_MSG_DX11_BUFFER_BUFFER_ACCESS_FLAG_COMBINATION_INVALID);
      dxBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
      dxBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
      if (mDescriptor.accessFlags & eAccessFlagGpuWrite)
      {
        dxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        if (mDescriptor.type == eTypeResource)
        {
          dxBufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
        }
      }
      else
      {
        dxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
      }
    }
  }
  
  // Resource buffer specifics
  if (mDescriptor.type == eTypeResource)
  {
    if (mDescriptor.gpuWriteAccess == eGraphicsWriteAccessRaw)
    {
      E_ASSERT_MSG(mDescriptor.accessFlags & IResource::eAccessFlagGpuWrite, E_ASSERT_MSG_DX11_BUFFER_GPU_WRITE_ACCESS_TYPE_INVALID, mDescriptor.gpuWriteAccess);
      dxBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
    }
    else
    {
      dxBufferDesc.StructureByteStride = static_cast<U32>(mDescriptor.elementSize);
      dxBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    }
  }

  // Configure the buffer initial data
  D3D11_SUBRESOURCE_DATA initialData;
  initialData.pSysMem = mData.GetPtr();
  initialData.SysMemPitch = 0;
  initialData.SysMemSlicePitch = 0;

  // Release the previous buffer & recreate it
  Win32::ReleaseCom(mpDXBuffer);
  ID3D11Device* pDXDevice = GDXDevice;
  if (pDXDevice->CreateBuffer(&dxBufferDesc, &initialData, &mpDXBuffer) != S_OK)
  {
    return false;
  }
  mAllocatedElementCount = mElementCount;

  // Create resource buffer views
  if (mDescriptor.type == eTypeResource)
  {
    if (!CreateShaderResourceView())
    {
      return false;
    }
    if (mDescriptor.accessFlags & eAccessFlagGpuWrite && !CreateUnorderedAccessView())
    {
      return false;
    }
  } 
  mPendingDataFlag = false;

  return true;
}

bool Graphics::DX11Buffer::CreateShaderResourceView()
{
  E::Win32::ReleaseCom(mpDXShaderResourceView);
  D3D11_SHADER_RESOURCE_VIEW_DESC dxShaderResourceViewDesc;
  dxShaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
  dxShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
  dxShaderResourceViewDesc.Buffer.ElementOffset = 0;
  dxShaderResourceViewDesc.Buffer.ElementWidth = mElementCount;

  return (GDXDevice->CreateShaderResourceView(mpDXBuffer, &dxShaderResourceViewDesc, &mpDXShaderResourceView) == S_OK);
}

bool Graphics::DX11Buffer::CreateUnorderedAccessView()
{
  E::Win32::ReleaseCom(mpDXUnorderedAccessView);
  D3D11_UNORDERED_ACCESS_VIEW_DESC dxUnorderedAccessViewDesc;
  dxUnorderedAccessViewDesc.Format = DXGI_FORMAT_UNKNOWN;
  dxUnorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
  dxUnorderedAccessViewDesc.Buffer.FirstElement = 0;
  dxUnorderedAccessViewDesc.Buffer.NumElements = mElementCount;
  
  if (mDescriptor.gpuWriteAccess == eGraphicsWriteAccessRaw)
  {
    dxUnorderedAccessViewDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
  }
  else if (mDescriptor.type == eGraphicsWriteAccessStructured)
  {
    dxUnorderedAccessViewDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
  }
  else
  {
    E_ASSERT(mDescriptor.type == eGraphicsWriteAccessAppendConsume);
    dxUnorderedAccessViewDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
  }

  return (GDXDevice->CreateUnorderedAccessView(mpDXBuffer, &dxUnorderedAccessViewDesc, &mpDXUnorderedAccessView) == S_OK);
}

bool Graphics::DX11Buffer::Map()
{
  E_ASSERT_PTR(mpDXBuffer);
    
  // If vertex count is zero or it exceeds buffer capacity use the whole buffer
  E_ASSERT_MSG(mElementCount >= 0  && mElementCount <= mMaxElementCount, E_ASSERT_MSG_DX11_BUFFER_COUNT_VALUE);

  // Lock the buffer resource (GPU) to copy the raw data (CPU)
  ID3D11DeviceContext* pDXDeviceContext = GDXDeviceContext;
  D3D11_MAPPED_SUBRESOURCE resource;
  if (pDXDeviceContext->Map(mpDXBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) < 0)
  {
    return false;
  }
  memcpy(resource.pData, mData.GetPtr(), mDescriptor.elementSize * mElementCount);
  pDXDeviceContext->Unmap(mpDXBuffer, 0);
  mAllocatedElementCount = mElementCount;

  mPendingDataFlag = false;

  return true;
}
