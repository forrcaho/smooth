 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifdef __WIN32__
#	include <windows.h>
#endif

#include <smooth/gui/application/xulloader.h>
#include <smooth/gui/application/application.h>
#include <smooth/xml/xul/renderer.h>
#include <smooth/misc/args.h>
#include <smooth/init.h>

#ifdef __WIN32__
#	include <smooth/init.win32.h>

	int WINAPI S::GUI::LoadXUL(HWND hWnd, HINSTANCE shInstance, LPSTR szCmdLine, int iCmdShow)
	{
		hInstance = shInstance;

		ArgumentsParser	 args(NIL, szCmdLine);

		GUI::Application::SetCommand(args.GetCommand());
		GUI::Application::SetArguments(args.GetArguments());

		GUI::Application::GetStartupDirectory();
		GUI::Application::GetApplicationDirectory();

		Init();

		XULLoader	*loader = new XULLoader(szCmdLine);

		loader->Loop();

		Object::DeleteObject(loader);

		Free();

		return 0;
	}
#endif

S::GUI::XULLoader::XULLoader(const String &xulFile)
{
	xulRenderer = new XML::XUL::Renderer();

	if (xulRenderer->LoadXUL(xulFile) == Success()) Add(xulRenderer->GetWidget());
}

S::GUI::XULLoader::~XULLoader()
{
	delete xulRenderer;
}
