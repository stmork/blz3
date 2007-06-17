/*
**
**	$Filename:	DlgCSG3.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for quadric CSG shapes
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

#include "AppLinesInclude.h"

#include "DlgCSG3.h"

/*************************************************************************
**                                                                      **
**                        CDlgShape3 implementation                     **
**                                                                      **
*************************************************************************/

CDlgCSG3::CDlgCSG3(CWnd* pParent /*=NULL*/)
	: CDlgCSG1(CDlgCSG3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSG3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCSG3::DoDataExchange(CDataExchange* pDX)
{
	CDlgCSG1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSG3)
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


BEGIN_MESSAGE_MAP(CDlgCSG3, CDlgCSG1)
	//{{AFX_MSG_MAP(CDlgCSG3)
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

int CDlgCSG3::b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create)
{
	CDlgCSG3 dlg;

	return CB3ShapeDialog::b3Edit(&dlg, shading_class_type, item, create);
}

void CDlgCSG3::b3Init()
{
	b3CSGShape3 *shape = (b3CSGShape3 *)m_Shape;

	// Call base class (Skipping CDlgCSG1)
	CB3ShapeDialog::b3Init();

	// Init vector group
	m_Base.b3Init(&shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Base,&shape->m_Dir1,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
	m_Dir2.b3Init(&shape->m_Base,&shape->m_Dir2,&m_xDir2Ctrl,&m_yDir2Ctrl,&m_zDir2Ctrl,&m_lenDir2Ctrl);
	m_Dir3.b3Init(&shape->m_Base,&shape->m_Dir3,&m_xDir3Ctrl,&m_yDir3Ctrl,&m_zDir3Ctrl,&m_lenDir3Ctrl);

	// Read from registry
	if (m_Creation)
	{
		m_Base.b3Read(b3MakeSection("base"));
		m_Dir1.b3Read(b3MakeSection("dir1"));
		m_Dir2.b3Read(b3MakeSection("dir2"));
		m_Dir3.b3Read(b3MakeSection("dir3"));
	}
}

BOOL CDlgCSG3::OnInitDialog() 
{
	CDlgCSG1::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Dir2.b3Set(m_DirMode);
	m_Dir3.b3Set(m_DirMode);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCSG3::b3SetDirMode(int dirmode)
{
	CDlgCSG1::b3SetDirMode(dirmode);
	m_Dir2.b3Set(m_DirMode);
	m_Dir3.b3Set(m_DirMode);
}

void CDlgCSG3::b3UpdateBase()
{
	CDlgCSG1::b3UpdateBase();
	m_Dir2.b3Update(m_DirMode);
	m_Dir3.b3Update(m_DirMode);
}

void CDlgCSG3::OnChangedDir2() 
{
	// TODO: Add your control notification handler code here
	m_Dir2.b3Update(m_DirMode);
}

void CDlgCSG3::OnChangedLen2() 
{
	// TODO: Add your control notification handler code here
	m_Dir2.b3UpdateLen(m_DirMode);
}

void CDlgCSG3::OnChangedDir3() 
{
	// TODO: Add your control notification handler code here
	m_Dir3.b3Update(m_DirMode);
}

void CDlgCSG3::OnChangedLen3() 
{
	// TODO: Add your control notification handler code here
	m_Dir3.b3UpdateLen(m_DirMode);
}

void CDlgCSG3::b3PostProcess() 
{
	// TODO: Add extra validation here
	b3CSGShape *shape = (b3CSGShape *)m_Shape;

	CDlgCSG1::b3PostProcess();
	if (m_Creation)
	{
		m_Dir2.b3Write(b3MakeSection("dir2"));
		m_Dir3.b3Write(b3MakeSection("dir3"));
	}
}
