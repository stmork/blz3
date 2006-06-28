/*
**
**	$Filename:	DlgShape2.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for area or disk
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

#include "DlgShape2.h"

/*************************************************************************
**                                                                      **
**                        CDlgShape3 implementation                     **
**                                                                      **
*************************************************************************/

CDlgShape2::CDlgShape2(CWnd* pParent /*=NULL*/)
	: CDlgShape1(CDlgShape2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShape2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgShape2::CDlgShape2(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDlgShape1(IDD, pParent)
{
}

void CDlgShape2::DoDataExchange(CDataExchange* pDX)
{
	CDlgShape1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShape2)
	DDX_Control(pDX, IDC_DIR2_X,   m_xDir2Ctrl);
	DDX_Control(pDX, IDC_DIR2_Y,   m_yDir2Ctrl);
	DDX_Control(pDX, IDC_DIR2_Z,   m_zDir2Ctrl);
	DDX_Control(pDX, IDC_DIR2_LEN, m_lenDir2Ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShape2, CDlgShape1)
	//{{AFX_MSG_MAP(CDlgShape2)
	ON_EN_KILLFOCUS(IDC_DIR2_X, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR2_Y, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR2_Z, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR2_LEN, OnChangedLen2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShape3 message handlers

int CDlgShape2::b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create)
{
	CDlgShape2 dlg;

	return CB3ShapeDialog::b3Edit(&dlg, shading_class_type, item,create);
}

void CDlgShape2::b3Init()
{
	b3Shape2 *shape = (b3Shape2 *)m_Shape;

	// Call base class (Skipping CDlgShape1)
	CB3SpanningShapeDialog::b3Init();

	// Init vector group
	m_Base.b3Init(&shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Base,&shape->m_Dir1,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
	m_Dir2.b3Init(&shape->m_Base,&shape->m_Dir2,&m_xDir2Ctrl,&m_yDir2Ctrl,&m_zDir2Ctrl,&m_lenDir2Ctrl);

	// Read from registry
	if (m_Creation)
	{
		m_Base.b3Read(b3MakeSection("base"));
		m_Dir1.b3Read(b3MakeSection("dir1"));
		m_Dir2.b3Read(b3MakeSection("dir2"));
	}
}

BOOL CDlgShape2::OnInitDialog() 
{
	CDlgShape1::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Dir2.b3Set(m_DirMode);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShape2::b3SetDirMode(int dirmode)
{
	CDlgShape1::b3SetDirMode(dirmode);
	m_Dir2.b3Set(m_DirMode);
}

void CDlgShape2::b3UpdateBase()
{
	CDlgShape1::b3UpdateBase();
	m_Dir2.b3Update(m_DirMode);
}

void CDlgShape2::OnChangedDir2() 
{
	// TODO: Add your control notification handler code here
	m_Dir2.b3Update(m_DirMode);
}

void CDlgShape2::OnChangedLen2() 
{
	// TODO: Add your control notification handler code here
	m_Dir2.b3UpdateLen(m_DirMode);
}

void CDlgShape2::b3PostProcess() 
{
	CDlgShape1::b3PostProcess();
	if (m_Creation)
	{
		m_Dir2.b3Write(b3MakeSection("dir2"));
	}
}
