/*
**
**	$Filename:	gif.cc $  
**	$Release:	Dortmund 1997, 2004 $
**	$Revision$
**	$Date$
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/
  
/*
**	$Log$
**	Revision 1.1  2004/10/07 10:33:47  sm
**	- Added some GIF routine module.
**
**
*/
  
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
