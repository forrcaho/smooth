 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/dirdlg.h>
#include <smooth/misc/i18n.h>
#include <smooth/gui/window/window.h>

#include <shlobj.h>

int CALLBACK	 BrowseCallbackProc(HWND, UINT, LPARAM, LPARAM);

S::GUI::Dialogs::DirSelection::DirSelection()
{
	caption		= I18n::Translator::defaultTranslator->TranslateString("Select directory");
	directory	= NIL;
}

S::GUI::Dialogs::DirSelection::~DirSelection()
{
}

S::Int S::GUI::Dialogs::DirSelection::ShowDialog()
{
	if (Setup::enableUnicode)
	{
		BROWSEINFOW	 infow;
		wchar_t		*bufferw = new wchar_t [32768];

		for (Int i = 0; i < 32768; i++) bufferw[i] = 0;

		if (parentWindow != NIL)	infow.hwndOwner = (HWND) parentWindow->GetSystemWindow();
		else				infow.hwndOwner = NIL;

		infow.pidlRoot		= NIL;
		infow.pszDisplayName	= bufferw;
		infow.lpszTitle		= caption;
		infow.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
		infow.lpfn		= &BrowseCallbackProc;
		infow.lParam		= (LPARAM) this;
		infow.iImage		= 0;

		ITEMIDLIST	*idlist = SHBrowseForFolderW(&infow);

		SHGetPathFromIDListW(idlist, bufferw);

		CoTaskMemFree(idlist);

		directory = NIL;

		if (bufferw != NIL && bufferw[0] != 0) directory = bufferw;

		delete [] bufferw;
	}
	else
	{
		BROWSEINFOA	 infoa;
		char		*buffera = new char [32768];

		for (Int i = 0; i < 32768; i++) buffera[i] = 0;

		if (parentWindow != NIL)	infoa.hwndOwner = (HWND) parentWindow->GetSystemWindow();
		else				infoa.hwndOwner = NIL;

		infoa.pidlRoot		= NIL;
		infoa.pszDisplayName	= buffera;
		infoa.lpszTitle		= caption;
		infoa.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
		infoa.lpfn		= &BrowseCallbackProc;
		infoa.lParam		= (LPARAM) this;
		infoa.iImage		= 0;

		ITEMIDLIST	*idlist = SHBrowseForFolderA(&infoa);

		SHGetPathFromIDListA(idlist, buffera);

		CoTaskMemFree(idlist);

		directory = NIL;

		if (buffera != NIL && buffera[0] != 0) directory = buffera;

		delete [] buffera;
	}

	if (directory != NIL)
	{
		if (directory[directory.Length() - 1] != '\\') directory[directory.Length()] = '\\';
	
		return Success;
	}
	else
	{
		return Failure;
	}
}

S::Int S::GUI::Dialogs::DirSelection::SetDirName(String nDirectory)
{
	directory = nDirectory;

	return Success;
}

S::String S::GUI::Dialogs::DirSelection::GetDirName()
{
	return directory;
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && ((S::GUI::Dialogs::DirSelection *) lpData)->GetDirName() != NIL)
	{
		if (S::Setup::enableUnicode)	SendMessageW(hwnd, BFFM_SETSELECTION, true, (LPARAM) (wchar_t *) ((S::GUI::Dialogs::DirSelection *) lpData)->GetDirName());
		else				SendMessageA(hwnd, BFFM_SETSELECTION, true, (LPARAM) (wchar_t *) ((S::GUI::Dialogs::DirSelection *) lpData)->GetDirName());
	}

	return 0;
}
