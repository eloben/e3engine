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

/** @file DX11Texture2D.cpp
This file defines the DX11Texture2D class.
*/

#include <GraphicsPch.h>
#include "DX11Texture2D.h"
#include "DX11Viewport.h"
#include "DX11Core.h"
#include <DirectXTex\DDSTextureLoader.h>
#include <DirectXTex\WICTextureLoader.h>

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
DX11 assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_DX11_TEXTURE_2D_TYPE_FILE_INVALID  "ITexture2D cannot be of type file"
#define E_ASSERT_MSG_DX11_TEXTURE_2D_FORMAT_UNKNOWN     "ITexture2D format is unknown"
#define E_ASSERT_MSG_DX11_TEXTURE_2D_UNIT_COUNT_VALUE   "ITexture2D unit count cannot be greater than %d"

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

static const DXGI_FORMAT kDX11TextureFormatTable[] =
{
  DXGI_FORMAT_R8G8B8A8_UNORM,
  DXGI_FORMAT_B8G8R8A8_UNORM,
  DXGI_FORMAT_BC1_UNORM,
  DXGI_FORMAT_BC2_UNORM,
  DXGI_FORMAT_BC3_UNORM,

  DXGI_FORMAT_R32_FLOAT,
  DXGI_FORMAT_R32G32B32_FLOAT,
  DXGI_FORMAT_R16G16B16A16_FLOAT,
  DXGI_FORMAT_R32G32B32A32_FLOAT,

  DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
  DXGI_FORMAT_BC1_UNORM_SRGB,
  DXGI_FORMAT_BC2_UNORM_SRGB,
  DXGI_FORMAT_BC3_UNORM_SRGB,

  DXGI_FORMAT_R16_TYPELESS,       // SRV compatible alternative to DXGI_FORMAT_D16_UNORM 
  DXGI_FORMAT_R24G8_TYPELESS,     // SRV compatible alternative to DXGI_FORMAT_D24_UNORM_S8_UINT,
  DXGI_FORMAT_R32_TYPELESS,       // SRV compatible alternative to DXGI_FORMAT_D32_FLOAT
  DXGI_FORMAT_R32G8X24_TYPELESS,  // SRV compatible alternative to DXGI_FORMAT_D32_FLOAT_S8X24_UINT

  DXGI_FORMAT_UNKNOWN,
};

static const DXGI_FORMAT kDX11FromSRGBTextureFormatTable[] =
{
  DXGI_FORMAT_R8G8B8A8_UNORM,
  DXGI_FORMAT_B8G8R8A8_UNORM,
  DXGI_FORMAT_BC1_UNORM,
  DXGI_FORMAT_BC2_UNORM,
  DXGI_FORMAT_BC3_UNORM,

  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,

  DXGI_FORMAT_R8G8B8A8_UNORM,
  DXGI_FORMAT_B8G8R8A8_UNORM,
  DXGI_FORMAT_BC1_UNORM,
  DXGI_FORMAT_BC2_UNORM,
  DXGI_FORMAT_BC3_UNORM,

  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,

  DXGI_FORMAT_UNKNOWN,
};

static const DXGI_FORMAT kDX11ToSRGBTextureFormatTable[] =
{
  DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
  DXGI_FORMAT_BC1_UNORM_SRGB,
  DXGI_FORMAT_BC2_UNORM_SRGB,
  DXGI_FORMAT_BC3_UNORM_SRGB,

  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,

  DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
  DXGI_FORMAT_BC1_UNORM_SRGB,
  DXGI_FORMAT_BC2_UNORM_SRGB,
  DXGI_FORMAT_BC3_UNORM_SRGB,

  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,
  DXGI_FORMAT_UNKNOWN,

  DXGI_FORMAT_UNKNOWN,
};

static const Graphics::ITexture2D::Format kGpuTexture2DFormatTable[] =
{
  Graphics::ITexture2D::eFormatCount,
};

/*----------------------------------------------------------------------------------------------------------------------
DX11Texture2D public initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11Texture2D::DX11Texture2D()
  : mpDXTexture(nullptr)
  , mpDXShaderResourceView(nullptr)
  , mpDXUnorderedAccessView(nullptr)
{}

Graphics::DX11Texture2D::~DX11Texture2D()
{
	Finalize();
}

bool Graphics::DX11Texture2D::Initialize(const Descriptor& desc)
{
  // Attempt to create the const ITexture2DInstance& texture2D
  if (!CreateTexture2D(desc))
  {
    return false;
  }

  // Check for shader resource view
  if (desc.accessFlags & eAccessFlagGpuRead && !CreateShaderResourceView(desc))
  {  
    Finalize();
    return false;
  }
  // Check for unordered access view
  if (desc.accessFlags & ITexture2D::eAccessFlagGpuWrite && !CreateUnorderedAccessView(desc))
  {
    Finalize();
    return false;
  }

  mDescriptor = desc;
  return true;
}

bool Graphics::DX11Texture2D::Initialize(const FilePath& filePath)
{
  WFilePath filePathWstr;
  Text::Utf8ToWide(filePathWstr, filePath);
  return CreateTexture2DFromFile(filePathWstr);
}

bool Graphics::DX11Texture2D::Initialize(IViewportInstance viewport)
{
  return CreateTexture2DFromViewport(viewport);
}

void Graphics::DX11Texture2D::Finalize()
{
  Win32::ReleaseCom(mpDXUnorderedAccessView);
  Win32::ReleaseCom(mpDXShaderResourceView);
  Win32::ReleaseCom(mpDXTexture);
  DirectX::CleanUpWIC();
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Texture2D accessors
----------------------------------------------------------------------------------------------------------------------*/

U32 Graphics::DX11Texture2D::GetAccessFlags() const
{
  return mDescriptor.accessFlags;
}

const Graphics::ITexture2D::Descriptor& Graphics::DX11Texture2D::GetDescriptor() const
{
  return mDescriptor;
}

ID3D11ShaderResourceView* Graphics::DX11Texture2D::GetDXShaderResourceView() const
{
	return mpDXShaderResourceView;
}

ID3D11Texture2D* Graphics::DX11Texture2D::GetDXTexture() const
{
  return mpDXTexture;
}

ID3D11UnorderedAccessView* Graphics::DX11Texture2D::GetDXUnorderedAccessView() const
{
  return mpDXUnorderedAccessView;
}

Graphics::IResource::ResourceType Graphics::DX11Texture2D::GetResourceType() const
{
  return eResourceTypeTexture2D;
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Texture2D private methods
----------------------------------------------------------------------------------------------------------------------*/

bool Graphics::DX11Texture2D::CreateShaderResourceView(const Descriptor& desc)
{
  D3D11_TEXTURE2D_DESC dxTextureDesc;
  mpDXTexture->GetDesc(&dxTextureDesc);

  D3D11_SHADER_RESOURCE_VIEW_DESC dxShaderResourceViewDesc;
  dxShaderResourceViewDesc.Format = dxTextureDesc.Format;
  if (desc.type == ITexture2D::eTypeDepthTarget)
  {
    switch (desc.format)
    {
    case ITexture2D::eFormatDepth16:
      dxShaderResourceViewDesc.Format = DXGI_FORMAT_R16_FLOAT;
      break;
    case ITexture2D::eFormatDepth24S8:
      dxShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
      break;
    case ITexture2D::eFormatDepth32:
      dxShaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
      break;
    case ITexture2D::eFormatDepth32S8:
      dxShaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
      break;
    }
  }

  if (desc.unitCount == 1)
  {
    dxShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    D3D11_TEX2D_SRV dxTexture2DDesc;
    dxTexture2DDesc.MipLevels = 1;
    dxTexture2DDesc.MostDetailedMip = 0;
    dxShaderResourceViewDesc.Texture2D = dxTexture2DDesc;
  }
  else
  {
    dxShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    D3D11_TEX2D_ARRAY_SRV dxTexture2DArrayDesc;
    dxTexture2DArrayDesc.MipLevels = 1;
    dxTexture2DArrayDesc.MostDetailedMip = 0;
    dxTexture2DArrayDesc.ArraySize = desc.unitCount;
    dxTexture2DArrayDesc.FirstArraySlice = 0;
    dxShaderResourceViewDesc.Texture2DArray = dxTexture2DArrayDesc;
  }

  return (GDXDevice->CreateShaderResourceView(mpDXTexture, &dxShaderResourceViewDesc, &mpDXShaderResourceView) == S_OK);
}

bool Graphics::DX11Texture2D::CreateTexture2D(const Descriptor& desc)
{
  E_ASSERT_MSG(
    desc.unitCount <= D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION, 
    E_ASSERT_MSG_DX11_TEXTURE_2D_UNIT_COUNT_VALUE, 
    D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION);

  D3D11_TEXTURE2D_DESC dxTextureDesc;
  ZeroMemory(&dxTextureDesc, sizeof(dxTextureDesc));
  dxTextureDesc.Width = desc.width;
  dxTextureDesc.Height = desc.height;
  dxTextureDesc.MipLevels = desc.mipLevelCount;
  dxTextureDesc.ArraySize = desc.unitCount;
  dxTextureDesc.Format = kDX11TextureFormatTable[desc.format];
  dxTextureDesc.SampleDesc.Count = 1;
  dxTextureDesc.Usage = D3D11_USAGE_DEFAULT;
  dxTextureDesc.CPUAccessFlags = 0;
  dxTextureDesc.MiscFlags = 0;

  // Verify that the const ITexture2DInstance& texture2D type is not file const ITexture2DInstance& texture2D
  E_ASSERT_MSG(
    desc.type != ITexture2D::eTypeFile, 
    E_ASSERT_MSG_DX11_TEXTURE_2D_TYPE_FILE_INVALID);

  if (desc.type == ITexture2D::eTypeColorTarget)
  {    
    dxTextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
  }
  else if (desc.type == ITexture2D::eTypeDepthTarget)
  {
    dxTextureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
  }

  if (desc.accessFlags & eAccessFlagGpuRead)
  {
    dxTextureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
  }

  if (desc.accessFlags & eAccessFlagGpuWrite)
  {
    dxTextureDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
  }

  return (GDXDevice->CreateTexture2D(&dxTextureDesc, nullptr, &mpDXTexture) == S_OK);
}

bool Graphics::DX11Texture2D::CreateTexture2DFromFile(const WFilePath& wfilePath)
{
  // Create the const ITexture2DInstance& texture2D from file
  // Use GDXDeviceContext version in order to generate mipmaps
  if (DirectX::CreateDDSTextureFromFileEx(GDXDevice, GDXDeviceContext, wfilePath.GetPtr(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, true, (ID3D11Resource**)&mpDXTexture, &mpDXShaderResourceView) != S_OK)
  {
    if (DirectX::CreateWICTextureFromFileEx(GDXDevice, GDXDeviceContext, wfilePath.GetPtr(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, true, (ID3D11Resource**)&mpDXTexture, &mpDXShaderResourceView) != S_OK)
      return false;
  }

  D3D11_TEXTURE2D_DESC dxTextureDesc;
  mpDXTexture->GetDesc(&dxTextureDesc);
  mDescriptor.type = ITexture2D::eTypeFile;
  mDescriptor.format = ITexture2D::eFormatCount;
  mDescriptor.width = dxTextureDesc.Width;
  mDescriptor.height = dxTextureDesc.Height;
  mDescriptor.unitCount = dxTextureDesc.ArraySize;
  mDescriptor.accessFlags = eAccessFlagGpuRead;

  // Check whether the file const ITexture2DInstance& texture2D format is known
  for (U32 i = 0; i < ITexture2D::eFormatCount; ++i)
  {
    if (dxTextureDesc.Format == kDX11TextureFormatTable[i])
    {
      mDescriptor.format = static_cast<ITexture2D::Format>(i);
      break;
    }
  }

  E_ASSERT_MSG(
    mDescriptor.format != ITexture2D::eFormatCount, 
    E_ASSERT_MSG_DX11_TEXTURE_2D_FORMAT_UNKNOWN);

  return true;
}

bool Graphics::DX11Texture2D::CreateTexture2DFromViewport(IViewportInstance viewport)
{
  DX11ViewportInstance dxViewport = viewport;
  if (dxViewport->GetDXSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&mpDXTexture)) != S_OK)
  {
    return false;
  }

  D3D11_TEXTURE2D_DESC dxTextureDesc;
  mpDXTexture->GetDesc(&dxTextureDesc);
  mDescriptor.type = ITexture2D::eTypeColorTarget;
  mDescriptor.format = ITexture2D::eFormatCount;
  mDescriptor.width = dxTextureDesc.Width;
  mDescriptor.height = dxTextureDesc.Height;
  mDescriptor.unitCount = dxTextureDesc.ArraySize;

  // Check whether the file const ITexture2DInstance& texture2D format is known
  for (U32 i = 0; i < ITexture2D::eFormatCount; ++i)
  {
    if (dxTextureDesc.Format == kDX11TextureFormatTable[i])
    {
      mDescriptor.format = static_cast<ITexture2D::Format>(i);
      break;
    }
  }

  E_ASSERT_MSG(
    mDescriptor.format != ITexture2D::eFormatCount, 
    E_ASSERT_MSG_DX11_TEXTURE_2D_FORMAT_UNKNOWN);

  return true;
}

bool Graphics::DX11Texture2D::CreateUnorderedAccessView(const Descriptor& desc)
{
  D3D11_TEXTURE2D_DESC dxTextureDesc;
  mpDXTexture->GetDesc(&dxTextureDesc);

  D3D11_UNORDERED_ACCESS_VIEW_DESC dxUnorderedAccessViewDesc;
  dxUnorderedAccessViewDesc.Format = dxTextureDesc.Format;
  
  if (desc.unitCount == 1)
  {
    dxUnorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    D3D11_TEX2D_UAV dxTexture2DDesc;
    dxTexture2DDesc.MipSlice = 0;
    dxUnorderedAccessViewDesc.Texture2D = dxTexture2DDesc;
  }
  else
  {
    dxUnorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
    D3D11_TEX2D_ARRAY_UAV dxTexture2DArrayDesc;
    dxTexture2DArrayDesc.MipSlice = 0;
    dxTexture2DArrayDesc.FirstArraySlice = 0;
    dxUnorderedAccessViewDesc.Texture2DArray = dxTexture2DArrayDesc;
  }
  
  return (GDXDevice->CreateUnorderedAccessView(mpDXTexture, &dxUnorderedAccessViewDesc, &mpDXUnorderedAccessView) == S_OK);
}
