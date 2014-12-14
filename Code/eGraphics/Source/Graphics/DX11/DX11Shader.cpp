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

/** @file DX11Shader.cpp
This file defines the DX11Shader class.
*/

#include <GraphicsPch.h>
#include "DX11Shader.h"
#include "DX11Core.h"
#include <fstream>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
DX11Buffer assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_DX11_SHADER_MODEL_VERSION_INVALID          "Shader model version (%d) is not valid"
#define E_ASSERT_MSG_DX11_SHADER_COMPILATION_DESCRIPTOR_INVALID "Shader compilation descriptor is invalid:\n- FilePath: %s\n- EntryPoint: %s"
#define E_ASSERT_MSG_DX11_SHADER_COMPILATION_FAILED             "Shader compilation failed: %s"

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

static const String kVertexShaderMain = "VS";
static const String kVertexShaderVersion = "vs_5_0";
static const String kHullShaderMain = "HS";
static const String kHullShaderVersion = "hs_5_0";
static const String kDomainShaderMain = "DS";
static const String kDomainShaderVersion = "ds_5_0";
static const String kGeometryShaderMain = "GS";
static const String kGeometryShaderVersion = "gs_5_0";
static const String kPixelShaderMain = "PS";
static const String kPixelShaderVersion = "ps_5_0";
static const String kComputeShaderMain = "CS";
static const String kComputeShaderVersion = "cs_5_0";

static const char* kHlslShaderVersionPrefixTable[] = { "vs_", "hs_", "ds_", "gs_", "ps_", "cs_" };
static const char* kHlslShaderVersionEnding = "_0";

/*----------------------------------------------------------------------------------------------------------------------
DX11ShaderIncludeHandler
----------------------------------------------------------------------------------------------------------------------*/
class DX11ShaderIncludeHandler: public ID3D10Include
{
public:
  DX11ShaderIncludeHandler(const FilePath& shaderFilePath)
    : mRootDirectory(FileSystem::Directory::GetParent(shaderFilePath)) {}

  STDMETHOD(Open)(D3D_INCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID *ppData, UINT *pBytes) 
  {
    E_ASSERT_PTR(pFileName);
    E_ASSERT_PTR(ppData);
    E_ASSERT_PTR(pBytes);

    FileSystem::Path filePath = mRootDirectory;
    if (filePath.GetLength()) filePath.Append("\\", 1);
    filePath.Append(pFileName, Text::GetLength(pFileName));
    // Retrieve file info
    if (!FileSystem::File::GetInfo(filePath.GetPtr(), mFileInfo)) return S_FALSE;
    // Read file
    std::ifstream ifstream(filePath.GetPtr(), std::ios::in);
    E_ASSERT(ifstream.is_open());
    mFileData.Reserve(mFileInfo.byteSize);
    ifstream.read(mFileData.GetPtr(), mFileInfo.byteSize);
    E_ASSERT(ifstream.gcount() <= Math::NumericLimits<U32>::Max());
    U32 readSize = static_cast<U32>(ifstream.gcount());
    mFileData[readSize] = 0;
    *ppData = mFileData.GetPtr();
    *pBytes = readSize;

    return S_OK;
  }

  STDMETHOD(Close)(LPCVOID) { return S_OK; }

private:
  FilePath                        mRootDirectory;
  Containers::DynamicArray<char>  mFileData;
  FileInfo                        mFileInfo;
} ;//gDX11ShaderIncludeHandler;

/*----------------------------------------------------------------------------------------------------------------------
DX11Shader initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11Shader::DX11Shader()
  : mpDXComputeShader(nullptr)
  , mpDXDomainShader(nullptr)
  , mpDXGeometryShader(nullptr)
  , mpDXHullShader(nullptr)
  , mpDXPixelShader(nullptr)
  , mpDXVertexShader(nullptr)
{
}

Graphics::DX11Shader::~DX11Shader()
{
	Finalize();
}

bool Graphics::DX11Shader::Initialize(const Descriptor& desc)
{
  E_ASSERT_MSG(
    desc.modelVersion > 0,
    E_ASSERT_MSG_DX11_SHADER_MODEL_VERSION_INVALID,
    desc.modelVersion);

  mDescriptor = desc;

  // Get shader macros
  Containers::DynamicArray<D3D10_SHADER_MACRO> dxShaderMacroList;
  // The last structure in the array serves as a terminator and must have all members set to 0 (DynamicArray enforces construction in non POD declared types)
  dxShaderMacroList.Reserve(mDescriptor.macroList.GetSize() + 1);
  for (U32 i = 0; i < mDescriptor.macroList.GetSize(); ++i)
  {
    dxShaderMacroList[i].Name = mDescriptor.macroList[i].GetPtr();
    dxShaderMacroList[i].Definition = "1";
  }

  for (U32 i = 0; i < eStageCount; ++i)
  {
    // Compile shader stage
    if (mDescriptor.stages[i].filePath.GetLength())
    {
      E_ASSERT_MSG(
        mDescriptor.stages[i].entryPoint.GetLength(),       
        E_ASSERT_MSG_DX11_SHADER_COMPILATION_DESCRIPTOR_INVALID, 
        mDescriptor.stages[i].filePath.GetPtr(), 
        mDescriptor.stages[i].entryPoint.GetPtr());

      // Initialize an include handler
      DX11ShaderIncludeHandler shaderIncludeHandle(mDescriptor.stages[i].filePath);

      // Get shader modelVersion
      StringBuffer shaderVersion(kHlslShaderVersionPrefixTable[i]);
      shaderVersion << mDescriptor.modelVersion << kHlslShaderVersionEnding;

      // Compile shader file
      ID3D11Device* pDXDevice = GDXDevice;
      ID3D10Blob*	pCompiledShader = nullptr;
      ID3D10Blob*	pErrorMessage = nullptr;

      WFilePath wfilePath;
      Text::Utf8ToWide(wfilePath, mDescriptor.stages[i].filePath); 
	  if (D3DCompileFromFile(
		  wfilePath.GetPtr(),
		  dxShaderMacroList.GetPtr(),
		  &shaderIncludeHandle,
		  mDescriptor.stages[i].entryPoint.GetPtr(),
		  shaderVersion.GetPtr(),
		  0,
		  0,
		  &pCompiledShader,
		  &pErrorMessage) == S_OK)
	    {
        HRESULT hr = 0;
        switch (i)
        {
        case eStageVertex:
          hr = pDXDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), nullptr, &mpDXVertexShader);
          break;
        case eStageHull:
          hr = pDXDevice->CreateHullShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), nullptr, &mpDXHullShader);
          break;
        case eStageDomain:
          hr = pDXDevice->CreateDomainShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), nullptr, &mpDXDomainShader);
          break;
        case eStageGeometry:
          hr = pDXDevice->CreateGeometryShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), nullptr, &mpDXGeometryShader);
          break;
        case eStagePixel:
          hr = pDXDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), nullptr, &mpDXPixelShader);
          break;
        case eStageCompute:
          hr = pDXDevice->CreateComputeShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), nullptr, &mpDXComputeShader);
          break;
        }
        E_ASSERT(hr == S_OK);
        Win32::ReleaseCom(pCompiledShader);
        Win32::ReleaseCom(pErrorMessage);
      }
      else
      {
        #ifdef E_DEBUG
        E_ASSERT_ALWAYS(E_ASSERT_MSG_DX11_SHADER_COMPILATION_FAILED, pErrorMessage ? pErrorMessage->GetBufferPointer() : "Shader not found");
        #endif
        Finalize();
        return false;
      }
    }
  }

  return true;
}

void Graphics::DX11Shader::Finalize()
{
  Win32::ReleaseCom(mpDXComputeShader);
  Win32::ReleaseCom(mpDXPixelShader);
  Win32::ReleaseCom(mpDXGeometryShader);
  Win32::ReleaseCom(mpDXDomainShader);
  Win32::ReleaseCom(mpDXHullShader);
  Win32::ReleaseCom(mpDXVertexShader);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Shader accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IShader::Descriptor& Graphics::DX11Shader::GetDescriptor() const
{
  return mDescriptor;
}

ID3D11ComputeShader* Graphics::DX11Shader::GetDXComputeShader() const
{
  return mpDXComputeShader;
}

ID3D11DomainShader* Graphics::DX11Shader::GetDXDomainShader() const
{
  return mpDXDomainShader;
}

ID3D11GeometryShader* Graphics::DX11Shader::GetDXGeometryShader() const
{
  return mpDXGeometryShader;
}

ID3D11HullShader* Graphics::DX11Shader::GetDXHullShader() const
{
  return mpDXHullShader;
}

ID3D11PixelShader* Graphics::DX11Shader::GetDXPixelShader() const
{
  return mpDXPixelShader;
}

ID3D11VertexShader* Graphics::DX11Shader::GetDXVertexShader() const
{
  return mpDXVertexShader;
}
