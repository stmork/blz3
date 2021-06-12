/*
**
**	$Filename:	b3TxSaveTGA.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving a TGA image
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

#include "blz3/system/b3Memory.h"
#include "blz3/image/b3Tx.h"

#include "b3TxSaveInfo.h"

#define BUFFERSIZE 64000

/*************************************************************************
**                                                                      **
**                          TARGA                                       **
**                                                                      **
*************************************************************************/

class b3InfoTGA : protected b3TxSaveInfo
{
	b3_u08    *   m_SaveData;
	b3_index      m_SaveAs, m_SaveIndex;

public:
	b3InfoTGA(b3Tx * tx, const char * filename);
	~b3InfoTGA();
	void  b3Write();
};

b3InfoTGA::b3InfoTGA(b3Tx * tx, const char * filename) :
	b3TxSaveInfo(tx, filename)
{
	m_SaveData = b3TypedAlloc<b3_u08>(BUFFERSIZE + 16);
	if (m_SaveData == nullptr)
	{
		m_File.b3Close();
		b3Free();
		b3PrintF(B3LOG_NORMAL, "Save Targa 24: not enough memory!\n");
		B3_THROW(b3TxException, B3_TX_MEMORY);
	}

	m_File.b3Write(m_SaveData, m_SaveIndex = 18);
}

void b3InfoTGA::b3Write()
{
	b3_pkd_color a;
	b3_coord     t, u, y;
	b3_count     i;

	// https://www.fileformat.info/format/tga/egff.htm
	for (y = 0; y < m_Tx->ySize; y++)
	{
		m_Tx->b3GetRow(m_ThisRow, y);

		t = 0;
		while (t < m_Tx->xSize)
		{
			a = m_ThisRow[t];
			if (((t + 1) < m_Tx->xSize) && (a == m_ThisRow[t + 1]))
			{
				i = 0;
				t++;
				while ((t < m_Tx->xSize) && (a == m_ThisRow[t]) && (i < 127))
				{
					t++;
					i++;
				}

				m_SaveData[0] = 128 |  (i & 0x7f);
				m_SaveData[1] =  a        & 0xff;
				m_SaveData[2] = (a >>  8) & 0xff;
				m_SaveData[3] = (a >> 16) & 0xff;
				m_File.b3Write(m_SaveData, 4);
			}
			else
			{
				i = 1;
				u = t + i + 1;
				m_SaveIndex = 0;
				while ((u < m_Tx->xSize) && (i < 127) &&
					(m_ThisRow[u - 1] != m_ThisRow[u]))
				{
					i++;
					u++;
				}
				m_SaveData[0] = (i - 1);
				m_File.b3Write(m_SaveData, 1);
				while (i != 0)
				{
					a = m_ThisRow[t++];
					m_SaveData[0] =  a        & 0xff;
					m_SaveData[1] = (a >>  8) & 0xff;
					m_SaveData[2] = (a >> 16) & 0xff;
					m_File.b3Write(m_SaveData, 3);
					i--;
				}
			}
		}
	}
}

b3InfoTGA::~b3InfoTGA()
{
	B3_ASSERT(m_Tx->xSize < 65535);
	B3_ASSERT(m_Tx->ySize < 65535);

	m_SaveData[ 0] =  0;
	m_SaveData[ 1] =  0;
	m_SaveData[ 2] = 10;								/* komprimiert */
	m_SaveData[ 3] =  0;
	m_SaveData[ 4] =  0;
	m_SaveData[ 5] =  0;
	m_SaveData[ 6] =  0;
	m_SaveData[ 7] =  0;
	m_SaveData[ 8] =  0;
	m_SaveData[ 9] =  0;
	m_SaveData[10] =  0;
	m_SaveData[11] =  0;
	m_SaveData[12] = m_Tx->xSize & 255;
	m_SaveData[13] = m_Tx->xSize >> 8;
	m_SaveData[14] = m_Tx->ySize & 255;
	m_SaveData[15] = m_Tx->ySize >> 8;
	m_SaveData[16] = 24;								/* 24 Bit */
	m_SaveData[17] = 0x20;							/* von oben nach unten */

	m_File.b3Seek(0, B3_SEEK_START);
	m_File.b3Write(m_SaveData, 18);
}

b3_result b3Tx::b3SaveTGA(const char * filename)
{
	b3PrintF(B3LOG_FULL, "Saving TGA: %s\n", filename);

	b3InfoTGA info(this, filename);
	info.b3Write();
	return B3_OK;
}
