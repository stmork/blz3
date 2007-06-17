/*
**
**	$Filename:	b3ColorField.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - color field control
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

#include "b3SystemInclude.h"
#include "blz3/system/b3ColorField.h"
#include "blz3/system/b3PaintDC.h"

/*************************************************************************
**                                                                      **
**                        Draw a color field                            **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3ColorField, CStatic)
	//{{AFX_MSG_MAP(CB3ColorField)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3ColorField::CB3ColorField()
{
	m_Color = 0x808080;
}

void CB3ColorField::b3SetColor(b3_pkd_color color)
{
	m_Color = color;
	InvalidateRect (null);
}

void CB3ColorField::OnPaint()
{
	CB3PaintDC dc(this);
	CRect      rect;

	GetClientRect(&rect);
	dc.FillSolidRect(&rect,UL_2_COLORREF(m_Color));
}

/*************************************************************************
**                                                                      **
**                        Draw a colored line to show its thickness     **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3LineWidth, CStatic)
	//{{AFX_MSG_MAP(CB3LineWidth)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3LineWidth::CB3LineWidth()
{
	m_Color = 0x808080;
	m_Width = 1;
}

b3_res CB3LineWidth::b3SetColor(
	b3_pkd_color color,
	b3_res       width)
{
	CRect    rect;
	b3_coord diff,y;

	// Which width is allowed?
	GetClientRect(&rect);
	diff = (rect.right  - rect.left) >> 1;
	y    = (rect.bottom + rect.top) >> 1;
	if (width > diff)
	{
		width = diff;
	}
	if (width > y)
	{
		width = y;
	}

	// Copy values and refresh
	m_Color = color;
	m_Width = width;
	InvalidateRect (null);

	return width;
}

void CB3LineWidth::OnPaint()
{
	CB3PaintDC dc(this);
	CRect      rect;
	CPen       pen(PS_SOLID,m_Width,UL_2_COLORREF(m_Color));
	b3_coord   x1,x2,y,diff;

	// Get dimensions to draw line dynamically
	GetClientRect(&rect);
	diff = (rect.right  - rect.left) >> 2;
	x1   =  rect.left   + diff;
	x2   =  rect.right  - diff;
	y    = (rect.bottom + rect.top) >> 1;

	// First draw new background then draw line
	dc.FillSolidRect(&rect,GetSysColor(COLOR_BTNFACE));
	dc.SelectObject(&pen);
	dc.MoveTo(x1,y);
	dc.LineTo(x2,y);
}

/*************************************************************************
**                                                                      **
**                        Draw a colored font                           **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3FontField, CStatic)
	//{{AFX_MSG_MAP(CB3FontField)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3FontField::CB3FontField()
{
	m_fgColor = 0x808080;
	m_bgColor = 0x000000;
	m_Font    = null;
}

void CB3FontField::b3SetColor(
	b3_pkd_color  fgColor,
	b3_pkd_color  bgColor,
	LOGFONT      *font)
{
	// Copy values and refresh
	m_fgColor = fgColor;
	m_bgColor = bgColor;
	m_Font    = font;
	InvalidateRect (null);
}

b3_bool CB3FontField::b3IsEmpty()
{
	b3_u08   *ptr,sum = 0;
	b3_index  size,i;

	ptr = (b3_u08 *)m_Font;
	if (ptr != null)
	{
		size = sizeof(LOGFONT);
		for (i = 0;i < size;i++)
		{
			sum |= *ptr++;
		}
	}
	return sum == 0;
}

void CB3FontField::OnPaint()
{
	CB3PaintDC    dc(this);
	CFont         font;
	CFont        *old;
	CRect         rect;
	char         *text = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

	GetClientRect(&rect);
	font.CreateFontIndirect(m_Font);
	old = dc.SelectObject(&font);
	dc.SetTextColor(UL_2_COLORREF(m_fgColor));
	dc.SetBkColor  (UL_2_COLORREF(m_bgColor));
	dc.ExtTextOut  (0,0,ETO_CLIPPED|ETO_OPAQUE,&rect,text,(UINT)strlen(text),NULL);
	dc.SelectObject(old);
}

/*************************************************************************
**                                                                      **
**                        Draw a color field                            **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3HSVField, CStatic)
	//{{AFX_MSG_MAP(CB3HSVField)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3HSVField::CB3HSVField()
{
	m_Mode = B3_HSV_H;
	m_H    = 0.0;
	m_S    = 0.0;
	m_V    = 1.0;
}

void CB3HSVField::b3SetHSV(
	b3_hsv_mode mode,
	b3_f64        h,
	b3_f64        s,
	b3_f64        v)
{
	m_Mode = mode;
	m_H    = h;
	m_S    = s;
	m_V    = v;
	InvalidateRect (null);
}

b3_bool CB3HSVField::b3HSVtoRGB(
	b3_f64  h,b3_f64  s,b3_f64  v,
	b3_f64 &r,b3_f64 &g,b3_f64 &b)
{
	// Convert to RGB
	// [Hehe: Drehstrom!]
	r = sin(h);
	g = sin(h - 3.14159265 * 2.0 / 3.0);
	b = sin(h + 3.14159265 * 2.0 / 3.0);

	// bottom clipping
	if (r < 0.0) r = 0.0;
	if (g < 0.0) g = 0.0;
	if (b < 0.0) b = 0.0;

	// Compute s & v
	r = ((r * s) + (1.0 - s)) * v;
	g = ((g * s) + (1.0 - s)) * v;
	b = ((b * s) + (1.0 - s)) * v;

	return true;
}

inline COLORREF CB3HSVField::b3ComputeColorHSV(b3_f64 angle,b3_f64 distance,b3_f64 volume)
{
	double r,g,b;

	b3HSVtoRGB(angle,distance,volume,r,g,b);

	// Clipping
	if (r > 1.0) r = 1.0;
	if (g > 1.0) g = 1.0;
	if (b > 1.0) b = 1.0;

	return RGB(r * 255,g * 255,b * 255);
}

void CB3HSVField::OnPaint()
{
	CB3PaintDC  dc(this);
	CRect       rect;
	CPen       *oldPen,color;
	b3_res      x,xSize,ySize;
	b3_f64      angle,distance;

	GetClientRect(&rect);
	xSize = rect.Width();
	ySize = rect.Height();

	switch (m_Mode)
	{
	case B3_HSV_H:
		// Update hue...
		// ...saturation stays constant
		distance = m_S;
		for (x = 0;x < xSize;x++)
		{
			angle    = (b3_f64)x / (b3_f64)xSize * 2.0 * 3.14159265;

			// Draw grey ramp
			color.CreatePen(PS_SOLID,1,b3ComputeColorHSV(angle,distance,m_V));
			oldPen = dc.SelectObject(&color);
			dc.MoveTo(x,0);
			dc.LineTo(x,ySize);
			dc.SelectObject(oldPen);
			color.DeleteObject();
		}
		break;

	case B3_HSV_S:
		// Update saturation...
		// ... color (hue) stays constant
		angle = m_H;
		for (x = 0;x < xSize;x++)
		{
			distance = (b3_f64)x / (b3_f64)xSize;

			// Draw grey ramp
			color.CreatePen(PS_SOLID,1,b3ComputeColorHSV(angle,distance,m_V));
			oldPen = dc.SelectObject(&color);
			dc.MoveTo(x,0);
			dc.LineTo(x,ySize);
			dc.SelectObject(oldPen);
			color.DeleteObject();
		}
		break;

	case B3_HSV_V:
		// Update saturation...
		// ... color (hue) stays constant
		for (x = 0;x < xSize;x++)
		{
			double level = 255.0 * (b3_f64)x / (b3_f64)xSize;

			// Draw grey ramp
			color.CreatePen(PS_SOLID,1,RGB(level,level,level));
			oldPen = dc.SelectObject(&color);
			dc.MoveTo(x,0);
			dc.LineTo(x,ySize);
			dc.SelectObject(oldPen);
			color.DeleteObject();
		}
		break;
	}
}
