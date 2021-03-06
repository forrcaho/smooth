 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_THREADS_SEMAPHORE
#define H_OBJSMOOTH_THREADS_SEMAPHORE

namespace smooth
{
	namespace Threads
	{
		class Semaphore;
		class SemaphoreBackend;
	};
};

#include "../definitions.h"

namespace smooth
{
	namespace Threads
	{
		class SMOOTHAPI Semaphore
		{
			private:
				SemaphoreBackend	*backend;

				Int			 max;
			public:
							 Semaphore(Int, Void * = NIL);
							 Semaphore(const Semaphore &);
							~Semaphore();

				Semaphore &operator	 =(const Semaphore &);

				Int			 GetSemaphoreType() const;

				Void			*GetSystemSemaphore() const;

				Bool			 Wait();
				Bool			 TryWait();

				Bool			 Release();
		};

		/* A simple scoped locker for semaphores.
		 */
		class SMOOTHAPI Wait
		{
			private:
				Semaphore	&semaphore;
			public:
						 Wait(Semaphore &s) : semaphore(s)	{ semaphore.Wait(); }
						~Wait()					{ semaphore.Release(); }
		};
	};
};

#endif
