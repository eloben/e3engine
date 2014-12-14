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

// Created 21-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Main.cpp
This file defines the application main entry point.
*/

#include <GraphicsTestPch.h>
#include <Math/Random.h>

using namespace E;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  Application::Application& app = Application::Global::GetApplication();

  SimpleVertexUpdater simpleVertexUpdater;
  simpleVertexUpdater.Initialize(app.CreateMainWindow(512, 512, "Color Sample"));

  TextureVertexUpdater textureVertexUpdater;
  textureVertexUpdater.Initialize(app.CreateChildWindow(512, 512, "Texture Sample"));

  IndexedVertexUpdater indexedVertexUpdater;
  indexedVertexUpdater.Initialize(app.CreateChildWindow(512, 512, "Indexed Drawing Sample"));
  
  InstanceVertexUpdater instanceVertexUpdater;
  instanceVertexUpdater.Initialize(app.CreateChildWindow(512, 512, "Instancing Sample"));

  IndexedInstanceVertexUpdater indexedInstanceVertexUpdater;
  indexedInstanceVertexUpdater.Initialize(app.CreateChildWindow(512, 512, "Indexed Instancing Drawing Sample"));

  RenderToTextureUpdater renderToTextureUpdater;
  renderToTextureUpdater.Initialize(app.CreateChildWindow(512, 512, "Render to Texture Sample"));

  RenderDepthToTextureUpdater renderDepthToTextureUpdater;
  renderDepthToTextureUpdater.Initialize(app.CreateChildWindow(512, 512, "Render Depth to Texture Sample"));

  app.Run();

  return 0;
}

