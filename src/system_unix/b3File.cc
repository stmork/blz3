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
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "blz3/system/b3File.h"

#define DEFAULT_CACHESIZE 64000

/*************************************************************************
**                                                                      **
**                        b3File implementation                         **
**                                                                      **
*************************************************************************/

b3_count b3File::m_OpenFiles = 0;
b3Mutex  b3File::m_FilesOpenedMutex;

// Initialize an instance only
b3File::b3File()
{
	m_Index       =  0;
	m_BufferSize  =  0;
	m_File        = -1;
	m_Cache       =  nullptr;
}

// Instantiate as opened file
b3File::b3File(
	const char      *     Name,
	const b3_access_mode  AccessMode)
{
	m_Index       =  0;
	m_BufferSize  =  0;
	m_File        = -1;
	m_Cache       =  nullptr;

	if (!b3Open(Name, AccessMode))
	{
		B3_THROW(b3FileException, B3_FILE_NOT_FOUND);
	}
}

b3File::~b3File()
{
	if (m_File != -1)
	{
		b3Close();
	}
}

// Open a file for reading, writing or appending
bool b3File::b3Open(
	const char      *     Name,
	const b3_access_mode  AccessMode)
{
	char error_msg[1024];

	switch (AccessMode)
	{
	case B_READ :
	case T_READ :
		m_File = open(Name, O_RDONLY);
		if (m_File != -1)
		{
			b3CriticalSection lock(m_FilesOpenedMutex);

			m_OpenFiles++;
			return true;
		}
		else
		{
			if (strerror_r(errno, error_msg, sizeof(error_msg)) == 0)
			{
				b3PrintF(B3LOG_NORMAL, "File read error\n  filename: %s\n  error msg: %s\n",
					Name, error_msg);
			}
		}
		break;

	case B_WRITE :
	case T_WRITE :
		remove(Name);
		m_File = open(Name, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
		if (m_File != -1)
		{
			b3CriticalSection lock(m_FilesOpenedMutex);

			m_OpenFiles++;
			b3Buffer(DEFAULT_CACHESIZE);
			return true;
		}
		else
		{
			if (strerror_r(errno, error_msg, sizeof(error_msg)) == 0)
			{
				b3PrintF(B3LOG_NORMAL, "File write error\n  filename: %s\n  error msg: %s\n",
					Name, error_msg);
			}
		}
		break;

	case B_APPEND :
	case T_APPEND :
		m_File = open(Name, O_WRONLY | O_APPEND, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
		if (m_File != -1)
		{
			b3CriticalSection lock(m_FilesOpenedMutex);

			m_OpenFiles++;
			b3Buffer(DEFAULT_CACHESIZE);
			return true;
		}
		else
		{
			if (strerror_r(errno, error_msg, sizeof(error_msg)) == 0)
			{
				b3PrintF(B3LOG_NORMAL, "File append error\n  filename: %s\n  error msg: %s\n",
					Name, error_msg);
			}
			b3PrintF(B3LOG_NORMAL, "File append error\n  filename: %s\n  error msg: %s\n",
				Name, error_msg);
		}
		break;

	default :
		m_File = -1;
		break;
	}
	return false;
}

// Guess what
b3_size b3File::b3Read(
	void     *     buffer,
	const b3_size  buffer_size)

{
	b3_size readBytes;

	readBytes = read(m_File, (void *)buffer, buffer_size);
	return readBytes;
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
	if (m_Index > 0)
	{
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
	}

	return m_Index == 0;
}

b3_size b3File::b3Seek(
	const b3_offset    offset,
	const b3_seek_type SeekMode)
{
	b3_size OldPos;

	OldPos = lseek(m_File, 0L, SEEK_CUR);
	if (b3Flush())
	{
		switch (SeekMode)
		{
		case B3_SEEK_START :
			lseek(m_File, offset, SEEK_SET);
			return OldPos;
		case B3_SEEK_CURRENT :
			lseek(m_File, offset, SEEK_CUR);
			return OldPos;
		case B3_SEEK_END :
			lseek(m_File, offset, SEEK_END);
			return OldPos;
		}
	}
	lseek(m_File, 0L, SEEK_CUR);
	return OldPos;
}

b3_size b3File::b3Size()
{
	b3_size pos, size;

	b3Flush();

	// save old position
	pos  = lseek(m_File, 0L, SEEK_CUR);

	// Run to EOF
	lseek(m_File, 0L, SEEK_END);

	// save end position (= file size)
	size = lseek(m_File, 0L, SEEK_CUR);

	// Remember old position
	lseek(m_File, pos, SEEK_SET);
	return size;
}

bool b3File::b3Buffer(b3_size size)
{
	// Flush old buffer
	if (!b3Flush())
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
	b3Buffer(0);
	if (m_File != -1)
	{
		b3CriticalSection lock(m_FilesOpenedMutex);

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
		file_size = b3Size();
		file_buffer = b3TypedAlloc<b3_u08>(file_size);
		if (file_buffer != nullptr)
		{
			if (b3Read(file_buffer, file_size) == file_size)
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

	file_size   = b3Size();
	b3Close();

	if (error != B3_FILE_OK)
	{
		if (file_buffer != nullptr)
		{
			// We don't need the read buffer any more.
			b3Free(file_buffer);
		}
		B3_THROW(b3FileException, error);
	}

	return file_buffer;
}
