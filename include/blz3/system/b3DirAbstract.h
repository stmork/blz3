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

enum b3_path_type
{
	B3_NOT_EXISTANT = 0,
	B3_TYPE_DIR,
	B3_TYPE_FILE
};

enum b3_dir_error
{
	B3_DIR_ERROR = -1,
	B3_DIR_OK    =  0,
	B3_DIR_NOT_FOUND
};

class b3PathAbstract
{
protected:
	char m_Path[B3_FILESTRINGLEN];

public:
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

class b3DirAbstract
{
public:
	virtual b3_bool       b3OpenDir(const char *) = 0;
	virtual b3_path_type  b3DirNext(char *) = 0;
	virtual void          b3CloseDir() = 0;
};

#endif
