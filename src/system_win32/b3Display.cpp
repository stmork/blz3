/*
**
**      $Filename:      b3Display.c $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
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
**                        color display routines                        **
**                                                                      **
*************************************************************************/

void b3Display::b3Init(CB3ScrollView *view)
{
	if (view->IsKindOf(RUNTIME_CLASS(CB3ScrollView)))
	{
		m_View = view;
		m_Doc  = m_View->b3GetDocument();
	}
	else
	{
		throw new b3DisplayException(B3_DISPLAY_OPEN);
	}

	ASSERT(m_View != null);
}

void b3Display::b3Open(CB3ScrollView *view,b3_res xSize,b3_res ySize)
{
	// We should make sure, that we've collected some data
	// about us.
	b3Init(view);

	m_xs    = xSize;
	m_ys    = ySize;
	m_depth = 24;
#ifdef _DEBUG
	b3PrintF (B3LOG_NORMAL,"xSize: %4ld\n",m_xs);
	b3PrintF (B3LOG_NORMAL,"ySize: %4ld\n",m_ys);
	b3PrintF (B3LOG_NORMAL,"dep:   %4ld\n",m_depth);
#endif

	b3Tx *tx = m_Doc->m_Tx;

	tx->b3AllocTx(m_xs,m_ys,24);
	m_Buffer = (b3_pkd_color *)tx->b3GetData();
}

void b3Display::b3Close()
{
}

b3Display::b3Display(CB3ScrollView *view,const char *title)
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
	m_Title = (char *)title;
	b3Open(view,xSize,ySize);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu (default)\n",
		m_Title,
		m_xs,m_ys);
}

b3Display::b3Display(CB3ScrollView *view,b3_res xSize,b3_res ySize,const char *title)
{
	m_Title = (char *)title;
	b3Open(view,xSize,ySize);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu\n",
		m_Title,
		m_xs,m_ys);
}

b3Display::~b3Display()
{
	b3Close();
}


void b3Display::b3GetRes(b3_res &xSize,b3_res &ySize)
{
	xSize = m_xs;
	ySize = m_ys;
}

void b3Display::b3PutRow(b3Row *row)
{
	b3_coord y = row->y;

	b3LongMemCopy(&m_Buffer[y * m_xs],row->buffer,m_xs);
	if ((row->y & 0x1f) == 0)
	{
		m_View->OnRefresh(m_View,B3_UPDATE_TX,null);
	}
}

void b3Display::b3PutPixel(b3_coord x,b3_coord y,b3_pkd_color Color)
{
	if ((x < 0) || (x >= m_xs)) return;
	if ((y < 0) || (y >= m_ys)) return;
	m_Buffer[y * m_xs + x] = Color;
}

b3_pkd_color  b3Display::b3GetPixel(b3_coord x,b3_coord y)
{
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
	m_View->OnRefresh(m_View,B3_UPDATE_TX,null);
}
