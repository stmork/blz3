/*
**
**	$Filename:	b3TxSaveTGA.cc $
**	$Release:	Dortmund 2001 $
**	$Revision:	2.04 
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving a TGA image
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

#include "blz3/image/b3Tx.h"

#define BUFFERSIZE 64000

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/11/08 19:31:33  sm
**	- Nasty CR/LF removal!
**	- Added TGA/RGB8/PostScript image saving.
**	- Hoping to win Peter H. for powerful MFC programming...
**
**	
*/

/*************************************************************************
**                                                                      **
**                          TARGA                                       **
**                                                                      **
*************************************************************************/

class b3InfoTGA : protected b3Mem
{
	b3File        m_File;
	b3Tx         *m_Tx;
	b3_u08       *m_SaveData;
	b3_pkd_color *m_ThisRow;
	b3_u08        m_SaveBuffer[128];
	b3_index      m_SaveAs,m_SaveIndex,m_SaveYPos;

public:
	      b3InfoTGA(b3Tx *tx,const char *filename);
	     ~b3InfoTGA();
	void  b3Write();
};

b3InfoTGA::b3InfoTGA(b3Tx *tx,const char *filename)
{
	m_SaveData = (b3_u08 *)b3Alloc(BUFFERSIZE + 16);
	if (m_SaveData == null)
	{
		b3PrintF (B3LOG_NORMAL,"Save Targa 24: not enough memory!\n");
		throw new b3TxException(B3_TX_MEMORY);
	}

	m_ThisRow = (b3_pkd_color *)b3Alloc(tx->xSize * sizeof(b3_pkd_color));
	if (m_ThisRow == null)
	{
		b3Free();
		b3PrintF (B3LOG_NORMAL,"Save Targa 24: not enough memory!\n");
		throw new b3TxException(B3_TX_MEMORY);
	}

	m_Tx = tx;
	m_Tx->b3Name(filename);
	if(!m_File.b3Open(filename,B_WRITE))
	{
		b3Free();
		b3PrintF(B3LOG_NORMAL,"Save Targa: file \"%s\" not created!\n",filename);
		throw new b3TxException(B3_TX_NOT_SAVED);
	}

	m_File.b3Write (m_SaveData,m_SaveIndex = 18);
}

void b3InfoTGA::b3Write()
{
	b3_pkd_color a;
	b3_coord     t,u,y;
	b3_count     i;

	for (y = 0;y < m_Tx->ySize;y++)
	{
		m_SaveYPos = y;
		m_Tx->b3GetRow(m_ThisRow,y);
		t = 0;
		while (t < m_Tx->xSize)
		{
			a = m_ThisRow[t];
			if (a == m_ThisRow[t+1])
			{
				i = 0;
				t++;
				while ((a == m_ThisRow[t]) && (t < m_Tx->xSize) && (i < 127))
				{
					t++;
					i++;
				}

				m_SaveData[0] = 128 | i;
				m_SaveData[1] = a;
				m_SaveData[2] = a >>  8;
				m_SaveData[3] = a >> 16;
				m_File.b3Write(m_SaveData,4);
			}
			else
			{
				i = 1;
				u = t+i;
				m_SaveIndex = 0;
				while ((m_ThisRow[u] != m_ThisRow[u+1]) && (u < m_Tx->xSize) && (i < 127))
				{
					i++;
					u++;
				}
				m_SaveData[0] = (i-1);
				m_File.b3Write(m_SaveData,1);
				while (i!=0)
				{
					a = m_ThisRow[t++];
					m_SaveData[0] = a;
					m_SaveData[1] = a >>  8;
					m_SaveData[2] = a >> 16;
					m_File.b3Write (m_SaveData,3);
					i--;
				}
			}
		}
		m_ThisRow += m_Tx->xSize;
	}
}

b3InfoTGA::~b3InfoTGA()
{
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
	m_SaveData[14] = m_SaveYPos & 255;
	m_SaveData[15] = m_SaveYPos >> 8;
	m_SaveData[16] = 24;								/* 24 Bit */
	m_SaveData[17] = 0x20;							/* von oben nach unten */

	m_File.b3Seek (0,B3_SEEK_START);
	m_File.b3Write(m_SaveData,18);
	m_File.b3Close();
}

b3_result b3Tx::b3SaveTGA(const char *filename)
{
	b3InfoTGA info(this,filename);
	info.b3Write();
	return B3_OK;
}
