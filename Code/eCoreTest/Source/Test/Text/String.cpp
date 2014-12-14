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

/** @file String.cpp
This file defines String test functions.
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

bool Test::String::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::String::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::String::RunPerformanceTest");

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

bool Test::String::RunFunctionalityTest()
{
  std::cout << "[Test::String::RunFunctionalityTest]" << std::endl;
  
  /*
  template<U32 SizeOther>
  CharArray(const CharArray<CharType, SizeOther>& other);
  CharArray(const char* str); // Non-explicit to be used by friend operators
  CharArray(const char* str, U32 length);
  ~CharArray();*/

  E::String str32("A wonderful string");
  std::cout << str32.GetPtr() << std::endl;

  E::String strCopy32(str32.GetPtr());
  std::cout << strCopy32.GetPtr() << std::endl;

  E::String shortenStrCopy32(str32.GetPtr(), 8);
  std::cout << shortenStrCopy32.GetPtr() << std::endl;

  E::String strCopy64(str32);
  std::cout << strCopy64.GetPtr() << std::endl;

  /*
  template <typename CharType, U32 Size>
  friend CharArray<CharType, Size> operator+(const CharArray<CharType, Size>& a, const CharArray<CharType, Size>& b);
  template <typename CharType, U32 Size>
  friend CharArray<CharType, Size> operator+(const CharArray<CharType, Size>& a, const char* b);
  template <typename CharType, U32 Size>
  friend CharArray<CharType, Size> operator+(const char* a, const CharArray<CharType, Size>& b);
  */

  E::String str64;
  str64 = "Copying " + str32;
  std::cout << str64.GetPtr() << std::endl;
  str64 = str64 + " !!";
  std::cout << str64.GetPtr() << std::endl;
  E::String str128 = str64 + strCopy64;
  std::cout << str128.GetPtr() << std::endl;


  /*
  template <typename CharType, U32 SizeA, U32 SizeB>
  friend bool	operator==(const CharArray<CharType, SizeA>& a, const CharArray<CharType, SizeB>& b);
  template <typename CharType, U32 Size>
  friend bool	operator==(const CharArray<CharType, Size>& a, const char* b);
  template <typename CharType, U32 Size>
  friend bool	operator==(const char* a, const CharArray<CharType, Size>& b);

  template <typename CharType, U32 SizeA, U32 SizeB>
  friend bool	operator!=(const CharArray<CharType, SizeA>& a, const CharArray<CharType, SizeB>& b);
  template <typename CharType, U32 Size>
  friend bool	operator!=(const CharArray<CharType, Size>& a, const char* b);
  template <typename CharType, U32 Size>
  friend bool	operator!=(const char* a, const CharArray<CharType, Size> &b);
  */

  std::cout << str32.GetPtr() << std::endl;
  std::cout << shortenStrCopy32.GetPtr() << std::endl;
  E_ASSERT(str32 != shortenStrCopy32);

  std::cout << str32.GetPtr() << std::endl;
  std::cout << strCopy32.GetPtr() << std::endl;
  E_ASSERT(str32 == strCopy32);

  E_ASSERT("A wonderful string" == str32);
  E_ASSERT(str32 == "A wonderful string");
  E_ASSERT(str32 != "A wonderful string ");
  E_ASSERT("A wonderful string " != str32);

  /*
  template <U32 SizeOther>
  CharArray<CharType, Size>&  operator=(const CharArray<CharType, SizeOther>& other); (already tested)
  CharType                    operator[](U32 index) const;
  CharType&                   operator[](U32 index);
  */

  E_ASSERT(str32[2] == 'w');
  str32[2] = 'W';
  E_ASSERT(str32[2] == 'W');

  /*
  U32                         GetLength() const;
  const CharType*             GetPtr() const; (already tested)
  U32                         GetSize() const;
  bool                        IsEqual(const CharType* str) const; (already tested)
  template <U32 SizeOther>
  bool                        IsEqual(const CharArray<CharType, SizeOther>& other) const; (already tested)
  */

  E_ASSERT(str32.GetLength() == 18 && str32.GetLength() <= str32.GetSize());

  /*
  void                        Clear();
  void                        Print(const CharType *fmt, ... );
  */
  str32.Clear();
  E_ASSERT(str32.GetLength() == 0 && str32[0] == 0);
  E_ASSERT(str32 == E::String::Empty());

  str128.Print("How useful is printing %f formatted string of %d characters!", 1.0f, str128.GetSize());
  std::cout << str128.GetPtr() << std::endl;

  str32.Print("A wonderful string");
  str128.Print("A wonderful stri");
  str128 += ('n');
  str128 += ('g');
  E_ASSERT(str128 == str32);
 
  /*-------------------------------------------------------------------------------
  Wchar
  -------------------------------------------------------------------------------*/
  /*
  template<U32 SizeOther>
  CharArray(const CharArray<CharType, SizeOther>& other);
  CharArray(const char* str); // Non-explicit to be used by friend operators
  CharArray(const char* str, U32 length);
  ~CharArray();*/

  E::WString wstr32(L"A wonderful string");
  std::wcout << wstr32.GetPtr() << std::endl;

  E::WString wstrCopy32(wstr32.GetPtr());
  std::wcout << wstrCopy32.GetPtr() << std::endl;

  E::WString shortenWstrCopy32(wstr32.GetPtr(), 8);
  std::wcout << shortenWstrCopy32.GetPtr() << std::endl;

  E::WString wstrCopy64(wstr32);
  std::wcout << wstrCopy64.GetPtr() << std::endl;

  /*
  template <typename CharType, U32 Size>
  friend CharArray<CharType, Size> operator+(const CharArray<CharType, Size>& a, const CharArray<CharType, Size>& b);
  template <typename CharType, U32 Size>
  friend CharArray<CharType, Size> operator+(const CharArray<CharType, Size>& a, const char* b);
  template <typename CharType, U32 Size>
  friend CharArray<CharType, Size> operator+(const char* a, const CharArray<CharType, Size>& b);
  */

  E::WString wstr64;
  wstr64 = L"Copying: " + wstr32;
  std::wcout << wstr64.GetPtr() << std::endl;
  wstr64 = wstr64 + L" !!";
  std::wcout << wstr64.GetPtr() << std::endl;
  E::WString wstr128 = wstr64 + wstrCopy64;
  std::wcout << wstr128.GetPtr() << std::endl;


  /*
  template <typename CharType, U32 SizeA, U32 SizeB>
  friend bool	operator==(const CharArray<CharType, SizeA>& a, const CharArray<CharType, SizeB>& b);
  template <typename CharType, U32 Size>
  friend bool	operator==(const CharArray<CharType, Size>& a, const char* b);
  template <typename CharType, U32 Size>
  friend bool	operator==(const char* a, const CharArray<CharType, Size>& b);

  template <typename CharType, U32 SizeA, U32 SizeB>
  friend bool	operator!=(const CharArray<CharType, SizeA>& a, const CharArray<CharType, SizeB>& b);
  template <typename CharType, U32 Size>
  friend bool	operator!=(const CharArray<CharType, Size>& a, const char* b);
  template <typename CharType, U32 Size>
  friend bool	operator!=(const char* a, const CharArray<CharType, Size> &b);
  */

  std::wcout << wstr32.GetPtr() << std::endl;
  std::wcout << shortenWstrCopy32.GetPtr() << std::endl;
  E_ASSERT(wstr32 != shortenWstrCopy32);

  std::wcout << wstr32.GetPtr() << std::endl;
  std::wcout << wstrCopy32.GetPtr() << std::endl;
  E_ASSERT(wstr32 == wstrCopy32);

  E_ASSERT(L"A wonderful string" == wstr32);
  E_ASSERT(wstr32 != L"A wonderful string ");

  /*
  template <U32 SizeOther>
  CharArray<CharType, Size>&  operator=(const CharArray<CharType, SizeOther>& other); (already tested)
  CharType                    operator[](U32 index) const;
  CharType&                   operator[](U32 index);
  */

  E_ASSERT(wstr32[2] == L'w');
  wstr32[2] = L'W';
  E_ASSERT(wstr32[2] == L'W');

  /*
  U32                         GetLength() const;
  const CharType*             GetPtr() const; (already tested)
  U32                         GetSize() const;
  bool                        IsEqual(const CharType* str) const; (already tested)
  template <U32 SizeOther>
  bool                        IsEqual(const CharArray<CharType, SizeOther>& other) const; (already tested)
  */

  E_ASSERT(wstr32.GetLength() == 18 && wstr32.GetLength() <= wstr32.GetSize());

  /*
  void                        Clear();
  void                        Print(const CharType *fmt, ... );
  */
  wstr32.Clear();
  E_ASSERT(wstr32.GetLength() == 0 && wstr32[0] == 0);
  E_ASSERT(wstr32 == E::WString::Empty());

  wstr128.Print(L"How useful is printing a formatted string of %d characters!", wstr128.GetSize());
  std::wcout << wstr128.GetPtr() << std::endl;

  wstr32.Print(L"A wonderful string");
  wstr128.Print(L"A wonderful stri");
  wstr128 += (L'n');
  wstr128 += (L'g');
  E_ASSERT(wstr128 == wstr32);

  // UTF-8 conversion test;
  E::String utf8String("An amazing string in UTF-8. Let's do some conversion test!!");
  E::WString wideString;
  std::cout << "[UTF8] " << utf8String.GetPtr() << std::endl;
  E::Text::Utf8ToWide(wideString, utf8String);
  std::wcout << "[Wide] " << wideString.GetPtr() << std::endl;
  E::String utf8StringBack;
  E::Text::WideToUtf8(utf8StringBack, wideString);
  std::cout << "[UTF8] " << utf8StringBack.GetPtr() << std::endl;
  E_ASSERT(utf8String == utf8StringBack);
  E::WString wideStringBack;
  E::Text::Utf8ToWide(wideStringBack, utf8StringBack);
  E_ASSERT(wideString == wideStringBack);

  return true;
}

bool Test::String::RunPerformanceTest()
{
  std::cout << "[Test::String::RunPerformanceTest]" << std::endl;

  return true;
}
