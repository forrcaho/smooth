 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ERROR_ENDOFFILE_
#define _H_OBJSMOOTH_ERROR_ENDOFFILE_

namespace smooth
{
	namespace Error
	{
		class EndOfFile;
	};
};

#include "../error.h"

namespace smooth
{
	namespace Error
	{
		class SMOOTHAPI EndOfFile : public Error
		{
			public:
				static const Int	 classID;

							 EndOfFile();
		};
	};
};

#endif
