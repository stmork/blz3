/*
**
**	$Filename:	b3SelectTexture.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 479 $
**	$Date: 2003-02-09 14:58:14 +0100 (So, 09 Feb 2003) $
**	$Author: sm $
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

class CB3SelectLoadTexture
{
	static const char *m_RegEntry;

public:
	static b3_bool b3Select(b3Tx **tx,char *name);
};

class CB3SelectSaveTexture
{
	static const char *m_RegEntry;

public:
	static       b3_bool  b3Select(b3Path &name,const char *tx_name);
	static const char    *b3GetLastFilename(char *filename);
};

#endif
