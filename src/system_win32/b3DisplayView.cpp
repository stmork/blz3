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

#include "blz3/system/b3DisplayView.h"
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
**	Revision 1.9  2005/12/06 15:10:08  smork
**	- Windows const update.
**
**	Revision 1.8  2005/10/16 12:11:22  sm
**	- Some HDR fixes.
**	
**	Revision 1.7  2005/10/09 14:39:41  sm
**	- Added HDR image processing
**	
**	Revision 1.6  2002/08/15 13:56:44  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.5  2002/08/11 11:03:41  sm
**	- Moved b3Display and b3Row classes from base lib into system
**	  independend lib.
**	- Made b3TimeSpan more system independend;-)
**	
**	Revision 1.4  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.3  2001/12/23 14:59:05  sm
**	- Updated new b3Display interface
**	
**	Revision 1.2  2001/11/04 21:12:14  sm
**	- New CB3ShowRaytrace control
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

void b3DisplayView::b3Open(CB3ScrollView *view,const b3_res xSize, const b3_res ySize)
{
	// We should make sure, that we've collected some data
	// about us.
	B3_ASSERT(view != null);

	if (view->IsKindOf(RUNTIME_CLASS(CB3ScrollView)))
	{
		m_View = view;
		m_Doc  = m_View->b3GetDocument();
	}
	else
	{
		B3_THROW(b3DisplayException,B3_DISPLAY_OPEN);
	}

	b3SetRowRefreshCount();
	m_xMax  = xSize;
	m_yMax  = ySize;
	m_depth = 128;
	m_RowCounter = 0;
#ifdef _DEBUG
	b3PrintF (B3LOG_NORMAL,"xSize: %4ld\n",m_xMax);
	b3PrintF (B3LOG_NORMAL,"ySize: %4ld\n",m_yMax);
	b3PrintF (B3LOG_NORMAL,"dep:   %4ld\n",m_depth);
#endif

	m_Tx->b3AllocTx(m_xMax,m_yMax,m_depth);
	m_Buffer = (b3_color *)m_Tx->b3GetData();
	if (!m_Tx->b3IsHDR())
	{
		B3_THROW(b3DisplayException,B3_DISPLAY_ERROR);
	}
}

b3DisplayView::b3DisplayView(
	CB3ScrollView *view,
	const char    *title) : b3Display(view->b3GetDocument()->m_Tx)
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
		m_xMax,m_yMax);
}

b3DisplayView::b3DisplayView(
	CB3ScrollView *view,
	const b3_res   xSize,
	const b3_res   ySize,
	const char    *title) : b3Display(view->b3GetDocument()->m_Tx)
{
	m_Title = (char *)title;
	b3Open(view,xSize,ySize);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu\n",
		m_Title,
		m_xMax,m_yMax);
}

b3DisplayView::~b3DisplayView()
{
	b3PrintF(B3LOG_FULL,"Closing view display...\n");
}

void b3DisplayView::b3SetRowRefreshCount(b3_count refresh_count)
{
	m_RowRefreshCount = refresh_count;
}

void b3DisplayView::b3PutRow(const b3Row *row)
{
	b3Display::b3PutRow(row);
	if ((m_RowCounter++ % m_RowRefreshCount) == 0)
	{
		m_View->OnRefresh(m_View,B3_UPDATE_TX,null);
	}
}

void b3DisplayView::b3PutTx(b3Tx *tx)
{
	b3Display::b3PutTx(tx);
	m_View->OnRefresh(m_View,B3_UPDATE_TX,null);
}

void b3DisplayView::b3Wait()
{
	m_View->OnRefresh(m_View,B3_UPDATE_TX,null);
}
