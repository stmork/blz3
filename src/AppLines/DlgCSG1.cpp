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
#include "DlgCSGMode.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**
**	Revision 1.2  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
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
	: CB3ShapeDialog(CDlgCSG1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSG1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgCSG1::CDlgCSG1(UINT IDD,CWnd* pParent /*=NULL*/)
	: CB3ShapeDialog(IDD, pParent)
{
}


void CDlgCSG1::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
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


BEGIN_MESSAGE_MAP(CDlgCSG1, CB3ShapeDialog)
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

	return CB3ShapeDialog::b3Edit(&dlg,item,create);
}

const char *CDlgCSG1::b3GetSection()
{
	return "csg1";
}

void CDlgCSG1::b3Init()
{
	b3CSGSphere *shape = (b3CSGSphere *)m_Shape;

	m_Base.b3Init(&shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Base,&shape->m_Dir,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
}

BOOL CDlgCSG1::OnInitDialog() 
{
	CB3ShapeDialog::OnInitDialog();
	
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
	CB3ShapeDialog::b3SetDirMode(dirmode);
	m_Base.b3Set();
	m_Dir1.b3Set(m_DirMode);
}

void CDlgCSG1::b3UpdateBase()
{
	CB3ShapeDialog::b3UpdateBase();
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
	CB3ShapeDialog::OnOK();
	if (m_Creation)
	{
		m_Base.b3Write(b3GetSection() + CString(".base"));
		m_Dir1.b3Write(b3GetSection() + CString(".dir1"));
	}
}
