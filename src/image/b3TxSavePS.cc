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

#include "b3TxSaveInfo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
**                          PS                                          **
**                                                                      **
*************************************************************************/

class b3InfoPS : protected b3TxSaveInfo
{
public:
	      b3InfoPS(b3Tx *tx,const char *filename);
	     ~b3InfoPS();
	void  b3Write();
};

b3InfoPS::b3InfoPS(b3Tx *tx,const char *filename) :
	b3TxSaveInfo(tx,filename,TWRITE)
{
	b3_res  xSize,ySize;
	b3_f64  xScale,yScale,width=20;

	xSize  = m_Tx->xSize;
	ySize  = m_Tx->ySize;
	xScale = width * 72 / 2.54;
	yScale = xScale * ySize / xSize;

	fprintf (m_FileHandle, "%%!PS-Adobe-2.0\n");
	fprintf (m_FileHandle, "%%%%Creator: Blizzard III - Steffen A. Mork\n");
	fprintf (m_FileHandle, "%%%%EndComments\n\n");

	fprintf (m_FileHandle, "gsave\n");
	fprintf (m_FileHandle, "/picstr %ld string def\n", xSize + xSize + xSize);
	fprintf (m_FileHandle, "30 120 translate\n");
	fprintf (m_FileHandle, "%f %f scale\n", xScale, yScale);
	fprintf (m_FileHandle, "%ld %ld 8\n",xSize,ySize);
	fprintf (m_FileHandle, "[%ld 0 0 -%ld 0 %ld]\n",xSize,ySize,ySize);
	fprintf (m_FileHandle, "{currentfile picstr readhexstring pop}\n");
	fprintf (m_FileHandle, "image\n");
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

			fprintf (m_FileHandle, "%02xu",
				(b3_pkd_color)(r * 0.35 + g * 0.51 + b * 0.14));
		}
	}
	fprintf (m_FileHandle,"\n");
}

b3InfoPS::~b3InfoPS()
{
	fprintf (m_FileHandle, "grestore\n");
	fprintf (m_FileHandle, "showpage\n");
}

b3_result b3Tx::b3SavePS(const char *filename)
{
	b3InfoPS info(this,filename);
	info.b3Write();
	return B3_OK;
}
