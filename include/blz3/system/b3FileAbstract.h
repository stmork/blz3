/*
**
**	$Filename:	b3FileAbstract.h $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Basic Input Output Functions
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_FILEABSTRACT_H
#define B3_SYSTEM_FILEABSTRACT_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Exception.h"

/*************************************************************************
**                                                                      **
**                        Some access rights                            **
**                                                                      **
*************************************************************************/

#ifndef S_IRUSR

// RWX mask for owner
#	define						S_IRUSR	0400
#	define						S_IWUSR	0200
#	define						S_IXUSR	0100
#	define						S_IRWXU	(S_IRUSR|S_IWUSR|S_IXUSR)

// RWX mask for group
#	define						S_IRGRP	0040
#	define						S_IWGRP	0020
#	define						S_IXGRP	0010
#	define						S_IRWXG	(S_IRGRP|S_IWGRP|S_IXGRP)

// RWX mask for other
#	define						S_IROTH	0004
#	define						S_IWOTH	0002
#	define						S_IXOTH	0001
#	define						S_IRWXO	(S_IROTH|S_IWOTH|S_IXOTH)

#endif

/*************************************************************************
**                                                                      **
**                        seek modes                                    **
**                                                                      **
*************************************************************************/

/**
 * These enumerations lists the types of file offset handling.
 *
 * @see b3FileAbstract#b3Seek.
 */
enum b3_seek_type
{
	B3_SEEK_START = 0, //!< File offset is from file start.
	B3_SEEK_CURRENT,   //!< File offset is from current file position.
	B3_SEEK_END        //!< File offset is from file end.
};

/*************************************************************************
**                                                                      **
**                        access modes for b3Open()                     **
**                                                                      **
*************************************************************************/

/**
 * These enumerations lists the possible file access methods.
 *
 * @see b3FileAbstract#b3Open.
 */
enum b3_access_mode
{
	B_READ   = 0x6272, //!< Binary read.
	T_READ   = 0x7472, //!< Text read.
	B_WRITE  = 0x6277, //!< Binary write.
	T_WRITE  = 0x7477, //!< Text write.
	B_APPEND = 0x6261, //!< Binary append.
	T_APPEND = 0x7461  //!< Text append.
};

/*************************************************************************
**                                                                      **
**                        error codes for b3File                        **
**                                                                      **
*************************************************************************/

/**
 * This enumeration lists the possible file errors.
 */
enum b3_file_error
{
	B3_FILE_ERROR = -1,   //!< General file error.
	B3_FILE_OK    =  0,   //!< File IO is OK.
	B3_FILE_NOT_FOUND,    //!< File not found.
	B3_FILE_NOT_READ,     //!< Could not read file.
	B3_FILE_NOT_WRITTEN,  //!< Could not write file (permissions).
	B3_FILE_MEMORY,       //!< Not enough memory for file handling.
	B3_FILE_NOT_OPEN      //!< Could not open file.
};

typedef b3Exception<b3_file_error,0x46494c> b3FileException;

/*************************************************************************
**                                                                      **
**                        b3FileAbstract definition                     **
**                                                                      **
*************************************************************************/

/**
 * This abstract class is an interface to cached file IO.
 */
class B3_PLUGIN b3FileAbstract
{
public:
	/**
	 * This method opens a file using a filename and an access mode.
	 *
	 * @param filename The file name to use.
	 * @param mode     The access mode.
	 * @return         True if the file could successfully opened.
	 */
	virtual b3_bool  b3Open      (const char *filename,const b3_access_mode mode) = 0;

	/**
	 * This method reads a specified amount of bytes into a specified buffer. It returns
	 * how many bytes could really be read. If there are less bytes read than there were
	 * requested than this need not be an error.
	 *
	 * @param read_buffer The buffer where to store the read bytes.
	 * @param size        The number of bytes to read.
	 * @return The really number of read bytes.
	 */
	virtual b3_size  b3Read      (void *read_buffer,const b3_size size) = 0;

	/**
	 * This method writes the specified amount of bytes from a buffer. It returns
	 * the really written amount of bytes. If this number is less than the specified
	 * amount of bytes it is an indicator for an error.
	 *
	 * @param write_buffer The buffer with the bytes to write.
	 * @param size The number of bytes to write.
	 * @return The really written number of bytes.
	 */
	virtual b3_size  b3Write     (const void * write_buffer,const b3_size size) = 0;

	/**
	 * This method flushes the write cache.
	 *
	 * @see b3Buffer.
	 */
	virtual b3_bool  b3Flush     () = 0;

	/**
	 * This method moves the byte position inside the file. The position can be moved
	 * from the actual position, from start and from the end of the file.
	 *
	 * @param offset The offset to move.
	 * @param type   The seek type to use.
	 * @return The old file position from start.
	 * @see b3_seek_type
	 */
	virtual b3_size  b3Seek      (const b3_offset offset,const b3_seek_type type) = 0;

	/**
	 * This method returns the actual file size.
	 *
	 * @return The actual file size.
	 */
	virtual b3_size  b3Size      () = 0;
	
	/**
	 * This method resizes the file cache. It calls b3Flush() to write old
	 * content.
	 *
	 * @param new_cache_size The new cache size.
	 * @return Succes if the cache was resized.
	 */
	virtual b3_bool  b3Buffer    (const b3_size new_cache_size) = 0;

	/**
	 * This method closes the file.
	 */
	virtual void     b3Close     () = 0;
};

#endif
