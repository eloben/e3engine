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

/** @file DX11RenderTarget.h
This file contains the declaration of the DX11RenderTarget class.
*/

#ifndef E3_DX11_RENDER_TARGET_H
#define E3_DX11_RENDER_TARGET_H

#include "DX11Core.h"

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11RenderTarget
----------------------------------------------------------------------------------------------------------------------*/
class DX11RenderTarget : public IRenderTarget
{
public:
  DX11RenderTarget();
  ~DX11RenderTarget();
  bool                            Initialize(const Descriptor& desc);
  void                            Finalize();

  const Descriptor&               GetDescriptor() const;
  ID3D11DepthStencilView*         GetDXDepthTarget() const;
  ID3D11RenderTargetView*const *  GetDXColorTargets() const;

  void                            Clear(const Color& clearColor = Color::eBlack, U8 clearFlags = eClearFlagAll);
  void                            Draw(VertexPrimitive vertexPrimitive, U32 vertexCount, U32 indexCount, U32 instanceCount, U32 startVertex, U32 startIndex, U32 startInstance);
  void                            DrawIndirect(VertexPrimitive vertexPrimitive, U32 vertexCount, U32 indexCount, U32 instanceCount, U32 startVertex, U32 startIndex, U32 startInstance);

private:
  Descriptor                      mDescriptor;
  ID3D11DepthStencilView*         mpDXDepthStencilView;
  ID3D11RenderTargetView*         mpDXRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

  bool                            CreateDepthStencilView(const ITexture2DInstance& texture2D);
  bool                            CreateRenderTargetView(U32 renderTargetIndex, const ITexture2DInstance& texture2D, U32 sliceIndex = 0);
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(DX11RenderTarget)
};

/*----------------------------------------------------------------------------------------------------------------------
DX11RenderTarget types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<DX11RenderTarget> DX11RenderTargetInstance;
}
}

#endif

