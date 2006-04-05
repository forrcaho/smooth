 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SLIDER_
#define _H_OBJSMOOTH_SLIDER_

namespace smooth
{
	namespace GUI
	{
		class Slider;
		class Hotspot;
	};
};

#include "../widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Slider : public Widget
		{
			private:
				Int			 dummyVariable;

				Int			 mouseBias;
				Bool			 dragging;

				Void			 UpdateHotspotPositions();
			protected:
				Int			*variable;

				Int			 startValue;
				Int			 endValue;

				Hotspot			*clickHotspot;
				Hotspot			*dragHotspot;
			public:
				static const Int	 classID;

							 Slider(const Point &, const Size &, Int, Int * = NIL, Int = 0, Int = 100);
				virtual			~Slider();

				virtual Int		 Paint(Int);
			accessors:
				Int			 SetRange(Int, Int);

				Int			 SetValue(Int);
				Int			 GetValue();
			signals:
				Signal1<Void, Int>	 onValueChange;
			slots:
				Void			 OnMouseClick(const Point &);

				Void			 OnMouseDragStart(const Point &);
				Void			 OnMouseDrag(const Point &);
				Void			 OnMouseDragEnd(const Point &);
		};
	};
};

#endif