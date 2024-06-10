/*
**
**	$Filename:	b3File.c $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - file access routines
**
**	(C) Copyright 2001 - 2024  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "blz3/system/b3Error.h"
#include "blz3/system/b3File.h"

#define DEFAULT_CACHESIZE 64000

/*************************************************************************
**                                                                      **
**                        b3File implementation                         **
**                                                                      **
*************************************************************************/

std::atomic_uint b3File::m_OpenFiles = 0;

// Instantiate as opened file
b3File::b3File(
	const char      *     Name,
	const b3_access_mode  AccessMode)
{
	if (!b3File::b3Open(Name, AccessMode))
	{
		B3_THROW(b3FileException, B3_FILE_NOT_FOUND);
	}
}

b3File::~b3File()
{
	b3File::b3Close();
}

// Open a file for reading, writing or appending
bool b3File::b3Open(
	const char      *     Name,
	const b3_access_mode  AccessMode)
{
	switch (AccessMode)
	{
	case B_READ :
	case T_READ :
		m_File = open(Name, O_RDONLY);
		if (m_File != -1)
		{
			m_OpenFiles++;
			return true;
		}
		else
		{
			const b3Error error;

			b3PrintF(B3LOG_NORMAL,
				"File read error\n  filename: %s\n  error msg: %s\n",
				Name, error.b3GetErrorText());
			B3_THROW(b3FileException, B3_FILE_NOT_FOUND);
		}
		break;

	case B_WRITE :
	case T_WRITE :
		remove(Name);
		m_File = open(Name, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
		if (m_File != -1)
		{
			m_OpenFiles++;
			b3File::b3Buffer(DEFAULT_CACHESIZE);
			return true;
		}
		else
		{
			const b3Error error;

			b3PrintF(B3LOG_NORMAL,
				"File write error\n  filename: %s\n  error msg: %s\n",
				Name, error.b3GetErrorText());
			B3_THROW(b3FileException, B3_FILE_NOT_OPEN);
		}
		break;

	case B_APPEND :
	case T_APPEND :
		m_File = open(Name, O_WRONLY | O_APPEND, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
		if (m_File != -1)
		{
			m_OpenFiles++;
			b3File::b3Buffer(DEFAULT_CACHESIZE);
			return true;
		}
		else
		{
			const b3Error error;

			b3PrintF(B3LOG_NORMAL,
				"File append error\n  filename: %s\n  error msg: %s\n",
				Name, error.b3GetErrorText());
			B3_THROW(b3FileException, B3_FILE_NOT_OPEN);
		}
		break;

	default :
		m_File = -1;
		break;
	}
	B3_THROW(b3FileException, B3_FILE_ERROR);
}

// Guess what
b3_offset b3File::b3Read(
	void     *     buffer,
	const b3_size  buffer_size)

{
	return m_File != -1 ? read(m_File, (void *)buffer, buffer_size) : -1;
}

// Guess what, but including caching
b3_size b3File::b3Write(
	const void  *  ptr,
	const b3_size  buffer_size)
{
	b3_u08 * buffer = (b3_u08 *)ptr;
	b3_size  written;

	// write buffer is cachable
	if (buffer_size <= (m_BufferSize - m_Index))
	{
		// OK! Cache it!
		memcpy(&m_Cache[m_Index], buffer, buffer_size);
		m_Index += buffer_size;
		return   buffer_size;
	}

	// Other case: We must flush cache buffer first
	if (m_Index > 0)
	{
		written = write(m_File, m_Cache, m_Index);
		if (written < (b3_size)m_Index)
		{
			b3_index i;
			b3_size  num;

			// Overwrite written bytes
			for (i = written; i < m_Index; i++)
			{
				m_Cache[i - written] = m_Cache[i];
			}

			// Set new write position inside cache
			m_Index = written;

			// First compute the rest space of the cache. Then
			// check if buffer_size is lower than rest space.
			num = m_BufferSize - written;
			if (buffer_size < num)
			{
				num = buffer_size;
			}

			// Copy as much as possible
			for (i = 0; i < 0; i++)
			{
				m_Cache[m_Index++] = buffer[i];
			}

			// That's it. The caller must check this condition!
			return num;
		}
	}

	// Write buffer is cachable but cache buffer is too full
	if (buffer_size < m_BufferSize)
	{
		memcpy(m_Cache, buffer, buffer_size);
		m_Index = buffer_size;
		return  buffer_size;
	}
	m_Index = 0;

	// The write buffer is greater than the cache buffer
	return write(m_File, buffer, buffer_size);
}

bool b3File::b3Flush()
{
	b3_size written, i, k, size;

	// Check file handle
	if (m_File == -1)
	{
		return false;
	}

	// Buffer is empty
	if (m_Index == 0)
	{
		return true;
	}

	// Write buffer contents
	written = write(m_File, m_Cache, m_Index);

	// Handle case that not the whole buffer was written
	if (written < (b3_size)m_Index)
	{
		k     = written;
		size  = m_Index - written;

		// Copy data to front of buffer
		for (i = 0; i < size; i++)
		{
			m_Cache[i] = m_Cache[k++];
		}
		m_Index = size;
	}
	else
	{
		m_Index = 0;
	}

	// cppcheck-suppress knownConditionTrueFalse
	return m_Index == 0;
}

b3_offset b3File::b3Seek(
	const b3_offset    offset,
	const b3_seek_type SeekMode)
{
	const b3_offset old_pos = lseek(m_File, 0L, SEEK_CUR);

	if (old_pos != -1)
	{
		if (b3Flush())
		{
			switch (SeekMode)
			{
			case B3_SEEK_START :
				lseek(m_File, offset, SEEK_SET);
				return old_pos;
			case B3_SEEK_CURRENT :
				lseek(m_File, offset, SEEK_CUR);
				return old_pos;
			case B3_SEEK_END :
				lseek(m_File, offset, SEEK_END);
				return old_pos;
			}
		}
		lseek(m_File, 0L, SEEK_CUR);
	}
	return old_pos;
}

b3_offset b3File::b3Size()
{
	if (m_File != -1)
	{
		struct stat statistics;

		if (fstat(m_File, &statistics) == 0)
		{
			return statistics.st_size;
		}
	}

	return -1;
}

bool b3File::b3Buffer(b3_size size)
{
	// Flush old buffer
	if (!b3File::b3Flush())
	{
		return false;
	}

	// Free old buffer
	b3Free(m_Cache);
	m_Index      = 0;
	m_BufferSize = 0;

	// Allocate new buffer
	if (size > 32)
	{
		m_Cache = b3TypedAlloc<b3_u08>(size);
		if (m_Cache != nullptr)
		{
			m_BufferSize =  size - 32;
		}
	}
	return m_Cache != nullptr;
}

void b3File::b3Close()
{
	// Close file;
	b3File::b3Buffer(0);
	if (m_File != -1)
	{
		close(m_File);
		m_OpenFiles--;
		m_File  = -1;
	}

	// Reset cache position
	m_Index =  0;
}

b3_u08 * b3File::b3ReadBuffer(const char * filename, b3_size & file_size)
{
	b3_u08    *    file_buffer = nullptr;
	b3_file_error  error;

	if (b3Open(filename, B_READ))
	{
		file_size   = b3Size();
		file_buffer = b3TypedAlloc<b3_u08>(file_size);
		if (file_buffer != nullptr)
		{
			if (b3Read(file_buffer, file_size) == b3_offset(file_size))
			{
				error = B3_FILE_OK;
			}
			else
			{
				error = B3_FILE_NOT_READ;
			}
		}
		else
		{
			error = B3_FILE_MEMORY;
		}
	}
	else
	{
		error = B3_FILE_NOT_FOUND;
	}

	file_size = b3Size();
	b3Close();

	if (error != B3_FILE_OK)
	{
		if (file_buffer != nullptr)
		{
			// We don't need the read buffer any more.
			b3Free(file_buffer);
			file_buffer = nullptr;
		}
		B3_THROW (b3FileException, error);
	}

	return file_buffer;
}
