/*
**
**	$Filename:	b3DirAbstract.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Basic directory operations
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_DIRABSTRACT_H
#define B3_SYSTEM_DIRABSTRACT_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Exception.h"

/**
 * This enumeration lists the types of a file system item.
 *
 * @see b3DirAbstract
 */
enum b3_path_type
{
	B3_NOT_EXISTANT = 0, //!< File system item not existant.
	B3_TYPE_DIR,         //!< File system item is a directory.
	B3_TYPE_FILE         //!< File system item is a file.
};

/**
 * This enumeration lists error states for directory handling.
 */
enum b3_dir_error
{
	B3_DIR_ERROR = -1,   //!< General directory error.
	B3_DIR_OK    =  0,   //!< Directory handling OK.
	B3_DIR_NOT_FOUND     //!< Directory not found.
};

/**
 * This class handles with paths.
 */
class B3_PLUGIN b3PathAbstract
{
protected:
	char m_Path[B3_FILESTRINGLEN];

public:
			inline b3PathAbstract()
			{
			}

	        inline b3PathAbstract(const char *path)
			{
				strcpy (m_Path,path);
			}

	virtual void b3Empty        () = 0;
	virtual void b3LinkFileName (const char *path,const char *name) = 0;
	virtual void b3SplitFileName(char *path,char *name) = 0;
	virtual void b3RemoveExt    (const char *filename) = 0;
	virtual void b3RemoveExt    () = 0;
	virtual void b3ParentName   (const char *filename) = 0;
	virtual void b3ParentName   () = 0;
	virtual void b3ExtractExt   (const char *filename) = 0;
	virtual void b3ExtractExt   () = 0;
	virtual void b3Correct      (const char *path) = 0;
	virtual void b3Correct      () = 0;
	virtual void b3Format       (const char *format,...) = 0;
	virtual void b3Append       (const char *ext) = 0;

	inline operator char * ()
	{
		return m_Path;
	}

	inline operator const char *()
	{
		return m_Path;
	}

private:
	static void b3RemoveDelimiter(char *path);
};

/**
 * This class handles directory listing.
 */
class B3_PLUGIN b3DirAbstract
{
public:
	/**
	 * This method opens a directory for listing.
	 *
	 * \param *dirname The directory name to list.
	 * \return True on success.
	 */
	virtual b3_bool       b3OpenDir(const char *dirname) = 0;

	/**
	 * This method retrieves the next directory entry.
	 *
	 * \param *direntry The name of the directory entry.
	 * \return Type of directory entry.
	 */
	virtual b3_path_type  b3DirNext(char *direntry) = 0;

	/**
	 * This method closes this handler for directory listing.
	 */
	virtual void          b3CloseDir() = 0;
};

#endif
