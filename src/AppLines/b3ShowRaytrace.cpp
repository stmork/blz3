/*
**
**	$Filename:	b3ShowRaytrace.cpp $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3ShowRaytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**
**	Revision 1.2  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
**	Revision 1.1  2001/11/04 21:12:14  sm
**	- New CB3ShowRaytrace control
**	
**	
*/

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
		if (m_Tx.b3AllocTx(m_xSize,m_ySize,24))
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
