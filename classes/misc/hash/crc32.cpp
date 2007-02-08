/* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/hash/crc32.h>

S::Int	 S::Hash::CRC32::table[256];
S::Bool	 S::Hash::CRC32::initialized = False;

S::Hash::CRC32::CRC32(const Buffer<UnsignedByte> &iBuffer) : buffer(iBuffer)
{
	if (!initialized) InitTable();
}

S::Hash::CRC32::~CRC32()
{
}

S::Void S::Hash::CRC32::InitTable()
{
	UnsignedLong	 ulPolynomial = 0x04c11db7;

	for (Int i = 0; i <= 0xFF; i++)
	{
		table[i] = Reflect(i, 8) << 24;

		for (Int j = 0; j < 8; j++) table[i] = (table[i] << 1) ^ (table[i] & (1 << 31) ? ulPolynomial : 0);

		table[i] = Reflect(table[i], 32);
	}

	initialized = True;
}

S::UnsignedLong S::Hash::CRC32::Reflect(UnsignedLong ref, char ch)
{
	UnsignedLong	 value(0);

	for (Int i = 1; i < (ch + 1); i++)
	{
		if (ref & 1) value |= 1 << (ch - i);

		ref >>= 1;
	}

	return value;
}

S::Int S::Hash::CRC32::Compute() const
{
	UnsignedLong	 ulCRC(0xffffffff);
	Int		 len = buffer.Size();
	UnsignedByte	*pBuffer = buffer;

	while (len--) ulCRC = (ulCRC >> 8) ^ table[(ulCRC & 0xFF) ^ *pBuffer++];

	return ulCRC ^ 0xffffffff;
}