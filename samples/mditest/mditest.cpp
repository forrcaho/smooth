 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "mditest.h"

Int smooth::Main()
{
	MDITest	*app = new MDITest();

	Loop();

	delete app;

	return 0;
}

MDITest::MDITest()
{
	SetText("MDITest");

	mainWnd			= new Window("MDITest");
	mainWnd_titlebar	= new Titlebar(true, true, true);
	mainWnd_statusbar	= new Statusbar("Ready");
	mainWnd_client		= new MDIClient();
	mainWnd_menubar		= new Menubar();
	menu_file		= new PopupMenu();

	menu_file->AddEntry("New", NIL, Proc(&MDITest::NewMDI), this);
	menu_file->AddEntry();
	menu_file->AddEntry("Exit", NIL, Proc(mainWnd->*(&Window::Close)), mainWnd);

	mainWnd_menubar->AddEntry("File", NIL, NULLPROC, menu_file);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_statusbar);
	mainWnd->RegisterObject(mainWnd_client);
	mainWnd->RegisterObject(mainWnd_menubar);

	mainWnd->SetMetrics(Point(50, 50), Size(700, 500));
	mainWnd->SetIcon(SI_DEFAULT);
	mainWnd->SetKillProc(KillProc(&MDITest::ExitProc), this);
}

MDITest::~MDITest()
{
	mainWnd->UnregisterObject(mainWnd_statusbar);
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_client);
	mainWnd->UnregisterObject(mainWnd_menubar);

	UnregisterObject(mainWnd);

	delete mainWnd_statusbar;
	delete mainWnd_titlebar;
	delete mainWnd_client;
	delete mainWnd_menubar;
	delete menu_file;
	delete mainWnd;
}

Bool MDITest::ExitProc()
{
	Int	 id = SMOOTH::MessageBox("Do you really want to quit?", "Exit program", MB_YESNO, IDI_QUESTION);

	switch (id)
	{
		case IDYES:
			return true;
		default:
		case IDNO:
			return false;
	}
}

Void MDITest::NewMDI()
{
}
