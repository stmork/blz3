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

#ifndef B3_IMAGE_TXPOOL_H
#define B3_IMAGE_TXPOOL_H

#include "blz3/b3Config.h"
#include "blz3/image/b3Tx.h"

class b3TxPath : public b3Link<b3TxPath>, public b3Path
{
public:
	b3TxPath(const char *path);
};

class b3TxPool : public b3Mem
{
	b3Base<b3Tx>     m_Pool;
	b3Base<b3TxPath> m_SearchPath;
public:
	                     b3TxPool();
	                    ~b3TxPool();
	       b3Tx         *b3LoadTexture(const char *name);
	       void          b3AddPath(const char *path);
	       b3Base<b3Tx> *b3GetTxHead();
private:
	       b3_bool       b3ReloadTexture(b3Tx *texture,const char *name=null);
};

extern b3TxPool texture_pool;

#endif
