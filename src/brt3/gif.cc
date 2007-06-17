/*
**
**	$Filename:	gif.cc $  
**	$Release:	Dortmund 1997, 2004 $
**	$Revision: 1454 $
**	$Date: 2006-07-02 10:10:09 +0200 (So, 02 Jul 2006) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Some GIF tool routines
**
**	(C) Copyright 1997, 2004  Steffen A. Mork
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

#include "gif.h"

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

b3_size WriteGIF (
	b3File  &out,
	b3_u08  *buffer,
	b3_u08  *data,
	b3_size  index)
{
	b3_size size;

	size = data - &buffer[index];
	if (size <= 0)
	{
		return index;
	}
	out.b3Write (&buffer[index],size);
	index = data - buffer;
	return index;
}
