/*
**
**	$Filename:	b3ShowColor.cpp $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - displaying an HSV color cycle
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3ShowColor.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/12/03 18:37:51  sm
**	- Added light distribution curve control.
**
**	Revision 1.1  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3ShowColor: Display a color cycle which     **
**                                      displays a selected color.      **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CB3ShowColor, CStatic)
	//{{AFX_MSG_MAP(CB3ShowColor)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3ShowColor::CB3ShowColor()
{
	m_xSize = 0;
	m_ySize = 0;
}

b3_bool CB3ShowColor::b3Update()
{
	CDC           *dc;
	CRect          rect;
	b3Tx           colorTx;
	b3_pkd_color  *lPtr,color;
	b3_coord       x,xHalf;
	b3_coord       y,yHalf;
	b3_coord       radius = 5;
	b3_f64         fx,fy,angle,distance;
	b3_f64         r,g,b;
	b3_bool        result = false;

	GetClientRect(&rect);

	m_xSize = rect.Width();
	m_ySize = rect.Height();
	xHalf   = m_xSize >> 1;
	yHalf   = m_ySize >> 1;

	if (colorTx.b3AllocTx(m_xSize,m_ySize,24))
	{
		lPtr = (b3_pkd_color *)colorTx.b3GetData();
		for (y = 0;y < m_ySize;y++)
		{
			for (x = 0;x < m_xSize;x++)
			{
				fx = (b3_f64)(x - xHalf) / (b3_f64)(xHalf - radius);
				fy = (b3_f64)(y - yHalf) / (b3_f64)(yHalf - radius);

				distance = sqrt(fx * fx + fy * fy);
				if (distance < 1.0)
				{
					angle = atan2(fy,fx);
				
					// [Hehe: Drehstrom!]
					r     = sin(angle);
					g     = sin(angle - 3.14159265 * 2.0 / 3.0);
					b     = sin(angle + 3.14159265 * 2.0 / 3.0);

					// Clipping
					if (r < 0.0) r = 0.0;
					if (g < 0.0) g = 0.0;
					if (b < 0.0) b = 0.0;

					r = ((r * distance) + (1.0 - distance)) * 255.0 * v;
					g = ((g * distance) + (1.0 - distance)) * 255.0 * v;
					b = ((b * distance) + (1.0 - distance)) * 255.0 * v;

					// Clipping
					if (r > 255.0) r = 255.0;
					if (g > 255.0) g = 255.0;
					if (b > 255.0) b = 255.0;

					// Make sure values are valid.
					ASSERT(r <= 255.0);
					ASSERT(g <= 255.0);
					ASSERT(b <= 255.0);

					color =
						((unsigned long)r << 16) |
						((unsigned long)g <<  8) |
						((unsigned long)b);
				}
				else
				{
					color = 0x00cccccc;
				}
				*lPtr++ = color;
			}
		}

		// Setup bitmap
		dc = GetDC();
		m_DDB.b3InitImage(&colorTx,dc);
		ReleaseDC(dc);
		Invalidate();
		result = true;
	}
	return result;
}

void CB3ShowColor::OnPaint()
{
	CPaintDC       dc(this);
	CBrush         mark(RGB(255,255,255));
	CBrush        *oldBrush;
	CPen           pen(PS_SOLID,1,RGB(0,0,0));
	CPen          *oldPen;
	CRect          rect;
	CPoint         center;
	b3_coord       xHalf,yHalf,radius = 5;
	b3_coord       x,y;

	xHalf = m_xSize >> 1;
	yHalf = m_ySize >> 1;

	x = (b3_coord)(cos(h) * s * (xHalf - radius));
	y = (b3_coord)(sin(h) * s * (yHalf - radius));
	center.x = xHalf + x;
	center.y = yHalf + y;
	rect.SetRect(
		center.x - radius,center.y - radius,
		center.x + radius,center.y + radius);

	CStatic::OnPaint();
	oldBrush = dc.SelectObject(&mark);
	oldPen   = dc.SelectObject(&pen);
	dc.BitBlt(0,0,m_xSize,m_ySize,m_DDB.b3GetDC(),0,0,SRCCOPY);
	dc.Ellipse(&rect);
	dc.SelectObject(oldBrush);
	dc.SelectObject(oldPen);
}
