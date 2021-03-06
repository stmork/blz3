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
#include "blz3/system/b3Exception.h"
#include "blz3/system/b3DirAbstract.h"

#define S_ISDIR(m)          ((m) & S_IFDIR)

typedef b3Exception<b3_dir_error, 0x444952> b3DirException;

class b3Path : public b3PathAbstract
{
public:
	inline      b3Path()
	{
	}
	inline      b3Path(const char * path) : b3PathAbstract(path)
	{
	}

	void b3Empty();
	void b3LinkFileName(const char * path, const char * name);
	void b3SplitFileName(char * path, char * name);
	void b3RemoveExt(const char * filename);
	void b3RemoveExt();
	void b3ParentName(const char * filename);
	void b3ParentName();
	void b3ExtractExt(const char * filename);
	void b3ExtractExt();
	void b3Correct(const char * path);
	void b3Correct();
	void b3Format(const char * format, ...);
	void b3Append(const char * ext);
	static void b3LinkFileName(char * full, const char * path, const char * name);
	static void b3SplitFileName(const char * full, char * path, char * name);
	static void b3ParentName(const char * path, char * result);
	static void b3ParentName(char * parent);
	static void b3RemoveExt(const char * filename, char * result);
	static void b3RemoveExt(char * result);
	static void b3ExtractExt(const char * filename, char * ext);
	static void b3ExtractExt(char * ext);
	static void b3Correct(const char * path, char * result);
	static void b3Correct(char * path);

	static b3_bool b3OpenDialog(
		const char * default_name,
		const char * default_ext,
		const char * file_filter,
		char    *   file_name);

	static b3_bool b3SaveDialog(
		const char * default_name,
		const char * default_ext,
		const char * file_filter,
		char    *   file_name);

private:
	static void    b3RemoveDelimiter(char * path);
	static b3_bool b3FileDialog(
		const char * default_name,
		const char * default_ext,
		const char * file_filter,
		char    *   file_name,
		bool        is_open);
};

class b3DirSystem
{
protected:
	HANDLE           handle;	// handle of FindFirstFile
	WIN32_FIND_DATA  entry;		// structure with file information

protected:
	b3DirSystem();
};

class b3Dir : public b3DirAbstract, public b3Mem, public b3Path, protected b3DirSystem
{
	b3_path_type  type;		// type like BExists()
public:
	b3Dir();
	b3Dir(const char *);
	virtual      ~b3Dir();
	b3_bool       b3OpenDir(const char *);
	b3_path_type  b3DirNext(char *);
	void          b3CloseDir();

	static b3_bool       b3ChDir(const char * new_working_dir);
	static b3_bool       b3ChkDir(const char * test_dir);
	static b3_bool       b3MkDir(const char * new_dir);
	static b3_bool       b3CreatePath(const char * base_path, const char * new_dir);
	static b3_path_type  b3Exists(const char * path);

private:
	static void          b3CreateBasePath(const char * base, const char * dir);
	static void          b3CreateFullPath(const char * dir);
};

class b3Folder : b3Path
{
	BROWSEINFO info;
public:
	b3Folder(CWnd * parent);
	b3_bool b3SelectFolder(CString & path, const char * title, const char * root = null);
	b3_bool b3SelectFolder(CString & path, const int   title_id, const char * root = null);
private:
	ITEMIDLIST      *     b3ConvertItemIDList(const char * szPath);
	static int __stdcall  b3Callback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
};

extern b3_bool b3OpenDialog(const char * default_file, const char * default_ext, const char * file_filter, char * file_name);
extern b3_bool b3SaveDialog(const char * default_file, const char * default_ext, const char * file_filter, char * file_name);

#endif
