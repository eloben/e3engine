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

/** @file File.cpp
This file defines file system utility functions to work with files and directories.
*/

#include <CorePch.h>
#ifdef WIN32
#include "Win32/FileImpl.h"
#endif

namespace E
{
namespace FileSystem
{
/*----------------------------------------------------------------------------------------------------------------------
File methods
----------------------------------------------------------------------------------------------------------------------*/

U32 File::GetFlags(const Path& path)
{
  return Impl::GetFlags(path);
}

bool File::GetInfo(const Path& path, File::Info& fileInfo)
{
  return Impl::GetInfo(path, fileInfo);
}

String File::GetName(const Path& path)
{
  return Impl::GetName(path);
}

bool File::Create(const Path& path)
{
  return Impl::Create(path);
}

bool File::Destroy(const Path& path)
{
  return Impl::Destroy(path);
}

bool File::Exists(const Path& path)
{
  return Impl::Exists(path);
}

/*----------------------------------------------------------------------------------------------------------------------
Directory methods
----------------------------------------------------------------------------------------------------------------------*/

bool Directory::IsEmpty(const Path& path)
{
  return Impl::IsEmpty(path);
}

Path Directory::GetBase()
{
  return Impl::GetBase();
}

bool Directory::GetChildren(const Path& path, PathList& directoryList, U8 fileFlags)
{
  return Impl::GetChildren(path, directoryList, fileFlags);
}

Path Directory::GetParent(const Path& path)
{
  return Impl::GetParent(path);
}

bool Directory::Create(const Path& path)
{
  return Impl::Create(path);
}

bool Directory::Destroy(const Path& path)
{
  return Impl::Destroy(path);
}
}
}
