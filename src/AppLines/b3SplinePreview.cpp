/*
**
**	$Filename:	b3ImagePreviewFileDlg.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Image file select dialog box
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"
#include "b3SplinePreview.h"

#ifndef B3_PICK_SIZE
#define B3_PICK_SIZE 2
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/03/05 22:12:33  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.2  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.1  2002/03/05 20:38:25  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3SplinePreview implementation               **
**                                                                      **
*************************************************************************/

CB3SplinePreview::CB3SplinePreview()
{
	m_Spline = null;
}

CB3SplinePreview::~CB3SplinePreview()
{
}


BEGIN_MESSAGE_MAP(CB3SplinePreview, CStatic)
	//{{AFX_MSG_MAP(CB3SplinePreview)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3SplinePreview message handlers

void CB3SplinePreview::OnPaint() 
{
	CPaintDC  dc(this); // device context for painting
	CPen      black(PS_SOLID,1,RGB(0,0,0));
	CPen      gray(PS_SOLID,1,RGB(140,140,140));
	CPen     *oldPen;
	b3_vector curve[B3_MAX_SUBDIV + 1];
	b3_count  i,max;
	b3_coord  x,y;
	
	// TODO: Add your message handler code here
	B3_ASSERT(m_Spline != null);

	// Clear drawing area
	dc.FillSolidRect(&m_Rect,RGB(255,255,255));

	// Draw control polygon
	oldPen = dc.SelectObject(&gray);
	b3ComputePoint(&m_Spline->controls[0],x,y);
	dc.MoveTo(x,y);
	for (i = 1;i < m_Spline->control_num;i++)
	{
		b3ComputePoint(&m_Spline->controls[i],x,y);
		dc.LineTo(x,y);
	}
	if (m_Spline->closed)
	{
		b3ComputePoint(&m_Spline->controls[0],x,y);
		dc.LineTo(x,y);
	}
	dc.SelectObject(oldPen);

	// Draw curve
	oldPen = dc.SelectObject(&black);
	max = m_Spline->b3DeBoor(curve);
	b3ComputePoint(&curve[0],x,y);
	dc.MoveTo(x,y);
	for (i = 1;i < max;i++)
	{
		b3ComputePoint(&curve[i],x,y);
		dc.LineTo(x,y);
	}
	dc.SelectObject(oldPen);

	// Draw control points;
	for (i = 0;i < m_Spline->control_num;i++)
	{
		b3ComputePoint(&m_Spline->controls[i],x,y);
		dc.FillSolidRect(
			x - B3_PICK_SIZE,y - B3_PICK_SIZE,
			B3_PICK_SIZE * 2 + 1,B3_PICK_SIZE * 2 + 1,
			RGB(255,16,64));
	}

	// Do not call CStatic::OnPaint() for painting messages
}

void CB3SplinePreview::b3Update(b3Spline *spline)
{
	b3_count i;
	b3_f64   rRatio,cRatio,xSize,ySize;

	m_Spline = spline;
	b3Vector::b3InitBound(&m_Lower,&m_Upper);
	for (i = 0;i < m_Spline->control_num;i++)
	{
		b3Vector::b3AdjustBound(&m_Spline->controls[i],&m_Lower,&m_Upper);
	}

	// Compute dimensions
	xSize = m_Upper.x - m_Lower.x;
	ySize = m_Upper.y - m_Lower.y;
	GetClientRect(&m_Rect);

	// Compute ratios
	rRatio = ySize / xSize;
	cRatio = (b3_f64)m_Rect.Height() / (b3_f64)m_Rect.Width();
	if (rRatio < cRatio)
	{
		m_Ratio   = 0.8 * m_Rect.Width() / xSize;
		m_xOffset = 0.1 * m_Rect.Width();
		m_yOffset = 0.5 * m_Rect.Height() - 0.5 * ySize * m_Ratio;
	}
	else
	{
		m_Ratio   = 0.8 * m_Rect.Height() / ySize;
		m_yOffset = 0.1 * m_Rect.Height();
		m_xOffset = 0.5 * m_Rect.Width() - 0.5 * xSize * m_Ratio;
	}
	Invalidate();
}

void CB3SplinePreview::b3ComputePoint(b3_vector *point, b3_coord &x, b3_coord &y)
{
	x = (b3_coord)floor((point->x - m_Lower.x) * m_Ratio + m_xOffset + 0.5);
	y = (b3_coord)floor((point->y - m_Lower.y) * m_Ratio + m_yOffset + 0.5);
}
