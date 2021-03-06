 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_WINDOWBACKEND
#define H_OBJSMOOTH_WINDOWBACKEND

namespace smooth
{
	namespace GUI
	{
		class WindowBackend;
	};
};

#include "../window.h"

#include "../../../graphics/surface.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 WINDOW_NONE = 0;

		class WindowBackend
		{
			private:
				static WindowBackend		*(*backend_creator)();
			protected:
				Short				 type;

				Surface				*drawSurface;

				Size				 sizeModifier;
				Rect				 updateRect;
			public:
				static Int			 SetBackend(WindowBackend *(*)());

				static WindowBackend		*CreateBackendInstance();

								 WindowBackend(Void * = NIL);
				virtual				~WindowBackend();

				Short				 GetWindowType() const;

				virtual Void			*GetSystemWindow() const;

				virtual Surface			*GetDrawSurface() const;

				virtual Int			 Open(const String &, const Point &, const Size &, Int);
				virtual Int			 Close();

				virtual Int			 RequestClose();

				virtual Int			 SetTitle(const String &);

				virtual Int			 SetIcon(const Bitmap &);
				virtual Int			 SetIconDirect(Void *);

				virtual Int			 SetProgressIndicator(Window::ProgressIndicatorState, Float);

				virtual Int			 EnableDropFiles(Bool);
				virtual const Array<String>	&GetDroppedFiles() const;

				virtual Int			 SetMinimumSize(const Size &);
				virtual Int			 SetMaximumSize(const Size &);

				virtual Int			 Show();
				virtual Int			 Hide();

				virtual Rect			 GetRestoredWindowRect() const;

				virtual Int			 SetMetrics(const Point &, const Size &);

				virtual Int			 Minimize();

				virtual Int			 Maximize();
				virtual Int			 Restore();

				virtual Int			 Raise();
			accessors:
				const Size			&GetSizeModifier() const;
				Void				 SetSizeModifier(const Size &);

				const Rect			&GetUpdateRect() const;
			signals:
				Callback0<Bool>			 doClose;

				Callback3<Int, Int, Int, Int>	 onEvent;

				Signal0<Void>			 onMinimize;
				Signal0<Void>			 onMaximize;

				Signal0<Void>			 onRestore;

				Signal0<Void>			 onCreate;
				Signal0<Void>			 onDestroy;
		};
	};
};

#endif
