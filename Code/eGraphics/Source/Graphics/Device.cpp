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

// Created 18-Jun-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Device.cpp
This file defines the Graphics library device accessor methods.
*/

#include <GraphicsPch.h>
#include "DX11/DX11Device.h"

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DeviceProvider
----------------------------------------------------------------------------------------------------------------------*/
class DeviceProvider
{
public:
  IDevice* GetDevice(IDevice::DeviceType deviceType);

private:
  typedef Memory::GenericFactory<IDevice>               IDeviceFactory;
  typedef Memory::AbstractFactory<IDevice, DX11Device>  DX11DeviceFactory;


  IDevice*            mDevices[IDevice::eDeviceTypeCount];
  IDeviceFactory      mDeviceFactory;
  DX11DeviceFactory   mDX11DeviceFactory;

  E_DECLARE_SINGLETON_ONLY(DeviceProvider);
};

/*----------------------------------------------------------------------------------------------------------------------
Global methods
----------------------------------------------------------------------------------------------------------------------*/

Graphics::IDevice* Graphics::Global::GetDevice(IDevice::DeviceType deviceType)
{
  return Singleton<Graphics::DeviceProvider>::GetInstance().GetDevice(deviceType);
}

/*----------------------------------------------------------------------------------------------------------------------
Graphics::DeviceProvider private initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Graphics::DeviceProvider::DeviceProvider() 
  : mDevices()
{
  E_DEBUG_MSG("Graphics::DeviceProvider allocator: 0x%p", Memory::Global::GetAllocator());
  mDeviceFactory.Register(&mDX11DeviceFactory, IDevice::eDeviceTypeDX11);
}

Graphics::DeviceProvider::~DeviceProvider()
{
  mDeviceFactory.CleanUp();
  mDeviceFactory.Unregister(&mDX11DeviceFactory);
}

/*----------------------------------------------------------------------------------------------------------------------
Graphics::DeviceProvider accessors
----------------------------------------------------------------------------------------------------------------------*/

Graphics::IDevice* Graphics::DeviceProvider::GetDevice(IDevice::DeviceType deviceType /* = Graphics::IDevice::eDeviceTypeDX11 */)
{
  if (mDevices[deviceType] == nullptr) mDevices[deviceType] = mDeviceFactory.Create(deviceType);
  E_ASSERT_PTR(mDevices[deviceType]);
  return mDevices[deviceType];
}
}
}
