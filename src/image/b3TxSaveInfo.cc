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
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
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
**                          RGB8                                        **
**                                                                      **
*************************************************************************/

b3TxSaveInfo::b3TxSaveInfo(b3Tx * tx, const char * filename, const char * write_mode)
{
	m_Tx = tx;
	m_Tx->b3Name(filename);
	memset(m_SaveBuffer, 0, sizeof(m_SaveBuffer));

	m_ThisRow = (b3_pkd_color *)b3Alloc(tx->xSize * sizeof(b3_pkd_color));
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
