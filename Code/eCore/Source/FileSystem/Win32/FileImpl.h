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

/** @file FileImpl.h
This file declares the Windows version of the file system utility functions.
*/

#ifndef E3_FILE_IMPL_H
#define E3_FILE_IMPL_H

namespace E
{
namespace FileSystem
{
/*----------------------------------------------------------------------------------------------------------------------
File::Impl

Please note that this namespace have the following usage contract:

1. Windows API defines a maximum path of 260 characters (defined by MAX_PATH). Although NTFS allows paths up to
32k, windows explorer does not support access to them (e.g. execution access to files, moving a deleted folder to
the recycle bin, etc.). However, 260 characters should be more enough to define a directory hierarchy. Take also 
note that the maximum path length can also be limited by E_INTERNAL_SETTING_PATH_SIZE.
----------------------------------------------------------------------------------------------------------------------*/
namespace File
{
namespace Impl
{
  U32       GetFlags(const Path& path);
  bool      GetInfo(const Path& path, Info& fileInfo);
  String	  GetName(const Path& path);
  bool      Create(const Path& path);
  bool      Destroy(const Path& path);
  bool		  Exists(const Path& path);
}
}

/*----------------------------------------------------------------------------------------------------------------------
Directory::Impl
----------------------------------------------------------------------------------------------------------------------*/
namespace Directory
{
namespace Impl
{
  bool 			IsEmpty(const Path& path);
  Path      GetBase();
  bool 	    GetChildren(const Path& path, PathList& directoryList, U8 fileFlags);
  Path      GetParent(const Path& path);
  bool      Create(const Path& path);
  bool      Destroy(const Path& path);
}
}
}
}

#endif
