 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_DRIVER_POSIX
#define H_OBJSMOOTH_DRIVER_POSIX

namespace smooth
{
	namespace IO
	{
		class DriverPOSIX;
	};
};

#include "../../definitions.h"
#include "../driver.h"

namespace smooth
{
	namespace IO
	{
		class SMOOTHAPI DriverPOSIX : public Driver
		{
			private:
				Int	 stream;
				Bool	 closeStream;
			public:
					 DriverPOSIX(const String &, Int);
					 DriverPOSIX(Int);
					~DriverPOSIX();

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
