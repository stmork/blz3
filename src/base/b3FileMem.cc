/*
**
**	$Filename:	b3FileMem.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Memory file access routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3FileMem.h"
#include "blz3/system/b3File.h"

/*************************************************************************
**                                                                      **
**                        b3FileMem implementation                      **
**                                                                      **
*************************************************************************/

b3FileMem::b3FileMem()
{
	m_Buffer     = null;
	m_BufferSize = 0;
	m_BufferPos  = 0;
	m_BufferMax  = 0;
	m_BufferInc  = 0;
}

// Instantiate as opened file
b3FileMem::b3FileMem (const b3_access_mode access_mode)
{
	m_Buffer     = null;
	m_BufferSize = 0;
	m_BufferPos  = 0;
	m_BufferMax  = 0;
	m_BufferInc  = 0;
	if (!b3Open(access_mode))
	{
		B3_THROW(b3FileException,B3_FILE_NOT_FOUND);
	}
}

// Instantiate as opened file
b3FileMem::b3FileMem (const char *file_name,const b3_access_mode access_mode)
{
	m_Buffer     = null;
	m_BufferSize = 0;
	m_BufferPos  = 0;
	m_BufferMax  = 0;
	m_BufferInc  = 0;
	if (!b3Open(file_name,access_mode))
	{
		B3_THROW(b3FileException,B3_FILE_NOT_FOUND);
	}
}

b3FileMem::~b3FileMem()
{
}

// Open a file for reading, writing or appending
b3_bool b3FileMem::b3Open (const b3_access_mode access_mode)
{
	b3_file_error error = B3_FILE_NOT_OPEN;

	switch (access_mode)
	{
	case B_WRITE :
	case T_WRITE :
		b3Close();
		// Walk through!
	case B_READ :
	case T_READ :
		if (b3Buffer (16000))
		{
			if (b3EnsureBufferSize(0))
			{
				b3Seek(0,B3_SEEK_START);
				return true;
			}
		}
		error = B3_FILE_MEMORY;
		// Walk through!
	case B_APPEND:
	case T_APPEND:
		break;
	}
	B3_THROW(b3FileException,error);
}

// Open a file for reading, writing or appending
b3_bool b3FileMem::b3Open (const char *file_name,const b3_access_mode access_mode)
{
	b3_file_error error = B3_FILE_NOT_OPEN;

	switch (access_mode)
	{
	case B_READ :
	case T_READ :
		if (b3Buffer (16000))
		{
			if (b3ReadBuffer(file_name))
			{
				b3Seek(0,B3_SEEK_START);
				return true;
			}
		}
		break;

	case B_WRITE:
	case T_WRITE:
		break;

	case B_APPEND:
	case T_APPEND:
		if (b3Buffer (16000))
		{
			if (b3ReadBuffer(file_name))
			{
				b3Seek(0,B3_SEEK_END);
				return true;
			}
		}
		break;
	}
	B3_THROW(b3FileException,error);
}

// Guess what
b3_size b3FileMem::b3Read (
	void          *buffer,
	const b3_size  buffer_size)
{
	b3_size read_bytes;

	read_bytes = B3_MIN(buffer_size,m_BufferSize - m_BufferPos);
	memcpy(buffer,&m_Buffer[m_BufferPos],read_bytes);
	m_BufferPos += read_bytes;

	return read_bytes;
}

// Guess what, but including caching
b3_size b3FileMem::b3Write (
	const void    *ptr,
	const b3_size  write_size)
{
	b3_size written_size;

	written_size = (b3EnsureBufferSize(m_BufferPos + write_size) ? write_size : m_BufferMax - m_BufferPos);

	memcpy(&m_Buffer[m_BufferPos],ptr,written_size);
	m_BufferPos += written_size;
	if (m_BufferPos > m_BufferSize)
	{
		m_BufferSize = m_BufferPos;
	}

	return written_size;
}

b3_bool b3FileMem::b3Flush ()
{
	return m_Buffer != null;
}

b3_size b3FileMem::b3Seek (
	const b3_offset    offset,
	const b3_seek_type SeekMode)
{
	b3_size   old_pos;
	b3_offset new_pos;

	old_pos = m_BufferPos;
	switch (SeekMode)
	{
	case B3_SEEK_START :
		new_pos = offset;
		break;

	case B3_SEEK_CURRENT :
		new_pos = (b3_offset)m_BufferPos + offset;
		break;

	case B3_SEEK_END :
		new_pos = (b3_offset)m_BufferSize + offset;
		break;

	default:
		new_pos = offset;
		break;
	}

	if (new_pos < 0)
	{
		new_pos = 0;
	}
	b3EnsureBufferSize(m_BufferPos = new_pos);
	if (m_BufferPos > m_BufferSize)
	{
		m_BufferSize = m_BufferPos;
	}
	return old_pos;
}

b3_size b3FileMem::b3Size ()
{
	return m_BufferSize;
}

b3_bool b3FileMem::b3Buffer (b3_size Size)
{
	m_BufferInc = Size;
	return true;
}

void b3FileMem::b3Close ()
{
	if (m_Buffer != null)
	{
		b3Free();
	}
	m_Buffer     = null;
	m_BufferSize = 0;
	m_BufferPos  = 0;
	m_BufferMax  = 0;
}

b3_bool b3FileMem::b3ReadBuffer(const char *filename)
{
	b3File   file;
	b3_u08  *file_buffer = null;
	b3_size  file_size;

	file_buffer = file.b3ReadBuffer(filename,file_size);
	if (file_buffer != null)
	{
		if (b3EnsureBufferSize(file_size))
		{
			memcpy(m_Buffer,file_buffer,file_size);
			m_BufferSize = file_size;
			m_BufferPos  = file_size;
			return true;
		}
	}
	return false;
}

b3_bool b3FileMem::b3EnsureBufferSize(b3_size new_size)
{
	b3_u08 *new_buffer;

	if (new_size > m_BufferMax)
	{
		new_size += m_BufferInc;

		new_buffer = (b3_u08 *)b3Alloc(new_size);
		if (new_buffer == null)
		{
			B3_THROW(b3FileException,B3_FILE_MEMORY);
		}
		memcpy(new_buffer,m_Buffer,m_BufferMax);
		b3Free(m_Buffer);
		m_Buffer    = new_buffer;
		m_BufferMax = new_size;
	}
	return true;
}
