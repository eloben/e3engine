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

/** @file Heap.h
This file declares utility methods for heap memory allocation. It also includes the method definitions for
non 16 bit aligned memory methods.
*/

#ifndef E3_HEAP_H
#define E3_HEAP_H

#include <Base.h>

#ifdef E_COMPILER_MSVC
#include "Msvc/HeapImpl.h"
#endif

namespace E 
{
namespace Memory
{
/*----------------------------------------------------------------------------------------------------------------------
Heap
----------------------------------------------------------------------------------------------------------------------*/
namespace Heap
{
inline void*  Allocate(size_t size) { return (size) ? malloc(size) : nullptr; }
inline void*  AllocateAligned(size_t alignment, size_t size) { return Impl::AllocateAligned(alignment, size); }
inline void   Deallocate(void* ptr) { free(ptr); }
inline void	  DeallocateAligned(void* ptr) { Impl::DeallocateAligned(ptr); }
}
}
}

#endif
