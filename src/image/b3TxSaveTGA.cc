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

#include "b3TxSaveInfo.h"

#define BUFFERSIZE 64000

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**
**	Revision 1.6  2002/02/17 21:58:11  sm
**	- Done UnCR
**	- Modified makefiles
**	
**	Revision 1.5  2002/02/01 16:08:26  sm
**	- Corrected assert in saving routines for formats which
**	  only support resolutions less than 65536.
**	
**	Revision 1.4  2002/02/01 15:41:52  sm
**	- Fixed saving TGA and RGB8 missing last line to save
**	
**	Revision 1.3  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	Revision 1.2  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
**	
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

class b3InfoTGA : protected b3TxSaveInfo
{
	b3_u08       *m_SaveData;
	b3_index      m_SaveAs,m_SaveIndex;

public:
	      b3InfoTGA(b3Tx *tx,const char *filename);
	     ~b3InfoTGA();
	void  b3Write();
};

b3InfoTGA::b3InfoTGA(b3Tx *tx,const char *filename) :
	b3TxSaveInfo(tx,filename)
{
	m_SaveData = (b3_u08 *)b3Alloc(BUFFERSIZE + 16);
	if (m_SaveData == null)
	{
		m_File.b3Close();
		b3Free();
		b3PrintF (B3LOG_NORMAL,"Save Targa 24: not enough memory!\n");
		throw b3TxException(B3_TX_MEMORY);
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

	m_File.b3Seek (0,B3_SEEK_START);
	m_File.b3Write(m_SaveData,18);
}

b3_result b3Tx::b3SaveTGA(const char *filename)
{
	b3InfoTGA info(this,filename);
	info.b3Write();
	return B3_OK;
}
