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

#include "blz3/system/b3Mem.h"

#define S_ISDIR(m)          ((m) & S_IFDIR)

#define B3_FILESTRINGLEN 128

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
protected:
	char path[B3_FILESTRINGLEN];

public:
	       void b3Empty();
	       void b3Correct      (const char *path = null);
	static void b3LinkFileName (char *full,const char *path,const char *name);
	static void b3SplitFileName(const char *full,char *path,char *name);
	static void b3ParentName   (const char *path,char *parent);
	static void b3ParentName   (char *parent);
	static void b3RemoveExt    (char *filename);

	inline operator char * ()
	{
		return path;
	}

	inline operator const char *()
	{
		return path;
	}

private:
	static void CorrectFilePath(char *path);
};

class b3DirSystem
{
protected:
	HANDLE           handle;	// handle of FindFirstFile
	WIN32_FIND_DATA  entry;		// structure with file information
};

class b3Dir : public b3Mem, public b3Path, public b3DirSystem
{
	b3_path_type  type;		// type like BExists()
public:
	              b3Dir();
	              b3Dir(const char *);
	             ~b3Dir();
	b3_bool       b3OpenDir(const char *);
	b3_path_type  b3DirNext(char *);
	void          b3CloseDir();

	static b3_bool       b3ChDir        (const char *new_working_dir);
	static b3_bool       b3ChkDir       (const char *test_dir);
	static b3_bool       b3MkDir        (const char *new_dir);
	static b3_bool       b3CreatePath   (const char *base_path,const char *new_dir);
	static b3_path_type  b3Exists       (const char *path);

private:
	static void          b3CreateBasePath(const char *base,const char *dir);
	static void          b3CreateFullPath(const char *dir);
};

class b3Folder : b3Path
{
	BROWSEINFO info;
public:
	        b3Folder(CWnd *parent);
	b3_bool b3SelectFolder(CString &path,const char *title,const char *root = null);
	b3_bool b3SelectFolder(CString &path,const int   title_id,const char *root = null);
private:
	ITEMIDLIST           *b3ConvertItemIDList(const char *szPath);
	static int __stdcall  b3Callback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
};

extern b3_bool b3OpenDialog   (const char *default_file,const char *default_ext,const char *file_filter,char *file_name);
extern b3_bool b3SaveDialog   (const char *default_file,const char *default_ext,const char *file_filter,char *file_name);

#endif
