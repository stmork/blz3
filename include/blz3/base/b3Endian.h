/*
**
**	$Filename:	b3Tx.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines (proto types)
**
**	(C) Copyright 2001 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_ENDIAN_H
#define B3_BASE_ENDIAN_H

#include "blz3/b3Config.h"

class b3Endian
{
public:
	static b3_u16   b3Get16         (void *Ptr);
	static b3_u32   b3Get32         (void *Ptr);
	
	static b3_u16   b3GetMot16      (void *Ptr);
	static b3_u32   b3GetMot32      (void *Ptr);
	static b3_f32   b3GetMotFloat   (void *Ptr);
	static b3_f64   b3GetMotDouble  (void *Ptr);
	
	static b3_u16   b3GetIntel16    (void *Ptr);
	static b3_u32   b3GetIntel32    (void *Ptr);
	static b3_f32   b3GetIntelFloat (void *Ptr);
	static b3_f64   b3GetIntelDouble(void *Ptr);

	static b3_size  b3ChangeEndian16(void *Ptr);
	static b3_size  b3ChangeEndian32(void *Ptr);
	static b3_size  b3ChangeEndian64(void *Ptr);
};

#endif
