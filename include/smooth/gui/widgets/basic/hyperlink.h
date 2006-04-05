 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_HYPERLINK_
#define _H_OBJSMOOTH_HYPERLINK_

namespace smooth
{
	namespace GUI
	{
		class Hyperlink;
		class Hotspot;
	};
};

#include "../widget.h"
#include "../../../graphics/bitmap.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Hyperlink : public Widget
		{
			protected:
				String			 linkURL;
				Bitmap			 linkBitmap;

				Hotspot			*hotspot;
			public:
				static const Int	 classID;

							 Hyperlink(const String &, const Bitmap &, const String &, const Point &, const Size & = Size(0, 0));
				virtual			~Hyperlink();

				virtual Int		 Paint(Int);
			accessors:
				Int			 SetText(const String &);

				Int			 SetBitmap(const Bitmap &);
				const Bitmap		&GetBitmap();

				Int			 SetURL(const String &);
				const String		&GetURL();
			slots:
				Void			 OnMouseOver();
				Void			 OnMouseOut();
				Void			 OnClickLink();
		};
	};
};

#endif