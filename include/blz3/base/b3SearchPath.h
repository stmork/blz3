/*
**
**	$Filename:	b3SearchPath.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Search path routines
**
**	(C) Copyright 2001 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_SEARCHPATH_H
#define B3_BASE_SEARCHPATH_H

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"
#include "blz3/system/b3Dir.h"

/**
 * This class implements a single search path.
 *
 * @see b3SearchPath
 */
class B3_PLUGIN b3PathEntry : public b3Link<b3PathEntry>, public b3Path
{
	b3PathEntry() = delete;

public:
	/**
	 * This constructor initializes this instance with a path.
	 */
	explicit b3PathEntry(const char * path);
};

/**
 * This class represents a list of search paths to find single file names.
 *
 * @warning
 * The file name handling is not buffer overflow safe!
 */
class B3_PLUGIN b3SearchPath
{
protected:
	b3Base<b3PathEntry> m_SearchPath; //!< This is the list of search paths.

public:
	/**
	 * This constructor initializes the path search list.
	 */
	b3SearchPath() = default;

	/**
	 * This destructor deinitializes the instance using the b3Empty() method.
	 */
	virtual ~b3SearchPath();

	/**
	 * This method adds a search path to the list. The given path must be
	 * a directory. Other file system items are simply ignored.
	 *
	 * @param path The new search path.
	 */
	void     b3AddPath(const char * path);

	/**
	 * This deletes all entries from the path search list.
	 */
	void     b3Empty();

	/**
	 * This method checks wether the given file name is in the search path
	 * as file. The fully qualified file name may returned if wanted.
	 *
	 * @param testname The name to test. This file may contain parent directory
	 * names. A @c nullptr always leads into @c false as return value.
	 * @param fullname The resulting full qualified file name. This may be
	 * @c nullptr.
	 * @return True if a full qualified file name was found.
	 * @see b3Path::b3LinkFileName
	 */
	b3_bool  b3IsValid(const char * testname, char * fullname = nullptr);

	/**
	 * This method cuts the search path from the full qualified name if any. If no
	 * matching path was found the full name is returned.
	 *
	 * @param fullname The full qualified name to test.
	 * @param shortname The cut name.
	 * @return True if a matching search path was found.
	 */
	b3_bool  b3CutName(const char * fullname, char * shortname);
};

#endif
