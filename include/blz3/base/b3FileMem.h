/*
**
**	$Filename:	b3FileMem.h $
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

#ifndef B3_BASE_FILEMEM_H
#define B3_BASE_FILEMEM_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3FileAbstract.h"

/*************************************************************************
**                                                                      **
**                        b3FileMem itself                              **
**                                                                      **
*************************************************************************/

/**
 * This class provides memory bases file IO. Note that the file is lost
 * after destruction.
 */
class B3_PLUGIN b3FileMem : public b3FileAbstract, public b3Mem
{
	b3_size    m_BufferPos;  //!< Actual file position.
	b3_size    m_BufferMax;  //!< buffer size.
	b3_size    m_BufferSize; //!< file size.
	b3_u08  *  m_Buffer;     //!< buffer itself.
	b3_size    m_BufferInc;  //!< buffer increment.

public:
	/**
	 * This constructor initializes the instance.
	 */
	b3FileMem();

	/**
	 * This constructor initializes the instance with the specified access mode.
	 * only write access is useful with this constructor.
	 *
	 * @param mode The access mode.
	 * @throws b3FileException
	 */
	b3FileMem(const b3_access_mode mode);

	/**
	 * This constructor opens an existing file for reading or appending. It reads the
	 * complete content of the file into the cache buffer using the b3ReadBuffer
	 * method.
	 *
	 * @param filename The file to handle.
	 * @param mode     The access mode.
	 * @throws b3FileException
	 */
	b3FileMem(const char * filename, const b3_access_mode mode);

	/**
	 * This destructor cleans up the instance. It calls b3Close().
	 */
	~b3FileMem();

	/**
	 * This method initializes the instance with the specified access mode.
	 * only write access is useful with this constructor.
	 *
	 * @param mode The access mode.
	 * @throws b3FileException
	 */
	b3_bool  b3Open(const b3_access_mode mode);

	/**
	 * This method opens an existing file for reading or appending. It reads the
	 * complete content of the file into the cache buffer using the b3ReadBuffer
	 * method.
	 *
	 * @param filename The file to handle.
	 * @param mode     The access mode.
	 * @return True if the file open was successful.
	 * @throws b3FileException
	 */
	b3_bool  b3Open(const char * filename, const b3_access_mode mode);
	b3_size  b3Read(void * read_buffer, const b3_size size);

	/**
	 * This method reads the complete content of the specified file into the
	 * cache buffer of this instance.
	 *
	 * @param filename The file to cache.
	 * @return True if the operation was successful.
	 */
	b3_bool  b3ReadBuffer(const char * filename);
	b3_size  b3Write(const void * write_buffer, const b3_size size);
	b3_bool  b3Flush();
	b3_size  b3Seek(const b3_offset offset, const b3_seek_type type);
	b3_size  b3Size();
	b3_bool  b3Buffer(const b3_size new_cache_size);
	void     b3Close();

private:
	b3_bool  b3EnsureBufferSize(b3_size new_size);
};

#endif
