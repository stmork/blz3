/*
**
**	$Filename:	b3TxSaveRGB8.cc $
**	$Release:	Dortmund 2001 $
**	$Revision:	2.04 
**	$Date$
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

#include "b3TxSaveInfo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2002/08/15 13:56:43  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**
**	Revision 1.6  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.5  2002/02/17 21:58:11  sm
**	- Done UnCR
**	- Modified makefiles
**	
**	Revision 1.4  2002/02/01 16:08:26  sm
**	- Corrected assert in saving routines for formats which
**	  only support resolutions less than 65536.
**	
**	Revision 1.3  2002/02/01 15:41:52  sm
**	- Fixed saving TGA and RGB8 missing last line to save
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
			else OldAmount++;			/* Wiederholungsfall */
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

b3_result b3Tx::b3SaveRGB8(const char *filename)
{
	b3InfoRGB8 info(this,filename);
	info.b3Write();
	return B3_OK;
}
