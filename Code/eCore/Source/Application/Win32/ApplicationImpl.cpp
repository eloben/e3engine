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

// Created 16-Nov-2014 by Elías Lozada-Benavente
//
// $Revision: $
// $Date: $
// $Author: $

/** @file Application.cpp
This file defines the Application class.
*/

#include <CorePch.h>
#include "ApplicationImpl.h"

namespace E
{
namespace Application
{
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
Window initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Window::Window() : mActiveFlag(false) {}

void Window::Initialize(const Descriptor& desc)
{
  mDescriptor = desc;
}

/*----------------------------------------------------------------------------------------------------------------------
Window accessors
----------------------------------------------------------------------------------------------------------------------*/

const IWindow::Descriptor& Window::GetDescriptor() const
{
  return mDescriptor;
}

IWindow::KeyDownEventCallback& Window::GetKeyDownEventCallback()
{
  return mKeyDownEventCallback;
}

IWindow::KeyUpEventCallback& Window::GetKeyUpEventCallback()
{
  return mKeyUpEventCallback;
}

IWindow::MouseDownEventCallback& Window::GetMouseDownEventCallback()
{
  return mMouseDownEventCallback;
}

IWindow::MouseUpEventCallback& Window::GetMouseUpEventCallback()
{
  return mMouseUpEventCallback;
}

IWindow::FocusEventCallback& Window::GetFocusEventCallback()
{
  return mFocusEventCallback;
}

IWindow::FocusLostEventCallback& Window::GetFocusLostEventCallback()
{
  return mFocusLostEventCallback;
}

IWindow::UpdateEventCallback& Window::GetUpdateEventCallback()
{
  return mUpdateEventCallback;
}

bool Window::IsActive() const
{
  return mActiveFlag;
}

/*----------------------------------------------------------------------------------------------------------------------
Window methods
----------------------------------------------------------------------------------------------------------------------*/

/** Update
The display devices internal WndProc function. All messages being passed to this
function are relative to the window it owns.
*/
LRESULT Window::WindowProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // Determine message type
  switch (msg)
  {
  case WM_CLOSE:
    Global::GetApplication().DestroyWindow(this);
    PostQuitMessage(0);
    break;
  case WM_SIZE:
    mActiveFlag = (wParam != SIZE_MINIMIZED);
    break;
  case WM_SETFOCUS:
    mFocusEventCallback.Raise(FocusEvent(this));
    break;
  case WM_KILLFOCUS:
    mFocusLostEventCallback.Raise(FocusLostEvent(this));
    break;
  case WM_KEYDOWN:
    mKeyDownEventCallback.Raise(KeyDownEvent(this, static_cast<Input::Key>(wParam)));
    break;
  case WM_KEYUP:
    mKeyUpEventCallback.Raise(KeyUpEvent(this, static_cast<Input::Key>(wParam)));
    break;
  case WM_LBUTTONDOWN:
    mMouseDownEventCallback.Raise(MouseDownEvent(this, Input::eMouseLeft));
    break;
  case WM_LBUTTONUP:
    mMouseUpEventCallback.Raise(MouseUpEvent(this, Input::eMouseLeft));
    break;
  case WM_MBUTTONDOWN:
    mMouseDownEventCallback.Raise(MouseDownEvent(this, Input::eMouseMiddle));
    break;
  case WM_MBUTTONUP:
    mMouseUpEventCallback.Raise(MouseUpEvent(this, Input::eMouseMiddle));
    break;
  case WM_RBUTTONDOWN:
    mMouseDownEventCallback.Raise(MouseDownEvent(this, Input::eMouseRight));
    break;
  case WM_RBUTTONUP:
    mMouseUpEventCallback.Raise(MouseUpEvent(this, Input::eMouseRight));
    break;
  default:
    return DefWindowProc(windowHandle, msg, wParam, lParam);
  }

  return 0;
}

/*----------------------------------------------------------------------------------------------------------------------
Application initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Application::Impl::Impl()
  : mpMainWindow(nullptr)
  , mExitFlag(false) 
{
}

Application::Impl::~Impl()
{
  mWindowFactory.CleanUp();
}

/*----------------------------------------------------------------------------------------------------------------------
Application methods
----------------------------------------------------------------------------------------------------------------------*/

IWindow* Application::Impl::CreateChildWindow(U32 width, U32 height, const String& title)
{
  if (mpMainWindow == nullptr)
  {
     ::MessageBox(::GetActiveWindow(), kApplicationChildWindowInitializationErrorMessage.GetPtr(), kApplicationErrorTitle.GetPtr(), MB_OK);
     mExitFlag = true;
     return nullptr;
  }

  IWindow::Descriptor windowDesc;
  windowDesc.mode = IWindow::eModeDefault;
  windowDesc.width = width;
  windowDesc.height = height;
  windowDesc.title = title;
  
  return CreateApplicationWindow(windowDesc, static_cast<HWND>(mpMainWindow->GetDescriptor().handle));
}

IWindow* Application::Impl::CreateMainWindow()
{
  IWindow::Descriptor windowDesc;
  windowDesc.mode = IWindow::eModeFullScreen;
  mpMainWindow = CreateApplicationWindow(windowDesc, nullptr);
  mExitFlag = mpMainWindow == nullptr;
  return mpMainWindow;
}

IWindow* Application::Impl::CreateMainWindow(U32 width, U32 height, const String& title)
{
  IWindow::Descriptor windowDesc;
  windowDesc.width = width;
  windowDesc.height = height;
  windowDesc.title = title;
  mpMainWindow = CreateApplicationWindow(windowDesc, nullptr);
  mExitFlag = mpMainWindow == nullptr;

  return mpMainWindow;
}

void Application::Impl::DestroyWindow(IWindow* pWindow)
{
  E_ASSERT_PTR(pWindow);
  ::DestroyWindow(static_cast<HWND>(pWindow->GetDescriptor().handle));
  mWindowList.RemoveIfFast(static_cast<Window*>(pWindow));
  if (pWindow == mpMainWindow)
  {
    mpMainWindow = nullptr;
    mExitFlag = true;
  }
}

void Application::Impl::Terminate()
{
  if (mpMainWindow)
  {
    // If the specified window is a parent or owner window, DestroyWindow automatically destroys the associated child or 
    // owned windows when it destroys the parent or owner window. The function first destroys child or owned windows, 
    // and then it destroys the parent or owner window.
    ::DestroyWindow(static_cast<HWND>(mpMainWindow->GetDescriptor().handle));
    mWindowList.Clear();
    mpMainWindow = nullptr;
    mExitFlag = true;
  }
}

/*----------------------------------------------------------------------------------------------------------------------
Application methods
----------------------------------------------------------------------------------------------------------------------*/

void Application::Impl::Run()
{
  MSG	msg;
  while (!mExitFlag) 
  {
    // Did we receive a message, or are we idling ?
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
    {
      if (msg.message == WM_QUIT) 
      {  
        mExitFlag = true;
      }

      TranslateMessage(&msg);
      DispatchMessage (&msg);
    } 
    else
    {	
      try
      {
        // Trigger window handler update
        for (auto pWindow : mWindowList)
        {
          if (pWindow->IsActive()) pWindow->GetUpdateEventCallback().Raise(UpdateEvent(pWindow));
        }
      }
      catch (const Exception&)
      {
        ::MessageBox(::GetActiveWindow(), kApplicationWindowExceptionMessage.GetPtr(), kApplicationErrorTitle.GetPtr(), MB_OK);
        mExitFlag = true;
      }
    }
  }
}

/*----------------------------------------------------------------------------------------------------------------------
Application private initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

Window* Application::Impl::CreateApplicationWindow(IWindow::Descriptor& windowDesc, HWND parentWindowHandle)
{
  // Register the window
  WNDCLASS windowClass;
  windowClass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  windowClass.lpfnWndProc    = WindowProc;
  windowClass.cbClsExtra     = 0;
  windowClass.cbWndExtra     = 0;
  windowClass.hInstance      = (HINSTANCE)GetModuleHandle(0);// hInstance;
  windowClass.hIcon	        = LoadIcon(nullptr, IDI_WINLOGO);
  windowClass.hCursor        = LoadCursor(nullptr, IDC_ARROW);
  windowClass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
  windowClass.lpszMenuName   = 0;
  windowClass.lpszClassName  = kApplicationName.GetPtr();
  ::RegisterClass(&windowClass);

  U32 windowCanvasStyle = 0;
  U32 windowCanvasSizeX = 0;
  U32 windowCanvasSizeY = 0;
  WString windowTitleWstr;

  if (windowDesc.mode == IWindow::eModeFullScreen)
  {
    // Calculate windowCanvas size from screen
    windowCanvasSizeX = GetSystemMetrics(SM_CXSCREEN);
    windowCanvasSizeY = GetSystemMetrics(SM_CYSCREEN);

    // Set the screen settings
    DEVMODE screenSettings;
    memset(&screenSettings, 0, sizeof(screenSettings));
    screenSettings.dmSize       = sizeof(screenSettings);
    screenSettings.dmPelsWidth  = windowCanvasSizeX;
    screenSettings.dmPelsHeight = windowCanvasSizeY;
    screenSettings.dmBitsPerPel = 32;			
    screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Apply screen settings
    ::ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);

    // Update window descriptor
    windowDesc.width = windowCanvasSizeX;
    windowDesc.height = windowCanvasSizeY;
  }
  else
  {
    if (windowDesc.mode == IWindow::eModeWindowless)
    {
      windowCanvasStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
      windowCanvasSizeX = windowDesc.width;
      windowCanvasSizeY = windowDesc.height;
    }
    else
    {
      windowCanvasStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

      // Calculate windowCanvas size to host desired window client area;
      RECT rc;
      rc.left   = 0;
      rc.top    = 0;
      rc.right  = windowDesc.width;
      rc.bottom = windowDesc.height;
      ::AdjustWindowRect(&rc, windowCanvasStyle, false);
      windowCanvasSizeX = (rc.right - rc.left);
      windowCanvasSizeY = (rc.bottom - rc.top);

      // Set window title
      Text::Utf8ToWide(windowTitleWstr, windowDesc.title);
    }
  }

  // Initialize COM.
  Win32::Global::InitializeCom();

  // Center the window position depending on its dimension
  U32 windowCanvasPositionX = Math::Min(GetSystemMetrics(SM_CXSCREEN) - windowCanvasSizeX, GetSystemMetrics(SM_CYSCREEN) - windowCanvasSizeY) / 2;
  U32 windowCanvasPositionY = windowCanvasPositionX;

  // If there is a parent window position the window to its right
  if (parentWindowHandle != 0)
  { 
    WINDOWINFO windowInfo;
    Memory::Zero(&windowInfo);
    ::GetWindowInfo(parentWindowHandle, &windowInfo);
    windowCanvasPositionX = windowInfo.rcWindow.right + kApplicationWindowDistance;
    windowCanvasPositionY = windowInfo.rcWindow.top;
  }

  Window* pWindow = mWindowFactory.Create();
  E_ASSERT(pWindow);

  // Create the window passing this as lpParam to allow the static WindowProc function accessing the Application instance. 
  // This way we will be able to use Application object stats through the WindowWindowProc.
  windowDesc.handle = ::CreateWindow(
    kApplicationName.GetPtr(),
    windowTitleWstr.GetPtr(),
    windowCanvasStyle,
    windowCanvasPositionX,
    windowCanvasPositionY,
    windowCanvasSizeX,  
    windowCanvasSizeY,
    parentWindowHandle,
    0,
    windowClass.hInstance,
    pWindow);

  // Bail on error
  if (!windowDesc.handle)
  {
    mWindowFactory.Destroy(pWindow);
    ::MessageBox(::GetActiveWindow(), kApplicationWindowInitializationErrorMessage.GetPtr(), kApplicationErrorTitle.GetPtr(), MB_OK);
    return nullptr;
  }
  
  // Initialize window object
  pWindow->Initialize(windowDesc);
  mWindowList.PushBack(pWindow);

  if (!mpMainWindow) mpMainWindow = pWindow;

  // Bring the window up on the screen and set the focus to the main window
  HWND windowHandle = static_cast<HWND>(windowDesc.handle);
  ::ShowWindow(windowHandle, SW_SHOW);
  ::SetForegroundWindow(windowHandle);
  if (parentWindowHandle == 0) ::SetFocus(windowHandle);

  return pWindow;
}


/*----------------------------------------------------------------------------------------------------------------------
Static Windows callback
----------------------------------------------------------------------------------------------------------------------*/

/**
WindowProc () (Static Callback)
his is the main message pump for ALL display devices, it captures the appropriate messages, and routes them through to the
application class for which it was intended, therefore giving full class access. It is VITALLY important that you should 
pass your 'this' pointer to the lpParam parameter of the CreateWindow function if you wish to be able to pass messages 
back to that app object.
*/

LRESULT CALLBACK Application::Impl::WindowProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // If this is a create message, trap the 'this' pointer passed in and store it within the window.
  if (msg == WM_NCCREATE)
  {
    #ifdef E_CPU_X64
    SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT FAR *)lParam)->lpCreateParams);
    #else
    SetWindowLongPtr(windowHandle, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);
    #endif
  }

  // Obtain the correct destination for this message
  #ifdef E_CPU_X64
  Window* pDestination = (Window*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
  #else
  Window* pDestination = (Window*)GetWindowLongPtr(windowHandle, GWL_USERDATA);
  #endif
  
  // If the windowHandle has a related class, pass it through
  if (pDestination) 
  {
    return pDestination->WindowProc(windowHandle, msg, wParam, lParam);
  }

  // No destination found, defer to system...
  return DefWindowProc(windowHandle, msg, wParam, lParam);
}
}
}
