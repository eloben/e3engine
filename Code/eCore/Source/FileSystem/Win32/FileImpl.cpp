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

/** @file FileImpl.cpp
This file defines the Windows version of the file system utility functions.
*/

#include <CorePch.h>
#include "FileImpl.h"

/*----------------------------------------------------------------------------------------------------------------------
Array assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_FILESYSTEM_DIRECTORY_BASE_LENGTH "Current directory is unreacheable"
#define E_ASSERT_MSG_FILESYSTEM_DIRECTORY_NOT_EMPTY   "Directory is not empty: %s"
#define E_ASSERT_MSG_FILESYSTEM_FILE_EXISTS           "File already exists: %s"
#define E_ASSERT_MSG_FILESYSTEM_PATH_LENGTH           "Path exceeds maximum length: %s"
#define E_ASSERT_MSG_FILESYSTEM_PATH_ACCESS           "Path access denied: %s"
#define E_ASSERT_MSG_FILESYSTEM_PATH_NOT_DIRECTORY    "Path is not a directory: %s"
#define E_ASSERT_MSG_FILESYSTEM_PATH_NOT_FILE         "Path is a directory: %s"

namespace E
{
namespace FileSystem
{
/*----------------------------------------------------------------------------------------------------------------------
FileSystem auxiliary
----------------------------------------------------------------------------------------------------------------------*/

static const char    kWinSeparatorCharacter = '\\';
static const WString kWinSelectAllTokenWstr(L"\\*.*");
static const WString kWinPathBeginTokenWstr(L"\\\\?\\");

U8            GetFileFlags(U32 winFileAttributes);
void          GetFileDate(const FILETIME& ft, Time::Date& date);
void          GetPath(const WFilePath& wfilePath, Path& path);
void          GetWinPath(const Path& path, WFilePath& wfilePath);

/*----------------------------------------------------------------------------------------------------------------------
File::Impl methods
----------------------------------------------------------------------------------------------------------------------*/

U32 File::Impl::GetFlags(const Path& path)
{
  WFilePath wfilePath;
  GetWinPath(path, wfilePath);
  U32 winFileAttributes = ::GetFileAttributes(wfilePath.GetPtr());
  return (winFileAttributes == INVALID_FILE_ATTRIBUTES) ? 0 : GetFileFlags(winFileAttributes);
}

bool File::Impl::GetInfo(const Path& path, Info& fileInfo)
{
  WIN32_FILE_ATTRIBUTE_DATA fileData;
  WFilePath wfilePath;
  GetWinPath(path, wfilePath);
  if (GetFileAttributesEx(wfilePath.GetPtr(), GetFileExInfoStandard, (void *) &fileData) == 0) return false;

  GetFileDate(fileData.ftCreationTime, fileInfo.creationTime);
  GetFileDate(fileData.ftLastAccessTime, fileInfo.lastAccessTime);
  GetFileDate(fileData.ftLastWriteTime, fileInfo.lastWriteTime);

#ifdef E_CPU_X64
  fileInfo.byteSize = fileData.nFileSizeHigh;
  fileInfo.byteSize = fileInfo.byteSize << 32;
#endif
  fileInfo.byteSize += fileData.nFileSizeLow;
  fileInfo.flags = GetFileFlags(fileData.dwFileAttributes);

  return true;
}

String File::Impl::GetName(const Path& path)
{
  String name;
  size_t index = path.GetLength();
  while (index-- > 0) if (path[index] == kWinSeparatorCharacter) break;
  // Do not include separator
  if (index > 0) name.Append(path.GetPtr() + (++index), path.GetLength() - index);

  return name;
}

bool File::Impl::Create(const Path& path)
{
  WFilePath wfilePath;
  GetWinPath(path, wfilePath);
  HANDLE handle = ::CreateFile(
    wfilePath.GetPtr(),
    FILE_ALL_ACCESS,
    FILE_SHARE_READ,
    nullptr,
    CREATE_NEW,
    FILE_ATTRIBUTE_NORMAL,
    nullptr);

  if (handle == INVALID_HANDLE_VALUE)
  {
    E_ASSERT_MSG(GetLastError() != ERROR_FILE_EXISTS, E_ASSERT_MSG_FILESYSTEM_FILE_EXISTS, path.GetPtr());
    E_ASSERT_MSG(GetLastError() != ERROR_PATH_NOT_FOUND, E_ASSERT_MSG_FILESYSTEM_PATH_LENGTH, path.GetPtr());
    E_ASSERT_MSG(GetLastError() != ERROR_SHARING_VIOLATION && GetLastError() != ERROR_ACCESS_DENIED, E_ASSERT_MSG_FILESYSTEM_PATH_ACCESS, path.GetPtr());
    return false;
  }
  else
  {
    ::CloseHandle(handle);
    return true;
  }
}

bool File::Impl::Destroy(const Path& path)
{
  WFilePath wfilePath;
  GetWinPath(path, wfilePath);
  //E_ASSERT_MSG(!(::GetFileAttributes(wfilePath.GetPtr()) & FILE_ATTRIBUTE_DIRECTORY), E_ASSERT_MSG_FILESYSTEM_PATH_NOT_DIRECTORY, path.GetPtr());
  if (::DeleteFile(wfilePath.GetPtr()) == 0)
  {
    E_ASSERT_MSG(GetLastError() != ERROR_PATH_NOT_FOUND, E_ASSERT_MSG_FILESYSTEM_PATH_LENGTH, path.GetPtr());
    E_ASSERT_MSG(GetLastError() != ERROR_SHARING_VIOLATION && GetLastError() != ERROR_ACCESS_DENIED, E_ASSERT_MSG_FILESYSTEM_PATH_ACCESS, path.GetPtr());
    return false;
  }
  return true;
}

bool File::Impl::Exists(const Path& path)
{
  return GetFlags(path) != 0;
}

/*----------------------------------------------------------------------------------------------------------------------
Directory::Impl methods
----------------------------------------------------------------------------------------------------------------------*/

bool Directory::Impl::IsEmpty(const Path& path)
{
  PathList dummyList;
  return !GetChildren(path, dummyList, File::eFlagAny);
}

Path Directory::Impl::GetBase()
{
  WFilePath wfilePath;
  U32 currentDirectoryPathLength = ::GetCurrentDirectory(MAX_PATH, &wfilePath[0]);
  wfilePath.SetLength(currentDirectoryPathLength);
  E_ASSERT_MSG(currentDirectoryPathLength > 0, E_ASSERT_MSG_FILESYSTEM_DIRECTORY_BASE_LENGTH);
  FilePath currentDirectory;
  Text::WideToUtf8(currentDirectory, wfilePath);
  
  return currentDirectory;
}

bool Directory::Impl::GetChildren(const Path& path, PathList& pathList, U8 fileFlags)
{
  E_ASSERT(path.GetLength() + kWinSelectAllTokenWstr.GetLength() <= E_INTERNAL_SETTING_PATH_SIZE);

  WFilePath wfilePath;
  GetWinPath(path, wfilePath);
  wfilePath += kWinSelectAllTokenWstr;

  WIN32_FIND_DATA fileData;
  HANDLE handle = ::FindFirstFile(wfilePath.GetPtr(), &fileData);
  // Invalid directory
  if (handle == INVALID_HANDLE_VALUE)
  {
    E_ASSERT_MSG(GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND, E_ASSERT_MSG_FILESYSTEM_PATH_ACCESS, path.GetPtr());
    return false;
  }
  // Empty directory
  if (!::FindNextFile(handle, &fileData))
  {
    U32 errorCode = GetLastError();
    if (errorCode != ERROR_NO_MORE_FILES)
    {
      ::FindClose(handle);
      return false;
    }
  }
  // Fill pathList
  FilePath childPath = path;
  childPath += kWinSeparatorCharacter;
  size_t childPathBaseLength = childPath.GetLength();
  while (::FindNextFile(handle, &fileData))
  {
    if (fileFlags & File::eFlagFile && !(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || 
      fileFlags & File::eFlagDirectory && (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      if (fileFlags & File::eFlagReadOnly && !(fileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) || 
        fileFlags & File::eFlagHidden && !(fileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) continue;

      // Create and add child path
      size_t fileNameLength = Text::GetLength(fileData.cFileName);
      Text::WideToUtf8(&childPath[childPathBaseLength], fileData.cFileName, fileNameLength);
      childPath.SetLength(childPathBaseLength + fileNameLength);
      pathList.PushBack(childPath);
      // Reset child path to base length
      childPath.SetLength(childPathBaseLength);
    }
  }
  ::FindClose(handle);

  DWORD errorCode = GetLastError();
  if (errorCode != ERROR_NO_MORE_FILES) return false;

  return true;
}

Path Directory::Impl::GetParent(const Path& path)
{
  Path parentDirectory = path;
  size_t index = parentDirectory.GetLength();
  while (--index > 0) if (parentDirectory[index] == kWinSeparatorCharacter) break;
  parentDirectory.SetLength(index);
  return parentDirectory;
}

bool Directory::Impl::Create(const Path& path)
{
  Containers::Stack<String> folderStack;
  FilePath currentDirectory = path;
  while (!File::Exists(currentDirectory))
  {
    folderStack.Push(File::GetName(currentDirectory));
    currentDirectory = GetParent(currentDirectory);
  }
  // Check for already existing directory
  if (folderStack.IsEmpty()) return false;

  WFilePath wfilePath;
  while (!folderStack.IsEmpty())
  {
    currentDirectory += kWinSeparatorCharacter;
    currentDirectory += folderStack.GetTop();
    GetWinPath(currentDirectory, wfilePath);
    // Return false if directory creation fails at any step
    if (::CreateDirectory(wfilePath.GetPtr(), nullptr) == 0)
    {
      E_ASSERT_MSG(GetLastError() != ERROR_PATH_NOT_FOUND, E_ASSERT_MSG_FILESYSTEM_PATH_LENGTH, currentDirectory.GetPtr());
      E_ASSERT_MSG(GetLastError() != ERROR_SHARING_VIOLATION && GetLastError() != ERROR_ACCESS_DENIED, E_ASSERT_MSG_FILESYSTEM_PATH_ACCESS, currentDirectory.GetPtr());
      return false;
    }
    folderStack.Pop();
  }
  return true;
}

bool Directory::Impl::Destroy(const Path& path)
{
  WFilePath wfilePath;
  GetWinPath(path, wfilePath);
  E_ASSERT_MSG(::GetFileAttributes(wfilePath.GetPtr()) & FILE_ATTRIBUTE_DIRECTORY, E_ASSERT_MSG_FILESYSTEM_PATH_NOT_DIRECTORY, path.GetPtr());
  if (::RemoveDirectory(wfilePath.GetPtr()) == 0)
  {
    E_ASSERT_MSG(GetLastError() != ERROR_PATH_NOT_FOUND, E_ASSERT_MSG_FILESYSTEM_PATH_LENGTH, path.GetPtr());
    E_ASSERT_MSG(GetLastError() != ERROR_SHARING_VIOLATION && GetLastError() != ERROR_ACCESS_DENIED, E_ASSERT_MSG_FILESYSTEM_PATH_ACCESS, path.GetPtr());
    E_ASSERT_MSG(GetLastError() != ERROR_DIR_NOT_EMPTY, E_ASSERT_MSG_FILESYSTEM_DIRECTORY_NOT_EMPTY, path.GetPtr() );
    return false;
  }
  return true;
}
/*
int DeleteDirectory(const std::string &refcstrRootDirectory,  bool              bDeleteSubdirectories = true)
{




  bool            bSubdirectory = false;       // Flag, indicating whether
  // subdirectories have been found
  HANDLE          hFile;                       // Handle to directory
  std::string     strFilePath;                 // Filepath
  std::string     strPattern;                  // Pattern
  WIN32_FIND_DATA FileInformation;             // File information


  strPattern = refcstrRootDirectory + "\\*.*";
  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(FileInformation.cFileName[0] != '.')
      {
        strFilePath.erase();
        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          if(bDeleteSubdirectories)
          {
            // Delete subdirectory
            int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
            if(iRC)
              return iRC;
          }
          else
            bSubdirectory = true;
        }
        else
        {
          // Set file attributes
          if(::SetFileAttributes(strFilePath.c_str(),
            FILE_ATTRIBUTE_NORMAL) == FALSE)
            return ::GetLastError();

          // Delete file
          if(::DeleteFile(strFilePath.c_str()) == FALSE)
            return ::GetLastError();
        }
      }
    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);

    DWORD dwError = ::GetLastError();
    if(dwError != ERROR_NO_MORE_FILES)
      return dwError;
    else
    {
      if(!bSubdirectory)
      {
        // Set directory attributes
        if(::SetFileAttributes(refcstrRootDirectory.c_str(),
          FILE_ATTRIBUTE_NORMAL) == FALSE)
          return ::GetLastError();

        // Delete directory
        if(::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
          return ::GetLastError();
      }
    }
  }

  return 0;
}*/

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary methods
----------------------------------------------------------------------------------------------------------------------*/

U8 GetFileFlags(U32 winFileAttributes)
{
  U8 fileFlags = static_cast<U8>((winFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? File::eFlagDirectory : File::eFlagFile);
  if (winFileAttributes & FILE_ATTRIBUTE_READONLY) fileFlags |= File::eFlagReadOnly;
  if (winFileAttributes & FILE_ATTRIBUTE_HIDDEN) fileFlags |= File::eFlagHidden;

  return fileFlags;
}

void GetFileDate(const FILETIME& ft, Time::Date& date)
{
  FILETIME localFt;
  FileTimeToLocalFileTime(&ft, &localFt);

  SYSTEMTIME st;
  FileTimeToSystemTime(&localFt, &st);

  date.year = st.wYear;
  date.month = st.wMonth;
  date.day = st.wDay;
  date.weekDay = st.wDayOfWeek;
  date.hour = st.wHour;
  date.minute = st.wMinute;
  date.second = st.wSecond;
  date.millisecond = st.wMilliseconds;
}

void GetPath(const WFilePath& wfilePath, Path& path)
{
  path.Clear();
  Text::WideToUtf8(path, wfilePath);
  //Text::WideToUtf8(&path[0], wfilePath.GetPtr() + 4, wfilePath.GetLength() - 4);
}

void GetWinPath(const Path& path, WFilePath& wfilePath)
{
  wfilePath.Clear();
  Text::Utf8ToWide(wfilePath, path);
  /*wfilePath += kWinPathBeginTokenWstr;
  Text::Utf8ToWide(&wfilePath[4], path.GetPtr(), path.GetLength());
  wfilePath.SetLength(path.GetLength() + 4);*/
}
}
}
