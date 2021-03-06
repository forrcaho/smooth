 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/console.h>

#if defined __WIN32__
#	include <windows.h>
#endif

#include <stdio.h>

S::System::Console::Console()
{
}

S::System::Console::Console(const Console &)
{
}

S::Int S::System::Console::SetTitle(const String &title)
{
#if defined __WIN32__
	SetConsoleTitle(title);
#endif

	return Success();
}

S::Int S::System::Console::OutputString(const String &string)
{
	printf("%s", (char *) string);
	fflush(stdout);

	return Success();
}

S::Int S::System::Console::OutputLine(const String &string)
{
	printf("%s\n", (char *) string);

	return Success();
}

S::Void S::System::Console::WaitForKey()
{
	getchar();
}
