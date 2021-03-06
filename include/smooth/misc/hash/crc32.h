 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_CRC32
#define H_OBJSMOOTH_CRC32

namespace smooth
{
	namespace Hash
	{
		class CRC32;
	};
};

#include "../string.h"
#include "../../templates/buffer.h"

namespace smooth
{
	namespace Hash
	{
		class SMOOTHAPI CRC32
		{
			private:
				static UnsignedInt32	 table[8][256];
				static Bool		 initialized;

				UnsignedInt32		 crc;

				static Bool		 InitTable();
				static UnsignedInt32	 Reflect(UnsignedInt32, char);
			public:
				static UnsignedInt32	 Compute(const UnsignedByte *, Int);
				static UnsignedInt32	 Compute(const Buffer<UnsignedByte> &);

							 CRC32();
							~CRC32();

				Bool			 Reset();

				Bool			 Feed(const UnsignedByte *, Int);
				Bool			 Feed(const Buffer<UnsignedByte> &);

				UnsignedInt32		 Finish();
		};
	};
};

#endif
