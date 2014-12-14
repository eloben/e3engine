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

// Created 22-Jul-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IVertexLayout.h
This file defines the IVertexLayout interface, the IVertexLayout::Descriptor class, IVertexLayout::ElementDescriptor class
and the ElementDescriptorList type. The IVertexLayout defines the layout of a the input vertices sent to the 
GPU. It also defines the GpuInputDescriptor class and the GpuInputDescriptorList type.
*/

#ifndef E3_IVERTEX_LAYOUT_H
#define E3_IVERTEX_LAYOUT_H

#include <Containers/DynamicArray.h>
#include <Memory/GarbageCollection.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IVertexLayout
----------------------------------------------------------------------------------------------------------------------*/
class IVertexLayout
{
public:
  struct Element
  {
    enum Type
    {
      eTypePosition,
      eTypeColor,
      eTypeNormal,
      eTypeTexCoord,
      eTypeID
    };

    enum Format
    {
      eFormatFloat,
      eFormatFloat2,
      eFormatFloat3,
      eFormatFloat4,
      eFormatHalf,
      eFormatHalf2,
      eFormatHalf4,
      eFormatUnsigned,
      eFormatUnsignedColor
    };

    Type   type;
    Format format;

    Element(Type type, Format format)
      : type(type)
      , format(format)
    {}
  };
 
  struct Descriptor
  {
    Containers::DynamicArray<Element> elements;
    size_t GetVertexSize() const 
    { 
      size_t vertexSize = 0;
      const size_t kElementSizeTable[] = { 4, 8, 12, 16, 2, 4, 8, 4, 4 };
      for (size_t i = 0; i < elements.GetSize(); ++i) vertexSize += kElementSizeTable[elements[i].format];
      return vertexSize;
    }
  };

  virtual                   ~IVertexLayout() {}
  virtual const Descriptor& GetDescriptor() const = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
IVertexLayout types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IVertexLayout> IVertexLayoutInstance;
}
}

/*----------------------------------------------------------------------------------------------------------------------
POD declarations
----------------------------------------------------------------------------------------------------------------------*/
E_DECLARE_POD(E::Graphics::IVertexLayout::Element)

#endif
