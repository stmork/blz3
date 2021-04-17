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

#pragma once

#ifndef B3_SYSTEM_FILE_H
#define B3_SYSTEM_FILE_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3FileAbstract.h"

#include <atomic>

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
	static std::atomic_uint  m_OpenFiles;

	b3_u08   *  m_Cache      = nullptr;  //!< Cache buffer.
	b3_offset   m_Index      =  0;       //!< Index in cache.
	b3_size     m_BufferSize =  0;       //!< Size of cache.
	int         m_File       = -1;       //!< Fileno.

public:
	/**
	 * This constructor initializes this instance. You need to call b3Open to do
	 * file IO.
	 */
	b3File() = default;

	/**
	 * This constructor prepares this instance to to file IO. It calls b3Open()
	 * to do this.
	 *
	 * @param filename The file to handle with.
	 * @param mode The file access mode.
	 */
	b3File(const char * filename, const b3_access_mode mode);
	virtual ~b3File();

	bool      b3Open(const char * filename, const b3_access_mode mode) override;
	b3_offset b3Read(void * read_buffer, const b3_size size) override;

	/**
	 * This method reads the complete content of the specified filename into a
	 * memory buffer. The memory is allocated by this instance and will be freed
	 * if this instance will be destroyed.
	 *
	 * @param filename The file to read in.
	 * @param filesize Where to store the resulting buffer size.
	 * @return The buffer of null if there occured an error.
	 */
	b3_u08  * b3ReadBuffer(const char * filename, b3_size & filesize);
	b3_size   b3Write(const void * write_buffer, b3_size size) override;
	bool      b3Flush() override;
	b3_offset b3Seek(const b3_offset offset, const b3_seek_type type) override;
	b3_offset b3Size() override;
	bool      b3Buffer(const b3_size new_cache_size) override;
	void      b3Close() override;
};

#endif
