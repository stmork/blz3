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

#include "blz3/b3Config.h"
#include "AppLines.h"
#include "AppLinesView.h"
#include "DlgFulcrum.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
}


void CDlgFulcrum::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialogbar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFulcrum)
	DDX_Control(pDX, IDC_FULCRUM_Z, m_zCtrl);
	DDX_Control(pDX, IDC_FULCRUM_Y, m_yCtrl);
	DDX_Control(pDX, IDC_FULCRUM_X, m_xCtrl);
	//}}AFX_DATA_MAP
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

BOOL CDlgFulcrum::OnInitDialogBar()
{
	if (!CB3Dialogbar::OnInitDialogBar())
	{
		return FALSE;
	}
	m_xCtrl.b3SetDigits(5,2);
	m_yCtrl.b3SetDigits(5,2);
	m_zCtrl.b3SetDigits(5,2);
	return TRUE;
}

void CDlgFulcrum::b3GetData()
{
	if (m_pDoc != null)
	{
		m_pDoc->m_Info->m_Center.x = m_xCtrl.m_Value;
		m_pDoc->m_Info->m_Center.y = m_yCtrl.m_Value;
		m_pDoc->m_Info->m_Center.z = m_zCtrl.m_Value;
	}
}

void CDlgFulcrum::b3SetData()
{
	m_xCtrl.EnableWindow(m_pDoc != null);
	m_yCtrl.EnableWindow(m_pDoc != null);
	m_zCtrl.EnableWindow(m_pDoc != null);
	if (m_pDoc != null)
	{
		m_xCtrl.b3SetValue(m_pDoc->m_Info->m_Center.x);
		m_yCtrl.b3SetValue(m_pDoc->m_Info->m_Center.y);
		m_zCtrl.b3SetValue(m_pDoc->m_Info->m_Center.z);
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
