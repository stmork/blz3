/*
**
**	$Filename:	DlgObjectCopy.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for copying objects
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "DlgObjectCopy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/01/08 16:21:58  sm
**	- Added center to copy dialog
**
**	Revision 1.1  2002/01/07 21:19:30  sm
**	- Adding object copy dialog
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgObjectCopy implementation                 **
**                                                                      **
*************************************************************************/

CDlgObjectCopy::CDlgObjectCopy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjectCopy::IDD, pParent)
{
	CWinApp *app = AfxGetApp();

	//{{AFX_DATA_INIT(CDlgObjectCopy)
	m_NumCopies = app->GetProfileInt(CB3ClientString(),"copy count", 1);
	//}}AFX_DATA_INIT
	m_OrigBBox = null;
	m_Move.x   = atof(app->GetProfileString(CB3ClientString(),"move object x", "100.0"));
	m_Move.y   = atof(app->GetProfileString(CB3ClientString(),"move object y",   "0.0"));
	m_Move.z   = atof(app->GetProfileString(CB3ClientString(),"move object z",   "0.0"));
	m_Rotate.x = atof(app->GetProfileString(CB3ClientString(),"rotate object x", "0.0"));
	m_Rotate.y = atof(app->GetProfileString(CB3ClientString(),"rotate object y", "0.0"));
	m_Rotate.z = atof(app->GetProfileString(CB3ClientString(),"rotate object z","90.0"));
}


void CDlgObjectCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjectCopy)
	DDX_Control(pDX, IDC_OFFSET_Z, m_zCtrlOffset);
	DDX_Control(pDX, IDC_OFFSET_Y, m_yCtrlOffset);
	DDX_Control(pDX, IDC_OFFSET_X, m_xCtrlOffset);
	DDX_Control(pDX, IDC_ROT_Z, m_zCtrlRot);
	DDX_Control(pDX, IDC_ROT_Y, m_yCtrlRot);
	DDX_Control(pDX, IDC_ROT_X, m_xCtrlRot);
	DDX_Control(pDX, IDC_CENTER_Z, m_xCtrlCenter);
	DDX_Control(pDX, IDC_CENTER_Y, m_yCtrlCenter);
	DDX_Control(pDX, IDC_CENTER_X, m_zCtrlCenter);
	DDX_Text(pDX, IDC_NUM_COPIES, m_NumCopies);
	DDV_MinMaxUInt(pDX, m_NumCopies, 1, 1023);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjectCopy, CDialog)
	//{{AFX_MSG_MAP(CDlgObjectCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjectCopy message handlers

BOOL CDlgObjectCopy::OnInitDialog() 
{
	CDialog::OnInitDialog();
	B3_ASSERT(m_Center != null);

	m_xCtrlOffset.b3SetValue(m_Move.x);
	m_yCtrlOffset.b3SetValue(m_Move.y);
	m_zCtrlOffset.b3SetValue(m_Move.z);
	m_xCtrlRot.b3SetValue(m_Rotate.x);
	m_yCtrlRot.b3SetValue(m_Rotate.y);
	m_zCtrlRot.b3SetValue(m_Rotate.z);
	m_xCtrlCenter.b3SetValue(m_Center->x);
	m_yCtrlCenter.b3SetValue(m_Center->y);
	m_zCtrlCenter.b3SetValue(m_Center->z);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
