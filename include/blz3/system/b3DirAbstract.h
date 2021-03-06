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

#pragma once

#ifndef B3_SYSTEM_DIRABSTRACT_H
#define B3_SYSTEM_DIRABSTRACT_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Exception.h"
#include "blz3/system/b3Memory.h"

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
	alignas(16) char m_Path[B3_FILESTRINGLEN];

public:
	/**
	 * The default constructor supplies an empty path definition.
	 */
	inline b3PathAbstract()
	{
		m_Path[0] = 0;
	}

	/**
	 * This constructor initializes the path with the given filename.
	 *
	 * @param path The filename for initialization.
	 */
	inline b3PathAbstract(const char * path)
	{
		b3Mem::b3StrCpy(m_Path, path, sizeof(m_Path));
	}

	/**
	 * The comparision operator compares if two b3Path instances are
	 * literally equal.
	 *
	 * @param other The other instance to compare to.
	 * @return True if both contents are literally equal.
	 */
	inline bool operator==(const b3PathAbstract & other) const
	{
		return strcmp(m_Path, other.m_Path) == 0;
	}

	/**
	 * The comparision operator compares if a b3Path instance and a C like
	 * string are literally equal.
	 *
	 * @param other The other instance to compare to.
	 * @return True if both contents are literally equal.
	 */
	inline bool operator==(const char * other) const
	{
		return strcmp(m_Path, other) == 0;
	}

	/**
	 * The comparision operator compares if two b3Path instances are not
	 * literally equal.
	 *
	 * @param other The other instance to compare to.
	 * @return True if both contents are literally not equal.
	 */
	inline bool operator!=(const b3PathAbstract & other) const
	{
		return !operator ==(other);
	}

	/**
	 * This cast operator returns the actual filename.
	 *
	 * @return  The actual filename.
	 */
	inline operator char * ()
	{
		return m_Path;
	}

	/**
	 * This cast operator returns the actual filename as const.
	 *
	 * @return  The actual filename as const char pointer.
	 */
	inline operator const char * () const
	{
		return m_Path;
	}

	/**
	 * This cast operator returns the actual filename as a temporary string
	 * instance.
	 *
	 * @return The path as std::string.
	 */
	inline operator std::string () const
	{
		return std::string(m_Path);
	}

	/**
	 * The index operator gives a reference to the corresponding character
	 * inside the path.
	 *
	 * @note The contents of the reference may be modified.
	 *
	 * @param index The path index to access.
	 * @return The reference to the indexed character.
	 */
	inline char & operator [] (const b3_count index)
	{
		return m_Path[index];
	}

	/**
	 * The index operator gives a const reference to the corresponding character
	 * inside the path.
	 *
	 * @note The contents of the reference may not be modified.
	 *
	 * @param index The path index to access.
	 * @return The const reference to the indexed character.
	 */
	inline const char & operator [] (const b3_count index) const
	{
		return m_Path[index];
	}

	/**
	 * This method removes any filename in this instance.
	 */
	virtual void b3Empty() = 0;

	/**
	 * This method concatenates a directory name and a filename and puts
	 * the result into this instance.
	 *
	 * @param path The directory name component.
	 * @param name The filename component.
	 */
	virtual void b3LinkFileName(const char * path, const char * name) = 0;

	/**
	 * This method splits the filename of this instance into the directory
	 * and filename components. If the path is a directory the filename
	 * component is set to zero length. The oomponent pointer may be null
	 * for convenience.
	 *
	 * @param path The directory name component.
	 * @param name The filename component.
	 */
	virtual void b3SplitFileName(char * path, char * name) = 0;

	/**
	 * This method removes from the given filename the file extension
	 * and puts the result into this instance.
	 *
	 * \param *filename The filename for removing the extension.
	 */
	virtual void b3RemoveExt(const char * filename) = 0;

	/**
	 * This method removes the file extension from this instance.
	 */
	virtual void b3RemoveExt() = 0;

	/**
	 * This method removes from the given file name the filename component
	 * if any and additionally the directory component to get the parent
	 * directory. The result is copied into this instance.
	 *
	 * @param filename The filename to get the parent directory name.
	 */

	virtual void b3ParentName(const char * filename) = 0;

	/**
	 * This method removes from the actual file name the filename component
	 * if any and additionally the directory component to get the parent
	 * directory.
	 */
	virtual void b3ParentName() = 0;

	/**
	 * This method extracts the file extension from the given filename and
	 * puts the resulting extension into this instance. The resulting extension
	 * contains no trailing dot.
	 *
	 * @param filename The filename where to extract the file extension.
	 */
	virtual void b3ExtractExt(const char * filename) = 0;

	/**
	 * This method extracts the file extension from this instance.
	 */
	virtual void b3ExtractExt() = 0;

	/**
	 * This method corrects the syntax of the given filename and stores the result
	 * into this instance.
	 *
	 * @param path the filename to correct.
	 */
	virtual void b3Correct(const char * path) = 0;

	/**
	 * This method corrects the syntax of the filename of this instance.
	 */
	virtual void b3Correct() = 0;

	/**
	 * This method creates a printf() format like way to create a filename in a safe
	 * way without a risk of a buffer overflow.
	 *
	 * @param format The format string.
	 * @param ... The arguments.
	 */
	virtual void b3Format(const char * format, ...)
	__attribute__((format(printf, 2, 3))) = 0;

	/**
	 * This method appends a single character to the filename of this instance
	 * in a safe way without a risk of a buffer overflow. So it may happen that
	 * the character may not be appended.
	 *
	 * @param ext The single character to append.
	 */

	virtual void b3Append(const char ext) = 0;
	/**
	 * This method appends a string to the filename of this instance in a safe way
	 * without a risk of a buffer overflow.
	 *
	 * @param ext The text to append.
	 */

	virtual void b3Append(const char * ext) = 0;

private:
	static void b3RemoveDelimiter(char * path);
};

/**
 * This class handles directory listing. An example for scanning a
 * directory looks like this:
 *
@verbatim
	b3Path       name;
	b3Dir        dir;
	b3_path_type type;
	b3_bool      loop = true;

	dir.b3OpenDir(startDir);
	do
	{
		type = dir.b3DirNext(name);

		switch (type)
		{
		case B3_TYPE_FILE:
			// name is a file
			break;

		case B3_TYPE_DIR:
			// name is a directory
			break;

		case B3_NOT_EXISTANT:
		default :
			// name is undefined
			loop = false;
			break;
		}
	}
	while (loop);
	dir.b3CloseDir();
@endverbatim
 */
class B3_PLUGIN b3DirAbstract
{
public:
	/**
	 * This method opens a directory for listing.
	 *
	 * @param dirname The directory name to list.
	 * @return True on success.
	 */
	virtual b3_bool       b3OpenDir(const char * dirname) = 0;

	/**
	 * This method retrieves the next directory entry.
	 *
	 * @param direntry The name of the directory entry.
	 * @return Type of directory entry.
	 */
	virtual b3_path_type  b3DirNext(char * direntry) = 0;

	/**
	 * This method closes this handler for directory listing.
	 */
	virtual void          b3CloseDir() = 0;
};

#endif
