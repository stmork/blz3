/*
**
**	$Filename:	b3SelectTexture.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Selecting an image or texture
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SELECT_TEXTURE_H
#define B3_SELECT_TEXTURE_H

#include "blz3/image/b3Tx.h"

class CB3SelectTexture
{
public:
	static b3_bool b3Select(b3Tx **tx,char *name);
};

#endif
