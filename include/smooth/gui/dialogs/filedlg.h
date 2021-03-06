 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_FILEDLG
#define H_OBJSMOOTH_FILEDLG

namespace smooth
{
	namespace GUI
	{
		namespace Dialogs
		{
			class FileSelection;
		};
	};
};

#include "dialog.h"

namespace smooth
{
	namespace GUI
	{
		namespace Dialogs
		{
			/* File dialog mode constants - SFM_OPEN is default
			 */
			const Short	 SFM_OPEN	= 0;
			const Short	 SFM_SAVE	= 1;

			/* File dialog flags - SFD_FILEMUSTEXIST is always set for SFM_OPEN mode
			 */
			const Short	 SFD_ALLOWMULTISELECT	= 512;
			const Short	 SFD_FILEMUSTEXIST	= 4096;
			const Short	 SFD_CONFIRMOVERWRITE	= 2;

			class SMOOTHAPI FileSelection : public Dialog
			{
				protected:
					Array<String>		 filterNames;
					Array<String>		 filters;
					Array<String>		files;

					Short			 flags;
					Short			 mode;

					String			 defExt;
					String			 defPath;
					String			 defFile;
				public:
								 FileSelection();
					virtual			~FileSelection();

					const Error		&ShowDialog();
				accessors:
					Int			 SetMode(Short);
					Int			 SetFlags(Short);

					Int			 SetInitialPath(const String &);
					Int			 SetDefaultExtension(const String &);

					Int			 AddFilter(const String &, const String &);

					Int			 SetFileName(const String &);
					const String		&GetFileName() const;

					const Array<String>	&GetFileNames() const;
			};
		};
	};
};

#endif
