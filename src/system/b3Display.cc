/*
**
**      $Filename:      b3Display.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**      $Id$
**
**      Blizzard III - Controlling a color display
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard includes                             **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Display.h"
#include "blz3/base/b3Aux.h"

#define no_SYNC
#define no_DEBUG

#ifdef _DEBUG
#define LOW_RES
#endif

#define READY 128

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2002/08/11 11:03:40  sm
**	- Moved b3Display and b3Row classes from base lib into system
**	  independend lib.
**	- Made b3TimeSpan more system independend;-)
**
**	Revision 1.7  2002/01/22 17:11:17  sm
**	- brt3 is now able to save images. The selection of image type
**	  is unsoved yet.
**	- Better b3DisplayView in Un*x port.
**	- Fixed stricmp() in Un*x port.
**	
**	Revision 1.6  2001/12/23 10:58:38  sm
**	- Accelerated b3Display.
**	- Fixed YUV conversion.
**	- Accelerated ILBM access to image  pixel/row.
**	
**	Revision 1.5  2001/12/08 21:37:38  sm
**	- Added "No Gfx" support
**	
**	Revision 1.4  2001/11/08 19:31:33  sm
**	- Nasty CR/LF removal!
**	- Added TGA/RGB8/PostScript image saving.
**	- Hoping to win Peter H. for powerful MFC programming...
**	
**	Revision 1.3  2001/11/04 21:12:14  sm
**	- New CB3ShowRaytrace control
**	
**	Revision 1.2  2001/11/04 12:15:15  sm
**	- Renaming some attributes...
**	- Taking account to redesign of b3Display
**	
**	Revision 1.1  2001/11/04 10:54:14  sm
**	- Redesigned b3Display for control use.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        color display routines                        **
**                                                                      **
*************************************************************************/

b3Display::b3Display(b3Tx *tx)
{
	m_xMax   = tx->xSize;
	m_yMax   = tx->ySize;
	m_depth  = tx->depth;
	m_Buffer = (b3_pkd_color *)tx->b3GetData();
	m_Tx     = tx;
	m_OwnTx  = false;
}

b3Display::b3Display()
{
	b3_coord xSize;
	b3_coord ySize;

#ifdef LOW_RES
	xSize = 240;
	ySize = 180;
#else
	xSize = 768;
	ySize = 576;
#endif
	b3Init(xSize,ySize,"");
}

b3Display::b3Display(const char *title)
{
	b3_coord xSize;
	b3_coord ySize;

#ifdef LOW_RES
	xSize = 240;
	ySize = 180;
#else
	xSize = 768;
	ySize = 576;
#endif
	b3Init(xSize,ySize,title);
}

b3Display::b3Display(b3_res xSize,b3_res ySize,const char *title)
{
	b3Init(xSize,ySize,title);
}

void b3Display::b3Init(b3_res xSize,b3_res ySize,const char *title)
{
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu\n",
		title,
		xSize,ySize);
	m_OwnTx = true;
	m_xMax  = xSize;
	m_yMax  = ySize;
	m_depth = 24;
	m_Tx    = new b3Tx();
	m_Tx->b3AllocTx(xSize,ySize,m_depth);
	m_Buffer = (b3_pkd_color *)m_Tx->b3GetData();
}

b3Display::~b3Display()
{
	b3PrintF(B3LOG_FULL,"Closing display...\n");
	if ((m_OwnTx) && (m_Tx != null))
	{
		delete m_Tx;
	}
}


void b3Display::b3GetRes(b3_res &xSize,b3_res &ySize)
{
	xSize = m_xMax;
	ySize = m_yMax;
}

void b3Display::b3PutRow(b3Row *row)
{
	b3_coord      y = row->m_y;
	b3_pkd_color *src,*dst;

	B3_ASSERT(m_Buffer != null);
	src = row->m_buffer;
	dst = &m_Buffer[y * m_xMax];
	if (src != dst)
	{
		b3LongMemCopy(dst,src,m_xMax);
	}
}

void b3Display::b3PutTx(b3Tx *tx)
{
	b3_coord y;

	for (y = 0;y < m_yMax;y++)
	{
		tx->b3GetRow(&m_Buffer[y * m_xMax],y);
	}
}

void b3Display::b3PutPixel(b3_coord x,b3_coord y,b3_pkd_color Color)
{
	B3_ASSERT(m_Buffer != null);
	if ((x < 0) || (x >= m_xMax)) return;
	if ((y < 0) || (y >= m_yMax)) return;
	m_Buffer[y * m_xMax + x] = Color;
}

b3_pkd_color  b3Display::b3GetPixel(b3_coord x,b3_coord y)
{
	B3_ASSERT(m_Buffer != null);
	if ((x < 0) || (x >= m_xMax)) return 0;
	if ((y < 0) || (y >= m_yMax)) return 0;
	return (m_Buffer[y * m_xMax + x]);
}

b3_bool b3Display::b3IsCancelled(b3_coord x,b3_coord y)
{
	return false;
}

void b3Display::b3Wait()
{
}

b3_bool b3Display::b3SaveImage(const char *filename)
{
	return m_Tx->b3SaveImage(filename) == B3_OK;
}
