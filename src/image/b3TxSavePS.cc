/*
**
**	$Filename:	b3TxSavePS.cc $
**	$Release:	Dortmund 2001 $
**	$Revision:	2.04 
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving a PostScript image
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
**                          PS                                          **
**                                                                      **
*************************************************************************/

class b3InfoPS : protected b3Mem
{
	FILE         *m_File;
	b3Tx         *m_Tx;
	b3_pkd_color *m_ThisRow;

public:
	      b3InfoPS(b3Tx *tx,const char *filename);
	     ~b3InfoPS();
	void  b3Write();
};

b3InfoPS::b3InfoPS(b3Tx *tx,const char *filename)
{
	b3_res  xSize,ySize;
	b3_f64  xScale,yScale,width=20;

	m_ThisRow = (b3_pkd_color *)b3Alloc(tx->xSize * sizeof(b3_pkd_color));
	if (m_ThisRow == null)
	{
		b3PrintF (B3LOG_NORMAL,"Save RGB8: not enough memory!\n");
		throw new b3TxException(B3_TX_MEMORY);
	}

	m_Tx = tx;
	m_Tx->b3Name(filename);
	m_File = fopen (filename,TWRITE);
	if (m_File == null)
	{
		b3Free();
		b3PrintF (B3LOG_NORMAL,"Save PS: not enough memory!\n");
		throw new b3TxException(B3_TX_NOT_SAVED);
	}

	xSize  = m_Tx->xSize;
	ySize  = m_Tx->ySize;
	xScale = width * 72 / 2.54;
	yScale = xScale * ySize / xSize;

	fprintf (m_File, "%%!PS-Adobe-2.0\n");
	fprintf (m_File, "%%%%Creator: Blizzard III - Steffen A. Mork\n");
	fprintf (m_File, "%%%%EndComments\n\n");

	fprintf (m_File, "gsave\n");
	fprintf (m_File, "/picstr %ld string def\n", xSize + xSize + xSize);
	fprintf (m_File, "30 120 translate\n");
	fprintf (m_File, "%f %f scale\n", xScale, yScale);
	fprintf (m_File, "%ld %ld 8\n",xSize,ySize);
	fprintf (m_File, "[%ld 0 0 -%ld 0 %ld]\n",xSize,ySize,ySize);
	fprintf (m_File, "{currentfile picstr readhexstring pop}\n");
	fprintf (m_File, "image\n");
}

void b3InfoPS::b3Write()
{
	b3_coord     x,y;
	b3_pkd_color r, g, b;

	for (y = 0;y < m_Tx->ySize;y++)
	{
		m_Tx->b3GetRow(m_ThisRow,y);
		for (x = 0;x < m_Tx->xSize;x++)
		{
			r = (m_ThisRow[x] & 0xff0000) >> 16;
			g = (m_ThisRow[x] & 0x00f000) >>  8;
			b = (m_ThisRow[x] & 0x0000ff);

			fprintf (m_File, "%02xu",
				(b3_pkd_color)(r * 0.35 + g * 0.51 + b * 0.14));
		}
	}
	fprintf (m_File,"\n");
}

b3InfoPS::~b3InfoPS()
{
	fprintf (m_File, "grestore\n");
	fprintf (m_File, "showpage\n");

	fclose  (m_File);
}

b3_result b3Tx::b3SavePS(const char *filename)
{
	b3InfoPS info(this,filename);
	info.b3Write();
	return B3_OK;
}
