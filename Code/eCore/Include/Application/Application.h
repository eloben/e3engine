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

/** @file Application.h
This file declares the global Application class and the IWindow interface. Application is a helper class for executable 
application initialization allowing creation of several windows.
*/

#ifndef E3_APPLICATION_H
#define E3_APPLICATION_H

#include "Input.h"
#include <EventSystem/Event.h>
#include <Singleton.h>
#include <Text/String.h>

namespace E
{
namespace Application
{
// Forward declarations
class Application;
/*----------------------------------------------------------------------------------------------------------------------
Application::Global methods
----------------------------------------------------------------------------------------------------------------------*/
namespace Global
{
  E_API Application& GetApplication();
}

// Forward declarations
class IWindow;

/*----------------------------------------------------------------------------------------------------------------------
FocusEvent
----------------------------------------------------------------------------------------------------------------------*/
struct FocusEvent
{
  IWindow*      pWindow;
  explicit FocusEvent(IWindow* pWindow) : pWindow(pWindow) {}
};

/*----------------------------------------------------------------------------------------------------------------------
FocusLostEvent
----------------------------------------------------------------------------------------------------------------------*/
struct FocusLostEvent
{
  IWindow*      pWindow;
  explicit FocusLostEvent(IWindow* pWindow) : pWindow(pWindow) {}
};

/*----------------------------------------------------------------------------------------------------------------------
KeyDownEvent
----------------------------------------------------------------------------------------------------------------------*/	
struct KeyDownEvent
{
  IWindow*    pWindow;
  Input::Key  key;
  KeyDownEvent(IWindow* pWindow, Input::Key key) : pWindow(pWindow), key(key) {}
};

/*----------------------------------------------------------------------------------------------------------------------
KeyUpEvent
----------------------------------------------------------------------------------------------------------------------*/	
struct KeyUpEvent
{
  IWindow*    pWindow;
  Input::Key  key;
  KeyUpEvent(IWindow* pWindow, Input::Key key) : pWindow(pWindow), key(key) {}
};

/*----------------------------------------------------------------------------------------------------------------------
MouseDownEvent
----------------------------------------------------------------------------------------------------------------------*/	
struct MouseDownEvent
{
  IWindow*      pWindow;
  Input::Mouse  mouseButton;
  MouseDownEvent(IWindow* pWindow, Input::Mouse mouseButton) : pWindow(pWindow), mouseButton(mouseButton) {}
};

/*----------------------------------------------------------------------------------------------------------------------
MouseUpEvent
----------------------------------------------------------------------------------------------------------------------*/	
struct MouseUpEvent
{
  IWindow*      pWindow;
  Input::Mouse  mouseButton;
  MouseUpEvent(IWindow* pWindow, Input::Mouse mouseButton) : pWindow(pWindow), mouseButton(mouseButton) {}
};

/*----------------------------------------------------------------------------------------------------------------------
UpdateEvent
----------------------------------------------------------------------------------------------------------------------*/
struct UpdateEvent
{
  IWindow*      pWindow;
  explicit UpdateEvent(IWindow* pWindow) : pWindow(pWindow) {}
};

/*----------------------------------------------------------------------------------------------------------------------
IWindow
----------------------------------------------------------------------------------------------------------------------*/	
class IWindow
{
public:
  // Types
  typedef EventSystem::EventCallback<FocusEvent>      FocusEventCallback;
  typedef EventSystem::EventCallback<FocusLostEvent>  FocusLostEventCallback;
  typedef EventSystem::EventCallback<KeyDownEvent>    KeyDownEventCallback;
  typedef EventSystem::EventCallback<KeyUpEvent>      KeyUpEventCallback;
  typedef EventSystem::EventCallback<MouseDownEvent>  MouseDownEventCallback;
  typedef EventSystem::EventCallback<MouseUpEvent>    MouseUpEventCallback;
  typedef EventSystem::EventCallback<UpdateEvent>     UpdateEventCallback;

  enum Mode
  {
    eModeDefault,
    eModeWindowless,
    eModeFullScreen      
  };

  struct Descriptor
  {
    String    title;
    Ptr       handle;
    U32       width; 
    U32       height;
    Mode      mode;

    Descriptor()
      : handle(0)
      , width(0)
      , height(0)
      , mode(eModeDefault) {}
  }; 

  virtual ~IWindow() {}
  virtual const Descriptor&         GetDescriptor() const = 0;
  virtual FocusEventCallback&       GetFocusEventCallback() = 0;
  virtual FocusLostEventCallback&   GetFocusLostEventCallback() = 0;
  virtual KeyDownEventCallback&     GetKeyDownEventCallback() = 0;
  virtual KeyUpEventCallback&       GetKeyUpEventCallback() = 0;
  virtual MouseDownEventCallback&   GetMouseDownEventCallback() = 0;
  virtual MouseUpEventCallback&     GetMouseUpEventCallback() = 0;
  virtual UpdateEventCallback&      GetUpdateEventCallback() = 0;
  virtual bool                      IsActive() const = 0;

};

/*----------------------------------------------------------------------------------------------------------------------
Application
----------------------------------------------------------------------------------------------------------------------*/	
class Application
{
public:
  E_API IWindow*                    CreateChildWindow(U32 windowWidth, U32 windowHeight, const String& title = String::Empty());
  E_API IWindow*    		            CreateMainWindow();
  E_API IWindow*    		            CreateMainWindow(U32 windowWidth, U32 windowHeight, const String& title = String::Empty());
  E_API void                        DestroyWindow(IWindow* pWindow);
  E_API void				                Run();
  E_API void				                Terminate();
private:
  E_PIMPL mpImpl;

  E_DECLARE_SINGLETON_ONLY(Application)
};
}
}

#endif
