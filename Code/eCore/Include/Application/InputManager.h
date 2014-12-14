/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Engine

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
------------------------------------------------------------------------------------------------------------------------*/

// Create 14-Aug-2014 by Elías Lozada-Benavente
// Original 04-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file InputManager.h
This file defines the InputManager interface. The InputManager allows communication with keyboard and mouse input devices.
*/

#ifndef E3_INPUT_MANAGER_H
#define E3_INPUT_MANAGER_H

#include <Math/Vector2.h>
#include <Singleton.h>

namespace E 
{
namespace Application
{
// Forward declarations
class InputManager;
/*----------------------------------------------------------------------------------------------------------------------
Input::Global methods
----------------------------------------------------------------------------------------------------------------------*/
namespace Global
{
  E_API InputManager& GetInputManager();
}

// Forward declarations
class IWindow;
/*----------------------------------------------------------------------------------------------------------------------
InputManager
----------------------------------------------------------------------------------------------------------------------*/
class InputManager
{
public:
  // Accessors
  E_API const Vector2i& GetCursorPosition() const;
  E_API bool			      IsKeyDown(U8 key) const;
  E_API bool            IsKeyReleased(U8 key) const;
  E_API bool			      IsKeyUp(U8 key) const;
  E_API void            SetActiveWindow(IWindow* pWindow);

  // Methods
  E_API void			      CenterCursor();
  E_API void            Update();

private:
  E_PIMPL mpImpl;

  E_DECLARE_SINGLETON_ONLY(InputManager)
};
}
}

#endif
