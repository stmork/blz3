/*
**
**	$Filename:	b3Print.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - print control
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

#include <math.h>

#include "blz3/system/b3Print.h"

#define no_USE_PRINT
#define USE_STRETCH

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III CB3Print                         **
**                                                                      **
*************************************************************************/

CB3Print::CB3Print(
	b3Tx          *tx,
	b3_count       strips,
	b3_res         strip_height,
	CDC           *pDC,
	CPrintInfo    *pInfo,
	b3_bool        FitOnPage)
{
	b3_count whole_scale;
	b3_f64   scale;
	
	m_PrtWidth  = pDC->GetDeviceCaps (HORZRES);
	m_PrtHeight = pDC->GetDeviceCaps (VERTRES);
	m_xSize     = tx->xSize;
	m_ySize     = strip_height;
	m_Strips    = strips;
	m_Tx        = tx;

	// Compute dimensions
	scale           = (b3_f64)m_PrtWidth / (b3_f64)m_xSize;
	m_StripHeight   = (b3_res)floor(scale * strip_height);
	m_StripsPerPage = m_PrtHeight / m_StripHeight;
	m_NumPages      = (strips + m_StripsPerPage - 1) / m_StripsPerPage;
	if ((m_NumPages > 1) && (FitOnPage))
	{
		scale = (b3_f64)m_PrtHeight / (b3_f64)tx->ySize;
		m_StripHeight   = (b3_res)floor(scale * strip_height);
		m_StripsPerPage = m_PrtHeight / m_StripHeight;
		m_NumPages      = (strips + m_StripsPerPage - 1) / m_StripsPerPage;
	}

	// Compute resolution of scaled strip
	whole_scale     = m_PrtWidth    / m_Tx->xSize;

	// Check if we have to scale smaller. In this case
	// whole_scale will be 0. To prevent division
	// by zero we have to set this value to one due
	// to the fact that rescaling supports minimizing
	// an image.
	if (whole_scale == 0)
	{
		whole_scale     = 1;
		m_NeedGreyScale = true;
	}
	else
	{
		m_NeedGreyScale = false;
	}
	m_ScaleWidth    = m_PrtWidth    / whole_scale;
	m_ScaleHeight   = m_StripHeight / whole_scale;

	// TODO: add extra initialization before printing
	try
	{
		m_Print    = new CB3View;
		m_PrtLine  = new b3Tx;
	}
	catch (...)
	{
		throw b3NoMemory();
	}
	if (!m_PrtLine->b3AllocTx(m_xSize,m_ySize,24))
	{
		throw b3NoMemory();
	}

	// Do we need to rescale a strip?
	if ((m_ScaleWidth == m_xSize) && (m_ScaleHeight == m_ySize))
	{
		m_TxBuffer = null;
	}
	else
	{
		try
		{
			m_TxBuffer = new b3Tx;
			if (!m_TxBuffer->b3AllocTx(m_ScaleWidth,m_ScaleHeight,24))
			{
				delete m_TxBuffer;
				m_TxBuffer = null;
			}
		}
		catch (...)
		{
			m_TxBuffer = null;
		}
	}

	// Set page range
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(m_NumPages);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# printing... %ld strip(s) on %ld page(s)\n",
		strips,m_NumPages);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# original area:       %5ld x %5ld\n",
		m_Tx->xSize,m_Tx->ySize);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# print area:          %5ld x %5ld\n",
		m_PrtWidth,m_PrtHeight);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# original strip size: %5ld x %5ld\n",
		m_xSize,m_ySize);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# scaled strip size:   %5ld x %5ld\n",
		m_ScaleWidth,m_ScaleHeight);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# print strip size:    %5ld x %5ld\n",
		m_PrtWidth,m_StripHeight);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# number of pages:     %5ld\n",
		m_NumPages);
}

CB3Print::CB3Print(
	CDC        *pDC,
	CPrintInfo *pInfo,
	b3_bool     FitOnPage)
{
	m_PrtWidth  = pDC->GetDeviceCaps (HORZRES);
	m_PrtHeight = pDC->GetDeviceCaps (VERTRES);
	m_xSize     = 0;
	m_ySize     = 0;
	m_Strips    = 0;
	m_Tx        = null;

	// Compute dimensions
	m_StripHeight   = 0;
	m_StripsPerPage = 0;
	m_NumPages      = 1;
	m_NeedGreyScale = false;
	m_ScaleWidth    = 1;
	m_ScaleHeight   = 1;

	// TODO: add extra initialization before printing
	try
	{
		m_TxBuffer = null;
		m_PrtLine  = null;
		m_Print    = new CB3View;
	}
	catch (...)
	{
		throw b3NoMemory();
	}

	// Set page range
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(m_NumPages);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# printing... %ld page(s)\n",
		m_NumPages);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# print area:          %5ld x %5ld\n",
		m_PrtWidth,m_PrtHeight);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# original strip size: %5ld x %5ld\n",
		m_xSize,m_ySize);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# scaled strip size:   %5ld x %5ld\n",
		m_ScaleWidth,m_ScaleHeight);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# print strip size:    %5ld x %5ld\n",
		m_PrtWidth,m_StripHeight);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# number of pages:     %5ld\n",
		m_NumPages);
}

CB3Print::~CB3Print()
{
	delete m_Print;
	delete m_PrtLine;
	if (m_TxBuffer != null)
	{
		delete m_TxBuffer;
	}
}

void CB3Print::b3PrintPage(CDC *pDC,CPrintInfo *pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	CWaitCursor  wait_for_Ruediger_Schaper;
	b3_coord     yCur,num,line,CurLine;
	b3_res       CurHeight;
	b3_index     denom;
	b3_count	 max,rest;
	b3Tx        *TxBlit;

	TxBlit = (m_TxBuffer != null ? m_TxBuffer : m_PrtLine);

	// Set viewport mapping/size
	num   = m_PrtWidth;
	denom = m_xSize;
	pDC->SetMapMode      (MM_ANISOTROPIC);
	pDC->ScaleViewportExt(num,denom,num,denom);

	// Setup one strip
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# -------- start printing page %ld%s\n",
		pInfo->m_nCurPage,
		pInfo->m_bPreview ? " (preview)" : "");
	CurLine = (pInfo->m_nCurPage - 1) * m_StripsPerPage;
	rest    = m_Strips - CurLine;
	max     = B3_MIN(m_StripsPerPage,rest);

	// Print each strip
	for (line = 0;line < max;line++)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Print# creating strip %ld, strip %ld of page\n",
			CurLine + line,line);
		yCur = (CurLine + line) * m_ySize;
		CurHeight = m_ySize;
		m_PrtLine->b3Blit(m_Tx,
			0,0,
			m_xSize,CurHeight,
			0,yCur);
		if (m_TxBuffer != null)
		{
			// NOTE: m_TxBuffer is equal to TxBlit!
			if (m_NeedGreyScale)
			{
				m_TxBuffer->b3ScaleToGrey(m_PrtLine);
			}
			else
			{
				m_TxBuffer->b3Scale(m_PrtLine);
			}
		}
	
		// Make drawing visible
		m_Print->b3InitImage(TxBlit,pDC);
		m_Print->b3DrawView(pDC,0,line * m_ScaleHeight);
	}
	pDC->SetViewportExt(CSize(1,1));

	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# -------- stopping printing page %ld\n",
		pInfo->m_nCurPage);
}

void CB3Print::b3PrintPage(
	CDC        *srcDC,
	CDC        *pDC,
	CPrintInfo *pInfo,
	b3Tx     *pTX)
{
	CWaitCursor     wait_for_Ruediger_Schaper;
	unsigned long   xPrtSize,yPrtSize;
	unsigned long   xSrcSize,ySrcSize;
	unsigned long   xDstSize,yDstSize;
	double          SrcRatio,DstRatio;
	int             lines;
	b3Mem         mem;

	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# -------- start printing page %ld%s\n",
		pInfo->m_nCurPage,
		pInfo->m_bPreview ? " (preview)" : "");

	// Retrieve resolution of images
	xPrtSize  = pDC->GetDeviceCaps (HORZRES);
	yPrtSize  = pDC->GetDeviceCaps (VERTRES);
	xSrcSize  = pTX->xSize;
	ySrcSize  = pTX->ySize;

	// Compute right aspect ratio
	SrcRatio  = (double)ySrcSize / (double)xSrcSize;
	DstRatio  = (double)yPrtSize / (double)xPrtSize;
	if (SrcRatio < DstRatio)
	{
		xDstSize  = xPrtSize;
		yDstSize  = (long)(xDstSize * SrcRatio);
	}
	else
	{
		yDstSize  = yPrtSize;
		xDstSize  = (long)(yDstSize / SrcRatio);
	}

	// Init bitmap and inform about TAFs
	m_Print->b3InitImage(pTX,pDC);

	// ??? is xSrcSize right ???
	lines = m_Print->b3Print(srcDC,pDC,xDstSize,yDstSize);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Print# -------- stopping printing page %ld\n",
		pInfo->m_nCurPage);
}
