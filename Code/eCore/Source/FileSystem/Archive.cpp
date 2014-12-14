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

// Created 30-Aug-2014 by Elías Lozada-Benavente
// Based on original created 12-Feb-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Archive.cpp
This file defines file system utility functions to work with files and directories.
*/

#include <CorePch.h>
#include <fstream>

namespace E
{
namespace FileSystem
{
/*----------------------------------------------------------------------------------------------------------------------
Archive initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/	

Archive::Archive()
{

}

Archive::~Archive()
{
  Close();
}

/*----------------------------------------------------------------------------------------------------------------------
Archive accessors
----------------------------------------------------------------------------------------------------------------------*/	

bool Archive::IsOpen() const
{
  return mFileStream.is_open();
}

/*----------------------------------------------------------------------------------------------------------------------
Archive methods
----------------------------------------------------------------------------------------------------------------------*/	

void Archive::Close()
{
  mFileStream.close();
  mFilePath.Clear();
}

bool Archive::Open(const Path& filePath, OpenMode openMode /* = eOpenModeRead */)
{
  E_ASSERT(filePath.GetLength());
  if (openMode == eOpenModeRead)
  {
    mFileStream.open(filePath.GetPtr(), std::ios::in);
  }
  else if (openMode == eOpenModeWrite)
  {
    mFileStream.open(filePath.GetPtr(), std::ios::out);
  }
  else
  {
    mFileStream.open(filePath.GetPtr(), std::ios::app);
  }

  return mFileStream.is_open();
}

void Archive::Read(char* pTarget, size_t length)
{
  mFileStream.read(pTarget, length);
}

void Archive::Write(const char* pSource, size_t length)
{
  mFileStream.write(pSource, length);
}
}
}
