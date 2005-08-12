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

#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>

#include "blz3/b3Types.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Exception.h"
#include "blz3/system/b3DirAbstract.h"

typedef b3Exception<b3_dir_error,'DIR'> b3DirException;

/**
 * This class implements the b3PathAbstract class. It additionally provides some
 * method for file name/file path handling.
 */
class b3Path : public b3PathAbstract
{
public:
	/**
	 * This constructor initializes this instance.
	 */
	inline      b3Path() : b3PathAbstract()
	{
	}

	/**
	 * Thie constructor initializes the instance with the given path.
	 *
	 * @param path The filename for initializing.
	 */
	inline      b3Path(const char *path) : b3PathAbstract(path)
	{
	}

	       void b3Empty();
		   void b3LinkFileName (const char *path,const char *name);
	       void b3SplitFileName(char *path,char *name);
	       void b3RemoveExt    (const char *filename);
	       void b3RemoveExt    ();
		   void b3ParentName   (const char *filename);
	       void b3ParentName   ();
		   void b3ExtractExt   (const char *filename);
		   void b3ExtractExt   ();
	       void b3Correct      (const char *path);
		   void b3Correct      ();
		   void b3Format       (const char *format,...);
		   void b3Append       (const char *ext);

	/**
	 * This method concatenates a directory name and a filename and puts
	 * the result into the given pointer.
	 *
	 * @param *full The concatenated file name.
	 * @param *path The directory name component.
	 * @param *name The filename component.
	 */
	static void b3LinkFileName (char *full,const char *path,const char *name);

	/**
	 * This method splits the filename of the given full filename into the directory
	 * and filename components. If the path is a directory the filename
	 * component is set to zero length. The oomponent pointer may be null
	 * for convenience.
	 *
	 * @param *full The filename to split.
	 * @param *path The directory name component.
	 * @param *name The filename component.
	 */
	static void b3SplitFileName(const char *full,char *path,char *name);

	/**
	 * This method removes from the actual file name the filename component
	 * if any and additionally the directory component to get the parent
	 * directory.
	 *
	 * @param path The filesystem entry.
	 * @param result The resulting parent directory.
	 */
	static void b3ParentName   (const char *path,char *result);

	/**
	 * This method removes from the actual file name the filename component
	 * if any and additionally the directory component to get the parent
	 * directory.
	 *
	 * @param parent The filename as input and the parent directory as output.
	 */
	static void b3ParentName   (char *parent);

	/**
	 * This method removes the extension from a given filename and puts
	 * the result into an extra buffer.
	 *
	 * @param filename The filename for removing.
	 * @param result The filename without the extension
	 */
	static void b3RemoveExt    (const char *filename,char *result);

	/**
	 * This method removes the extension from the given filename.
	 *
	 * @param result The buffer for removing the extension.
	 */
	static void b3RemoveExt    (char *result);

	/**
	 * This method extracts the file extension from the given file name and
	 * puts it into an extra char field.
	 *
	 * @param filename The file name where to extract.
	 * @param ext The extenstion as result.
	 */
	static void b3ExtractExt   (const char *filename,char *ext);

	/**
	 * This method extracts the extension of the given filename and puts
	 * the result into the same buffer.
	 *
	 * @param *ext The filename as input and the extension as output.
	 */
	static void b3ExtractExt   (char *ext);

	/**
	 * This method corrects the syntax of the given filename and stores
	 * the result in the given result buffer.
	 *
	 * @param path The filename to correct.
	 * @param result The buffer for the corrected filename.
	 */
	static void b3Correct      (const char *path,char *result);

	/**
	 * This method corrects the syntax of the given filename and stores there
	 * the result.
	 *
	 * @param path The filename to correct.
	 */
	static void b3Correct      (char *path);
};

/**
 * This class implements the b3DirAbstract class and adds some
 * convenience methods.
 */
class b3Dir : public b3DirAbstract, public b3Mem, public b3Path
{
protected:
	DIR                 *dir;
public:
	/**
	 * This constructor initializes the directory list class.
	 */
	b3Dir();

	/**
	 * This constructor initializes the directory list class and
	 * calls the b3OpenDir() method.
	 *
	 * @param dirname The directory name to list.
	 */
	b3Dir(const char *dirname);

	/**
	 * This destructor deinitializes this class by calling
	 * b3Close().
	 */
	~b3Dir();

	b3_bool       b3OpenDir(const char *dirname);
	b3_path_type  b3DirNext(char *direntry);
	void          b3CloseDir();

	/**
	 * This method changes the current working directory to the specified one.
	 *
	 * @param newworkingdir The new working directory.
	 * @return True on success.
	 */
	static b3_bool       b3ChDir        (const char *newworkingdir);

	/**
	 * This method checks whether the specified filesystem entry exists as a directory.
	 *
	 * @param testdir The filesystem entry to check.
	 * @return True if the directory exists.
	 */
	static b3_bool       b3ChkDir       (const char *testdir);

	/**
	 * This method creates a new directory. The parent directory must exist.
	 *
	 * @param newdir The new directory to create.
	 * @return True on success.
	 */
	static b3_bool       b3MkDir        (const char *newdir);

	/**
	 * This method checks the given filesystem entry for existance and returns the
	 * type if so.
	 *
	 * @param path The filesystem entry to check.
	 * @return The entry type.
	 * @see b3_path_entry
	 */
	static b3_path_type  b3Exists       (const char *path);
};

#endif
