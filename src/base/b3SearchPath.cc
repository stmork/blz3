/*
**
**	$Filename:	b3SearchPath.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Search path routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3SearchPath.h"
#include "blz3/base/b3Aux.h"

/*************************************************************************
**                                                                      **
**                        Texture maintainance                          **
**                                                                      **
*************************************************************************/

b3PathEntry::b3PathEntry(const char * new_path) :
	b3Link<b3PathEntry>(sizeof(b3PathEntry))
{
	snprintf(m_Path, sizeof(m_Path), "%s", new_path != nullptr ? new_path : "");
}

void b3SearchPath::b3AddPath(const char * path)
{
	if (b3Dir::b3Exists(path) == B3_TYPE_DIR)
	{
		b3PathEntry * path_item = new (std::nothrow) b3PathEntry(path);

		if (path_item != nullptr)
		{
			b3PrintF(B3LOG_DEBUG, "Adding search path \"%s\"\n",
				(const char *)*path_item);
			m_SearchPath.b3Append(path_item);
		}
	}
#ifdef _DEBUG
	else
	{
		b3PrintF(B3LOG_DEBUG, "Path %s not added.\n", path);
	}
#endif
}

b3SearchPath::~b3SearchPath()
{
	b3Empty();
}

void b3SearchPath::b3Empty()
{
	m_SearchPath.b3Free();
}

b3_bool b3SearchPath::b3IsValid(
	const char * partial_filename,
	char    *    full_filename)
{
	b3PathEntry * path;

	if ((partial_filename != nullptr) && (strlen(partial_filename) > 0))
	{
		b3PrintF(B3LOG_FULL, "Trying \"%s\"...\n", (char *)partial_filename);
		if (b3Dir::b3Exists(partial_filename) == B3_TYPE_FILE)
		{
			if (full_filename != nullptr)
			{
				b3Mem::b3StrCpy(full_filename, partial_filename, B3_FILESTRINGLEN);
			}
			return true;
		}
		else
		{
			B3_FOR_BASE(&m_SearchPath, path)
			{
				b3Path fq_filename;

				fq_filename.b3LinkFileName((const char *)*path, partial_filename);
				b3PrintF(B3LOG_FULL, "Trying \"%s\"...", (const char *)full_filename);

				if (b3Dir::b3Exists(fq_filename) == B3_TYPE_FILE)
				{
					b3PrintF(B3LOG_FULL, " OK!\n");

					if (full_filename != nullptr)
					{
						b3Mem::b3StrCpy(full_filename, fq_filename, B3_FILESTRINGLEN);
					}
					return true;
				}
				else
				{
					b3PrintF(B3LOG_FULL, "\n");
				}
			}
		}
	}

	return false;
}

b3_bool b3SearchPath::b3CutName(const char * fullname, char * result)
{
	b3PathEntry * path;

	if (result == nullptr)
	{
		throw std::invalid_argument("Need result buffer!");
	}
	B3_FOR_BASE(&m_SearchPath, path)
	{
		b3_size shortLen = strlen(*path);

		if (strncmp(fullname, *path, shortLen) == 0)
		{
			while (((fullname[shortLen] == '/') ||
					(fullname[shortLen] == '\\')) &&
				(fullname[shortLen] != 0))
			{
				shortLen++;
			}
			b3Mem::b3StrCpy(result, &fullname[shortLen], B3_FILESTRINGLEN);
			return true;
		}
	}
	b3Mem::b3StrCpy(result, fullname, B3_FILESTRINGLEN);
	return false;
}
