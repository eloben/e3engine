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

// Created 09-Nov-2014 by Elías Lozada-Benavente
//
// $Revision: $
// $Date: $
// $Author: $

/** @file Application.cpp
This file defines the Application class.
*/

#include <CorePch.h>
#ifdef WIN32
#include "Win32/ApplicationImpl.h"
#endif

namespace E
{
namespace Application
{
/*----------------------------------------------------------------------------------------------------------------------
Application::Global methods
----------------------------------------------------------------------------------------------------------------------*/

Application& Global::GetApplication()
{
  return Singleton<Application>::GetInstance();
}

/*----------------------------------------------------------------------------------------------------------------------
Application auxiliary
----------------------------------------------------------------------------------------------------------------------*/

const WString kApplicationName(L"E::Application");
const WString kApplicationErrorTitle(L"Exception");
const WString kApplicationWindowInitializationErrorMessage(L"Unable to create application window");
const WString kApplicationChildWindowInitializationErrorMessage(L"Unable to create child window, main window must be created first (missing call to Initialize?)");
const WString kApplicationNullWindowExceptionMessage(L"Main window is nullptr");
const WString kApplicationWindowInitializationExceptionMessage(L"Unable to initialize window");
const WString kApplicationWindowExceptionMessage(L"Window exception");
const U32     kApplicationWindowDistance = 10;

/*----------------------------------------------------------------------------------------------------------------------
Application private initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Application::Application() : mpImpl(new Impl) {}

Application::~Application() {}

/*----------------------------------------------------------------------------------------------------------------------
Application methods
----------------------------------------------------------------------------------------------------------------------*/
IWindow* Application::CreateChildWindow(U32 width, U32 height, const String& title)
{
  return mpImpl->CreateChildWindow(width, height, title);
}

IWindow* Application::CreateMainWindow()
{
  return mpImpl->CreateMainWindow();
}

IWindow* Application::CreateMainWindow(U32 width, U32 height, const String& title)
{
  return mpImpl->CreateMainWindow(width, height, title);
}

void Application::DestroyWindow(IWindow* pWindow)
{
  mpImpl->DestroyWindow(pWindow);
}

void Application::Run()
{
  mpImpl->Run();
}

void Application::Terminate()
{
  mpImpl->Terminate();
}
}
}
