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

/** @file DX11Shader.h
This file declares the DX11Shader class.
*/

#ifndef E3_DX11_SHADER_H
#define E3_DX11_SHADER_H

// Forward declarations
struct ID3D10Blob;
struct ID3D11ComputeShader;
struct ID3D11DomainShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11Shader

Please note that this set of macros has the following usage contract:

1. eStageCount is used as the default value of mShaderStage and indicates that the shader is not initialized.
----------------------------------------------------------------------------------------------------------------------*/
class DX11Shader : public IShader
{
public:
  DX11Shader();
  ~DX11Shader();

  bool                  Initialize(const Descriptor& desc);
  void                  Finalize();

  // Accessors
  const Descriptor&     GetDescriptor() const;
  ID3D11ComputeShader*  GetDXComputeShader() const;
  ID3D11DomainShader*   GetDXDomainShader() const;
  ID3D11GeometryShader* GetDXGeometryShader() const;
  ID3D11HullShader*     GetDXHullShader() const;
  ID3D11PixelShader*    GetDXPixelShader() const;
  ID3D11VertexShader*   GetDXVertexShader() const;
  
private:
  Descriptor            mDescriptor;
  ID3D11ComputeShader*  mpDXComputeShader;
  ID3D11DomainShader*   mpDXDomainShader;
  ID3D11GeometryShader* mpDXGeometryShader;
  ID3D11HullShader*     mpDXHullShader;
  ID3D11PixelShader*    mpDXPixelShader;
  ID3D11VertexShader*   mpDXVertexShader;
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(DX11Shader)
};

/*----------------------------------------------------------------------------------------------------------------------
DX11Shader types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<DX11Shader> DX11ShaderInstance;
}
}

#endif
