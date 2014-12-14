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

/** @file DX11VertexLayout.cpp
This file defines the DX11VertexLayout class.
*/

#include <GraphicsPch.h>
#include "DX11VertexLayout.h"
#include "DX11Core.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

static char* kDX11VertexElementTypeTable[] = 
{
  "POSITION",
  "COLOR",
  "NORMAL",
  "TEXCOORD",
  "ID"
};

static const DXGI_FORMAT kDX11VertexElementFormatTable[] = 
{
  DXGI_FORMAT_R32_FLOAT,           // eFormatFloat
  DXGI_FORMAT_R32G32_FLOAT,        // eFormatFloat2
  DXGI_FORMAT_R32G32B32_FLOAT,     // eFormatFloat3
  DXGI_FORMAT_R32G32B32A32_FLOAT,  // eFormatFloat4
  DXGI_FORMAT_R16_FLOAT,           // eFormatHalf
  DXGI_FORMAT_R16G16_FLOAT,        // eFormatHalf2
  DXGI_FORMAT_R16G16B16A16_FLOAT,  // eFormatHalf4
  DXGI_FORMAT_R32_UINT,            // eFormatUnsigned
  DXGI_FORMAT_R8G8B8A8_UINT        // eFormatUnsignedColor
};

static char* kHlslVertexElementFormatTable[] = 
{
  "float",
  "float2",
  "float3",
  "float4",
  "float",
  "float2",
  "float4",
  "uint",
  "float4"
};

static const String kVertexShaderMain = "VS";
static const String kVertexShaderVersion = "vs_5_0";
static const String kDummyShaderHeader = "struct VSInput\n{\n";
static const String kDummyShaderFooter = "};\nvoid VS(VSInput input){}";

/*----------------------------------------------------------------------------------------------------------------------
Public initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11VertexLayout::DX11VertexLayout()
  : mpDXVertexLayout(0)
{
}

Graphics::DX11VertexLayout::~DX11VertexLayout()
{
  Finalize();
}

bool Graphics::DX11VertexLayout::Initialize(const Descriptor& desc)
{
  mDescriptor = desc;

  // Create a dummy shader to create the D3D input layout
  StringBuffer dummyShaderBuffer;
  dummyShaderBuffer << kDummyShaderHeader;
  String dummyShaderElementStr;
  Containers::DynamicArray<D3D11_INPUT_ELEMENT_DESC> d3dDescriptors(mDescriptor.elements.GetSize());
  for (U32 i = 0; i < mDescriptor.elements.GetSize(); ++i)
  {
    D3D11_INPUT_ELEMENT_DESC d3dDescriptor;
    d3dDescriptor.SemanticName = kDX11VertexElementTypeTable[mDescriptor.elements[i].type];
    d3dDescriptor.SemanticIndex = 0;
    d3dDescriptor.Format = kDX11VertexElementFormatTable[mDescriptor.elements[i].format];
    d3dDescriptor.InputSlot = 0;
    d3dDescriptor.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    d3dDescriptor.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    d3dDescriptor.InstanceDataStepRate = 0;
    d3dDescriptors[i] = d3dDescriptor;
    dummyShaderElementStr.Print("%s element%d : %s;\n",
      kHlslVertexElementFormatTable[mDescriptor.elements[i].format],
      i,
      kDX11VertexElementTypeTable[mDescriptor.elements[i].type]);
    dummyShaderBuffer << dummyShaderElementStr;
  }
  dummyShaderBuffer << kDummyShaderFooter;

  // Compile the dummy shader
  ID3D10Blob* pDXCompiledShader = nullptr;
  ID3D10Blob*	pErrorMessage = nullptr;
  if (D3DCompile(
    dummyShaderBuffer.GetPtr(),
    dummyShaderBuffer.GetLength(),
    nullptr,
    nullptr,
    nullptr,
    kVertexShaderMain.GetPtr(),
    kVertexShaderVersion.GetPtr(),
    0,
    0,
    &pDXCompiledShader,
    &pErrorMessage) != S_OK)
  {
    if (pErrorMessage)
    {
      WString errorMessageWstr;
      Text::Utf8ToWide(errorMessageWstr, String((char*)pErrorMessage->GetBufferPointer()));
      MessageBox(0, errorMessageWstr.GetPtr(), 0, 0);
    }
    return false;
  }

  // Create the D3D input layout
  if (GDXDevice->CreateInputLayout(
    &d3dDescriptors[0],
    static_cast<U32>(d3dDescriptors.GetSize()),
    pDXCompiledShader->GetBufferPointer(),
    pDXCompiledShader->GetBufferSize(),
    &mpDXVertexLayout) < 0)
  {
    return false;
  }

  // Release the dummy compiled shader
  Win32::ReleaseCom(pDXCompiledShader);

  return true;
}

void Graphics::DX11VertexLayout::Finalize()
{
  Win32::ReleaseCom(mpDXVertexLayout);
}

/*----------------------------------------------------------------------------------------------------------------------
IVertexLayout accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IVertexLayout::Descriptor&	Graphics::DX11VertexLayout::GetDescriptor() const
{
  return mDescriptor;
}

ID3D11InputLayout* Graphics::DX11VertexLayout::GetDXVertexLayout() const
{
  return mpDXVertexLayout;
}
