 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SURFACEXLIB
#define H_OBJSMOOTH_SURFACEXLIB

namespace smooth
{
	namespace GUI
	{
		class SurfaceXLib;
	};
};

#include "../surfacebackend.h"

namespace X11
{
	extern "C"
	{
#		define True	1
#		define False	0
#		define Bool	int
#		define Success	0

#		include <X11/Xlib.h>
#		include <X11/Xutil.h>
#		include <X11/Xft/Xft.h>

#		undef True
#		undef False
#		undef Bool
#		undef Success
	}
};

namespace smooth
{
	namespace GUI
	{
		const Short	 SURFACE_XLIB	= 2;

		class SurfaceXLib : public SurfaceBackend
		{
			private:
				static Short		 surfaceDPI;

				X11::Display		*display;
			protected:
				Size			 allocSize;

				X11::Window		 window;

				X11::Pixmap		 bitmap;
				X11::GC			 gc;

				Array<Rect *, Void *>	 paintRects;
			public:
							 SurfaceXLib(Void * = NIL, const Size & = Size());
							~SurfaceXLib();

				Int			 SetSize(const Size &);
				const Size		&GetSize() const;

				Int			 PaintRect(const Rect &);

				Int			 StartPaint(const Rect &);
				Int			 EndPaint();

				Void			*GetSystemSurface() const;

				Short			 GetSurfaceDPI() const;

				Int			 SetPixel(const Point &, const Color &);

				Int			 Line(const Point &, const Point &, const Color &);
				Int			 Box(const Rect &, const Color &, Int, const Size &);

				Int			 SetText(const String &, const Rect &, const Font &, Bool);

				Int			 BlitFromBitmap(const Bitmap &, const Rect &, const Rect &);
				Int			 BlitToBitmap(const Rect &, Bitmap &, const Rect &);
		};
	};
};

#endif
