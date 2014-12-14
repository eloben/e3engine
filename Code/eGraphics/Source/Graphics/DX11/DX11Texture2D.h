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

/** @file DX11Texture2D.h
This file declares the DX11Texture2D class.
*/

#ifndef E3_DX11_TEXTURE_2D_H
#define E3_DX11_TEXTURE_2D_H

#include "DX11Base.h"

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11Texture2D

Please note that this set of macros has the following usage contract:

1. File textures are not gamma corrected (the color value is linear not sRGB).
----------------------------------------------------------------------------------------------------------------------*/
class DX11Texture2D : public ITexture2D
{
public:
  DX11Texture2D();
  ~DX11Texture2D();

  bool                                Initialize(const Descriptor& desc);
  bool                                Initialize(const FilePath& filePath);
  bool                                Initialize(IViewportInstance viewport);
  void                                Finalize();

  // Accessors
  U32                                 GetAccessFlags() const;
  const Descriptor&                   GetDescriptor() const;
  ID3D11ShaderResourceView*	          GetDXShaderResourceView() const;
  ID3D11Texture2D*	                  GetDXTexture() const;
  ID3D11UnorderedAccessView*	        GetDXUnorderedAccessView() const;
  ResourceType                        GetResourceType() const;

private:
  ID3D11Texture2D*					          mpDXTexture;
  ID3D11ShaderResourceView*           mpDXShaderResourceView;
  ID3D11UnorderedAccessView*          mpDXUnorderedAccessView;
  Descriptor                          mDescriptor;

  bool                                CreateTexture2D(const Descriptor& desc);
  bool                                CreateTexture2DFromFile(const WFilePath& wfilePath);
  bool                                CreateTexture2DFromViewport(IViewportInstance viewport);
  bool                                CreateShaderResourceView(const Descriptor& desc);
  bool                                CreateUnorderedAccessView(const Descriptor& desc);

  E_DISABLE_COPY_AND_ASSSIGNMENT(DX11Texture2D);
};

/*----------------------------------------------------------------------------------------------------------------------
DX11Texture2D types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<DX11Texture2D> DX11Texture2DInstance;
}
}

#endif
