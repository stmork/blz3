/*
**
**	$Filename:	b3Dir.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - directory routines (proto types)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_DIR_H
#define B3_SYSTEM_DIR_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Mem.h"

#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>

#define FILESTRINGLEN 128

	typedef enum
{
	B3_NOT_EXISTANT = 0,
	B3_TYPE_DIR,
	B3_TYPE_FILE
} b3_path_type;


typedef enum
{
	B3_DIR_ERROR = -1,
	B3_DIR_OK    =  0,
	B3_DIR_NOT_FOUND
} b3_dir_error;

class b3DirException
{
protected:
	b3_dir_error error;

public:
	b3DirException(b3_dir_error error = B3_DIR_ERROR)
	{
		this->error = error;
	}

	b3_dir_error b3GetError()
	{
		return error;
	}
};

class b3Path
{
public:
	char path[FILESTRINGLEN];

public:
	static void b3LinkFileName (char *full,const char *path,const char *name);
	static void b3SplitFileName(const char *full,char *path,char *name);
	static void b3ParentName   (char *path,char *parent = null);
	static void b3RemoveExt    (char *);

	       void b3Empty();

	operator char * ()
	{
		return path;
	}

	operator const char *()
	{
		return path;
	}
};

class b3Dir : public b3Mem, public b3Path
{
protected:
	DIR                 *dir;
public:
	                     b3Dir();
	                     b3Dir(const char *dirName);
	                    ~b3Dir();
	       b3_bool       b3OpenDir(const char *dirName);
	       b3_path_type  b3DirNext(char *result);
	       void          b3CloseDir();

	static b3_bool       b3ChDir        (const char *new_working_dir);
	static b3_bool       b3ChkDir       (const char *test_dir);
	static b3_bool       b3MkDir        (const char *new_dir);
	static b3_bool       b3CreatePath   (const char *base_path,const char *new_dir);
	static b3_path_type  b3Exists       (const char *path);
};

#endif
