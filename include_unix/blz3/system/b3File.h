/*
**
**	$Filename:	b3File.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Basic Input Output Functions
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_FILE_H
#define B3_SYSTEM_FILE_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3FileAbstract.h"

/*************************************************************************
**                                                                      **
**                        b3File itself                                 **
**                                                                      **
*************************************************************************/

/**
 * This class handles file IO on a file of a file system.
 */
class b3File : public b3FileAbstract, public b3Mem
{
	static b3_count  m_OpenFiles;
	static b3Mutex   m_FilesOpenedMutex;

	b3_u08     *m_Cache;      //!< Cache buffer.
	b3_offset   m_Index;      //!< Index in cache.
	b3_size     m_BufferSize; //!< Size of cache.
	int         m_File;       //!< Fileno.
public:
	/**
	 * This constructor initializes this instance. You need to call b3Open to do
	 * file IO.
	 */
	b3File      ();

	/**
	 * This constructor prepares this instance to to file IO. It calls b3Open()
	 * to do this.
	 *
	 * @param filename The file to handle with.
	 * @param mode The file access mode.
	 */
	b3File      (const char *filename,const b3_access_mode mode);
	~b3File      ();

	b3_bool  b3Open      (const char *filename,const b3_access_mode mode);
	b3_size  b3Read      (void *read_buffer,const b3_size size);

	/**
	 * This method reads the complete content of the specified filename into a
	 * memory buffer. The memory is allocated by this instance and will be freed
	 * if this instance will be destroyed.
	 *
	 * @param filename The file to read in.
	 * @param filesize Where to store the resulting buffer size.
	 * @return The buffer of null if there occured an error.
	 */
	b3_u08  *b3ReadBuffer(const char *filename,b3_size &filesize);
	b3_size  b3Write     (const void * write_buffer,b3_size size);
	b3_bool  b3Flush     ();
	b3_size  b3Seek      (const b3_offset offset,const b3_seek_type type);
	b3_size  b3Size      ();
	b3_bool  b3Buffer    (const b3_size new_cache_size);
	void     b3Close     ();
};

#endif
