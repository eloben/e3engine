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

// Created 13-Aug-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file SafeCast.h
This file defines the safe cast template method which attempts to do a dynamic cast on debug to validate the the
default static_cast
*/

#ifndef E3_SAFE_CAST_H
#define E3_SAFE_CAST_H

#include <Assertion/Assert.h>

/*----------------------------------------------------------------------------------------------------------------------
SafeCast assertion message
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_SAFE_CAST_INVALID_CONVERSION "Invalid conversion between types"

/*----------------------------------------------------------------------------------------------------------------------
E_CAST
----------------------------------------------------------------------------------------------------------------------*/
#define E_CAST E::SafeCast
  
namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
SafeCast

Please note that this function has the following contract:

1. SafeCast checks dynamic_cast validity requiring the casted type to be polymorphic (at least one virtual function).
----------------------------------------------------------------------------------------------------------------------*/
template <typename T, typename U>
inline T* SafeCast(U* ptr)
{ 
  E_ASSERT_MSG(dynamic_cast<T*>(ptr) != nullptr, E_ASSERT_MSG_SAFE_CAST_INVALID_CONVERSION);
  return static_cast<T*>(ptr);
}
}

#endif
