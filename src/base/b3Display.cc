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

#include "blz3/base/b3Display.h"
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

b3Display::b3Display()
{
	m_xs     = 0;
	m_ys     = 0;
	m_depth  = 0;
	m_Buffer = null;
	m_Tx     = new b3Tx();
}

b3Display::b3Display(b3Tx *tx)
{
	m_xs     = tx->xSize;
	m_ys     = tx->ySize;
	m_depth  = tx->depth;
	m_Buffer = (b3_pkd_color *)tx->b3GetData();
	m_Tx     = tx;
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
	m_xs    = xSize;
	m_ys    = ySize;
	m_depth = 24;
	m_Tx    = new b3Tx();
	m_Tx->b3AllocTx(xSize,ySize,m_depth);
	m_Buffer = (b3_pkd_color *)m_Tx->b3GetData();
}

b3Display::~b3Display()
{
	b3PrintF(B3LOG_FULL,"Closing display...\n");
}


void b3Display::b3GetRes(b3_res &xSize,b3_res &ySize)
{
	xSize = m_xs;
	ySize = m_ys;
}

void b3Display::b3PutRow(b3Row *row)
{
	b3_coord      y = row->m_y;
	b3_pkd_color *src,*dst;

	B3_ASSERT(m_Buffer != null);
	src = row->m_buffer;
	dst = &m_Buffer[y * m_xs];
	if (src != dst)
	{
		b3LongMemCopy(dst,src,m_xs);
	}
}

void b3Display::b3PutPixel(b3_coord x,b3_coord y,b3_pkd_color Color)
{
	B3_ASSERT(m_Buffer != null);
	if ((x < 0) || (x >= m_xs)) return;
	if ((y < 0) || (y >= m_ys)) return;
	m_Buffer[y * m_xs + x] = Color;
}

b3_pkd_color  b3Display::b3GetPixel(b3_coord x,b3_coord y)
{
	B3_ASSERT(m_Buffer != null);
	if ((x < 0) || (x >= m_xs)) return 0;
	if ((y < 0) || (y >= m_ys)) return 0;
	return (m_Buffer[y * m_xs + x]);
}

b3_bool b3Display::b3IsCancelled(b3_coord x,b3_coord y)
{
	return false;
}

void b3Display::b3Wait()
{
}
