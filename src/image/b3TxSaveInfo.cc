/*
**
**	$Filename:	b3TxSaveInfo.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - 	File format encoder
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/image/b3Tx.h"

#include "b3TxSaveInfo.h"

/*************************************************************************
**                                                                      **
**                          PNG                                         **
**                                                                      **
*************************************************************************/

b3TxSaveInfo::b3TxSaveInfo(b3Tx * tx, const char * filename, const char * write_mode) :
	m_SaveBuffer{}
{
	m_Tx = tx;
	m_Tx->b3Name(filename);

	m_ThisRow = b3TypedAlloc<b3_pkd_color>(tx->xSize);
	if (m_ThisRow == nullptr)
	{
		b3PrintF(B3LOG_NORMAL, "Save Image: not enough memory!\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	if (write_mode == nullptr)
	{
		m_FileHandle = nullptr;
		if (!m_File.b3Open(filename, B_WRITE))
		{
			b3Free();
			b3PrintF(B3LOG_NORMAL, "Save Image: file \"%s\" not created!\n", filename);
			B3_THROW(b3TxException, B3_TX_NOT_SAVED);
		}
	}
	else
	{
		m_FileHandle = fopen(filename, write_mode);
		if (m_FileHandle == nullptr)
		{
			b3Free();
			b3PrintF(B3LOG_NORMAL, "Save Image: file \"%s\" not created!\n", filename);
			B3_THROW(b3TxException, B3_TX_NOT_SAVED);
		}
	}
}

b3TxSaveInfo::~b3TxSaveInfo()
{
	if (m_FileHandle != nullptr)
	{
		fclose(m_FileHandle);
	}
	else
	{
		m_File.b3Close();
	}
}
