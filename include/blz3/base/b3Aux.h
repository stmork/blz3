/*
**
**	$Filename:	b3Aux.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Master of little help
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_IMAGE_AUX_H
#define B3_IMAGE_AUX_H

#include "blz3/b3Config.h"

extern void b3LongMemSet(b3_u32 *data,b3_count DWordCount,b3_u32 pattern);

class b3RGB
{
public:
	b3_u08 r,g,b;
public:
	              b3RGB();
	b3_pkd_color  operator()();
	              operator b3_pkd_color();
	void          operator=(const b3_pkd_color &);
};

class b3Rect
{
public:
	b3_coord x1,y1;
	b3_coord x2,y2;

public:
	        b3Rect();
	b3_res  b3Width();
	b3_res  b3Height();
	b3_bool b3UpdateBound(b3Rect *);
	b3_bool b3CheckBound(b3Rect *);
};

#endif
