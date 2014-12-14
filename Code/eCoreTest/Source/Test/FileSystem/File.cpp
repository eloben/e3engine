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
// 
// $Revision: $
// $E::File::Date: $
// $Author: $

/** @file File.cpp
This file defines File test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

inline E::String ToString(const E::Time::Date& source)
{
  static const char* kWeekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

  E::String str;
  str.Print("%s %02d/%02d/%04d %02d:%02d:%02d",  kWeekDays[source.weekDay], source.day, source.month, source.year,
    source.hour, source.minute, source.second);
  return str;
}

inline void PrintFileInfo(const E::FileInfo& fileInfo)
{
  std::cout << "File info:" << std::endl;
  std::cout << "\tSize: " << fileInfo.byteSize / 1024 << " KB (" <<  fileInfo.byteSize << " bytes)" << std::endl;
  std::cout << "\tCreation time: " << ToString(fileInfo.creationTime).GetPtr() << std::endl;
  std::cout << "\tLast access time: " << ToString(fileInfo.lastAccessTime).GetPtr() << std::endl;
  std::cout << "\tLast write time: " << ToString(fileInfo.lastWriteTime).GetPtr() << std::endl;
}

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::File::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::File::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::File::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}


bool Test::File::RunFunctionalityTest()
{
  std::cout << "[Test::File::RunFunctionalityTest]" << std::endl;
  
  FilePath fakePath("SomeFakePath\\SomeFakeFolder");
  std::cout << fakePath.GetPtr() << " exists: " << FileSystem::File::Exists(fakePath) << std::endl;

  FilePath currentDirectory = FileSystem::Directory::GetBase();
  std::cout << "Current directory: " << currentDirectory.GetPtr() << " exists: " << FileSystem::File::Exists(currentDirectory) << std::endl;

  while (currentDirectory.GetLength() > 0)
  {
    currentDirectory = FileSystem::Directory::GetParent(currentDirectory);
    std::cout << "Parent directory: " << FileSystem::File::GetName(currentDirectory).GetPtr() << " exists: " << FileSystem::File::Exists(currentDirectory) << std::endl;
  }

  std::cout << "Non-existant file info: " << std::endl;
  FileSystem::File::Info fileInfo;
  FileSystem::File::GetInfo(fakePath, fileInfo);
  PrintFileInfo(fileInfo);

  std::cout << "Base directory file info: " << std::endl;
  if (FileSystem::File::GetInfo(FileSystem::Directory::GetBase(), fileInfo)) PrintFileInfo(fileInfo);

  std::cout << "Base directory children files: " << std::endl;
  FilePathList fileList;
  FileSystem::Directory::GetChildren(FileSystem::Directory::GetBase(), fileList, FileSystem::File::eFlagAny);
  for (auto it = begin(fileList); it != end(fileList); ++it)
  {
    FilePath childFile = *it;
    std::cout << childFile.GetPtr() << std::endl;
    if (FileSystem::File::GetInfo(childFile, fileInfo)) PrintFileInfo(fileInfo);
  }

  currentDirectory = FileSystem::Directory::GetParent(FileSystem::Directory::GetBase());
  E_ASSERT(!FileSystem::Directory::IsEmpty(currentDirectory));

  // Create / Destroy test
  currentDirectory = FileSystem::Directory::GetBase();
  currentDirectory += "\\Foo";
  E_ASSERT(FileSystem::Directory::Create(currentDirectory));
  E_ASSERT(!FileSystem::Directory::Create(currentDirectory));
  E_ASSERT(FileSystem::Directory::Destroy(currentDirectory));
  E_ASSERT(!FileSystem::Directory::Destroy(currentDirectory));

  // Create / Destroy test with path length > MAX_PATH
  FilePath longDirectoryPath = "\\\\?\\";
  longDirectoryPath += currentDirectory;
  longDirectoryPath += "\\OmgHowLongCanAFolderNameBe\\IDontKnowButIthinkItCanGoPrettyLong\\NoWonderWhyIAmGoingToExceed256CharacterLength\\OhReallyIAmSoSurprised\\LetsUseNumbersPlease\\OkHowAboutThatMyFriend0123456789\\YesThatsWhatIMean0123456789";
  std::cout << "Create [" << longDirectoryPath.GetPtr() << "]" << std::endl;
  E_ASSERT(FileSystem::Directory::Create(longDirectoryPath));
  
  // Delete recursively test
  //E_ASSERT(!FileSystem::Directory::Destroy(currentDirectory)); // Uncomment to assert not empty
  currentDirectory = longDirectoryPath;
  FilePath longBasePath = "\\\\?\\";
  longBasePath += FileSystem::Directory::GetBase();
  while (currentDirectory != longBasePath)
  {
    E_ASSERT(FileSystem::Directory::Destroy(currentDirectory));
    currentDirectory = FileSystem::Directory::GetParent(currentDirectory);
  }
  
  // Create / Destroy file test
  FilePath testFile = currentDirectory;
  testFile += "\\testFile.txt";
  E_ASSERT(FileSystem::File::Create(testFile));
  E_ASSERT(FileSystem::File::Destroy(testFile));

  return true;
}

bool Test::File::RunPerformanceTest()
{
  std::cout << "[Test::File::RunPerformanceTest]" << std::endl;

  return true;
}

