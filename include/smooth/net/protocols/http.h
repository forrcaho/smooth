 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_PROTOCOL_HTTP
#define H_OBJSMOOTH_PROTOCOL_HTTP

#include "protocol.h"
#include "../../io/outstream.h"

namespace smooth
{
	namespace Net
	{
		namespace Protocols
		{
			class SMOOTHAPI Parameter
			{
				public:
					String	 key;
					String	 value;

					Bool	 isFile;

						 Parameter(Int null = 0) { isFile = False; }
			};

			class SMOOTHAPI HTTP : public Protocol
			{
				friend size_t	 httpHeader(char *, size_t, size_t, void *);
				friend size_t	 httpWrite(char *, size_t, size_t, void *);
				friend int	 httpProgress(void *, double, double, double, double);

				private:
					Short			 mode;

					String			 proxy;
					Int			 proxyPort;
					Short			 proxyMode;
					String			 proxyUser;
					String			 proxyPass;

					Array<Parameter>	 requestFields;
					Array<Parameter>	 requestParameters;
					Buffer<UnsignedByte>	 requestBuffer;

					Array<Parameter>	 responseFields;

					String			 file;
					IO::OutStream		*out;

					Int64			 ulStart;
					Int64			 dlStart;

					String			 content;

					String			 GetParametersURLEncoded() const;
				public:
								 HTTP(const String &);
					virtual			~HTTP();

					Int			 SetHeaderField(const String &, const String &);	// Set header field
					String			 GetHeaderField(const String &) const;			// Get header field

					Int			 SetParameter(const String &, const String &);		// Set parameter
					Int			 SetParameterFile(const String &, const String &);	// Set file to be transmitted; forces HTTP_METHOD_POST

					Int			 SetMode(Short);					// Set mode (get or post)

					Int			 SetContent(const String &);				// Set content to be transmitted; forces HTTP_METHOD_POST

					Int			 SetProxy(const String &, Int);				// Set proxy host name and port
					Int			 SetProxyMode(Short);					// Set proxy mode
					Int			 SetProxyAuth(const String &, const String &);		// Set user name and password for proxy

					String			 GetResponseHeaderField(const String &) const;

					Int			 DownloadToFile(const String &);
			};

			const Short	 HTTP_METHOD_GET	= 0;
			const Short	 HTTP_METHOD_POST	= 1;

			const Short	 HTTP_PROXY_NONE	= 0;
			const Short	 HTTP_PROXY_HTTP	= 1;
			const Short	 HTTP_PROXY_HTTPS	= 2;
			const Short	 HTTP_PROXY_SOCKS4	= 3;
			const Short	 HTTP_PROXY_SOCKS5	= 4;
		};
	};
};

#endif
