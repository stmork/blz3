/*
**
**	$Filename:	DlgCSG3.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "AppLines.h"
#include "DlgCSG3.h"

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

int CDlgCSG3::b3Edit(b3Item *item,b3_bool create)
{
	CDlgCSG3 dlg;

	dlg.m_Creation = create;
	dlg.m_Shape    = (b3CSGShape *)item;
	return dlg.DoModal();
}

const char *CDlgCSG3::b3GetSection()
{
	return "csg3";
}

BOOL CDlgCSG3::OnInitDialog() 
{
	b3CSGShape3 *shape = (b3CSGShape3 *)m_Shape;

	m_Base.b3Init(&shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Base,&shape->m_Dir1,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
	m_Dir2.b3Init(&shape->m_Base,&shape->m_Dir2,&m_xDir2Ctrl,&m_yDir2Ctrl,&m_zDir2Ctrl,&m_lenDir2Ctrl);
	m_Dir3.b3Init(&shape->m_Base,&shape->m_Dir3,&m_xDir3Ctrl,&m_yDir3Ctrl,&m_zDir3Ctrl,&m_lenDir3Ctrl);
	CDlgCSG1::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_Creation)
	{
		m_Dir2.b3Read(b3GetSection() + CString(".dir2"));
		m_Dir3.b3Read(b3GetSection() + CString(".dir3"));
	}

	m_Dir2.b3Set(m_DirMode,true);
	m_Dir3.b3Set(m_DirMode,true);
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

void CDlgCSG3::OnOK() 
{
	// TODO: Add extra validation here
	b3CSGShape *shape = (b3CSGShape *)m_Shape;

	CDlgCSG1::OnOK();
	if (m_Creation)
	{
		m_Dir2.b3Write(b3GetSection() + CString(".dir2"));
		m_Dir3.b3Write(b3GetSection() + CString(".dir3"));
	}
	shape->m_Operation = b3CSGShape::m_CSGMode[m_CSGMode];
}
