/*
**
**	$Filename:	DlgCSG1.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for CSG spheres
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "DlgCSG1.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/02/24 17:45:31  sm
**	- Added CSG edit dialogs
**	- Corrected shape edit inheritance.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgShape3 implementation                     **
**                                                                      **
*************************************************************************/

CDlgCSG1::CDlgCSG1(CWnd* pParent /*=NULL*/)
	: CB3CSGDialog(CDlgCSG1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSG1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgCSG1::CDlgCSG1(UINT IDD,CWnd* pParent /*=NULL*/)
	: CB3CSGDialog(IDD, pParent)
{
}


void CDlgCSG1::DoDataExchange(CDataExchange* pDX)
{
	CB3CSGDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSG1)
	DDX_Control(pDX, IDC_BASE_X,   m_xBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Y,   m_yBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Z,   m_zBaseCtrl);
	DDX_Control(pDX, IDC_DIR1_X,   m_xDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_Y,   m_yDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_Z,   m_zDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_LEN, m_lenDir1Ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCSG1, CB3CSGDialog)
	//{{AFX_MSG_MAP(CDlgCSG1)
	ON_EN_KILLFOCUS(IDC_BASE_X, OnChangedBase)
	ON_EN_KILLFOCUS(IDC_BASE_Y, OnChangedBase)
	ON_EN_KILLFOCUS(IDC_BASE_Z, OnChangedBase)
	ON_EN_KILLFOCUS(IDC_DIR1_X, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_Y, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_Z, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_LEN, OnChangedLen1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCSG1 message handlers

int CDlgCSG1::b3Edit(b3Item *item,b3_bool create)
{
	CDlgCSG1 dlg;

	dlg.m_Creation = create;
	dlg.m_Shape    = (b3CSGSphere *)item;
	return dlg.DoModal();
}

const char *CDlgCSG1::b3GetSection()
{
	return "csg1";
}

BOOL CDlgCSG1::OnInitDialog() 
{
	b3CSGSphere *shape = (b3CSGSphere *)m_Shape;

	m_Base.b3Init(&shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Base,&shape->m_Dir,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
	CB3CSGDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_Creation)
	{
		m_Base.b3Read(b3GetSection() + CString(".base"));
		m_Dir1.b3Read(b3GetSection() + CString(".dir1"));
	}

	m_Base.b3Set(true);
	m_Dir1.b3Set(m_DirMode,true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCSG1::b3SetDirMode(int dirmode)
{
	CB3CSGDialog::b3SetDirMode(dirmode);
	m_Base.b3Set();
	m_Dir1.b3Set(m_DirMode);
}

void CDlgCSG1::b3UpdateBase()
{
	CB3CSGDialog::b3UpdateBase();
	m_Dir1.b3Update(m_DirMode);
}

void CDlgCSG1::OnChangedBase() 
{
	// TODO: Add your control notification handler code here
	m_Base.b3Update();
	b3UpdateBase();
}

void CDlgCSG1::OnChangedDir1() 
{
	// TODO: Add your control notification handler code here
	m_Dir1.b3Update(m_DirMode);
}

void CDlgCSG1::OnChangedLen1() 
{
	// TODO: Add your control notification handler code here
	m_Dir1.b3UpdateLen(m_DirMode);
}

void CDlgCSG1::OnOK() 
{
	// TODO: Add extra validation here
	CB3CSGDialog::OnOK();
	if (m_Creation)
	{
		m_Base.b3Write(b3GetSection() + CString(".base"));
		m_Dir1.b3Write(b3GetSection() + CString(".dir1"));
	}
}
