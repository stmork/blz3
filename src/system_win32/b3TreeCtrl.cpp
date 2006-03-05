/*
**
**	$Filename:	b3TreeCtrl.cpp $  
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - TreeCtrl handling
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "blz3/system/b3TreeCtrl.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/03/05 21:22:37  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.2  2002/02/01 15:04:09  sm
**	- Prepared shapes for icon conversion
**	- Added to save selected/first visible item in
**	  hierarchy dialog.
**	- Some print cleanups done.
**	- Fixed activation of b3SuperSample.
**	
**	Revision 1.1  2002/01/24 15:55:58  sm
**	- Fixed key handling on TreeCtrl (hierarchy dialog bar)
**	- Added support for conext menu depending on scene/object edit.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III - docking window collections     **
**                                                                      **
*************************************************************************/
									
CB3TreeCtrl::CB3TreeCtrl()
{
}

CB3TreeCtrl::~CB3TreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CB3TreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CB3TreeCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3TreeCtrl message handlers

BOOL CB3TreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->message == WM_KEYDOWN) && (GetEditControl() != null))
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
		case VK_DELETE:
		case VK_ESCAPE:
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;

		default:
			// Do nothing
			break;
		}
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CB3TreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT      uFlags;
	HTREEITEM item;

	item = HitTest(point,&uFlags);
	if ((item != NULL) && (uFlags & TVHT_ONITEM))
	{
//		Select(item,TVGN_CARET);
	}
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CB3TreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT      uFlags;
	HTREEITEM item;

	item = HitTest(point,&uFlags);
	if ((item != NULL) && (uFlags & TVHT_ONITEM))
	{
//		Select(item,TVGN_DROPHILITE);
	}
	CTreeCtrl::OnRButtonDown(nFlags, point);
}

HTREEITEM CB3TreeCtrl::b3FindLParam(HTREEITEM parent,LPARAM lParam)
{
	HTREEITEM  item,result;

	for(item  = GetNextItem(parent,TVGN_CHILD);
	    item != NULL;
		item  = GetNextItem(item,TVGN_NEXT))
	{
		if (lParam == (LPARAM)GetItemData(item))
		{
			// Found! Done...
			return item;
		}

		// Fast way out!
		result = b3FindLParam(item,lParam);
		if (result != null)
		{
			return result;
		}
	}
	return NULL;
}
