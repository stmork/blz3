/*
**
**	$Filename:	b3FileAbstract.h $
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

#ifndef B3_BASE_FILEABSTRACT_H
#define B3_BASE_FILEABSTRACT_H

#include "blz3/b3Types.h"
#include "blz3/base/b3Exception.h"

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

typedef enum
{
	B3_SEEK_START = 0,
	B3_SEEK_CURRENT,
	B3_SEEK_END
} b3_seek_type;

/*************************************************************************
**                                                                      **
**                        access modes for b3Open()                     **
**                                                                      **
*************************************************************************/

typedef enum
{
	B_READ   = 0x6272,
	T_READ   = 0x7472,
	B_WRITE  = 0x6277,
	T_WRITE  = 0x7477,
	B_APPEND = 0x6261,
	T_APPEND = 0x7461
} b3_access_mode;

/*************************************************************************
**                                                                      **
**                        error codes for b3File                        **
**                                                                      **
*************************************************************************/

typedef enum
{
	B3_FILE_ERROR = -1,
	B3_FILE_OK    =  0,
	B3_FILE_NOT_FOUND,
	B3_FILE_NOT_READ,
	B3_FILE_NOT_WRITTEN,
	B3_FILE_MEMORY,
	B3_FILE_NOT_OPEN
} b3_file_error;

typedef b3Exception<b3_file_error,'FIL'> b3FileException;

/*************************************************************************
**                                                                      **
**                        b3FileAbstract definition                     **
**                                                                      **
*************************************************************************/

class b3FileAbstract
{
public:
	virtual b3_bool  b3Open      (const char *file_name,const b3_access_mode mode) = 0;
	virtual b3_size  b3Read      (void *read_buffer,const b3_size size) = 0;
	virtual b3_size  b3Write     (const void * write_buffer,const b3_size size) = 0;
	virtual b3_bool  b3Flush     () = 0;
	virtual b3_size  b3Seek      (const b3_offset offset,const b3_seek_type type) = 0;
	virtual b3_size  b3Size      () = 0;
	virtual b3_bool  b3Buffer    (const b3_size new_cache_size) = 0;
	virtual void     b3Close     () = 0;
};

#endif
