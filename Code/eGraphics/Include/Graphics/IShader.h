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

/** @file IShader.h
This file defines the IShader interface. The IShader is the base interface for all GPU shaders.
*/

#ifndef E3_IPROGRAM_H
#define E3_IPROGRAM_H

#include <FileSystem/Path.h>
#include <Memory/GarbageCollection.h>

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
IShader
----------------------------------------------------------------------------------------------------------------------*/
class IShader
{
public:
  enum Stage
  {
    eStageVertex,
    eStageHull,
    eStageDomain,
    eStageGeometry,
    eStagePixel,
    eStageCompute,
    eStageCount
  };

  struct CompilationDescriptor
  {
    FilePath                filePath;
    String                  compileOptions;
    String                  entryPoint;
  };

  struct Descriptor
  {
    CompilationDescriptor             stages[IShader::eStageCount];
    Containers::DynamicArray<String>  macroList;
    U8                                modelVersion;

    Descriptor() 
      : modelVersion(0) {}
  };

  virtual                   ~IShader() {}
  virtual const Descriptor& GetDescriptor() const = 0;
};
    
/*----------------------------------------------------------------------------------------------------------------------
IShader types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<IShader> IShaderInstance;
}
}

#endif