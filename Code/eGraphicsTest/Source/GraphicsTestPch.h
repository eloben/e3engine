/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 21-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file GraphicsTestPch.h
This file defines includes all the headers required by the Gpu test application for pre compilation. 
*/

#ifndef E3_GPU_TEST_PCH_H
#define E3_GPU_TEST_PCH_H

/*----------------------------------------------------------------------------------------------------------------------
[Utilities]
----------------------------------------------------------------------------------------------------------------------*/

#define E_SETTING_CUSTOM_EXCEPTION_TYPES \
  E_EXCEPTION_TYPE(GraphicsTest)

#include <Base.h>
#include <Application/Application.h>
#include <Assertion/Exception.h>
#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <Math/Projection.h>

/*----------------------------------------------------------------------------------------------------------------------
[Gpu]
----------------------------------------------------------------------------------------------------------------------*/
#include <Graphics/Device.h>

/*----------------------------------------------------------------------------------------------------------------------
[Thirdparty]
----------------------------------------------------------------------------------------------------------------------*/
#ifdef E_DEBUG
#include <vld.h>
#endif

/*----------------------------------------------------------------------------------------------------------------------
[GpuTest]
----------------------------------------------------------------------------------------------------------------------*/
#include "SimpleVertexUpdater.h"
#include "TextureVertexUpdater.h"
#include "InstanceVertexUpdater.h"
#include "IndexedVertexUpdater.h"
#include "IndexedInstanceVertexUpdater.h"
#include "RenderToTextureUpdater.h"
#include "RenderDepthToTextureUpdater.h"

#endif