/*
**
**	$Filename:	DlgShape1.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for spheres
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
#include "DlgShape1.h"

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
**                        CDlgShape1 implementation                     **
**                                                                      **
*************************************************************************/

CDlgShape1::CDlgShape1(CWnd* pParent /*=NULL*/)
	: CB3ShapeDialog(CDlgShape1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShape1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgShape1::DoDataExchange(CDataExchange* pDX)
{
	CB3ShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShape1)
	DDX_Control(pDX, IDC_BASE_X,   m_xBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Y,   m_yBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Z,   m_zBaseCtrl);
	DDX_Control(pDX, IDC_DIR1_X,   m_xDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_Y,   m_yDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_Z,   m_zDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_LEN, m_lenDir1Ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShape1, CB3ShapeDialog)
	//{{AFX_MSG_MAP(CDlgShape1)
	ON_EN_KILLFOCUS(IDC_DIR1_X, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_Y, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_Z, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_LEN, OnChangedLen1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShape1 message handlers

int CDlgShape1::b3Edit(b3Item *item,b3_bool create)
{
	CDlgShape1 dlg;

	dlg.m_Creation = create;
	dlg.m_Shape    = (b3Sphere *)item;
	return dlg.DoModal();
}

BOOL CDlgShape1::OnInitDialog() 
{
	m_Base.b3Init(&m_Shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&m_Shape->m_Base,&m_Shape->m_Dir,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
	m_DirMode = AfxGetApp()->GetProfileInt(CB3ClientString(),"shape1.mode",m_DirMode);
	CB3ShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_Creation)
	{
		m_Base.b3Read("shape1.base");
		m_Dir1.b3Read("shape1.dir1");
	}

	m_Base.b3Set(true);
	m_Dir1.b3Set(m_DirMode,true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShape1::b3SetDirMode(int dirmode)
{
	m_Base.b3Set();
	m_Dir1.b3Set(m_DirMode);
}

void CDlgShape1::OnChangedDir1() 
{
	// TODO: Add your control notification handler code here
	m_Dir1.b3Update(m_DirMode);
}

void CDlgShape1::OnChangedLen1() 
{
	// TODO: Add your control notification handler code here
	m_Dir1.b3UpdateLen(m_DirMode);
}

void CDlgShape1::OnOK() 
{
	// TODO: Add extra validation here
	CB3ShapeDialog::OnOK();
	m_Base.b3Update();
	if (m_Creation)
	{
		m_Base.b3Write("shape1.base");
		m_Dir1.b3Write("shape1.dir1");
	}
	AfxGetApp()->WriteProfileInt(CB3ClientString(),"shape1.mode",m_DirMode);
	m_Shape->b3Recompute();
}
