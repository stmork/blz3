/*
**
**	$Filename:	DlgStepRotate.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setting angle steps
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
#include "DlgStepRotate.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2001/12/31 16:39:41  sm
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
**                        CDlgStepRotate implementation                 **
**                                                                      **
*************************************************************************/

CDlgStepRotate::CDlgStepRotate(CWnd* pParent /*=NULL*/)
	: CB3Dialogbar()
{
	//{{AFX_DATA_INIT(CDlgStepRotate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Info = null;
}


void CDlgStepRotate::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialogbar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStepRotate)
	DDX_Control(pDX, IDC_STEP_ROT_Z, m_zCtrl);
	DDX_Control(pDX, IDC_STEP_ROT_Y, m_yCtrl);
	DDX_Control(pDX, IDC_STEP_ROT_X, m_xCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStepRotate, CB3Dialogbar)
	//{{AFX_MSG_MAP(CDlgStepRotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStepRotate message handlers

BOOL CDlgStepRotate::OnInitDialog()
{
	if (!CB3Dialogbar::OnInitDialog())
	{
		return FALSE;
	}
	m_xCtrl.b3SetMin(epsilon);
	m_yCtrl.b3SetMin(epsilon);
	m_zCtrl.b3SetMin(epsilon);
	m_xCtrl.b3SetMax(180);
	m_yCtrl.b3SetMax(180);
	m_zCtrl.b3SetMax(180);
	m_xCtrl.b3SetDigits(5,2);
	m_yCtrl.b3SetDigits(5,2);
	m_zCtrl.b3SetDigits(5,2);
	return TRUE;
}

void CDlgStepRotate::b3GetData()
{
	if (m_Info != null)
	{
		m_Info->m_StepRotate.x = m_xCtrl.m_Value;
		m_Info->m_StepRotate.y = m_yCtrl.m_Value;
		m_Info->m_StepRotate.z = m_zCtrl.m_Value;
	}
}

void CDlgStepRotate::b3SetData()
{
	m_xCtrl.EnableWindow(m_Info != null);
	m_yCtrl.EnableWindow(m_Info != null);
	m_zCtrl.EnableWindow(m_Info != null);
	if (m_Info != null)
	{
		m_xCtrl.b3SetValue(m_Info->m_StepRotate.x);
		m_yCtrl.b3SetValue(m_Info->m_StepRotate.y);
		m_zCtrl.b3SetValue(m_Info->m_StepRotate.z);
	}
	else
	{
		m_xCtrl.SetWindowText("");
		m_yCtrl.SetWindowText("");
		m_zCtrl.SetWindowText("");
	}
}
