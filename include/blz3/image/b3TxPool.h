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
#include "blz3/base/b3SearchPath.h"
#include "blz3/image/b3Tx.h"

class b3TxPool : public b3Mem, public b3SearchPath
{
	b3Mutex          m_Mutex;
	b3Base<b3Tx>     m_Pool;
public:
	                     b3TxPool();
	                    ~b3TxPool();
	       b3Tx         *b3LoadTexture(const char *name);
	       b3Tx         *b3FindTexture(const char *name);
	       b3Base<b3Tx> *b3GetTxHead();
private:
	       b3_bool       b3ReloadTexture(b3Tx *texture,const char *name=null);
	       b3Tx         *b3FindTextureUnsafe(const char *name);
};

#endif
