/*
**
**	$Filename:	b3TxSavePS.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3TxInclude.h"

/*************************************************************************
**                                                                      **
**                          PS                                          **
**                                                                      **
*************************************************************************/

#define COLOR_PS

class b3InfoPS : protected b3TxSaveInfo
{
public:
	b3InfoPS(b3Tx * tx, const char * filename);
	~b3InfoPS();
	void  b3Write();
};

b3InfoPS::b3InfoPS(b3Tx * tx, const char * filename) :
	b3TxSaveInfo(tx, filename, B3_TWRITE)
{
	b3_res  xSize, ySize;

	xSize  = m_Tx->xSize;
	ySize  = m_Tx->ySize;

	fprintf(m_FileHandle, "%%!PS-Adobe-2.0\n");
	fprintf(m_FileHandle, "%%%%Creator: Blizzard III - Steffen A. Mork\n");
#ifdef GREY_PS
	fprintf(m_FileHandle, "%%%%EndComments\n\n");

	fprintf(m_FileHandle, "gsave\n");
	fprintf(m_FileHandle, "/picstr %ld string def\n", xSize + xSize + xSize);
	fprintf(m_FileHandle, "30 120 translate\n");
	fprintf(m_FileHandle, "%f %f scale\n", xScale, yScale);
	fprintf(m_FileHandle, "%ld %ld 8\n", xSize, ySize);
	fprintf(m_FileHandle, "[%ld 0 0 -%ld 0 %ld]\n", xSize, ySize, ySize);
	fprintf(m_FileHandle, "{currentfile picstr readhexstring pop}\n");
	fprintf(m_FileHandle, "image\n");
#else
	fprintf(m_FileHandle, "%%%%DocumentData: Clean7Bit\n");
	fprintf(m_FileHandle, "%%%%Origin: 0 0\n");
	fprintf(m_FileHandle, "%%%%BoundingBox: 0 0 %ld %ld\n", xSize, ySize);
	fprintf(m_FileHandle, "%%%%LanguageLevel: 1\n");
	fprintf(m_FileHandle, "%%%%Pages: (atend)\n");
	fprintf(m_FileHandle, "%%%%EndComments\n");
	fprintf(m_FileHandle, "%%%%BeginSetup\n");
	fprintf(m_FileHandle, "%%%%EndSetup\n");
	fprintf(m_FileHandle, "%%%%Page: 1 1\n");
	fprintf(m_FileHandle, "gsave\n");
	fprintf(m_FileHandle, "100 dict begin\n");
	fprintf(m_FileHandle, "%ld.000000 %ld.000000 scale\n", xSize, ySize);
	fprintf(m_FileHandle, "%%ImageData: %ld %ld 8 3 0 %ld 2 \"true 3 colorimage\"\n", xSize, ySize, xSize);
	fprintf(m_FileHandle, "/line0 %ld string def\n", xSize);
	fprintf(m_FileHandle, "/line1 %ld string def\n", xSize);
	fprintf(m_FileHandle, "/line2 %ld string def\n", xSize);
	fprintf(m_FileHandle, "%ld %ld 8\n", xSize, ySize);
	fprintf(m_FileHandle, "[%ld 0 0 -%ld 0 %ld]\n", xSize, ySize, ySize);
	fprintf(m_FileHandle, "{currentfile line0 readhexstring pop}bind\n");
	fprintf(m_FileHandle, "{currentfile line1 readhexstring pop}bind\n");
	fprintf(m_FileHandle, "{currentfile line2 readhexstring pop}bind\n");
	fprintf(m_FileHandle, "true 3 colorimage\n");
#endif
}

void b3InfoPS::b3Write()
{
	b3_coord     x, y;

#ifdef GREY_PS
	b3_pkd_color r, g, b;

	for(y = 0; y < m_Tx->ySize; y++)
	{
		m_Tx->b3GetRow(m_ThisRow, y);
		for(x = 0; x < m_Tx->xSize; x++)
		{
			r = (m_ThisRow[x] & 0xff0000) >> 16;
			g = (m_ThisRow[x] & 0x00f000) >>  8;
			b = (m_ThisRow[x] & 0x0000ff);

			fprintf(m_FileHandle, "%02x",
				(b3_pkd_color)(r * 0.35 + g * 0.51 + b * 0.14));
		}
		fprintf(m_FileHandle, "\n");
	}
#else
	b3_loop c;
	b3_coord shift;
	b3_pkd_color r;

	for(y = 0; y < m_Tx->ySize; y++)
	{
		m_Tx->b3GetRow(m_ThisRow, y);
		for(c = 0; c < 3; c++)
		{
			shift = (2 - c) << 3;
			for(x = 0; x < m_Tx->xSize; x++)
			{
				r = (m_ThisRow[x] & (0xff << shift)) >> shift;

				fprintf(m_FileHandle, "%02x", r);
			}
			fprintf(m_FileHandle, "\n");
		}
	}
#endif
}

b3InfoPS::~b3InfoPS()
{
#ifdef GREY_PS
	fprintf(m_FileHandle, "grestore\n");
	fprintf(m_FileHandle, "showpage\n");
#else
	fprintf(m_FileHandle, "end\n");
	fprintf(m_FileHandle, "grestore\n");
	fprintf(m_FileHandle, "showpage\n");
	fprintf(m_FileHandle, "%%%%Trailer\n");
	fprintf(m_FileHandle, "%%%%Pages: 1\n");
	fprintf(m_FileHandle, "%%%%EOF\n");
#endif
}

const b3_result b3Tx::b3SavePS(const char * filename)
{
	b3PrintF(B3LOG_FULL, "Saving PS: %s\n", filename);

	b3InfoPS info(this, filename);
	info.b3Write();
	return B3_OK;
}
