/*
**
**	$Filename:	b3ShowRaytrace.cpp $  
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - displaying a raytraced image inside a control
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
#include "blz3/system/b3ShowRaytrace.h"

/*************************************************************************
**                                                                      **
**                        CB3ShowRaytrace implementation                **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3ShowRaytrace, CB3ShowImage)

BEGIN_MESSAGE_MAP(CB3ShowRaytrace, CB3ShowImage)
	//{{AFX_MSG_MAP(CB3ShowRaytrace)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3ShowRaytrace::CB3ShowRaytrace()
{
	m_Display = null;
}

void CB3ShowRaytrace::OnDestroy() 
{
	CB3ShowImage::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_Display;
}

void CB3ShowRaytrace::b3Update(b3Scene *scene)
{
	CDC         *dc;
	CRect        rect;
	CWaitCursor  wait;

	if (m_Display == null)
	{
		GetClientRect(&rect);
		m_xPos  = 0;
		m_yPos  = 0;
		m_xSize = rect.Width();
		m_ySize = rect.Height();
		if (m_Tx.b3AllocTx(m_xSize, m_ySize, 128))
		{
			m_Display = new b3Display(&m_Tx);
		}
	}
	scene->b3Raytrace(m_Display);
	dc = GetDC();
	m_DDB.b3InitImage(&m_Tx,dc);
	ReleaseDC(dc);
	Invalidate();
}
