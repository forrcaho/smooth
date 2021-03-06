 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_DRIVER_ANSI
#define H_OBJSMOOTH_DRIVER_ANSI

namespace smooth
{
	namespace IO
	{
		class DriverANSI;
	};
};

#include <stdio.h>

#include "../../definitions.h"
#include "../driver.h"

namespace smooth
{
	namespace IO
	{
		class SMOOTHAPI DriverANSI : public Driver
		{
			private:
				FILE	*stream;
				Bool	 closeStream;
			public:
					 DriverANSI(const String &, Int);
					 DriverANSI(FILE *);
					~DriverANSI();

				Int	 ReadData(UnsignedByte *, Int);
				Int	 WriteData(UnsignedByte *, Int);

				Int64	 Seek(Int64);

				Bool	 Truncate(Int64);
				Bool	 Flush();

				Bool	 Close();

				Int64	 GetSize() const;
				Int64	 GetPos() const;
		};
	};
};

#endif
