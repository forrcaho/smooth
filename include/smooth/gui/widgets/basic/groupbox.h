 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_GROUPBOX_
#define _H_OBJSMOOTH_GROUPBOX_

namespace smooth
{
	namespace GUI
	{
		class GroupBox;
	};
};

#include "../layer.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI GroupBox : public Layer
		{
			public:
				static const Int	 classID;

							 GroupBox(const String &, const Point &, const Size &);
				virtual			~GroupBox();

				virtual Int		 Paint(Int);

				virtual Int		 Activate();
				virtual Int		 Deactivate();

				virtual Int		 Hide();
		};
	};
};

#endif