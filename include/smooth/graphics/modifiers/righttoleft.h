 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_RIGHTTOLEFT
#define H_OBJSMOOTH_RIGHTTOLEFT

#include "../../definitions.h"
#include "../forms/rect.h"

namespace smooth
{
	namespace GUI
	{
		class RightToLeftModifier
		{
			private:
				Size		 surfaceSize;

				Bool		 rightToLeft;
			public:
						 RightToLeftModifier();
						~RightToLeftModifier();

				Void		 SetSurfaceSize(const Size &);
				const Size	&GetSurfaceSize() const;

				Void		 SetRightToLeft(Bool);
				Bool		 GetRightToLeft() const;

				Int		 TranslateX(Int) const;
				Int		 TranslateY(Int) const;
				Point		 TranslatePoint(const Point &) const;
				Rect		 TranslateRect(const Rect &) const;
		};
	};
};

#endif
