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
#include <assert.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.15  2003/08/28 14:44:26  sm
**	- Further buffer overflow prevention:
**	  o added b3Path::b3Format
**	  o added b3Path::b3Append
**	- Further strcat/strcpy removal necessary
**
**	Revision 1.14  2003/05/26 11:20:52  sm
**	- Used wrong data types for b3Path::b3ExtractExt() methods. Now using
**	  signed versus unsigned int.
**	
**	Revision 1.13  2002/08/16 11:40:39  sm
**	- Changed vertex handling for use without OpenGL. Vertex computation
**	  is needed for bound computation which is needed for animation. There
**	  are still some problems so we have to work further on Windows for
**	  better debugging.
**	- b3ExtractExt searches from right instead from left.
**	
**	Revision 1.12  2002/08/15 13:56:44  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.11  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.10  2002/01/17 15:46:00  sm
**	- CAppRaytraceDoc.cpp cleaned up for later use from CAppObjectDoc.
**	- Opening a CAppRaytraceDoc for all image extensions.
**	
**	Revision 1.9  2002/01/06 21:38:18  sm
**	- Nasty Un CR/LF
**	- b3Dir fix. Not tested, yet!
**	- make count/make count_all update
**	
**	Revision 1.8  2002/01/06 16:30:47  sm
**	- Added Load/Save/Replace object
**	- Enhanced "New world"
**	- Added some non static methods to b3Dir (Un*x untested, yet!)
**	- Fixed missing sphere/ellipsoid south pole triangles
**	- Fixed Spline cylinder/ring triangle indexing
**	
**	Revision 1.7  2001/12/02 17:38:17  sm
**	- Removing nasty CR/LF
**	- Added b3ExtractExt()
**	- Added stricmp() for Un*x
**	- Fixed some defines
**	
**	Revision 1.6  2001/11/11 14:07:15  sm
**	- Adjusted b3Path to Un*x port.
**	
**	Revision 1.5  2001/10/19 14:46:58  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
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
	dir       = null;
	b3Empty();
}

// This constructor opens a dir entry directly when
// instantiated
b3Dir::b3Dir (const char *path)
{
	if (!b3OpenDir(path))
	{
		B3_THROW(b3DirException,B3_DIR_NOT_FOUND);
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
	if ((dir = opendir (open_path)) != null)
	{
		strcpy (m_Path, open_path);
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
		if ((entry = readdir(dir)) != null)
		{
			strcpy(name, entry->d_name);
			b3LinkFileName (fileName,m_Path, name);
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

void b3Path::b3Empty()
{
	m_Path[0] = 0;
}

void b3Path::b3Correct()
{
	b3Correct(m_Path);
}

void b3Path::b3Correct(char *input)
{
	b3Correct(input,input);
}

void b3Path::b3Correct(const char *input)
{
	b3Correct(input,m_Path);
}

void b3Path::b3Correct(const char *input,char *output)
{
	b3_index i,len;

	assert((input != null) && (output != null));
	len = strlen(input);
	for (i = 0;i < len;i++)
	{
		output[i] = (input[i] == '\\' ? '/' : input[i]);
	}
	output[i] = 0;
}

// Non static one...
void b3Path::b3LinkFileName(
	const char *param_path,
	const char *param_name)
{
	b3LinkFileName(m_Path,param_path,param_name);
}

// link a name to an existing file path to create a
// new full qualified filepath
void b3Path::b3LinkFileName(
	char       *File,
	const char *FilePath,
	const char *FileName)
{
	b3_size i,len;

	assert(File!=null);
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

// Non static one...
void b3Path::b3SplitFileName(
	char *param_path,
	char *param_name)
{
	b3SplitFileName(m_Path,param_path,param_name);
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

	if (File == null)
	{
		return;
	}

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

// Non static one...
void b3Path::b3ParentName()
{
	b3ParentName(m_Path);
}

// Source and destination are the same (static one)
void b3Path::b3ParentName(char *param_path)
{
	b3ParentName(param_path,param_path);
}

// Static one...
void b3Path::b3ParentName(const char *param_path)
{
	b3ParentName(param_path,m_Path);
}

// Get the parent directory of a directory or file
void b3Path::b3ParentName(
	const char *file,
	      char *parent)
{
	b3Path   actDir;
	b3_index i;
	b3_count len;

	assert((file != null) && (parent != null));

	// Source is a file so split directory first
	if (b3Dir::b3Exists(file) == B3_TYPE_FILE)
	{
		b3SplitFileName (file,actDir,null);
	}
	else
	{
		actDir.b3Correct(file);
		for (len = strlen(actDir) - 1;(len >= 0) && (((char *)actDir)[len] == '/');len--)
		{
			((char *)actDir)[len] = 0;
		}
	}

	// Remove trailing "/" or "\"
	len = strlen(actDir);
	for (i = len - 1;i >= 0;i--) switch(((char *)actDir)[i])
	{
		case '/' :
			((char *)actDir)[i] = 0;
			i = 0;
			break;

		default :
			((char *)actDir)[i] = 0;
			break;
	}

	// Copy to destination if available else
	// overwrite source
	strcpy (parent != null ? parent : (char *)file,actDir);
}

// Non static one...
void b3Path::b3RemoveExt()
{
	b3RemoveExt(m_Path);
}

// Static one
void b3Path::b3RemoveExt(char *input)
{
	b3RemoveExt(input,input);
}

// Non static one...
void b3Path::b3RemoveExt(const char *input)
{
	b3RemoveExt(input,m_Path);
}

// Remove extension of a file. This routine is needed
// for creating a new extension.
void b3Path::b3RemoveExt(const char *name,char *output)
{
	char    actPath[B3_FILESTRINGLEN];
	char    actName[B3_FILESTRINGLEN];
	b3_size i = 0;

	assert((name != null) && (output != null));
	b3Path::b3SplitFileName (name,actPath,actName);
	while(actName[i] != 0)
	{
		if (actName[i] == '.')
		{
			actName[i] = 0;
		}
		else
		{
			i++;
		}
	}
	b3Path::b3LinkFileName (output,actPath,actName);
}

// Non static one...
void b3Path::b3ExtractExt()
{
	b3ExtractExt(m_Path);
}

// Static one
void b3Path::b3ExtractExt(char *input)
{
	b3ExtractExt(input,input);
}

// Non static one...
void b3Path::b3ExtractExt(const char *input)
{
	b3ExtractExt(input,m_Path);
}

// Remove extension of a file. This routine is needed
// for creating a new extension.
void b3Path::b3ExtractExt(const char *filename,char *ext)
{
	char     actName[B3_FILESTRINGLEN];
	b3_index i;
	b3_count len;

	b3SplitFileName (filename,null,actName);
	len = strlen(actName);
	for (i = len - 1;i >= 0;i--)
	{
		if (actName[i] == '.')
		{
			strcpy(ext,&actName[i+1]);
			return;
		}
	}
	ext[0] = 0;
}

void b3Path::b3Format(const char *format,...)
{
	va_list  argptr;

	va_start (argptr,format);
	vsnprintf(m_Path,sizeof(m_Path),format,argptr);
	va_end   (argptr);
}

void b3Path::b3Append(const char *ext)
{
	b3_size i,k=0;

	for (i = strlen(m_Path);i < sizeof(m_Path);i++)
	{
		m_Path[i] = ext[k];
		if (ext[k] == 0)
		{
			return;
		}
		k++;
	}
}
