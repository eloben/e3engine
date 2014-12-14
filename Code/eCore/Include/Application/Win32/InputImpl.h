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

// Created 10-Nov-2014 by Elías Lozada-Benavente
//
// $Revision: $
// $Date: $
// $Author: $

/** @file InputImpl.h
This file defines the Window implementation of the library application input types.
*/

#ifndef E3_INPUT_IMPL_H
#define E3_INPUT_IMPL_H

#include <Base.h>

namespace E
{
namespace Application
{
namespace Input
{
enum Key
{
  // ANSII
  eKey0 = 0x30,
  eKey1 = 0x31,
  eKey2 = 0x32,
  eKey3 = 0x33,
  eKey4 = 0x34,
  eKey5 = 0x35,
  eKey6 = 0x36,
  eKey7 = 0x37,
  eKey8 = 0x38,
  eKey9 = 0x39,
  eKeyA = 0x41,
  eKeyB = 0x42,
  eKeyC = 0x43,
  eKeyD = 0x44,
  eKeyE = 0x45,
  eKeyF = 0x46,
  eKeyG = 0x47,
  eKeyH = 0x48,
  eKeyI = 0x49,
  eKeyJ = 0x4A,
  eKeyK = 0x4B,
  eKeyL = 0x4C,
  eKeyM = 0x4D,
  eKeyN = 0x4E,
  eKeyO = 0x4F,
  eKeyP = 0x50,
  eKeyQ = 0x51,
  eKeyR = 0x52,
  eKeyS = 0x53,
  eKeyT = 0x54,
  eKeyU = 0x55,
  eKeyV = 0x56,
  eKeyW = 0x57,
  eKeyX = 0x58,
  eKeyY = 0x59,
  eKeyZ = 0x5A,
  eKeyF1 = VK_F1,
  eKeyF2 = VK_F2,
  eKeyF3 = VK_F3,
  eKeyF4 = VK_F4,
  eKeyF5 = VK_F5,
  eKeyF6 = VK_F6,
  eKeyF7 = VK_F7,
  eKeyF8 = VK_F8,
  eKeyF9 = VK_F9,
  eKeyF10 = VK_F10,
  eKeyF11 = VK_F11,
  eKeyF12 = VK_F12,
  eKeyEscape = VK_ESCAPE,
  eKeySpace = VK_SPACE,
  eLeftShift = VK_LSHIFT,
  eRightShift = VK_RSHIFT,
  eLeftControl = VK_LCONTROL,
  eRightControl = VK_RCONTROL,
};

enum Mouse
{
  eMouseLeft    = VK_LBUTTON,
  eMouseRight   = VK_RBUTTON,
  eMouseMiddle  = VK_MBUTTON,
};
}
}
}

#endif
