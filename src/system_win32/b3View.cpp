/*
**
**	$Filename:	b3View.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - view display routines
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
#include "blz3/system/b3View.h"

#define no_USE_CLIPPING

/*************************************************************************
**                                                                      **
**                        b3View: preparing an image for                **
**                                drawing into CScrollView              **
**                                                                      **
*************************************************************************/

CB3View::CB3View()
{
	m_AutoMemDC          = false;
	b3SetPos  (0,0);
	b3PrintF (B3LOG_FULL,"### CLASS: b3View instantiated\n");
}

CB3View::~CB3View()
{
	b3DeinitBitmap();
}

// Set the position for this view.
void CB3View::b3SetPos(
	b3_coord x,
	b3_coord y)
{
	xPos = x;
	yPos = y;
	b3PrintF (B3LOG_FULL,
		"### CLASS: b3View # b3SetPos():     %4ld x %4ld\n",
		xPos,yPos);
}

/*************************************************************************
**                                                                      **
**                        The drawing routines...                       **
**                                                                      **
*************************************************************************/

void CB3View::b3UpdateView(
	b3Tx          *texture,
	CScrollView   *view,
	b3_coord       yStart,
	b3_coord       yEnd)
{
	CDC *srcDC;

	srcDC = view->GetDC();
	if (srcDC != null)
	{
		b3InitImage(texture,srcDC,yStart,yEnd);
		view->ReleaseDC(srcDC);
	}
	else
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3View # b3UpdateView(): source DC unspecified!\n");
	}
}

// Make the image visible!
// (--< Call from CB3View::OnDraw() >--)
void CB3View::b3DrawView(CDC *pDC,b3_coord xOrig,b3_coord yOrig)
{
	b3_bool result;

	b3PrintF (B3LOG_FULL,
		"### CLASS: b3View # b3DrawView(): Start -------------\n");
	result = (b3Draw(pDC,0,0,xOrig,yOrig) > 0);
	b3PrintF (B3LOG_FULL,
		"### CLASS: b3View # b3DrawView(): Stop  ------------- "
		"(%4ld x %4ld) (%ld,%ld) -> (%ld,%ld)%s\n",
		xSize,ySize,
		xPos,yPos,
		xOrig,yOrig,result ? "" : " (error)");
}

b3_count CB3View::b3Print(
	CDC           *displayDC,
	CDC           *printDC,
	b3_res  xDstSize,
	b3_res  yDstSize)
{
	b3_count lines;
	b3_bool  result;

	lines = b3Draw(printDC,xDstSize,yDstSize);
	result = (lines > 0);
	b3PrintF (B3LOG_FULL,
		"### CLASS: b3View # b3DrawView(): (%ld x %ld) (%ld,%ld)%s\n",
		xSize,ySize,
		xPos, yPos,
		result ? "" : " (error)");
	return lines;
}

b3_count CB3View::b3Draw(
	CDC      *pDC,
	b3_res    xDstSize,
	b3_res    yDstSize,
	b3_coord  xOrig,
	b3_coord  yOrig)
{
	CPalette *pal;
	b3_count  lines;
	b3_bool   result;

	// Define palette
	pal  = b3GetPalette();
	if ((pal != null) && (m_Mode == DIB_PAL_COLORS))
	{
		pDC->SelectPalette(pal,FALSE);
		pDC->RealizePalette();
	}

	// Setting up some default values
	if (xDstSize == 0) xDstSize = xSize;
	if (yDstSize == 0) yDstSize = ySize;

	// !!! Blitting Image !!!
	lines = ::StretchDIBits(
		pDC->m_hDC,			// where to draw
		xOrig,yOrig,		// dst position
		xDstSize,yDstSize,	// dst size
		xPos,yPos,			// src position
		xSize,ySize,		// src size
		m_Row,				// where is the bitmap data
		m_DIB,				// bitmap info
		m_Mode,
		SRCCOPY);
	result = (lines > 0);

	b3PrintF (B3LOG_FULL,
		"### CLASS: b3View # b3Draw(): (%ld x %ld) (%ld,%ld) -> (%ld x %ld) (%ld,%ld)%s\n",
		xSize,   ySize,
		xPos,    yPos,
		xDstSize,yDstSize,
		xOrig,   yOrig,
		result ? "" : " (error)");
	return lines;
}

#define PRINT_MEM (3 * 1024 * 1024)

