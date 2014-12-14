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

// Created 29-Jan-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file E::StringBuffer.cpp
This file defines E::StringBuffer test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace std;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary declarations
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::StringBuffer::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::StringBuffer::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::StringBuffer::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

//#include "Base.h"

enum Bubba
{
  eMyEnumValue,
};

bool Test::StringBuffer::RunFunctionalityTest()
{
  std::cout << "[Test::StringBuffer::RunFunctionalityTest]" << std::endl;

  /*
  CharList();
  CharList(const char* str); // Non-explicit to be used by friend operators
  explicit CharList(U32 size);
  ~CharList();
  */
  E::StringBuffer strBuffer;
  E::StringBuffer preFilledStrBuffer("An awesome string buffer");
  E::StringBuffer preSizedStrBuffer(256);
  
  /*
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator==(const CharList<CharType, AllocatorClass>& a, const CharList<CharType, AllocatorClass>& b);
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator==(const CharList<CharType, AllocatorClass>& a, const char* b);
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator==(const char* a, const CharList<CharType, AllocatorClass> &b);

  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator!=(const CharList<CharType, AllocatorClass>& a, const CharList<CharType, AllocatorClass>& b);
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator!=(const CharList<CharType, AllocatorClass>& a, const char* b);
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator!=(const char* a, const CharList<CharType, AllocatorClass> &b);
  */
  
  E::StringBuffer copyStrBuffer(preFilledStrBuffer);
  E::StringBuffer copyStrBufferString(preFilledStrBuffer.GetPtr(), preFilledStrBuffer.GetLength());
  E_ASSERT(copyStrBuffer == preFilledStrBuffer);
  E_ASSERT(copyStrBuffer == copyStrBufferString);
  E_ASSERT(copyStrBufferString == copyStrBuffer);
  E_ASSERT(copyStrBuffer == preFilledStrBuffer);
  E_ASSERT(copyStrBuffer != preSizedStrBuffer);
  E_ASSERT(copyStrBuffer == "An awesome string buffer");
  E_ASSERT("An awesome string buffer" == preFilledStrBuffer);
  E_ASSERT(preSizedStrBuffer != "An awesome string buffer");
  E_ASSERT("An awesome string buffer" != preSizedStrBuffer);

  /*
  CharType                      operator[](U32 index) const;
  CharType&                     operator[](U32 index);
  */
  E_ASSERT(preFilledStrBuffer[3] == 'a');
  preFilledStrBuffer[3] = 'A';
  E_ASSERT(preFilledStrBuffer[3] == 'A');

  /*
  CharList<CharType, AllocatorClass>& operator<<(const CharList<CharType, AllocatorClass>& str);
  CharList<CharType, AllocatorClass>& operator<<(const CharType* pStr);
  CharList<CharType, AllocatorClass>& operator<<(bool b);
  CharList<CharType, AllocatorClass>& operator<<(CharType c);
  CharList<CharType, AllocatorClass>& operator<<(I32 v);
  CharList<CharType, AllocatorClass>& operator<<(U32 v);
  CharList<CharType, AllocatorClass>& operator<<(I64 v);
  CharList<CharType, AllocatorClass>& operator<<(U64 v);
  CharList<CharType, AllocatorClass>& operator<<(F32 v);
  CharList<CharType, AllocatorClass>& operator<<(D64 v);
  */
  
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << ' ';
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << E::StringBuffer("Another brilliant string buffer");
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << ' ';
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << "This buffer is empty: ";
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << (strBuffer.GetLength() == 0);
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << ' ';
  strBuffer << 1132430350;
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << ' ';
  strBuffer << 1132430350.345345f;
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << ' ';
  strBuffer << 1132430350U;
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << ' ';
  strBuffer << 1132430350L;
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << ' ';
  strBuffer << 1132430350.2344554663;
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  strBuffer << E::StringBuffer("A E::StringBuffer");
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;
  //strBuffer.SetLength(1024); // trigger assert buffer needs to be allocated first
  strBuffer.SetLength(32);
  std::cout << "Shortened buffer [" << strBuffer.GetPtr() << "]" << std::endl;

  /*
  void                      Append(const CharType* pStr);
  void                      Append(const CharType* pStr, U32 length);
  void                      Clear();
  void                      Compact();
  */
  E_ASSERT(strBuffer.GetSize() != strBuffer.GetLength());
  strBuffer.Compact();
  E_ASSERT(strBuffer.GetSize() == strBuffer.GetLength());
  strBuffer.Clear();
  E_ASSERT(strBuffer.GetLength() == 0);
  strBuffer.Compact();
  E_ASSERT(strBuffer.GetSize() == 0);
  E::StringBuffer str("A splendid string");
  strBuffer.Append(str.GetPtr(), str.GetLength());
  std::cout << "[" << strBuffer.GetPtr() << "]" << std::endl;

  /*-------------------------------------------------------------------------------
  Wchar
  -------------------------------------------------------------------------------*/
  
  /*
  CharList();
  CharList(const char* str); // Non-explicit to be used by friend operators
  explicit CharList(U32 size);
  ~CharList();
  */
  E::WStringBuffer wstrBuffer;
  E::WStringBuffer preFilledWstrBuffer(L"An awesome string buffer");
  E::WStringBuffer preSizedWstrBuffer(256);
  
  /*
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator==(const CharList<CharType, AllocatorClass>& a, const CharList<CharType, AllocatorClass>& b);
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator==(const CharList<CharType, AllocatorClass>& a, const char* b);
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator==(const char* a, const CharList<CharType, AllocatorClass> &b);

  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator!=(const CharList<CharType, AllocatorClass>& a, const CharList<CharType, AllocatorClass>& b);
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator!=(const CharList<CharType, AllocatorClass>& a, const char* b);
  template <typename CharType, template<typename> class AllocatorClass>
  friend bool	operator!=(const char* a, const CharList<CharType, AllocatorClass> &b);
  */
  
  E::WStringBuffer copyWstrBuffer(preFilledWstrBuffer);
  E::WStringBuffer copyWstrBufferString(preFilledWstrBuffer.GetPtr(), preFilledWstrBuffer.GetLength());
  E_ASSERT(copyWstrBuffer == preFilledWstrBuffer);
  E_ASSERT(copyWstrBuffer == copyWstrBufferString);
  E_ASSERT(copyWstrBufferString == copyWstrBuffer);
  E_ASSERT(copyWstrBuffer != preSizedWstrBuffer);
  E_ASSERT(copyWstrBuffer == L"An awesome string buffer");
  E_ASSERT(L"An awesome string buffer" == preFilledWstrBuffer);
  E_ASSERT(preSizedWstrBuffer != L"An awesome string buffer");
  E_ASSERT(L"An awesome string buffer" != preSizedWstrBuffer);

  /*
  CharType                      operator[](U32 index) const;
  CharType&                     operator[](U32 index);
  */
  E_ASSERT(preFilledWstrBuffer[3] == 'a');
  preFilledWstrBuffer[3] = 'A';
  E_ASSERT(preFilledWstrBuffer[3] == 'A');

  /*
  CharList<CharType, AllocatorClass>& operator<<(const CharList<CharType, AllocatorClass>& str);
  CharList<CharType, AllocatorClass>& operator<<(const CharType* pStr);
  CharList<CharType, AllocatorClass>& operator<<(bool b);
  CharList<CharType, AllocatorClass>& operator<<(CharType c);
  CharList<CharType, AllocatorClass>& operator<<(I32 v);
  CharList<CharType, AllocatorClass>& operator<<(U32 v);
  CharList<CharType, AllocatorClass>& operator<<(I64 v);
  CharList<CharType, AllocatorClass>& operator<<(U64 v);
  CharList<CharType, AllocatorClass>& operator<<(F32 v);
  CharList<CharType, AllocatorClass>& operator<<(D64 v);
  */
  
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << L' ';
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << E::WStringBuffer(L"Another brilliant string buffer");
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << L' ';
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << L"This buffer is empty: ";
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << (wstrBuffer.GetLength() == 0);
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << L' ';
  wstrBuffer << 1132430350;
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << L' ';
  wstrBuffer << 1132430350.345345f;
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << L' ';
  wstrBuffer << 1132430350U;
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << L' ';
  wstrBuffer << 1132430350L;
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << L' ';
  wstrBuffer << 1132430350.2344554663;
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer << WStringBuffer(L"A Wstring");
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;
  wstrBuffer.SetLength(32);
  std::wcout << "Shortened buffer [" << wstrBuffer.GetPtr() << "]" << std::endl;
  /*
  void                      Append(const CharType* pStr);
  void                      Append(const CharType* pStr, U32 length);
  void                      Clear();
  void                      Compact();
  */
  E_ASSERT(wstrBuffer.GetSize() != wstrBuffer.GetLength());
  wstrBuffer.Compact();
  E_ASSERT(wstrBuffer.GetSize() == wstrBuffer.GetLength());
  wstrBuffer.Clear();
  E_ASSERT(wstrBuffer.GetLength() ==0);
  wstrBuffer.Compact();
  E_ASSERT(wstrBuffer.GetSize() ==0);
  E::WStringBuffer wstr(L"A splendid string");
  wstrBuffer.Append(wstr.GetPtr(), wstr.GetLength());
  std::wcout << L"[" << wstrBuffer.GetPtr() << L"]" << std::endl;

  // UTF-8 conversion test;
  E::StringBuffer utf8String("An amazing string buffer in UTF-8. Let's do some conversion test!!");
  E::WStringBuffer wideString;
  std::cout << "[UTF8] " << utf8String.GetPtr() << std::endl;
  E::Text::Utf8ToWide(wideString, utf8String);
  std::wcout << "[Wide] " << wideString.GetPtr() << std::endl;
  E::StringBuffer utf8StringBack;
  E::Text::WideToUtf8(utf8StringBack, wideString);
  std::cout << "[UTF8] " << utf8StringBack.GetPtr() << std::endl;
  E_ASSERT(utf8String == utf8StringBack);
  E::WStringBuffer wideStringBack;
  E::Text::Utf8ToWide(wideStringBack, utf8StringBack);
  E_ASSERT(wideString == wideStringBack);

  return true;
}

bool Test::StringBuffer::RunPerformanceTest()
{
  std::cout << "[Test::StringBuffer::RunPerformanceTest]" << std::endl;

  return true;
}
