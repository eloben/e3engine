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

// Created 25-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file VertexPrimitive.h
This file defines the VertexPrimitive enum type.
*/

#ifndef E3_VERTEX_PRIMITIVE_H
#define E3_VERTEX_PRIMITIVE_H

namespace E
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
VertexPrimitive
----------------------------------------------------------------------------------------------------------------------*/
enum VertexPrimitive
{
  eVertexPrimitiveUndefined,
  eVertexPrimitivePointList,		  // Treats each vertex as a single point.
  eVertexPrimitiveLineList,		    // Treats each pair of vertices as an independent line segment.
  eVertexPrimitiveLineStrip,		  // Draws a connected group of line segments from the first vertex to the last.
  eVertexPrimitiveTriangleList,	  // Treats each triplet of vertices as an independent triangle.
  eVertexPrimitiveTriangleStrip,	// Draws a connected group of triangles. One triangle is defined for each vertex presented after the first two vertices.
  eVertexPrimitiveCount
};
}
}

#endif
