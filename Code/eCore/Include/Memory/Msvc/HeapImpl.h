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

// Created 13-Sep-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file HeapImpl.h
This file declares the Windows version of the Memory heap utility methods.
*/

#ifndef E3_HEAP_IMPL_H
#define E3_HEAP_IMPL_H

#include <Math/Comparison.h>
#include <Assertion/Assert.h>

#include <cstdlib>
#include <cstring>

namespace E 
{
/*----------------------------------------------------------------------------------------------------------------------
Memory::Heap::Impl
----------------------------------------------------------------------------------------------------------------------*/
namespace Memory
{
namespace Heap
{
namespace Impl
{
void* AllocateAligned(size_t alignment, size_t size);
void	DeallocateAligned(void* p);
}
}
}

/*----------------------------------------------------------------------------------------------------------------------
Memory::Heap::Impl methods
----------------------------------------------------------------------------------------------------------------------*/
void* Memory::Heap::Impl::AllocateAligned(size_t alignment, size_t size)
{
  E_ASSERT_MSG(Math::IsPower2(alignment), E_ASSERT_MSG_MATH_POWER_OF_TWO_VALUE);
  if (!size) return nullptr;
  // Compute padded size
  const size_t alignmentMask = alignment - 1;
  size_t paddedSize = (size + alignmentMask) & ~alignmentMask;
  return _aligned_malloc(paddedSize, alignment);
}

void Memory::Heap::Impl::DeallocateAligned(void* p)
{
  if (p != nullptr )
    _aligned_free(p);
}
}

#endif
