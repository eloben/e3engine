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

// Created Aug-31-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Conversion.hlsl
This file defines shader type conversion utilities. 
Original implementation by Hawar Doghramachi (2014).
*/

/*----------------------------------------------------------------------------------------------------------------------
EncodeColor

Encodes a float3  color (range 0.0f - 1.0f) into an unsigned int with 8 bits per channel
----------------------------------------------------------------------------------------------------------------------*/
uint EncodeColor(in float3 color)
{	
  uint3 iColor = uint3(color*255.0f);
  uint colorMask = (iColor.r<<16u) | (iColor.g<<8u) | iColor.b;
  return colorMask;
}

// Decode specified mask into a float3 color (range 0.0f-1.0f).
/*----------------------------------------------------------------------------------------------------------------------
DecodeColor

Deodes an unsigned 8 bits per channel color into a float3
----------------------------------------------------------------------------------------------------------------------*/
float3 DecodeColor(in uint colorMask)
{
  float3 color;
  color.r = (colorMask>>24u) & 0x000000ff;
  color.g = (colorMask>>16u) & 0x000000ff;
  color.b = (colorMask>>8u)  & 0x000000ff;
  color /= 255.0f;
  return color;
}
