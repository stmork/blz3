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
#include "blz3/base/b3FileAbstract.h"

/*************************************************************************
**                                                                      **
**                        b3File itself                                 **
**                                                                      **
*************************************************************************/

class b3File : public b3FileAbstract, public b3Mem
{
	b3_u08     *Cache;		// cache buffer
	b3_index    Index;		// index in cache
	b3_size     Size;		// size of cache
	int         File;		// fileno
public:
	      b3File  ();
	      b3File  (const char *file_name,const b3_access_mode mode);
	     ~b3File  ();

	b3_bool  b3Open      (const char *file_name,const b3_access_mode mode);
	b3_size  b3Read      (void *read_buffer,const b3_size size);
	b3_u08  *b3ReadBuffer(const char *filename,b3_size &filesize);
	b3_size  b3Write     (const void * write_buffer,b3_size size);
	b3_bool  b3Flush     ();
	b3_size  b3Seek      (const b3_offset offset,const b3_seek_type type);
	b3_size  b3Size      ();
	b3_bool  b3Buffer    (const b3_size new_cache_size);
	void     b3Close     ();
};

#endif
