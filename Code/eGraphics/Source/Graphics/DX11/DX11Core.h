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

/** @file DX11Core.h
This file contains the DX11Core class. This class is a singleton and it is intended to be used as part of the DX11Device
class (friend class). DX11Core allows a global access point to the DX11 device and device context for all DX11 classes 
requiring them. DX11Core accessors will throw an exception if the target resource is null.
*/

#ifndef E3_DX11_CORE_H
#define E3_DX11_CORE_H

#include "DX11Base.h"

/*----------------------------------------------------------------------------------------------------------------------
DX11 assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_DX11_RENDER_DEVICE_NULL          " render device cannot be null"
#define E_ASSERT_MSG_DX11_RENDER_DEVICE_CONTEXT_NULL  " render device context cannot be null"

/*----------------------------------------------------------------------------------------------------------------------
DX11 macros
----------------------------------------------------------------------------------------------------------------------*/
#define GDXDevice         GDX11Core::GetInstance().GetDXDevice()
#define GDXDeviceContext  GDX11Core::GetInstance().GetDXDeviceContext()

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11Core
----------------------------------------------------------------------------------------------------------------------*/
class DX11Core
{
public:
  // Accessors
  ID3D11Device*	GetDXDevice() const
  {  
    E_ASSERT_MSG(mpDXDevice, E_ASSERT_MSG_DX11_RENDER_DEVICE_NULL); 
    return mpDXDevice;
  }

  ID3D11DeviceContext* GetDXDeviceContext() const
  { 
    E_ASSERT_MSG(mpDXDevice, E_ASSERT_MSG_DX11_RENDER_DEVICE_CONTEXT_NULL); 
    return mpDXDeviceContext;
  }

  Containers::List<D3D11_VIEWPORT>& GetDXViewportList()
  {
    return mDXViewportList;
  }

private:
  Containers::List<D3D11_VIEWPORT>  mDXViewportList;
  ID3D11Device*						          mpDXDevice;
  ID3D11DeviceContext*              mpDXDeviceContext;

  friend class DX11Device;

  E_DECLARE_SINGLETON_ONLY(DX11Core)
};

typedef Singleton<DX11Core> GDX11Core;

/*----------------------------------------------------------------------------------------------------------------------
DX11Core private initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

inline DX11Core::DX11Core()
  : mpDXDevice(nullptr)
  , mpDXDeviceContext(nullptr) {}

inline DX11Core::~DX11Core() {}
}
}

#endif
