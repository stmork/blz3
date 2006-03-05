/*
**
**	$Filename:	DlgFulcrum.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setting fulcrum coordinates
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include "DlgFulcrum.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.4  2002/03/08 16:46:14  sm
**	- Added new CB3IntSpinButtonCtrl. This is much
**	  better than standard integer CSpinButtonCtrl.
**	- Added a test control to test spin button controls
**	  and float control.
**	- Made spin button controls and float edit control
**	  DDXable. The spin button controls need only
**	  a simple edit field without any DDX CEdit reference
**	  or value reference inside a dialog.
**	- Changed dialogs to reflect new controls. This was a
**	  major cleanup which shortens the code in an elegant way.
**	
**	Revision 1.3  2001/12/31 16:39:40  sm
**	- Made hierarchy dialog a CDialogBar
**	
**	Revision 1.2  2001/12/26 18:17:56  sm
**	- More status bar information displayed (e.g. coordinates)
**	- Some minor UI updates
**	
**	Revision 1.1  2001/12/25 18:52:39  sm
**	- Introduced CB3Dialogbar for dialogs opened any time.
**	- Fulcrum fixed with snap to grid
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgFulcrum implementation                    **
**                                                                      **
*************************************************************************/

CDlgFulcrum::CDlgFulcrum(CWnd* pParent /*=NULL*/)
	: CB3Dialogbar()
{
	//{{AFX_DATA_INIT(CDlgFulcrum)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = null;
	m_xCtrl.b3SetDigits(5,2);
	m_yCtrl.b3SetDigits(5,2);
	m_zCtrl.b3SetDigits(5,2);
}


void CDlgFulcrum::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialogbar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFulcrum)
	DDX_Control(pDX, IDC_FULCRUM_Z, m_zCtrl);
	DDX_Control(pDX, IDC_FULCRUM_Y, m_yCtrl);
	DDX_Control(pDX, IDC_FULCRUM_X, m_xCtrl);
	//}}AFX_DATA_MAP
	if (m_pDoc != null)
	{
		m_xCtrl.b3DDX(pDX,m_pDoc->m_Info->m_Center.x);
		m_yCtrl.b3DDX(pDX,m_pDoc->m_Info->m_Center.y);
		m_zCtrl.b3DDX(pDX,m_pDoc->m_Info->m_Center.z);
	}
}


BEGIN_MESSAGE_MAP(CDlgFulcrum, CDialogBar)
	//{{AFX_MSG_MAP(CDlgFulcrum)
	ON_EN_KILLFOCUS(IDC_FULCRUM_X, OnKillfocusFulcrum)
	ON_EN_KILLFOCUS(IDC_FULCRUM_Y, OnKillfocusFulcrum)
	ON_EN_KILLFOCUS(IDC_FULCRUM_Z, OnKillfocusFulcrum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFulcrum message handlers

void CDlgFulcrum::b3GetData()
{
	UpdateData();
}

void CDlgFulcrum::b3SetData()
{
	m_xCtrl.EnableWindow(m_pDoc != null);
	m_yCtrl.EnableWindow(m_pDoc != null);
	m_zCtrl.EnableWindow(m_pDoc != null);
	if (m_pDoc != null)
	{
		UpdateData(FALSE);
	}
	else
	{
		m_xCtrl.SetWindowText("");
		m_yCtrl.SetWindowText("");
		m_zCtrl.SetWindowText("");
	}
}

void CDlgFulcrum::OnKillfocusFulcrum() 
{
	// TODO: Add your control notification handler code here
	b3GetData();
	if (m_pDoc != null)
	{
		m_pDoc->UpdateAllViews(NULL,B3_UPDATE_FULCRUM);
	}
}
