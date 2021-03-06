/*
**
**	$Filename:	b3ShowImage.cpp $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - displaying an image inside a control
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

#include "b3SystemInclude.h"
#include "blz3/system/b3ShowImage.h"
#include "blz3/system/b3PaintDC.h"

/*************************************************************************
**                                                                      **
**                        CB3ShowImage: Display an image inside         **
**                                      a static control                **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3ShowImage, CStatic)

BEGIN_MESSAGE_MAP(CB3ShowImage, CStatic)
	//{{AFX_MSG_MAP(CB3ShowImage)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3ShowImage::CB3ShowImage()
{
}

b3_bool CB3ShowImage::b3Load(const char *name)
{
	b3_bool success = false;

	if (name != null)
	{
		success = (m_Tx.b3LoadImage(name) == B3_OK);
	}
	else
	{
		m_Tx.b3FreeTx();
	}
	return success;
}

void CB3ShowImage::b3Aspect(
	b3Tx     *srcTx,
	b3_coord &xPos,
	b3_coord &yPos,
	b3_res   &xSize,
	b3_res   &ySize,
	b3_bool   keep_aspect)
{
	b3_f64 scale,scaleSrc,scaleDst;

	GetClientRect(&m_Rect);
	if (keep_aspect)
	{
		scaleSrc = (b3_f64)srcTx->ySize    / (b3_f64)srcTx->xSize;
		scaleDst = (b3_f64)m_Rect.Height() / (b3_f64)m_Rect.Width();
		if (scaleSrc > scaleDst)
		{
			// src is more portrait
			scale = (b3_f64)m_Rect.Height() / (b3_f64)srcTx->ySize;
			xSize = (b3_res)floor(scale * srcTx->xSize + 0.5);
			ySize =  m_Rect.Height();
			xPos  = (m_Rect.Width() - xSize) >> 1;
			yPos  = 0;
		}
		else
		{
			scale = (b3_f64)m_Rect.Width() / (b3_f64)srcTx->xSize;
			xSize =  m_Rect.Width();
			ySize = (b3_res)floor(scale * srcTx->ySize + 0.5);
			xPos  = 0;
			yPos  = (m_Rect.Height() - ySize) >> 1;
		}
	}
	else
	{
		xPos  = 0;
		yPos  = 0;
		xSize = m_Rect.Width();
		ySize = m_Rect.Height();
	}
}

b3_bool CB3ShowImage::b3Update(
	b3_bool filtered,
	b3_bool keep_aspect)
{
	CDC      *dc;
	b3Tx      scaled;
	b3_count  depth;

	if (m_Tx.b3IsLoaded())
	{
		b3Aspect(&m_Tx,m_xPos,m_yPos,m_xSize,m_ySize,keep_aspect);

		depth = m_Tx.depth;
		if ((m_xSize == m_Tx.xSize) && (m_ySize == m_Tx.ySize))
		{
			scaled.b3Copy(&m_Tx);
		}
		else
		{
			if (filtered)
			{
#if 1
				// Use optimal color depth
				scaled.b3AllocTx(m_xSize,m_ySize,depth <= 8 ? 8 : depth);
#else
				// Force true color
				scaled.b3AllocTx(m_xSize,m_ySize,24);
#endif
				scaled.b3ScaleToGrey(&m_Tx);
			}
			else
			{
				scaled.b3AllocTx(m_xSize,m_ySize,m_Tx.depth);
				scaled.b3Scale(&m_Tx);
			}
		}
		dc = GetDC();
		m_DDB.b3InitImage(&scaled,dc);
		ReleaseDC(dc);
		Invalidate();
	}
	return false;
}

void CB3ShowImage::OnPaint()
{
	CB3PaintDC    dc(this);

	GetClientRect(&m_Rect);
	dc.FillSolidRect(&m_Rect,GetSysColor(COLOR_BTNFACE));
	dc.BitBlt(m_xPos,m_yPos,m_xSize,m_ySize,m_DDB.b3GetDC(),0,0,SRCCOPY);
}

CB3ShowImage::operator b3Tx *()
{
	return &m_Tx;
}

void CB3ShowImage::b3Copy(b3Tx *original)
{
	m_Tx.b3Copy(original);
}
