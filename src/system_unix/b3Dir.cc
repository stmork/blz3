/*
**
**	$Filename:	b3Dir.c $
**	$Release:	Dortmund 1998 $
**	$Revision$
**	$Date$
**	$Author$
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
**                        b3Dir static methods                          **
**                                                                      **
*************************************************************************/

b3_path_type b3Dir::b3Exists(const char * Name)
{
	struct stat status;

	if ((Name == nullptr) || (stat(Name, &status) != 0))
	{
		return B3_NOT_EXISTANT;
	}

	return (S_ISDIR(status.st_mode) ? B3_TYPE_DIR : B3_TYPE_FILE);
}

bool b3Dir::b3ChDir(const char * new_working_dir)
{
	return (chdir(new_working_dir) == 0);
}

bool b3Dir::b3ChkDir(const char * Name)
{
	struct stat status;

	if (stat(Name, &status) != 0)
	{
		return false;
	}
	return S_ISDIR(status.st_mode);
}

bool b3Dir::b3MkDir(const char * Name)
{
	if (b3ChkDir(Name))
	{
		return true;
	}
	return mkdir(Name, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
}

bool b3Dir::b3RmDir(const char * dir)
{
	return rmdir(dir) == 0;
}

/*************************************************************************
**                                                                      **
**                        b3Dir methods                                 **
**                                                                      **
*************************************************************************/

b3Dir::b3Dir()
{
	dir = nullptr;
	b3Empty();
}

// This constructor opens a dir entry directly when
// instantiated
b3Dir::b3Dir(const char * path)
{
	if (!b3OpenDir(path))
	{
		B3_THROW(b3DirException, B3_DIR_NOT_FOUND);
	}
}

// The destructor deinitializes the dir entry if necessary
b3Dir::~b3Dir()
{
	b3CloseDir();
}

// Open a directory list
b3_bool b3Dir::b3OpenDir(const char * open_path)
{
	if ((dir = opendir(open_path)) != nullptr)
	{
		strcpy(m_Path, open_path);
	}
	return dir != nullptr;
}

b3_path_type b3Dir::b3DirNext(char * name)
{
	b3_path_type   type;
	char           filename[B3_FILESTRINGLEN];
	b3_bool        loop;

	name[0] = 0;
	do
	{
		struct dirent * entry;

		loop = false;
		type = B3_NOT_EXISTANT;
		if ((entry = readdir(dir)) != nullptr)
		{
			b3Mem::b3StrCpy(name, entry->d_name, B3_FILESTRINGLEN);
			b3LinkFileName(filename, m_Path, name);
			type = b3Exists(filename);
			if (type == B3_TYPE_DIR)
			{
				if (strcmp(name, "..") == 0)
				{
					loop = true;
				}
				if (strcmp(name, ".")  == 0)
				{
					loop = true;
				}
			}
		}
	}
	while (loop);

	return type;
}

void b3Dir::b3CloseDir()
{
	if (dir != nullptr)
	{
		closedir(dir);
		dir = nullptr;
		b3Empty();
	}
}

/*************************************************************************
**                                                                      **
**                        b3Path                                        **
**                                                                      **
*************************************************************************/

b3Path & b3Path::operator=(const char * path)
{
	const char * full_path = realpath(path, nullptr);

	if (full_path != nullptr)
	{
		b3Mem::b3StrCpy(m_Path, full_path, sizeof(m_Path));
		free((void *)full_path);
	}
	else if (m_Path != path)
	{
		b3Mem::b3StrCpy(m_Path, path, sizeof(m_Path));
	}

	return *this;
}

b3Path & b3Path::operator+=(const char * appendix)
{
	b3Path aux(m_Path);

	b3LinkFileName(aux, appendix);

	return *this;
}

b3Path b3Path::operator+(const char * appendix)
{
	b3Path result;

	result.b3LinkFileName(m_Path, appendix);
	return result;
}

void b3Path::b3Empty()
{
	m_Path[0] = 0;
}

void b3Path::b3Correct()
{
	b3Correct(m_Path);
}

void b3Path::b3Correct(char * input)
{
	b3Correct(input, input);
}

void b3Path::b3Correct(const char * input)
{
	b3Correct(input, m_Path);
}

void b3Path::b3Correct(const char * input, char * output)
{
	b3_index i, len;

	assert((input != nullptr) && (output != nullptr));
	len = strlen(input);
	for (i = 0; i < len; i++)
	{
		output[i] = (input[i] == '\\' ? DIR_DELIMITER : input[i]);
	}
	output[i] = 0;
}

// Non static one...
void b3Path::b3LinkFileName(
	const char * param_path,
	const char * param_name)
{
	b3LinkFileName(m_Path, param_path, param_name);
}

// link a name to an existing file path to create a
// new full qualified filepath
void b3Path::b3LinkFileName(
	char    *    File,
	const char * FilePath,
	const char * FileName)
{
	b3_size i, len;

	B3_ASSERT(File != nullptr);

#if 1
	File[0] = 0;
	if (FilePath != nullptr)
	{
		strncat(File, FilePath, B3_FILESTRINGLEN);
	}
	if (FileName != nullptr)
	{
		if ((FilePath != nullptr) && (strlen(FilePath) > 0))
		{
			std::string delimiter;

			delimiter += DIR_DELIMITER;
			strncat(File, delimiter.c_str(), B3_FILESTRINGLEN);
		}
		strncat(File, FileName, B3_FILESTRINGLEN);
	}
#else
	snprintf(File, B3_FILESTRINGLEN,
		"%s%s%s",
		FilePath != nullptr ? FilePath : "",
		FileName != nullptr ? "/" : "",
		FileName != nullptr ? FileName : "");
#endif

	// Convert Windows path to to something useful
	len = strlen(File);
	for (i = 0; i < len; i++)
	{
		if (File[i] == '\\')
		{
			File[i] = DIR_DELIMITER;
		}
	}
}

// Non static one...
void b3Path::b3SplitFileName(
	char * param_path,
	char * param_name)
{
	b3SplitFileName(m_Path, param_path, param_name);
}

// This routine splits a full qualified filename into
// 1. directory name in "path"
// 2a to a filename in "name" if "uncorrected" is a file
// 2b to a filename of zero length if "uncorrected" is a directory
void b3Path::b3SplitFileName(
	const char * File,
	char    *    FilePath,
	char    *    FileName)
{
	b3_size Length, i;
	b3_bool Dir = false;

	if (File == nullptr)
	{
		return;
	}

	if (b3Dir::b3Exists(File) == B3_TYPE_DIR)
	{
		if (FilePath != nullptr)
		{
			b3Mem::b3StrCpy(FilePath, File, B3_FILESTRINGLEN);
		}
		if (FileName != nullptr)
		{
			FileName[0] = 0;
		}
		return;
	}

	Length = strlen(File);
	if (FileName != nullptr)
	{
		if (Dir)
		{
			FileName[0] = 0;
		}
		else
		{
			i = Length;
			while ((i > 0) &&
				(File[i - 1] != DIR_DELIMITER) && (File[i - 1] != '~'))
			{
				i--;
			}
			b3Mem::b3StrCpy(FileName, &File[i], B3_FILESTRINGLEN);
		}
	}
	if (FilePath != nullptr)
	{
		b3Mem::b3StrCpy(FilePath, File, B3_FILESTRINGLEN);
		if (!Dir)
		{
			i = Length;
			while ((i > 0) && (File[i - 1] != DIR_DELIMITER) && (File[i - 1] != '~'))
			{
				i--;
			}
			if (i > 1)
			{
				i--;
			}
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
void b3Path::b3ParentName(char * param_path)
{
	b3ParentName(param_path, param_path);
}

// Static one...
void b3Path::b3ParentName(const char * param_path)
{
	b3ParentName(param_path, m_Path);
}

// Get the parent directory of a directory or file
void b3Path::b3ParentName(
	const char * file,
	char    *    parent)
{
	b3Path   actDir;
	b3_count len;

	B3_ASSERT((file != nullptr) && (parent != nullptr));

	// Source is a file so split directory first
	if (b3Dir::b3Exists(file) == B3_TYPE_FILE)
	{
		b3SplitFileName(file, actDir, nullptr);
	}
	else
	{
		actDir.b3Correct(file);
		for (len = strlen(actDir) - 1; (len >= 0) && (actDir[len] == DIR_DELIMITER); len--)
		{
			actDir[len] = 0;
		}
	}

	// Remove trailing "/" or "\"
	len = strlen(actDir);
	for (b3_index i = len - 1; i >= 0; i--) switch (actDir[i])
		{
		case DIR_DELIMITER :
			actDir[i] = 0;
			i = 0;
			break;

		default :
			actDir[i] = 0;
			break;
		}

	// Copy to destination if available else
	// overwrite source
	strcpy(parent != nullptr ? parent : (char *)file, actDir);
}

// Non static one...
void b3Path::b3RemoveExt()
{
	b3RemoveExt(m_Path);
}

// Static one
void b3Path::b3RemoveExt(char * input)
{
	b3RemoveExt(input, input);
}

// Non static one...
void b3Path::b3RemoveExt(const char * input)
{
	b3RemoveExt(input, m_Path);
}

// Remove extension of a file. This routine is needed
// for creating a new extension.
void b3Path::b3RemoveExt(const char * name, char * output)
{
	char    actPath[B3_FILESTRINGLEN];
	char    actName[B3_FILESTRINGLEN];
	b3_size i = 0;

	assert((name != nullptr) && (output != nullptr));
	b3Path::b3SplitFileName(name, actPath, actName);
	while (actName[i] != 0)
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
	b3Path::b3LinkFileName(output, actPath, actName);
}

// Non static one...
void b3Path::b3ExtractExt()
{
	b3ExtractExt(m_Path);
}

// Static one
void b3Path::b3ExtractExt(char * input)
{
	b3ExtractExt(input, input);
}

// Non static one...
void b3Path::b3ExtractExt(const char * input)
{
	b3ExtractExt(input, m_Path);
}

// Remove extension of a file. This routine is needed
// for creating a new extension.
void b3Path::b3ExtractExt(const char * filename, char * ext)
{
	char     actName[B3_FILESTRINGLEN];
	b3_index i;
	b3_count len;

	b3SplitFileName(filename, nullptr, actName);
	len = strlen(actName);
	for (i = len - 1; i >= 0; i--)
	{
		if (actName[i] == EXT_DELIMITER)
		{
			strcpy(ext, &actName[i + 1]);
			return;
		}
	}
	ext[0] = 0;
}

void b3Path::b3Format(const char * format, ...)
{
	va_list  argptr;

	va_start(argptr, format);
	vsnprintf(m_Path, sizeof(m_Path), format, argptr);
	va_end(argptr);
}

void b3Path::b3Append(const char * ext)
{
	b3_size i, k = 0;

	for (i = strlen(m_Path); i < sizeof(m_Path); i++)
	{
		m_Path[i] = ext[k];
		if (ext[k] == 0)
		{
			return;
		}
		k++;
	}
}

void b3Path::b3Append(const char ext)
{
	b3_size i = strlen(m_Path);

	if (i < (sizeof(m_Path) - 1))
	{
		m_Path[i++] = ext;
		m_Path[i]   = 0;
	}
}
