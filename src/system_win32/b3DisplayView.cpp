/*
**
**      $Filename:      b3DisplayView.cpp $
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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

void b3DisplayView::b3Open(CB3ScrollView *view,b3_res xSize,b3_res ySize)
{
	// We should make sure, that we've collected some data
	// about us.
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

	m_xs    = xSize;
	m_ys    = ySize;
	m_depth = 24;
	m_RowCounter = 0;
#ifdef _DEBUG
	b3PrintF (B3LOG_NORMAL,"xSize: %4ld\n",m_xs);
	b3PrintF (B3LOG_NORMAL,"ySize: %4ld\n",m_ys);
	b3PrintF (B3LOG_NORMAL,"dep:   %4ld\n",m_depth);
#endif

	b3Tx *tx = m_Doc->m_Tx;

	tx->b3AllocTx(m_xs,m_ys,m_depth);
	m_Buffer = (b3_pkd_color *)tx->b3GetData();
}

b3DisplayView::b3DisplayView(
	CB3ScrollView *view,
	const char    *title)
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

b3DisplayView::b3DisplayView(
	CB3ScrollView *view,
	b3_res         xSize,
	b3_res         ySize,
	const char    *title)
{
	m_Title = (char *)title;
	b3Open(view,xSize,ySize);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu\n",
		m_Title,
		m_xs,m_ys);
}

b3DisplayView::~b3DisplayView()
{
}


void b3DisplayView::b3PutRow(b3Row *row)
{
	b3Display::b3PutRow(row);
	if ((m_RowCounter++ & 0x1f) == 0)
	{
		m_View->OnRefresh(m_View,B3_UPDATE_TX,null);
	}
}

void b3DisplayView::b3Wait()
{
	m_View->OnRefresh(m_View,B3_UPDATE_TX,null);
}
