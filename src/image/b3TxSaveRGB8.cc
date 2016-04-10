/*
**
**	$Filename:	b3TxSaveRGB8.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving a RGB8 image
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

class b3InfoRGB8 : protected b3TxSaveInfo
{
	b3_pkd_color  DataRGB8,OldValue;
	b3_count      OldAmount;

public:
	b3InfoRGB8(b3Tx *tx,const char *filename);
	~b3InfoRGB8();
	void  b3Write();
};

b3InfoRGB8::b3InfoRGB8(b3Tx *tx,const char *filename) :
		b3TxSaveInfo(tx,filename)
{
	m_File.b3Write (m_SaveBuffer,48);
	DataRGB8  =  40;
	OldAmount =   0;
	OldValue  =   0;
}

void b3InfoRGB8::b3Write()
{
	b3_coord x,y;

	for (y = 0;y < m_Tx->ySize;y++)
	{
		m_Tx->b3GetRow(m_ThisRow,y);
		for (x = 0;x < m_Tx->xSize;x++)
		{
			if (OldAmount == 0)
			{
				OldValue  = m_ThisRow[x];
				OldAmount = 1;
			}							/* Schreibfall */
			else if ((OldValue!=m_ThisRow[x])||(OldAmount >= 127))
			{
				m_SaveBuffer[0] = (OldValue & 0xff0000) >> 16;
				m_SaveBuffer[1] = (OldValue & 0x00ff00) >>  8;
				m_SaveBuffer[2] =  OldValue & 0x0000ff;
				m_SaveBuffer[3] =  OldAmount;

				if (m_File.b3Write(m_SaveBuffer,4) < 4)
				{
					B3_THROW(b3TxException,B3_TX_NOT_SAVED);
				}
				DataRGB8 += 4;

				OldAmount = 1;
				OldValue  = m_ThisRow[x];
			}
			else
			{
				// Repeat
				OldAmount++;
			}
		}
	}
}

b3InfoRGB8::~b3InfoRGB8()
{
	B3_ASSERT(m_Tx->xSize < 65535);
	B3_ASSERT(m_Tx->ySize < 65535);

	m_SaveBuffer[0] = (OldValue & 0xff0000) >> 16;
	m_SaveBuffer[1] = (OldValue & 0x00ff00) >>  8;
	m_SaveBuffer[2] =  OldValue & 0x0000ff;
	m_SaveBuffer[3] =  OldAmount;
	m_File.b3Write(m_SaveBuffer,4);
	DataRGB8 += 4;

	m_SaveBuffer[ 0] = 'F';
	m_SaveBuffer[ 1] = 'O';
	m_SaveBuffer[ 2] = 'R';
	m_SaveBuffer[ 3] = 'M';
	m_SaveBuffer[ 4] = (DataRGB8 & 0x7f000000 ) >> 24;
	m_SaveBuffer[ 5] = (DataRGB8 & 0x00ff0000 ) >> 16;
	m_SaveBuffer[ 6] = (DataRGB8 & 0x0000ff00 ) >>  8;
	m_SaveBuffer[ 7] =  DataRGB8 & 0x000000ff;
	m_SaveBuffer[ 8] = 'R';
	m_SaveBuffer[ 9] = 'G';
	m_SaveBuffer[10] = 'B';
	m_SaveBuffer[11] = '8';
	m_SaveBuffer[12] = 'B';
	m_SaveBuffer[13] = 'M';
	m_SaveBuffer[14] = 'H';
	m_SaveBuffer[15] = 'D';
	m_SaveBuffer[16] =
		m_SaveBuffer[17] =
			m_SaveBuffer[18] =  0;
	m_SaveBuffer[19] = 20;
	m_SaveBuffer[20] = m_Tx->xSize >> 8;
	m_SaveBuffer[21] = m_Tx->xSize & 255;
	m_SaveBuffer[22] = m_Tx->ySize >> 8;
	m_SaveBuffer[23] = m_Tx->ySize & 255;
	m_SaveBuffer[24] =
		m_SaveBuffer[25] =
			m_SaveBuffer[26] =
				m_SaveBuffer[27] =  0;
	m_SaveBuffer[28] = 24;								/* Anzahl Farben */
	m_SaveBuffer[29] =  0;
	m_SaveBuffer[30] =  4;								/* Packmodus */
	m_SaveBuffer[31] =  0;
	m_SaveBuffer[32] =  0;
	m_SaveBuffer[33] =  0;
	m_SaveBuffer[34] =									/* Aspect Ratio */
		m_SaveBuffer[35] =  1;
	m_SaveBuffer[36] = m_Tx->xSize >> 8;
	m_SaveBuffer[37] = m_Tx->xSize & 255;
	m_SaveBuffer[38] = m_Tx->ySize >> 8;
	m_SaveBuffer[39] = m_Tx->ySize & 255;
	m_SaveBuffer[40] = 'B';
	m_SaveBuffer[41] = 'O';
	m_SaveBuffer[42] = 'D';
	m_SaveBuffer[43] = 'Y';
	DataRGB8 -= 40;
	m_SaveBuffer[44] = (DataRGB8 & 0x7f000000 ) >> 24;
	m_SaveBuffer[45] = (DataRGB8 & 0xff0000 )   >> 16;
	m_SaveBuffer[46] = (DataRGB8 & 0xff00 )     >>  8;
	m_SaveBuffer[47] =  DataRGB8 & 0xff;

	m_File.b3Seek (0,B3_SEEK_START);
	m_File.b3Write(m_SaveBuffer,48);
}

const b3_result b3Tx::b3SaveRGB8(const char *filename)
{
	b3PrintF(B3LOG_FULL, "Saving RGB8: %s\n", filename);

	b3InfoRGB8 info(this,filename);
	info.b3Write();
	return B3_OK;
}
