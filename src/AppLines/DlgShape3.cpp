/*
**
**	$Filename:	DlgShape3.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for quadric shapes
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
#include "DlgShape3.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/02/23 22:02:49  sm
**	- Added shape/object edit.
**	- Added shape/object deletion.
**	- Added (de-)activation even for shapes.
**	- Added create/change dialogs for following shapes:
**	  o sphere
**	  o area, disk
**	  o cylinder, cone, ellipsoid, box
**	- Changed hierarchy to reflect these changes.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgShape3 implementation                     **
**                                                                      **
*************************************************************************/

CDlgShape3::CDlgShape3(CWnd* pParent /*=NULL*/)
	: CB3ShapeDialog(CDlgShape3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShape3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgShape3::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShape3)
	DDX_Control(pDX, IDC_BASE_X,   m_xBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Y,   m_yBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Z,   m_zBaseCtrl);
	DDX_Control(pDX, IDC_DIR1_X,   m_xDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_Y,   m_yDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_Z,   m_zDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_LEN, m_lenDir1Ctrl);
	DDX_Control(pDX, IDC_DIR2_X,   m_xDir2Ctrl);
	DDX_Control(pDX, IDC_DIR2_Y,   m_yDir2Ctrl);
	DDX_Control(pDX, IDC_DIR2_Z,   m_zDir2Ctrl);
	DDX_Control(pDX, IDC_DIR2_LEN, m_lenDir2Ctrl);
	DDX_Control(pDX, IDC_DIR3_X,   m_xDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_Y,   m_yDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_Z,   m_zDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_LEN, m_lenDir3Ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShape3, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CDlgShape3)
	ON_EN_KILLFOCUS(IDC_DIR1_X, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_Y, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_Z, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_LEN, OnChangedLen1)
	ON_EN_KILLFOCUS(IDC_DIR2_X, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR2_Y, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR2_Z, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR2_LEN, OnChangedLen2)
	ON_EN_KILLFOCUS(IDC_DIR3_X, OnChangedDir3)
	ON_EN_KILLFOCUS(IDC_DIR3_Y, OnChangedDir3)
	ON_EN_KILLFOCUS(IDC_DIR3_Z, OnChangedDir3)
	ON_EN_KILLFOCUS(IDC_DIR3_LEN, OnChangedLen3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShape3 message handlers

int CDlgShape3::b3Edit(b3Item *item,b3_bool create)
{
	CDlgShape3 dlg;

	dlg.m_Creation = create;
	dlg.m_Shape    = (b3Shape3 *)item;
	return dlg.DoModal();
}

BOOL CDlgShape3::OnInitDialog() 
{
	m_Base.b3Init(&m_Shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&m_Shape->m_Base,&m_Shape->m_Dir1,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
	m_Dir2.b3Init(&m_Shape->m_Base,&m_Shape->m_Dir2,&m_xDir2Ctrl,&m_yDir2Ctrl,&m_zDir2Ctrl,&m_lenDir2Ctrl);
	m_Dir3.b3Init(&m_Shape->m_Base,&m_Shape->m_Dir3,&m_xDir3Ctrl,&m_yDir3Ctrl,&m_zDir3Ctrl,&m_lenDir3Ctrl);
	m_DirMode = AfxGetApp()->GetProfileInt(CB3ClientString(),"shape3.mode",m_DirMode);
	CB3ShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_Creation)
	{
		m_Base.b3Read("shape3.base");
		m_Dir1.b3Read("shape3.dir1");
		m_Dir2.b3Read("shape3.dir2");
		m_Dir3.b3Read("shape3.dir3");
	}

	m_Base.b3Set(true);
	m_Dir1.b3Set(m_DirMode,true);
	m_Dir2.b3Set(m_DirMode,true);
	m_Dir3.b3Set(m_DirMode,true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShape3::b3SetDirMode(int dirmode)
{
	m_Base.b3Set();
	m_Dir1.b3Set(m_DirMode);
	m_Dir2.b3Set(m_DirMode);
	m_Dir3.b3Set(m_DirMode);
}

void CDlgShape3::OnChangedDir1() 
{
	// TODO: Add your control notification handler code here
	m_Dir1.b3Update(m_DirMode);
}

void CDlgShape3::OnChangedLen1() 
{
	// TODO: Add your control notification handler code here
	m_Dir1.b3UpdateLen(m_DirMode);
}

void CDlgShape3::OnChangedDir2() 
{
	// TODO: Add your control notification handler code here
	m_Dir2.b3Update(m_DirMode);
}

void CDlgShape3::OnChangedLen2() 
{
	// TODO: Add your control notification handler code here
	m_Dir2.b3UpdateLen(m_DirMode);
}

void CDlgShape3::OnChangedDir3() 
{
	// TODO: Add your control notification handler code here
	m_Dir3.b3Update(m_DirMode);
}

void CDlgShape3::OnChangedLen3() 
{
	// TODO: Add your control notification handler code here
	m_Dir3.b3UpdateLen(m_DirMode);
}

void CDlgShape3::OnOK() 
{
	// TODO: Add extra validation here
	CB3ShapeDialog::OnOK();
	m_Base.b3Update();
	if (m_Creation)
	{
		m_Base.b3Write("shape3.base");
		m_Dir1.b3Write("shape3.dir1");
		m_Dir2.b3Write("shape3.dir2");
		m_Dir3.b3Write("shape3.dir3");
	}
	AfxGetApp()->WriteProfileInt(CB3ClientString(),"shape3.mode",m_DirMode);
	m_Shape->b3Recompute();
}
