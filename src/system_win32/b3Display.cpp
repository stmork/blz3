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

#include "blz3/b3Config.h"
#include "blz3/system/b3Display.h"
#include "blz3/system/b3Log.h"

#define no_SYNC
#define no_DEBUG

#define READY 128

/*************************************************************************
**                                                                      **
**                        color display routines                        **
**                                                                      **
*************************************************************************/

static b3_bool    display_init = false;
static b3_bool    is_mdi;
static b3Display *single_display;

void b3Display::b3Init()
{
	CWinApp   *app  = AfxGetApp();
	CFrameWnd *main = (CFrameWnd *)app->m_pMainWnd;

	if (!display_init)
	{

		single_display = null;
		is_mdi         = app->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd));

		// Done!
		display_init   = true;
	}

	if (is_mdi)
	{
		// Here we should open a new CDocument
		throw new b3DisplayException(B3_DISPLAY_ERROR);
	}
	else
	{
		if (single_display != null)
		{
			throw new b3DisplayException(B3_DISPLAY_OPEN);
		}
		else
		{
			CView *view = main->GetActiveView();

			if (view->IsKindOf(RUNTIME_CLASS(CB3ScrollView)))
			{
				m_View         = (CB3ScrollView *)view;
				single_display = this;
			}
			else
			{
				throw new b3DisplayException(B3_DISPLAY_OPEN);
			}
		}
	}

	ASSERT_VALID(m_View);
}

void b3Display::b3Open(b3_res xSize,b3_res ySize)
{
	// We should make sure, that we've collected some data
	// about us.
	b3Init();

	m_xs = m_xMax = xSize;
	m_ys = m_yMax = ySize;
#ifdef _DEBUG
	b3PrintF (B3LOG_NORMAL,"xMax:   %4ld\n",xSize);
	b3PrintF (B3LOG_NORMAL,"yMax:   %4ld\n",ySize);
	b3PrintF (B3LOG_NORMAL,"dep:    %4ld\n",m_depth);
#endif
}

void b3Display::b3Close()
{
	if (single_display == this)
	{
		single_display = null;
	}
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
	m_Title = (char *)title;
	b3Open(xSize,ySize);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu (default)\n",
		m_Title,
		m_xs,m_ys);
}

b3Display::b3Display(b3_res xSize,b3_res ySize,const char *title)
{
	m_Title = (char *)title;
	b3Open(xSize,ySize);
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
}

void b3Display::b3PutPixel(b3_coord x,b3_coord y,b3_pkd_color Color)
{
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
}
