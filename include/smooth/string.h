 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_STRING_
#define _H_OBJSMOOTH_STRING_

namespace smooth
{
	class String;
	class Timer;
};

#include "definitions.h"
#include "array.h"

namespace smooth
{
	class SMOOTHAPI String
	{
		private:
			wchar_t			*wString;
			Int			 stringSize;

			static char		*origInputFormat;
			static char		*origOutputFormat;

			static char		*inputFormat;
			static char		*outputFormat;

			static Array<char *>	 allocatedBuffers;

			Void			 ImportFormat(const char *, const char *);
		public:
						 String();
						 String(const int);
						 String(const char *);
						 String(const wchar_t *);
						 String(const String &);

						~String();

			static Void		 ClearTemporaryBuffers();

			Void			 Clean();

			static char		*SetInputFormat(const char *);
			static char		*SetOutputFormat(const char *);

			char			*ConvertTo(const char *);

			String			&Append(const char *);
			String			&Append(const wchar_t *);
			String			&Append(const String &);

			String			&Copy(const char *);
			String			&Copy(const wchar_t *);
			String			&Copy(const String &);

			String			&CopyN(const char *, const Int);
			String			&CopyN(const wchar_t *, const Int);
			String			&CopyN(const String &, const Int);

			Int			 Compare(const char *);
			Int			 Compare(const wchar_t *);
			Int			 Compare(const String &);

			Int			 CompareN(const char *, const Int);
			Int			 CompareN(const wchar_t *, const Int);
			Int			 CompareN(const String &, const Int);

			String			&Fill(const Int);
			String			&FillN(const Int, const Int);

			Int			 Length();

			Int			 ToInt();
			Float			 ToFloat();

			static String		 FromInt(const Int);
			static String		 FromFloat(const Float);

			wchar_t &operator	 [](const int);
			wchar_t &operator	 [](const Int);

			operator		 char *();
			operator		 wchar_t *();

			String &operator	 =(const int);
			String &operator	 =(const char *);
			String &operator	 =(const wchar_t *);
			String &operator	 =(const String &);

			Bool operator		 ==(const int);
			Bool operator		 ==(const char *);
			Bool operator		 ==(const wchar_t *);
			Bool operator		 ==(const String &);

			Bool operator		 !=(const int);
			Bool operator		 !=(const char *);
			Bool operator		 !=(const wchar_t *);
			Bool operator		 !=(const String &);
	};

	Int	 ConvertString(const char *, Int, const char *, char *, Int, const char *);
};

#endif
