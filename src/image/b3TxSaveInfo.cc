/*
**
**	$Filename:	b3TxSaveInfo.cc $
**	$Release:	Dortmund 2001 $
**	$Revision:	2.04 
**	$Date$
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

#include "b3TxSaveInfo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2002/08/15 13:56:43  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**
**	Revision 1.3  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.2  2002/02/01 15:41:52  sm
**	- Fixed saving TGA and RGB8 missing last line to save
**	
**	Revision 1.1  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
**	
**	
*/

/*************************************************************************
**                                                                      **
**                          RGB8                                        **
**                                                                      **
*************************************************************************/

b3TxSaveInfo::b3TxSaveInfo(b3Tx *tx,const char *filename,const char *write_mode)
{
	m_Tx = tx;
	m_Tx->b3Name(filename);
	memset(m_SaveBuffer,0,sizeof(m_SaveBuffer));

	m_ThisRow = (b3_pkd_color *)b3Alloc(tx->xSize * sizeof(b3_pkd_color));
	if (m_ThisRow == null)
	{
		b3PrintF (B3LOG_NORMAL,"Save RGB8: not enough memory!\n");
		B3_THROW(b3TxException,B3_TX_MEMORY);
	}

	if (write_mode == null)
	{
		m_FileHandle = null;
		if (!m_File.b3Open(filename,B_WRITE))
		{
			b3Free();
			b3PrintF(B3LOG_NORMAL,"Save Image: file \"%s\" not created!\n",filename);
			B3_THROW(b3TxException,B3_TX_NOT_SAVED);
		}
	}
	else
	{
		m_FileHandle = fopen(filename,write_mode);
		if (m_FileHandle == null)
		{
			b3Free();
			b3PrintF(B3LOG_NORMAL,"Save Image: file \"%s\" not created!\n",filename);
			B3_THROW(b3TxException,B3_TX_NOT_SAVED);
		}
	}
}

b3TxSaveInfo::~b3TxSaveInfo()
{
	if (m_FileHandle != null)
	{
		fclose(m_FileHandle);
	}
	else
	{
		m_File.b3Close();
	}
}
