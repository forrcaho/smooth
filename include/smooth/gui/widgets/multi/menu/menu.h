 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_MENU
#define H_OBJSMOOTH_MENU

namespace smooth
{
	namespace GUI
	{
		class Menu;

		class PopupMenu;
	};
};

#include "../generic/container.h"
#include "menuentry.h"

namespace smooth
{
	namespace GUI
	{
		abstract class SMOOTHAPI Menu : public Container
		{
			public:
				static const Short	 classID;

							 Menu();
				virtual			~Menu();

				virtual MenuEntry	*AddEntry(const String & = NIL, const Bitmap & = NIL, PopupMenu * = NIL, Bool * = NIL, Int * = NIL, Int iCode = 0) = 0;
			accessors:
				MenuEntry		*GetNthEntry(Int n) const	{ return (MenuEntry *) Container::GetNthEntry(n); }
		};
	};
};

#endif
