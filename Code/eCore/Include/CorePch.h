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

// Created 20-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file CorePch.h
This file defines includes all the headers required by the Utilities library for pre compilation. 
*/

#ifndef E3_UTILITIES_PCH_H
#define E3_UTILITIES_PCH_H

/*----------------------------------------------------------------------------------------------------------------------
[Utilities]
----------------------------------------------------------------------------------------------------------------------*/
#include <Base.h>
#include <Application/Application.h>
#include <Application/InputManager.h>
#include <Assertion/Assert.h>
#include <Assertion/Exception.h>
#include <Containers/Stack.h>
#include <FileSystem/Archive.h>
#include <FileSystem/File.h>
#include <Math/Random.h>
#include <Text/String.h>
#include <Threads/ThreadPool.h>
#include <Time/Time.h>
#include <Serialization/XmlSerializer.h>
#include <Singleton.h>
#include <Win32/ComUtil.h>

/*----------------------------------------------------------------------------------------------------------------------
[Thirdparty]
----------------------------------------------------------------------------------------------------------------------*/
#ifdef E_DEBUG
  #include <vld.h>
#endif

#endif