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

#include "b3SystemInclude.h"
#include "stdafx.h"
#include "blz3/system/b3File.h"
#include "io.h"
#include "fcntl.h"

#define DEFAULT_CACHESIZE 64000

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.8  2005/01/02 19:15:25  sm
**	- Fixed signed/unsigned warnings
**	
**	Revision 1.7  2004/05/13 08:14:29  sm
**	- Fixed some uninitialized variables.
**	
**	Revision 1.6  2004/05/13 07:53:26  sm
**	- Fixed b3File caching problem which occured becaus
**	  of a variable hiding problem.
**	
**	Revision 1.5  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**	
**	Revision 1.4  2002/08/15 13:56:44  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.3  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.2  2001/07/02 19:52:03  sm
**	- Cleaning up comments
**	
**	Revision 1.1  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3File implementation                         **
**                                                                      **
*************************************************************************/

b3_count b3File::m_OpenFiles = 0;

// Initialize an instance only
b3File::b3File()
{
	m_Index       =  0;
	m_BufferSize  =  0;
	m_File        = -1;
	m_Cache       =  null;
}

// Instantiate as opened file
b3File::b3File (
	const char           *Name,
	const b3_access_mode  AccessMode)
{
	m_Index       =  0;
	m_BufferSize  =  0;
	m_File        = -1;
	m_Cache       =  null;

	if (!b3Open(Name,AccessMode))
	{
		B3_THROW(b3FileException,B3_FILE_NOT_FOUND);
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
b3_bool b3File::b3Open (
	const char           *Name,
	const b3_access_mode  AccessMode)
{
	switch (AccessMode)
	{
		case B_READ :
		case T_READ :
			m_File = _open(Name,O_RDONLY|
					(AccessMode == B_READ ? O_BINARY : O_TEXT));
			if (m_File != -1)
			{
				m_OpenFiles++;
				return true;
			}
			break;

		case B_WRITE :
		case T_WRITE :
			remove (Name);
			m_File = _open(Name,O_WRONLY|O_CREAT|
				(AccessMode == B_WRITE ? O_BINARY : O_TEXT),
				S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
			if (m_File != -1)
			{
				m_OpenFiles++;
				b3Buffer (DEFAULT_CACHESIZE);
				return true;
			}
			break;

		case B_APPEND :
		case T_APPEND :
			m_File = _open(Name,O_WRONLY|O_APPEND|
				(AccessMode == B_WRITE ? O_BINARY : O_TEXT),
				S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
			if (m_File != -1)
			{
				m_OpenFiles++;
				b3Buffer (DEFAULT_CACHESIZE);
				return true;
			}
			break;

		default :
			m_File = -1;
			break;
	}
	return false;
}

// Guess what
b3_size b3File::b3Read (
	void          *buffer,
	const b3_size  buffer_size)

{
	b3_size readBytes;

	readBytes = _read (m_File,(void *)buffer,(unsigned int)buffer_size);
	return readBytes;
}

// Guess what, but including caching
b3_size b3File::b3Write (
	const void    *ptr,
	const b3_size  buffer_size)
{
	b3_u08   *buffer = (b3_u08 *)ptr;
	b3_count  written;

	// write buffer is cachable
	if (buffer_size <= (m_BufferSize - m_Index))
	{
		// OK! Cache it!
		memcpy (&m_Cache[m_Index],buffer,buffer_size);
		m_Index += (b3_index)buffer_size;
		return   buffer_size;
	}

	// Other case: We must flush cache buffer first
	if (m_Index > 0)
	{
		written = _write (m_File,m_Cache,m_Index);
		if (written < m_Index)
		{
			b3_index i;
			b3_size  num;

			// Overwrite written bytes
			for (i = written;i < m_Index;i++)
			{
				m_Cache[i-written] = m_Cache[i];
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
			for (i = 0;i < 0;i++)
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
		memcpy (m_Cache,buffer,buffer_size);
		m_Index = (b3_index)buffer_size;
		return  buffer_size;
	}
	m_Index = 0;

	// The write buffer is greater than the cache buffer
	return (b3_size)_write(m_File,buffer,(unsigned int)buffer_size);
}

b3_bool b3File::b3Flush ()
{
	b3_index written,i,k,size;

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
		written = _write (m_File,m_Cache,m_Index);

		// Handle case that not the whole buffer was written
		if (written < m_Index)
		{
			k     = written;
			size  = m_Index - written;

			// Copy data to front of buffer
			for (i = 0;i < size;i++)
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

b3_size b3File::b3Seek (
	const b3_offset    offset,
	const b3_seek_type SeekMode)
{
	b3_size OldPos;

	OldPos = _lseek(m_File,0L,SEEK_CUR);
	if (b3Flush())
	{
		switch (SeekMode)
		{
			case B3_SEEK_START :
				_lseek(m_File,offset,SEEK_SET);
				return OldPos;
			case B3_SEEK_CURRENT :
				_lseek(m_File,offset,SEEK_CUR);
				return OldPos;
			case B3_SEEK_END :
				_lseek(m_File,offset,SEEK_END);
				return OldPos;
		}
	}
	_lseek(m_File,0L,SEEK_CUR);
	return OldPos;
}

b3_size b3File::b3Size ()
{
	b3_size pos,size;

	b3Flush ();

	// save old position
	pos  = _lseek (m_File,0L,SEEK_CUR);

	// Run to EOF
	       _lseek (m_File,0L,SEEK_END);

	// save end position (= file size)
	size = _lseek (m_File,0L,SEEK_CUR);

	// Remember old position
	_lseek (m_File,(long)pos,SEEK_SET);
	return size;
}

b3_bool b3File::b3Buffer (b3_size size)
{
	// Flush old buffer
	if (!b3Flush())
	{
		return false;
	}

	// Free old buffer
	b3Free (m_Cache);
	m_Index      = 0;
	m_BufferSize = 0;

	// Allocate new buffer
	if (size > 32)
	{
		m_Cache = (b3_u08 *)b3Alloc (size);
		if (m_Cache != null) 
		{
			m_BufferSize =  size - 32;
		}
	}
	return m_Cache != null;
}

void b3File::b3Close ()
{
	// Close file;
	b3Buffer(0);
	if (m_File != -1)
	{
		_close  (m_File);
		m_OpenFiles--;
		m_File  = -1;
	}

	// Reset cache position
	m_Index =  0;
}

b3_u08 *b3File::b3ReadBuffer(const char *filename,b3_size &file_size)
{
	b3_u08        *file_buffer = null;
	b3_file_error  error;

	if (b3Open(filename,B_READ))
	{
		file_size = b3Size();
		file_buffer = (b3_u08 *)b3Alloc(file_size);
		if (file_buffer != null)
		{
			if (b3Read(file_buffer,file_size) == file_size)
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
		if (file_buffer != null)
		{
			// We don't need the read buffer any more.
			b3Free(file_buffer);
		}
		B3_THROW(b3FileException,error);
	}

	return file_buffer;
}
