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

#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>

#include "blz3/b3Types.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Exception.h"
#include "blz3/system/b3DirAbstract.h"

typedef b3Exception<b3_dir_error,'DIR'> b3DirException;

class b3Path : public b3PathAbstract
{
public:
	inline      b3Path() : b3PathAbstract()
	{
	}

	inline      b3Path(const char *path) : b3PathAbstract(path)
	{
	}

	       void b3Empty();
		   void b3LinkFileName (const char *path,const char *name);
	       void b3SplitFileName(char *path,char *name);
	       void b3RemoveExt    (const char *filename);
	       void b3RemoveExt    ();
		   void b3ParentName   (const char *filename);
	       void b3ParentName   ();
		   void b3ExtractExt   (const char *filename);
		   void b3ExtractExt   ();
	       void b3Correct      (const char *path);
		   void b3Correct      ();
		   void b3Format       (const char *format,...);
		   void b3Append       (const char *ext);
	static void b3LinkFileName (char *full,const char *path,const char *name);
	static void b3SplitFileName(const char *full,char *path,char *name);
	static void b3ParentName   (const char *path,char *result);
	static void b3ParentName   (char *parent);
	static void b3RemoveExt    (const char *filename,char *result);
	static void b3RemoveExt    (char *result);
	static void b3ExtractExt   (const char *filename,char *ext);
	static void b3ExtractExt   (char *ext);
	static void b3Correct      (const char *path,char *result);
	static void b3Correct      (char *path);
};

class b3Dir : public b3DirAbstract, public b3Mem, public b3Path
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
