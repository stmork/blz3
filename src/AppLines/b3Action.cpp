/*
**
**      $Filename:      b3Action.cpp $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - What to do on mouse input
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3Action.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/08/21 18:01:11  sm
**	- Minor updates
**
**
*/

/*************************************************************************
**                                                                      **
**                        CB3Action default implementation              **
**                                                                      **
*************************************************************************/

CB3Action::CB3Action(CAppLinesView *window)
{
	m_View   = window;
	m_Button = B3_MB_UP;
}

void CB3Action::b3DispatchMouseMove  (b3_coord x,b3_coord y)
{
	if ((x != m_xLast) || (y != m_yLast))
	{
		switch (m_Button)
		{
		case B3_MB_UP:
			b3MouseMove(x,y);
			break;
		case B3_MB_LEFT:
			b3LMove(x,y);
			break;
		case B3_MB_MIDDLE:
			b3MMove(x,y);
			break;
		case B3_MB_RIGHT:
			b3RMove(x,y);
			break;
		}
		m_xLast = x;
		m_yLast = y;
	}
}

void CB3Action::b3DispatchLButtonDown(b3_coord x,b3_coord y)
{
	m_View->SetCapture();
	m_Button = B3_MB_LEFT;
	m_xStart = m_xLast = x;
	m_yStart = m_yLast = y;
	b3LDown(x,y);
}

void CB3Action::b3DispatchLButtonUp  (b3_coord x,b3_coord y)
{
	b3LUp(x,y);
	m_Button = B3_MB_UP;
	::ReleaseCapture();
}

void CB3Action::b3DispatchMButtonDown(b3_coord x,b3_coord y)
{
	m_View->SetCapture();
	m_Button = B3_MB_MIDDLE;
	m_xStart = m_xLast = x;
	m_yStart = m_yLast = y;
	b3MDown(x,y);
}

void CB3Action::b3DispatchMButtonUp(b3_coord x,b3_coord y)
{
	b3MUp(x,y);
	m_Button = B3_MB_UP;
	::ReleaseCapture();
}

void CB3Action::b3DispatchRButtonDown(b3_coord x,b3_coord y)
{
	m_View->SetCapture();
	m_Button = B3_MB_RIGHT;
	m_xStart = m_xLast = x;
	m_yStart = m_yLast = y;
	b3RDown(x,y);
}

void CB3Action::b3DispatchRButtonUp(b3_coord x,b3_coord y)
{
	b3RUp(x,y);
	m_Button = B3_MB_UP;
	::ReleaseCapture();
}

void CB3Action::b3GetRelCoord(
	b3_coord  x,
	b3_coord  y,
	b3_f64   &xRel,
	b3_f64   &yRel)
{
	CRect  rect;
	b3_f64 xSize,ySize;

	m_View->GetClientRect(&rect);
	xSize = rect.Width();
	ySize = rect.Height();
	xRel  = x / xSize;
	yRel  = y / ySize;
}

void CB3Action::b3MouseMove(b3_coord x,b3_coord y)
{
}

void CB3Action::b3LDown(b3_coord x,b3_coord y)
{
}

void CB3Action::b3LMove(b3_coord x,b3_coord y)
{
}

void CB3Action::b3LUp(b3_coord x,b3_coord y)
{
}

void CB3Action::b3MDown(b3_coord x,b3_coord y)
{
}

void CB3Action::b3MMove(b3_coord x,b3_coord y)
{
}

void CB3Action::b3MUp(b3_coord x,b3_coord y)
{
}

void CB3Action::b3RDown(b3_coord x,b3_coord y)
{
}

void CB3Action::b3RMove(b3_coord x,b3_coord y)
{
}

void CB3Action::b3RUp(b3_coord x,b3_coord y)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionMagnify implementation               **
**                                                                      **
*************************************************************************/

CB3ActionMagnify::CB3ActionMagnify(CAppLinesView *window) :
	CB3Action(window)
{
}

void CB3ActionMagnify::b3LMove(b3_coord x,b3_coord y)
{
	m_View->b3DrawRect(m_xStart,m_yStart,m_xLast,m_yLast);
	m_View->b3DrawRect(m_xStart,m_yStart,x,y);
}

void CB3ActionMagnify::b3LUp(b3_coord x,b3_coord y)
{
	b3_f64 x1,y1,x2,y2;

	b3GetRelCoord(m_xStart,m_yStart,x1,y1);
	b3GetRelCoord(x,y,x2,y2);
	m_View->m_RenderView.b3Select(x1,y1,x2,y2);
	m_View->b3Update(B3_UPDATE_VIEW);
	m_View->b3UnsetMagnification();
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectSelect implementation          **
**                                                                      **
*************************************************************************/

CB3ActionObjectSelect::CB3ActionObjectSelect(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectMove implementation            **
**                                                                      **
*************************************************************************/

CB3ActionObjectMove::CB3ActionObjectMove(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectRotate implementation          **
**                                                                      **
*************************************************************************/

CB3ActionObjectRotate::CB3ActionObjectRotate(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectScale implementation           **
**                                                                      **
*************************************************************************/

CB3ActionObjectScale::CB3ActionObjectScale(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraMove implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraMove::CB3ActionCameraMove(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraTurn implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraTurn::CB3ActionCameraTurn(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraRotate implementation          **
**                                                                      **
*************************************************************************/

CB3ActionCameraRotate::CB3ActionCameraRotate(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraView implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraView::CB3ActionCameraView(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionLightTurn implementation             **
**                                                                      **
*************************************************************************/

CB3ActionLightTurn::CB3ActionLightTurn(CAppLinesView *window) :
	CB3Action(window)
{
}
