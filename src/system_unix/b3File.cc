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
#include "blz3/system/b3File.h"

#define DEFAULT_CACHESIZE 64000

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2002/02/20 20:23:58  sm
**	- Some type cleanups done.
**
**	Revision 1.5  2001/10/16 15:21:24  sm
**	- Minor changes to compile with GCC 3.x
**	
**	Revision 1.4  2001/10/09 20:47:02  sm
**	- some further texture handling.
**	
**	Revision 1.3  2001/07/01 16:48:00  sm
**	- FILESTRINGLEN -> B3_FILESTRINGLEN
**	- Cleaned up some makefiles
**	- Cleaned up some CVS conflicts
**	
**	Revision 1.2  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
*/

/*************************************************************************
**                                                                      **
**                        b3File implementation                         **
**                                                                      **
*************************************************************************/

static b3Mutex        files_opened_mutex;
static unsigned long  files_opened;

// Initialize an instance only
b3File::b3File()
{
	Index =  0;
	Size  =  0;
	File  = -1;
	Cache =  null;
}

// Instantiate as opened file
b3File::b3File (
	const char           *Name,
	const b3_access_mode  AccessMode)
{
	Cache =  null;
	Size  =  0;
	Index =  0;
	File  = -1;

	if (!b3Open(Name,AccessMode))
	{
		throw new b3FileException(B3_FILE_NOT_FOUND);
	}
}

b3File::~b3File()
{
	if (File != -1)
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
			File = open(Name,O_RDONLY);
			if (File != -1)
			{
				files_opened_mutex.b3Lock();
				files_opened++;
				files_opened_mutex.b3Unlock();
				return true;
			}
			break;

		case B_WRITE :
		case T_WRITE :
			remove (Name);
			File = open(Name,O_WRONLY|O_CREAT,S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
			if (File != -1)
			{
				files_opened_mutex.b3Lock();
				files_opened++;
				files_opened_mutex.b3Unlock();
				b3Buffer (DEFAULT_CACHESIZE);
				return true;
			}
			break;

		case B_APPEND :
		case T_APPEND :
			File = open(Name,O_WRONLY|O_APPEND,S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
			if (File != -1)
			{
				files_opened_mutex.b3Lock();
				files_opened++;
				files_opened_mutex.b3Unlock();
				b3Buffer (DEFAULT_CACHESIZE);
				return true;
			}
			break;

		default :
			File = null;
			break;
	}
	return false;
}

// Guess what
b3_size b3File::b3Read (
	void    *buffer,
	const b3_size buffer_size)

{
	long readBytes;

	readBytes = read (File,buffer,buffer_size);
	return readBytes;
}

// Guess what, but including caching
b3_size b3File::b3Write (
	const void    *ptr,
	const b3_size  buffer_size)
{
	b3_u08  *buffer = (b3_u08 *)ptr;
	b3_size  written;

	// write buffer is cachable
	if (buffer_size <= (Size - Index))
	{
		// OK! Cache it!
		memcpy (&Cache[Index],buffer,buffer_size);
		Index += buffer_size;
		return   buffer_size;
	}

	// Other case: We must flush cache buffer first
	written = write (File,Cache,Index);
	if (written < (b3_size)Index)
	{
		b3_offset i;
		b3_size   num;

		// Overwrite written bytes
		for (i = written;i < Index;i++)
		{
			Cache[i-written] = Cache[i];
		}

		// Set new write position inside cache
		Index = written;

		// First compute the rest space of the cache. Then
		// check if buffer_size is lower than rest space.
		num = Size - written;
		if (buffer_size < num)
		{
			num = buffer_size;
		}

		// Copy as much as possible
		for (i = 0;i < 0;i++)
		{
			Cache[Index++] = buffer[i];
		}

		// That's it. The caller must check this condition!
		return num;
	}

	// Write buffer is cachable but cache buffer is too full
	if (buffer_size < Size)
	{
		memcpy (Cache,buffer,buffer_size);
		Index = buffer_size;
		return  buffer_size;
	}
	Index = 0;

	// The write buffer is greater than the cache buffer
	return (write(File,buffer,buffer_size));
}

b3_bool b3File::b3Flush ()
{
	b3_size   i,k,Size;
	b3_offset written;

	// Buffer is empty
	if (Index == 0)
	{
		return true;
	}

	// Write buffer contents
	written = write (File,Cache,Index);

	// Handle case that not the whole buffer was written
	if (written < Index)
	{
		k     = written;
		Size  = Index - written;

		// Copy data to front of buffer
		for (i = 0;i < Size;i++)
		{
			Cache[i] = Cache[k++];
		}
		Index = Size;
	}
	else
	{
		Index = 0;
	}

	return Index == 0;
}

b3_size b3File::b3Seek (
	const b3_offset    offset,
	const b3_seek_type SeekMode)
{
	b3_size OldPos;

	OldPos = lseek(File,0L,SEEK_CUR);
	if (b3Flush())
	{
		switch (SeekMode)
		{
			case B3_SEEK_START :
				lseek(File,offset,SEEK_SET);
				return OldPos;
			case B3_SEEK_CURRENT :
				lseek(File,offset,SEEK_CUR);
				return OldPos;
			case B3_SEEK_END :
				lseek(File,offset,SEEK_END);
				return OldPos;
		}
	}
	lseek(File,0L,SEEK_CUR);
	return OldPos;
}

b3_size b3File::b3Size ()
{
	long OldPos,Size;

	b3Flush ();

	// save old position
	OldPos = lseek (File,0L,SEEK_CUR);

	// Run to EOF
	         lseek (File,0L,SEEK_END);

	// save end position (= file size)
	Size   = lseek (File,0L,SEEK_CUR);

	// Remember old position
	         lseek (File,OldPos,SEEK_SET);
	return Size;
}

b3_bool b3File::b3Buffer (b3_size Size)
{
	// Flush old buffer
	if (!b3Flush())
	{
		return false;
	}

	// Free old buffer
	b3Free (Cache);
	Size  = 0;
	Index = 0;

	// Allocate new buffer
	if (Size > 32)
	{
		Cache = (unsigned char *)b3Alloc (Size);
		if (Cache) 
		{
			Size -= 32;
		}
	}
	return Cache != null;
}

void b3File::b3Close ()
{
	// Close file;
	if (File != -1)
	{
		b3Flush ();
		close  (File);
		files_opened_mutex.b3Lock();
		files_opened--;
		files_opened_mutex.b3Unlock();
		File  = -1;
	}

	// Free Cache
	if (Cache)
	{
		b3Free (Cache);
		Cache = null;
		Size  =  0;
	}

	// Reset cache position
	Index =  0;
}

#include "blz3/system/b3Log.h"

b3_u08 *b3File::b3ReadBuffer(const char *filename,b3_size &file_size)
{
	b3_u08        *file_buffer = null;
	b3_file_error  error = B3_FILE_ERROR;

	if (b3Open(filename,B_READ))
	{
		file_size   = b3Size();
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
		throw new b3FileException(error);
	}

	return file_buffer;
}
