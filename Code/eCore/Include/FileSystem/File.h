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

// Created 30-Aug-2014 by Elías Lozada-Benavente
// Based on original created 12-Feb-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file File.h
This file declares file system utility functions to work with files and directories.
*/

#ifndef E3_FILE_H
#define E3_FILE_H

#include "Path.h"
#include <Containers/List.h>
#include <Time/Time.h>

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
FileSystem
----------------------------------------------------------------------------------------------------------------------*/
namespace FileSystem
{
typedef Containers::List<Path> PathList;

/*----------------------------------------------------------------------------------------------------------------------
File

Please note that this namespace have the following usage contract:

1. File uses Path assuming UTF-8 encoding.
2. Maximum path length is defined by E_INTERNAL_SETTING_PATH_SIZE. However, there may be platform specific 
limitations in that regard. Refer to the implementation class contract.
----------------------------------------------------------------------------------------------------------------------*/
namespace File
{
enum Flags
{
  eFlagFile       = 1,
  eFlagDirectory  = 2,
  eFlagReadOnly   = 4,
  eFlagHidden     = 8,
  eFlagAny        = eFlagFile | eFlagDirectory
};

/*----------------------------------------------------------------------------------------------------------------------
Info
----------------------------------------------------------------------------------------------------------------------*/
struct Info
{
  Time::Date  creationTime;
  Time::Date  lastAccessTime;
  Time::Date  lastWriteTime;
  size_t      byteSize;
  U8          flags;

  Info() : byteSize(0) {}
};

E_API U32     GetFlags(const Path& path);
E_API bool    GetInfo(const Path& path, Info& fileInfo);
E_API String	GetName(const Path& path);
E_API bool    Create(const Path& path);
E_API bool		Destroy(const Path& path);
E_API bool	  Exists(const Path& path);
}

/*----------------------------------------------------------------------------------------------------------------------
Directory

Please note that this namespace have the following usage contract:

1. Directory uses Path assuming UTF-8 encoding.
----------------------------------------------------------------------------------------------------------------------*/
namespace Directory
{
E_API bool 		IsEmpty(const Path& path);
E_API Path    GetBase();
E_API bool 	  GetChildren(const Path& path, PathList& directoryList, U8 fileFlags = File::eFlagAny);
E_API Path    GetParent(const Path& path);
E_API bool    Create(const Path& path);
E_API bool    Destroy(const Path& path);
}

}
/*----------------------------------------------------------------------------------------------------------------------
File types
----------------------------------------------------------------------------------------------------------------------*/
typedef Containers::List<FileSystem::Path> FilePathList;
typedef FileSystem::File::Info FileInfo;
typedef FileSystem::File::Flags FileFlags;
}

#endif
