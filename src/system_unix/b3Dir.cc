/*
**
**	$Filename:	b3Dir.c $
**	$Release:	Dortmund 1998 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III directory routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Dir.h"
#include <sys/types.h>
#include <sys/stat.h>	
#include <unistd.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2001/10/17 14:46:03  sm
**	- Adding triangle support.
**	- Renaming b3TriangleShape into b3Triangles and introducing
**	  new b3TriangleShape as base class. This results in
**	  source file renaming, too.
**	- Fixing soft shadow bug.
**	- Only scene loading background image when activated.
**	- Fixing LDC spline initialization.
**	- Converting Windows paths into right paths on Un*x
**
**	Revision 1.3  2001/07/08 12:30:06  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Dir static methods                          **
**                                                                      **
*************************************************************************/

b3_path_type b3Dir::b3Exists (const char *Name)
{
	struct stat status;

	if (stat(Name,&status) != 0)
	{
		return B3_NOT_EXISTANT;
	}

	return (S_ISDIR(status.st_mode) ? B3_TYPE_DIR : B3_TYPE_FILE);
}

b3_bool b3Dir::b3ChDir (const char *Name)
{
	return (chdir(Name) == 0);
}

b3_bool b3Dir::b3ChkDir (const char *Name)
{
	struct stat status;

	if (stat(Name,&status) != 0)
	{
		return false;
	}
	return S_ISDIR(status.st_mode);
}

b3_bool b3Dir::b3MkDir (const char *Name)
{
	if (b3ChkDir (Name))
	{
		return true;
	}
	return (mkdir(Name,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)==0);
}

/*************************************************************************
**                                                                      **
**                        b3Dir methods                                 **
**                                                                      **
*************************************************************************/

b3Dir::b3Dir()
{
	dir     = null;
	path[0] = 0;
}

// This constructor opens a dir entry directly when
// instantiated
b3Dir::b3Dir (const char *path)
{
	if (!b3OpenDir(path))
	{
		throw new b3DirException(B3_DIR_NOT_FOUND);
	}
}

// The destructor deinitializes the dir entry if necessary
b3Dir::~b3Dir()
{
	b3CloseDir();
}

// Open a directory list
b3_bool b3Dir::b3OpenDir (const char *open_path)
{
	if (dir = opendir (open_path))
	{
		strcpy (path, open_path);
	}
	return dir != null;
}

b3_path_type b3Dir::b3DirNext (char *name)
{
	struct dirent *entry;
	b3_path_type   type;
	char           fileName[B3_FILESTRINGLEN];
	b3_bool        loop;

	name[0] = 0;
	do
	{
		loop = false;
		type = B3_NOT_EXISTANT;
		if (entry = readdir(dir))
		{
			strcpy(name, entry->d_name);
			b3LinkFileName (fileName,path, name);
			type = b3Exists (fileName);
			if (type == B3_TYPE_DIR)
			{
				if (strcmp(name,"..") == 0) loop = true;
				if (strcmp(name,".")  == 0) loop = true;
			}
		}
	}
	while (loop);

	return type;
}

void b3Dir::b3CloseDir ()
{
	if (dir)
	{
		closedir (dir);
		dir = null;
		b3Empty();
	}
}

/*************************************************************************
**                                                                      **
**                        b3Path                                        **
**                                                                      **
*************************************************************************/

// link a name to an existing file path to create a
// new full qualified filepath
void b3Path::b3LinkFileName(
	char       *File,
	const char *FilePath,
	const char *FileName)
{
	b3_size i,len;

	if (File==null) return;
	File[0] = 0;
	if (FilePath != null) strcpy (File,FilePath);
	if (FileName != null)
	{
		if (File[0] != 0) strcat (File,"/");
		strcat (File,FileName);
	}

	// Convert Windows path to to something useful
	len = strlen(File);
	for (i = 0;i < len;i++)
	{
		if (File[i] == '\\') File[i] = '/';
	}
}

// This routine splits a full qualified filename into
// 1. directory name in "path"
// 2a to a filename in "name" if "uncorrected" is a file
// 2b to a filename of zero length if "uncorrected" is a directory
void b3Path::b3SplitFileName(
	const char *File,
	char       *FilePath,
	char       *FileName)
{
	b3_size Length,i;
	b3_bool Dir=false;

	if (File==null) return;

	if (b3Dir::b3Exists(File) == B3_TYPE_DIR)
	{
		if (FilePath) strcpy (FilePath,File);
		if (FileName) FileName[0] = 0;
		return;
	}

	Length = strlen(File);
	if (FileName)
	{
		if (Dir) FileName[0] = 0;
		else
		{
			i = Length;
			while ((i > 0)&&(File[i-1] != '/')&&(File[i-1] != '~')) i--;
			strcpy (FileName,&File[i]);
		}
	}
	if (FilePath)
	{
		strcpy (FilePath,File);
		if (!Dir)
		{
			i = Length;
			while ((i > 0)&&(File[i-1] != '/')&&(File[i-1] != '~')) i--;
			if (i > 1) i--;
			FilePath[i] = 0;	
		}
	}
}

// Remove extension of a file. This routine is needed
// for creating a new extension.
void b3Path::b3RemoveExt(char *name)
{
	char    actPath[B3_FILESTRINGLEN];
	char    actName[B3_FILESTRINGLEN];
	b3_size i,len;

	b3Path::b3SplitFileName (name,actPath,actName);
	len = strlen(actName);
	for (i = 0;i < len;i++)
	{
		if (actName[i] == '.')
		{
			actName[i] = 0;
			len        = i;
		}
	}
	b3Path::b3LinkFileName (name,actPath,actName);
}

void b3Path::b3Empty()
{
	path[0] = 0;
}
