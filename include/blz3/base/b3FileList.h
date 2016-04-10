/*
**
**	$Filename:	b3FileList.h $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - file list creation
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_BASE_FILELIST_H
#define B3_BASE_FILELIST_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Dir.h"
#include "blz3/base/b3Compare.h"
#include "blz3/base/b3List.h"

/**
 * This class represents one file list entry.
 *
 * @see b3FileList
 */
class B3_PLUGIN b3FileEntry : public b3Link<b3FileEntry>, public b3Comparator<b3FileEntry>
{
	b3Path       name;

public:
	/**
	 * This constructor creates one file list entry from the given file name.
	 *
	 * @param new_name The new file name.
	 */
	b3FileEntry(const char *new_name);

	/**
	 * This method implements the comparison method for the b3FileList::b3Sort
	 * method.
	 *
	 * @param compare The file entry to compare with.
	 * @return The position indicator.
	 */
	const int    b3Cmp(const b3FileEntry *compare) const;

	/**
	 * This method returns the file name of this entry.
	 *
	 * @return File name.
	 */
	const char  *b3Name() const;
};

/**
 * This class holds a file list.
 *
 * @see b3FileEntry.
 */
class b3FileList
{
	b3Base<b3FileEntry> list;
public:
	/**
	 * This constructor initializes this instance.
	 */
	b3FileList();

	/**
	 * This costructor creates a file list from the given path.
	 *
	 * @param dir_name The path.
	 */
	b3FileList(const char *dir_name);

	/**
	 * This destructor deinitializes this instance.
	 */
	~b3FileList();

	/**
	 * This method creates a file list from the given path. The list
	 * container is emptied before file collection. The file collection
	 * is recursive using the b3RecCreateList() method. After collection
	 * the file list is sorted.
	 *
	 * @param dir_name The path.
	 */
	void           b3CreateList(const char *dir_name);

	/**
	 * This method deletes the file list.
	 */
	void           b3DeleteList();

	/**
	 * This method returns the first entry in the file list.
	 *
	 * @return The first file list entry.
	 */
	b3FileEntry   *b3First() const;

	/**
	 * This element adds a file list entry.
	 */
	const b3_bool  b3Add(const char *file_name);

	/**
	 * This method sorts the file entries alphabetically.
	 */
	void           b3Sort();

	/**
	 * This method recursively creates a file list from the given path.
	 *
	 * @param dir_name The path.
	 */
	void           b3RecCreateList(const char *dir_name);

	/**
	 * Thie method returns the amount of file entries.
	 */
	inline const b3_count b3GetCount() const
	{
		return list.b3GetCount();
	}
};

#endif
