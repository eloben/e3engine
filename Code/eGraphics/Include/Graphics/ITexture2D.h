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

/** @file ITexture2D.h
This file defines the ITexture2D interface.
*/

#ifndef E3_ITEXTURE_2D_H
#define E3_ITEXTURE_2D_H

#include <Graphics/IResource.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
ITexture2D

Please note that this interface has the following usage contract:

- String initializer implies a file const ITexture2DInstance& texture2D type initialization (eTexture2DFile).
----------------------------------------------------------------------------------------------------------------------*/
class ITexture2D : public IResource
{
public:
  enum Type
  {
    eTypeFile,
    eTypeColorTarget,
    eTypeDepthTarget
  };

  enum Format 
  { 
    // Color formats
    eFormatRGBA8,
    eFormatBGRA8,
    eFormatDXT1,
    eFormatDXT3,
    eFormatDXT5,
    eFormatR32,
    eFormatRGB32,
    eFormatRGBA16, 
    eFormatRGBA32,
    // Color sRGB formats
    eFormatRGBA8sRGB,
    eFormatBGRA8sRGB,
    eFormatDXT1sRGB,
    eFormatDXT3sRGB,
    eFormatDXT5sRGB,
    // Depth formats
    eFormatDepth16,
    eFormatDepth24S8, 
    eFormatDepth32,
    eFormatDepth32S8,

    eFormatCount
  }; 

  struct Descriptor 
  { 
    Type    type; 
    Format  format; 
    U32     width; 
    U32     height; 
    U32     mipLevelCount;
    U32     unitCount;
    U8      accessFlags;

    Descriptor::Descriptor() 
      : type(eTypeFile) 
      , format(eFormatCount) 
      , width(0) 
      , height(0) 
      , mipLevelCount(0)
      , unitCount(0)
      , accessFlags(eAccessFlagNone) {} 
  }; 

  virtual const Descriptor& GetDescriptor() const = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ITexture2D types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<ITexture2D> ITexture2DInstance;
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Graphics::ITexture2D::Descriptor)

#endif
