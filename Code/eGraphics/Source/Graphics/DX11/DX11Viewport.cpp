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
// Created 20-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file DX11Viewport.cpp
This file contains the DX11Viewport class methods definition.
*/

#include <GraphicsPch.h>
#include "DX11Viewport.h"
#include "DX11Core.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
DX11Viewport auxiliary method declarations
----------------------------------------------------------------------------------------------------------------------*/

void FindDisplayRefreshRate(IDXGIFactory* pDXGIFactory, DXGI_SWAP_CHAIN_DESC& dxSwapChainDesc);

/*----------------------------------------------------------------------------------------------------------------------
DX11Viewport initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DX11Viewport::DX11Viewport()
  : mpDXSwapChain(nullptr)
{
}

Graphics::DX11Viewport::~DX11Viewport()
{
	Finalize();
}

bool Graphics::DX11Viewport::Initialize(const Descriptor& desc)
{
  mDescriptor = desc;
  
  IDXGIDevice* pDXGIDevice;
  GDXDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
  
  IDXGIAdapter* pDXGIAdapter;
  pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
  
  IDXGIFactory*	pDXGIFactory;
  pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pDXGIFactory);

  // Initialize the swap chain description
  DXGI_SWAP_CHAIN_DESC dxSwapChainDesc;
  ZeroMemory(&dxSwapChainDesc, sizeof(dxSwapChainDesc));
  dxSwapChainDesc.BufferCount = 1;
  dxSwapChainDesc.BufferDesc.Width = mDescriptor.width;
  dxSwapChainDesc.BufferDesc.Height = mDescriptor.height;
  dxSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //DXGI_FORMAT_R8G8B8A8_UNORM DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
  dxSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
  dxSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

  // Check for vertical sync and try finding the appropriate refresh rate
  if (mDescriptor.flags & IViewport::eViewportFlagEnableVerticalSync) FindDisplayRefreshRate(pDXGIFactory, dxSwapChainDesc);

  // Set the back buffer usage
  dxSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  dxSwapChainDesc.OutputWindow = static_cast<HWND>(mDescriptor.windowHandle);

  // Disable multi-sampling
  dxSwapChainDesc.SampleDesc.Count = 1;
  dxSwapChainDesc.SampleDesc.Quality = 0;
  dxSwapChainDesc.Windowed = !(mDescriptor.flags & IViewport::eViewportFlagEnableFullScreen);

  // Set the scan line ordering and scaling to unspecified
  dxSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  dxSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  // Discard the back buffer contents after presenting
  dxSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  // Do not set the advanced flags.
  dxSwapChainDesc.Flags = 0;

  // Create the swap chain
  if (pDXGIFactory->CreateSwapChain(static_cast<IUnknown*>(GDXDevice), &dxSwapChainDesc, &mpDXSwapChain) == S_OK)
  {
    // Create the viewport
    D3D11_VIEWPORT dxViewport;
    dxViewport.Width = static_cast<F32>(mDescriptor.width);
    dxViewport.Height = static_cast<F32>(mDescriptor.height);
    dxViewport.MinDepth = 0.0f;
    dxViewport.MaxDepth = 1.0f;
    dxViewport.TopLeftX = 0;
    dxViewport.TopLeftY = 0;

    Containers::List<D3D11_VIEWPORT>& dxViewportList = GDX11Core::GetInstance().GetDXViewportList();
    dxViewportList.PushBack(dxViewport);
    GDXDeviceContext->RSSetViewports(static_cast<U32>(dxViewportList.GetCount()), dxViewportList.GetPtr());
  }

  Win32::ReleaseCom(pDXGIFactory);
  Win32::ReleaseCom(pDXGIAdapter);
  Win32::ReleaseCom(pDXGIDevice);

  return mpDXSwapChain != nullptr;
}

void Graphics::DX11Viewport::Finalize()
{
  D3D11_VIEWPORT dxViewport;
  dxViewport.Width = static_cast<F32>(mDescriptor.width);
  dxViewport.Height = static_cast<F32>(mDescriptor.height);
  dxViewport.MinDepth = 0.0f;
  dxViewport.MaxDepth = 1.0f;
  dxViewport.TopLeftX = 0;
  dxViewport.TopLeftY = 0;

  Containers::List<D3D11_VIEWPORT>& dxViewportList = GDX11Core::GetInstance().GetDXViewportList();
  dxViewportList.RemoveIfFast(dxViewport);
  GDXDeviceContext->RSSetViewports(static_cast<U32>(dxViewportList.GetCount()), dxViewportList.GetPtr());

  Win32::ReleaseCom(mpDXSwapChain);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Viewport accessors
----------------------------------------------------------------------------------------------------------------------*/

const Graphics::IViewport::Descriptor& Graphics::DX11Viewport::GetDescriptor() const
{
  return mDescriptor;
}

IDXGISwapChain* Graphics::DX11Viewport::GetDXSwapChain() const
{
  return mpDXSwapChain;
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Viewport methods
----------------------------------------------------------------------------------------------------------------------*/

bool Graphics::DX11Viewport::Update()
{
  return (mpDXSwapChain->Present(mDescriptor.flags & IViewport::eViewportFlagEnableVerticalSync ? 1 : 0, 0) == S_OK);
}

/*----------------------------------------------------------------------------------------------------------------------
DX11Viewport auxiliary methods
----------------------------------------------------------------------------------------------------------------------*/

void FindDisplayRefreshRate(IDXGIFactory* pDXGIFactory, DXGI_SWAP_CHAIN_DESC& dxSwapChainDesc)
{
  IDXGIAdapter*	pDXGIAdapter;
  if (pDXGIFactory->EnumAdapters(0, &pDXGIAdapter) == S_OK)
  { 
    IDXGIOutput* pDXGIAdapterOutput;
    if (pDXGIAdapter->EnumOutputs(0, &pDXGIAdapterOutput) == S_OK)
    {
      U32 displayModeCount = 0;
      if (pDXGIAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_ENUM_MODES_INTERLACED, &displayModeCount, nullptr) == S_OK)
      {
        // Create a list to hold all the possible display modes for this monitor-video card combination.
        DXGI_MODE_DESC* pDisplayModeList = E_NEW(DXGI_MODE_DESC, displayModeCount);
        if (pDisplayModeList && pDXGIAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_ENUM_MODES_INTERLACED, &displayModeCount, pDisplayModeList) == S_OK)
        {
          // Search the display mode list for a matching window width and height and store its refresh rate numerator and denominator
          dxSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
          dxSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

          for(U32 i = 0; i < displayModeCount; i++)
          {
            if (
              pDisplayModeList[i].Width == dxSwapChainDesc.BufferDesc.Width &&
              pDisplayModeList[i].Height == dxSwapChainDesc.BufferDesc.Height)
            {
              dxSwapChainDesc.BufferDesc.RefreshRate = pDisplayModeList[i].RefreshRate;
              break;
            }
          }
        }
        E_DELETE(pDisplayModeList);
      }
      Win32::ReleaseCom(pDXGIAdapterOutput);
    }
    Win32::ReleaseCom(pDXGIAdapter);
  }
}
