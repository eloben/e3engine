/*--------------------------------------------------------------------------------------------------------------------------------------
This source file is part of the E2 Utilities

Copyright (c) 2010-2012 Elías Lozada-Benavente

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
---------------------------------------------------------------------------------------------------------------------------------------*/

// Created 09-Feb-2010 by Elías Lozada-Benavente
// 
// $Revision: 94 $
// $Date: 2012-07-17 15:46:15 +0100 (Tue, 17 Jul 2012) $
// $Author: Eloben $

/** @file ThreadException.h
This file defines an exception class for Threads.
*/

#ifndef E_THREAD_EXCEPTION_H
#define E_THREAD_EXCEPTION_H

#include "Exception.h"

namespace E
{
	class ThreadException : public Exception
	{
	public:
		explicit ThreadException(const String& description) : Exception("ThreadException: " + description) {}
	};
}

#endif 
