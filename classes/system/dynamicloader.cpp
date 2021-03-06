 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/dynamicloader.h>
#include <smooth/files/directory.h>
#include <smooth/io/instream.h>
#include <smooth/gui/application/application.h>
#include <smooth/foreach.h>

#ifdef __linux__
#	include <smooth/backends/xlib/backendxlib.h>
#endif

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <stdlib.h>
#	include <dlfcn.h>
#endif

const S::Short	 S::System::DynamicLoader::classID = S::Object::RequestClassID();

S::System::DynamicLoader::DynamicLoader(const String &module) : handle(NIL)
{
#if defined __WIN32__
	static String	 dllExt		= ".dll";
	static String	 versionPattern	= dllExt;
#elif defined __APPLE__
	static String	 dllExt		= ".dylib";
	static String	 versionPattern	= String(".*").Append(dllExt);
#else
	static String	 dllExt		= ".so";
	static String	 versionPattern	= String(dllExt).Append(".*");
#endif

#ifdef __WIN32__
	if (module[1] != ':')
	{
		/* Try the supplied module name in application directory.
		 */
		handle = LoadLibrary(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
	}

	if (handle == NIL)
	{
		/* Try the supplied module name system wide.
		 */
		handle = LoadLibrary(String(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
	}
#else
	Int	 dlopenFlags = RTLD_NOW | RTLD_LOCAL;

#if defined __FreeBSD__ || defined __NetBSD__
	dlopenFlags |= RTLD_NODELETE;
#endif

	if (!module.StartsWith("/"))
	{
		/* Try the supplied module name in application directory.
		 */
		handle = dlopen(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(dllExt) || module.Contains(String(dllExt).Append(".")) ? String() : dllExt), dlopenFlags);
	}

	if (handle == NIL)
	{
		/* Try the supplied module name system wide.
		 */
		handle = dlopen(String(module).Append(module.EndsWith(dllExt) || module.Contains(String(dllExt).Append(".")) ? String() : dllExt), dlopenFlags);
	}

	if (handle == NIL)
	{
		const Array<String> &directories = GetLibraryDirectories();

		/* Try loading an unversioned library.
		 */
		foreach (const String &path, directories)
		{
			Directory		 directory(path);
			const Array<File>	&files = directory.GetFilesByPattern(String(module.StartsWith("lib") || module.Contains("/") ? String() : "lib").Append(module).Append(module.EndsWith(dllExt) || module.Contains(String(dllExt).Append(".")) ? String() : dllExt));

			if (files.Length() > 0)
			{
				handle = dlopen((String) files.GetFirst(), dlopenFlags);
			}

			if (handle != NIL) return;
		}

		/* Try loading a versioned library.
		 */
		foreach (const String &path, directories)
		{
			Directory		 directory(path);
			const Array<File>	&files = directory.GetFilesByPattern(String(module.StartsWith("lib") || module.Contains("/") ? String() : "lib").Append(module).Append(module.EndsWith(dllExt) || module.Contains(String(dllExt).Append(".")) ? String() : versionPattern));

			if (files.Length() > 0)
			{
				handle = dlopen((String) files.GetFirst(), dlopenFlags);
			}

			if (handle != NIL) return;
		}
	}
#endif
}

S::System::DynamicLoader::~DynamicLoader()
{
#ifdef __WIN32__
	if (handle != NIL) FreeLibrary((HINSTANCE) handle);
#else
#if defined __OpenBSD__ || defined __HAIKU__
	/* OpenBSD and Haiku do not support RTLD_NODELETE yet,
	 * so we cannot close .so files without risking a crash.
	 */
#else
	if (handle != NIL) dlclose(handle);
#endif
#endif
}

S::Void *S::System::DynamicLoader::GetFunctionAddress(const String &functionName) const
{
	if (handle == NIL) return NIL;

#ifdef __WIN32__
	return (Void *) GetProcAddress((HINSTANCE) handle, functionName);
#else
	return (Void *) dlsym(handle, functionName);
#endif
}

S::Void *S::System::DynamicLoader::GetSystemModuleHandle() const
{
	return handle;
}

const S::Array<S::String> &S::System::DynamicLoader::GetLibraryDirectories()
{
	static Array<String>	 directories;

#ifndef __WIN32__
	if (directories.Length() == 0)
	{
		/* Look for /usr/lib and /usr/local/lib on all systems.
		 */
		if (Directory("/usr/lib").Exists())	    directories.Add("/usr/lib");
		if (Directory("/usr/local/lib").Exists())   directories.Add("/usr/local/lib");

#if defined __APPLE__
		/* Look for library directories of ports projects.
		 */
		if (Directory("/opt/local/lib").Exists())   directories.Add("/opt/local/lib");
		if (Directory("/sw/lib").Exists())	    directories.Add("/sw/lib");
#elif defined __linux__
		/* Query X11 library path to handle architecture-specific library paths on Linux.
		 */
		Dl_info	 info = { 0 };

		dladdr((void *) &X11::XOpenDisplay, &info);

		const String	 libraryPath = File(info.dli_fname).GetFilePath();

		directories.Add(libraryPath);
#elif defined __HAIKU__
		/* Query smooth library path to handle architecture suffix on Haiku.
		 */
		Dl_info	 info = { 0 };

		dladdr((void *) &S::System::DynamicLoader::GetLibraryDirectories, &info);

		const String	 libraryPath = File(info.dli_fname).GetFilePath();

		directories.Add(libraryPath);

		if	(libraryPath.Contains("/system/non-packaged/")) directories.Add(libraryPath.Replace("/system/non-packaged/", "/system/"));
		else if (libraryPath.Contains("/system/"))		directories.Add(libraryPath.Replace("/system/", "/system/non-packaged/"));
#elif defined __NetBSD__
		/* Packages live in /usr/pkg on NetBSD.
		 */
		if (Directory("/usr/pkg/lib").Exists())	    directories.Add("/usr/pkg/lib");
#endif

		/* Parse /etc/ld.so.conf if it exists.
		 */
		if (File("/etc/ld.so.conf").Exists())	    ParseDirectoryList("/etc/ld.so.conf", directories);

		/* Parse LD_LIBRARY_PATH environment variable.
		 */
		const Array<String>	&paths = String(getenv("LD_LIBRARY_PATH")).Explode(":");

		foreach (const String &path, paths) if (Directory(path).Exists()) directories.Add(path);

		String::ExplodeFinish();
	}
#endif

	return directories;
}

S::Void S::System::DynamicLoader::ParseDirectoryList(const String &pattern, Array<String> &directories)
{
#ifndef __WIN32__
	Directory		 directory(File(pattern).GetFilePath());
	const Array<File>	&files = directory.GetFilesByPattern(File(pattern).GetFileName());

	foreach (const File &file, files)
	{
		IO::InStream	 in(IO::STREAM_FILE, file);

		while (in.GetPos() < in.Size())
		{
			String	 line = in.InputLine();

			if	(line == NIL)			continue;
			else if (line.StartsWith("#"))		continue;
			else if (line.StartsWith("include "))	ParseDirectoryList(line.Tail(line.Length() - 8), directories);
			else					directories.Add(line);
		}
	}
#endif
}
