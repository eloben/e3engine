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

// Created 20-Feb-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file CoreTestPch.h
This file defines includes all the headers required by the Utilities test application for pre compilation. 
*/

#ifndef E3_UTILITIES_TEST_PCH_H
#define E3_UTILITIES_TEST_PCH_H

/*----------------------------------------------------------------------------------------------------------------------
[Utilities]
----------------------------------------------------------------------------------------------------------------------*/

#define E_SETTING_OVERRIDE_NEW_AND_DELETE

#define E_SETTING_CUSTOM_MEMORY_TAGS \
  E_ALLOCATOR_TAG(TestNew) \
  E_ALLOCATOR_TAG(TestDelete)

#define E_SETTING_CUSTOM_EXCEPTION_TYPES \
  E_EXCEPTION_TYPE(Custom)

#include <IntrusivePtr.h>
#include <SharedPtr.h>
#include <Assertion/Assert.h>
#include <EventSystem/Event.h>
#include <Containers/DynamicArray.h>
#include <Containers/List.h>
#include <Containers/Map.h>
#include <Containers/Queue.h>
#include <Containers/Stack.h>
#include <Containers/Array.h>
#include <FileSystem/File.h>
#include <Math/Random.h>
#include <Math/Hash.h>
#include <Math/Algorithm.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <Math/Quaternion.h>
#include <Memory/Factory.h>
#include <Memory/GarbageCollection.h>
#include <Serialization/ByteSerializer.h>
#include <Serialization/StringSerializer.h>
#include <Serialization/XmlSerializer.h>
#include <Singleton.h>
#include <Text/String.h>
#include <Threads/ThreadPool.h>
#include <Threads/Atomic.h>
#include <Time/Timer.h>
#include <WeakPtr.h>

/*----------------------------------------------------------------------------------------------------------------------
[STL]
----------------------------------------------------------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>

/*----------------------------------------------------------------------------------------------------------------------
[UtilitiesTest]
----------------------------------------------------------------------------------------------------------------------*/

#include "Test/Common.h"
#include "Test/Containers/Array.h"
#include "Test/Containers/DynamicArray.h"
#include "Test/Containers/List.h"
#include "Test/Text/String.h"
#include "Test/Text/StringBuffer.h"
#include "Test/Text/StringBuffer.h"
#include "Test/Serialization/Serialization.h"
#include "Test/Math/Hash.h"
#include "Test/Containers/Map.h"
#include "Test/Containers/Queue.h"
#include "Test/Containers/Stack.h"
#include "Test/FileSystem/File.h"
#include "Test/Time/Time.h"
#include "Test/Math/Vector.h"
#include "Test/Math/Matrix.h"
#include "Test/Math/Quaternion.h"
#include "Test/Memory/Allocator.h"
#include "Test/Memory/Factory.h"
#include "Test/Memory/GarbageCollection.h"
#include "Test/SmartPointers/IntrusivePtr.h"
#include "Test/SmartPointers/SharedPtr.h"
#include "Test/SmartPointers/WeakPtr.h"
#include "Test/Threads/ConditionVariable.h"
#include "Test/Threads/Thread.h"
#include "Test/EventSystem/Event.h"
#include "Test/Math/Algorithm.h"

/*----------------------------------------------------------------------------------------------------------------------
[Thirdparty]
----------------------------------------------------------------------------------------------------------------------*/
#ifdef E_DEBUG
  #include <vld.h>

  #define E_DEBUG_MATRIX4(matrix) \
        { \
  E::StringBuffer sb; \
  sb << E_TO_STR(matrix); \
  for (U32 i = 0; i < 4; ++i) \
      { \
  sb << "\n"; \
  for (U32 j = 0; j < 4; ++j) sb << matrix(i, j) << ", "; \
      } \
  sb << "\n"; \
  E_DEBUG_OUTPUT(sb.GetPtr()); \
        }\

  #define E_DEBUG_VECTOR4(vector) \
        { \
  E::StringBuffer sb; \
  sb << E_TO_STR(vector); \
  sb << " " << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << "\n"; \
  E_DEBUG_OUTPUT(sb.GetPtr()); \
        }
#else
  #define E_DEBUG_MATRIX4
  #define E_DEBUG_VECTOR4
#endif

// Known warning: using D3DX from DX June 2010 SDK which conflicts with Windows SDK
#pragma warning(push)
#pragma warning (disable:4005)
#include <d3dx10math.h>
#pragma warning(pop)

#endif
