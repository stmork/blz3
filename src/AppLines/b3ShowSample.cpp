/*
**
**	$Filename:	b3ShowSample.cpp $  
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - displaying a sampled image inside a control
**
**	(C) Copyright 2004  Steffen A. Mork
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

#include "AppLinesInclude.h"
#include "b3ShowSample.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/03/05 22:12:33  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.1  2004/04/09 12:08:05  sm
**	- New CStatic control introduced for material sampling.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CB3ShowRaytrace implementation                **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3ShowSample, CB3ShowImage)

BEGIN_MESSAGE_MAP(CB3ShowSample, CB3ShowImage)
	//{{AFX_MSG_MAP(CB3ShowSample)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3ShowSample::CB3ShowSample()
{
}

void CB3ShowSample::b3Update(b3Sampler *sampler)
{
	CDC         *dc;
	CWaitCursor  wait;

	sampler->b3Sample();
	dc = GetDC();
	m_DDB.b3InitImage(&m_Tx,dc);
	ReleaseDC(dc);
	Invalidate();
}

void CB3ShowSample::b3Init()
{
	CRect        rect;

	GetClientRect(&rect);
	m_xPos  = 0;
	m_yPos  = 0;
	m_xSize = rect.Width();
	m_ySize = rect.Height();
	m_Tx.b3AllocTx(m_xSize,m_ySize,24);
}
