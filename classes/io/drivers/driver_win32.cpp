 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_win32.h>

S::IO::DriverWin32::DriverWin32(const String &fileName, Int mode) : Driver()
{
	closeStream = False;

	switch (mode)
	{
		default:
			lastError = IO_ERROR_BADPARAM;
			return;
		case 0:				// open a file for appending data
			if (Setup::enableUnicode)	stream = CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			else				stream = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			Seek(GetSize());
			break;
		case 1:				// create or overwrite a file
			if (Setup::enableUnicode)	stream = CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			else				stream = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
		case 2:				// open a file for reading data
			if (Setup::enableUnicode)	stream = CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			else				stream = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
		case 3:				// open a file in read only mode
			if (Setup::enableUnicode)	stream = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			else				stream = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
	}

	if (stream == INVALID_HANDLE_VALUE)
	{
		lastError = IO_ERROR_UNEXPECTED;

		return;
	}

	closeStream = true;
}

S::IO::DriverWin32::DriverWin32(HANDLE iStream) : Driver()
{
	stream		= iStream;
	closeStream	= false;
}

S::IO::DriverWin32::~DriverWin32()
{
	if (closeStream) CloseHandle(stream);
}

S::Int S::IO::DriverWin32::ReadData(UnsignedByte *data, Int dataSize)
{
	Unsigned long	 bytes;

	ReadFile(stream, (void *) data, (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos())), &bytes, NULL);

	return bytes;
}

S::Int S::IO::DriverWin32::WriteData(UnsignedByte *data, Int dataSize)
{
	UnsignedLong	 bytes;

	WriteFile(stream, (void *) data, dataSize, &bytes, NULL);

	return bytes;
}

S::Int64 S::IO::DriverWin32::Seek(Int64 newPos)
{
	Int32	 hi32 = newPos >> 32;
	Int64	 lo32 = SetFilePointer(stream, newPos, &hi32, FILE_BEGIN);

	return (Int64) hi32 << 32 | lo32;
}

S::Int64 S::IO::DriverWin32::GetSize()
{
	Int32	 hi32 = 0;
	Int64	 lo32 = GetFileSize(stream, (UnsignedLong *) &hi32);

	return (Int64) hi32 << 32 | lo32;
}

S::Int64 S::IO::DriverWin32::GetPos()
{
	Int32	 hi32 = 0;
	Int64	 lo32 = SetFilePointer(stream, 0, &hi32, FILE_CURRENT);

	return (Int64) hi32 << 32 | lo32;
}
