 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ITERATOR_
#define _H_OBJSMOOTH_ITERATOR_

#include "../array.h"

namespace smooth
{
	template <class t> class Iterator
	{
		protected:
			Int		 index;
			const Array<t>	&array;
		public:
					 Iterator(const Array<t> &iArray) : array(iArray)	{ index = -1; }
			virtual		~Iterator()						{ }

			Bool		 HasNext() const					{ return index + 1 < array.GetNOfEntries(); }

			const t		&Next()							{ return array.GetNth(++index); }
	};
};

#endif
