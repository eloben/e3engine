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

/** @file DX11RasterState.h
This file declares the DX11RasterState class.
*/

#ifndef E3_DX11_RASTER_STATE_H
#define E3_DX11_RASTER_STATE_H

// Forward declarations
struct ID3D10Blob;
struct ID3D11RasterizerState;

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11RasterState
----------------------------------------------------------------------------------------------------------------------*/
class DX11RasterState : public IRasterState
{
public:
  DX11RasterState();
  ~DX11RasterState();

  bool                    Initialize(const Descriptor& desc);
  void                    Finalize();

  const Descriptor&       GetDescriptor() const;
  ID3D11RasterizerState*  GetDXRasterState() const;

private:
  Descriptor              mDescriptor;
  ID3D11RasterizerState*  mpDXRasterState;
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(DX11RasterState)
};

/*----------------------------------------------------------------------------------------------------------------------
DX11RasterState types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<DX11RasterState> DX11RasterStateInstance;
}
}

#endif
