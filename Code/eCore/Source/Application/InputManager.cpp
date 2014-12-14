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

/** @file InputManager.cpp
This file defines the InputManager class.
*/

#include <CorePch.h>
#ifdef WIN32
#include "Win32/InputManagerImpl.h"
#endif

namespace E
{
namespace Application
{
/*----------------------------------------------------------------------------------------------------------------------
Application::Global methods
----------------------------------------------------------------------------------------------------------------------*/

InputManager& Global::GetInputManager()
{
  return Singleton<InputManager>::GetInstance();
}

/*----------------------------------------------------------------------------------------------------------------------
InputManager private initialization / finalization
------------------------------------------------------------------------------------------------------------------------*/

InputManager::InputManager()
  : mpImpl(new Impl()) {}

InputManager::~InputManager() {}

/*----------------------------------------------------------------------------------------------------------------------
InputManager accessors
------------------------------------------------------------------------------------------------------------------------*/

const Vector2i& InputManager::GetCursorPosition() const
{
  return mpImpl->GetCursorPosition();
}

bool InputManager::IsKeyDown(U8 key) const
{
  return mpImpl->IsKeyDown(key);
}

bool InputManager::IsKeyReleased(U8 key) const
{
  return mpImpl->IsKeyReleased(key);
}

bool InputManager::IsKeyUp(U8 key) const
{
  return mpImpl->IsKeyUp(key);
}

void InputManager::SetActiveWindow(IWindow* pWindow)
{
  mpImpl->SetActiveWindow(pWindow);
}

/*----------------------------------------------------------------------------------------------------------------------
Public Methods
------------------------------------------------------------------------------------------------------------------------*/

void InputManager::CenterCursor()
{
  mpImpl->CenterCursor();
}

void InputManager::Update()
{
  mpImpl->Update();
}
}
}
