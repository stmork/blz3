/*
**
**	$Filename:	b3TxSaveInfo.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Info about file encoder
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_IMAGE_TXSAVEINFO_H
#define B3_IMAGE_TXSAVEINFO_H

#include "blz3/image/b3Tx.h"
#include "blz3/system/b3File.h"

class b3TxSaveInfo : protected b3Mem
{
protected:
	FILE     *     m_FileHandle;
	b3File         m_File;
	b3Tx     *     m_Tx;
	b3_pkd_color * m_ThisRow;
	b3_u08         m_SaveBuffer[128];

public:
	b3TxSaveInfo(b3Tx * tx, const char * filename, const char * write_mode = nullptr);

	b3TxSaveInfo(const b3TxSaveInfo & other) = delete;
	b3TxSaveInfo & operator=(const b3TxSaveInfo & other) = delete;

	virtual ~b3TxSaveInfo();
};

#endif
