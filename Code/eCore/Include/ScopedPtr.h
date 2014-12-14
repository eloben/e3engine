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

// Created 07-Feb-2014 by Elías Lozada-Benavente
// Original created 2012-07-17 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file SmartPtr.h
This file defines an non-intrusive smart pointer which implements a RAII idiom, automatically releasing the contained
pointer when the coped pointer is destroyed.
*/

#ifndef E3_SCOPED_POINTER_H
#define E3_SCOPED_POINTER_H

#include <Base.h>
#include <Memory/Allocator.h>

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
ScopedPtr

Please note that this class has the following usage contract: 

1. GetPtr methods are not included for safety reasons, however if the internal pointer must be externally released the
operator combination &* can still be used.
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class ScopedPtr
{
public:
  explicit ScopedPtr(T* ptr);
  ~ScopedPtr();

  // Operators
  T&            operator*() const;
  T*            operator->() const;
  bool          operator==(const T* ptr) const;    
  bool          operator!=(const T* ptr) const;  

private:
  T*            mpPtr;

  E_DISABLE_COPY_AND_ASSSIGNMENT(ScopedPtr)
};

/*----------------------------------------------------------------------------------------------------------------------
ScopedPtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline ScopedPtr<T>::ScopedPtr(T* ptr)
  : mpPtr(ptr)
{
}

template <class T>
inline ScopedPtr<T>::~ScopedPtr()
{
  E_DELETE(mpPtr);
}

/*----------------------------------------------------------------------------------------------------------------------
ScopedPtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline T& ScopedPtr<T>::operator*() const
{
  E_ASSERT_PTR(mpPtr);
  return *mpPtr;
}

template <class T>
inline T* ScopedPtr<T>::operator->() const
{
  E_ASSERT_PTR(mpPtr);
  return mpPtr;
}

template <class T>
inline bool ScopedPtr<T>::operator==(const T* ptr) const
{
  return mpPtr == ptr;
}

template <class T>
inline bool ScopedPtr<T>::operator!=(const T* ptr) const
{
  return mpPtr != ptr;
}
}

#endif
