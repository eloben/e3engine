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

/** @file Application.h
This file declares the global Application class and the IWindow interface. Application is a helper class for executable 
application initialization allowing creation of several windows.
*/

#ifndef E3_APPLICATION_IMPL_H
#define E3_APPLICATION_IMPL_H

namespace E
{
namespace Application
{
/*----------------------------------------------------------------------------------------------------------------------
Window
----------------------------------------------------------------------------------------------------------------------*/	
class Window : public IWindow
{
public:
                            Window();
  void                      Initialize(const Descriptor& desc);

  const Descriptor&         GetDescriptor() const;
  FocusEventCallback&       GetFocusEventCallback();
  FocusLostEventCallback&   GetFocusLostEventCallback();
  KeyDownEventCallback&     GetKeyDownEventCallback();
  KeyUpEventCallback&       GetKeyUpEventCallback();
  MouseDownEventCallback&   GetMouseDownEventCallback();
  MouseUpEventCallback&     GetMouseUpEventCallback();
  UpdateEventCallback&      GetUpdateEventCallback();
  bool                      IsActive() const;

  void                      Update();
  LRESULT 	                WindowProc(HWND windowHandle, UINT Message, WPARAM wParam, LPARAM lParam);
  
private:
  Descriptor                mDescriptor;
  FocusEventCallback        mFocusEventCallback;
  FocusLostEventCallback    mFocusLostEventCallback;
  KeyDownEventCallback      mKeyDownEventCallback;
  KeyUpEventCallback        mKeyUpEventCallback;
  MouseDownEventCallback    mMouseDownEventCallback;
  MouseUpEventCallback      mMouseUpEventCallback;
  UpdateEventCallback       mUpdateEventCallback;
  bool                      mActiveFlag;
};

/*----------------------------------------------------------------------------------------------------------------------
Application
----------------------------------------------------------------------------------------------------------------------*/	
class Application::Impl : public Memory::ProxyAllocated
{
public:
                            Impl();
                            ~Impl();

  IWindow*                  CreateChildWindow(U32 windowWidth, U32 windowHeight, const String& title);
  IWindow*    		          CreateMainWindow();
  IWindow*    		          CreateMainWindow(U32 windowWidth, U32 windowHeight, const String& title);
  void                      DestroyWindow(IWindow* pWindow);
  void				              Run();
  void				              Terminate();

private:
  typedef Containers::List<Window*> WindowList;
  typedef Memory::ConcreteFactory<Window> WindowFactory;

  WindowFactory             mWindowFactory;
  Window*                   mpMainWindow;
  WindowList                mWindowList;
  bool                      mExitFlag;

  Window*                   CreateApplicationWindow(IWindow::Descriptor& windowDesc, HWND parentWindowHandle);
 
  static LRESULT CALLBACK   WindowProc(HWND windowHandle, UINT Message, WPARAM wParam, LPARAM lParam);

  E_DISABLE_COPY_AND_ASSSIGNMENT(Impl)
};
}
}

#endif
