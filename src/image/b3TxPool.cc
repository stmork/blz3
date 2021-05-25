/*
**
**	$Filename:	b3TxPool.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Texture pool
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
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

#include "b3TxInclude.h"

/*************************************************************************
**                                                                      **
**                        Texture maintainance                          **
**                                                                      **
*************************************************************************/

b3TxPool::b3TxPool()
{
	m_Pool.b3InitBase(CLASS_TEXTURE);
}

b3TxPool::~b3TxPool()
{
	b3CriticalSection lock(m_Mutex);

	B3_DELETE_BASE(&m_Pool, tx);
}

b3Base<b3Tx> * b3TxPool::b3GetTxHead()
{
	return &m_Pool;
}

b3_bool b3TxPool::b3ReloadTexture(b3Tx * tx, const char * Name)
{
	b3Path  fullname;
	bool    result = b3IsValid(Name, fullname);

	// Check result of texture load
	if (result)
	{
		result = (tx->b3LoadImage(fullname) == B3_OK);
		b3PrintF(B3LOG_DEBUG, "IMG POOL # Image \"%s\" %sloaded.\n",
			tx->b3Name(), result ? "" : "not ");
	}
	else
	{
		tx->b3Name(Name);
		b3PrintF(B3LOG_DEBUG, "IMG POOL # Image \"%s\" not available!\n",
			tx->b3Name());
	}
	return result;
}

b3Tx * b3TxPool::b3FindTexture(const char * ParamName)
{
	b3CriticalSection lock(m_Mutex);

	return b3FindTextureUnsafe(ParamName);
}

b3Tx * b3TxPool::b3FindTextureUnsafe(const char * ParamName)
{
	b3Path       Name;
	b3Tx    *    tx;
	const char * txName;
	b3_size      txLen, nameLen;
	b3_offset    diff;

	Name.b3Correct(ParamName);
	B3_FOR_BASE(&m_Pool, tx)
	{
		txName  = tx->b3Name();
		txLen   = strlen(txName);
		nameLen = strlen(Name);
		diff    = (b3_offset)txLen - (b3_offset)nameLen;
		if (strcmp(&txName[diff >= 0 ? diff : 0], Name) == 0)
		{
			return tx;
		}
	}
	return tx;
}

b3Tx * b3TxPool::b3LoadTexture(const char * Name)
{
	b3Tx * tx;

	// find existing texture
	b3PrintF(B3LOG_DEBUG, "IMG POOL # Image \"%s\" to load.\n", Name);

	b3CriticalSection lock(m_Mutex);

	tx = b3FindTextureUnsafe(Name);
	if (tx == nullptr)
	{
		// OK, create new texture
		tx = new b3Tx();

		// load data and insert in internal list
		b3ReloadTexture(tx, Name);
		m_Pool.b3Append(tx);
	}
	else
	{
		b3PrintF(B3LOG_DEBUG, "IMG POOL # Image \"%s\" found.\n",
			tx->b3Name());
	}

	return tx;
}

void b3TxPool::b3Dump()
{
	b3Tx * tx;

	b3PrintF(B3LOG_NORMAL, "Image pool dump with %d textures.\n",
		m_Pool.b3GetCount());
	B3_FOR_BASE(&m_Pool, tx)
	{
		b3PrintF(B3LOG_NORMAL, "TX: %5d x %5d - %s\n",
			tx->xSize,
			tx->ySize,
			tx->b3Name());
	}
}

void b3TxPool::b3Free()
{
	m_Pool.b3Free();
}
