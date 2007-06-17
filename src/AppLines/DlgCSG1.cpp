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

#include "AppLinesInclude.h"

#include "DlgCSG1.h"
#include "DlgCSGMode.h"

/*************************************************************************
**                                                                      **
**                        CDlgShape3 implementation                     **
**                                                                      **
*************************************************************************/

CDlgCSG1::CDlgCSG1(CWnd* pParent /*=NULL*/)
	: CB3SpanningShapeDialog(CDlgCSG1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSG1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgCSG1::CDlgCSG1(UINT IDD,CWnd* pParent /*=NULL*/)
	: CB3SpanningShapeDialog(IDD, pParent)
{
}


void CDlgCSG1::DoDataExchange(CDataExchange* pDX)
{
	CB3SpanningShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSG1)
	DDX_Control(pDX, IDC_BASE_X,   m_xBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Y,   m_yBaseCtrl);
	DDX_Control(pDX, IDC_BASE_Z,   m_zBaseCtrl);
	DDX_Control(pDX, IDC_DIR1_X,   m_xDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_Y,   m_yDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_Z,   m_zDir1Ctrl);
	DDX_Control(pDX, IDC_DIR1_LEN, m_lenDir1Ctrl);
	//}}AFX_DATA_MAP
	m_Base.b3DDX(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCSG1, CB3SpanningShapeDialog)
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

int CDlgCSG1::b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create)
{
	CDlgCSG1 dlg;

	return CB3SpanningShapeDialog::b3Edit(&dlg, shading_class_type, item, create);
}

void CDlgCSG1::b3Init()
{
	b3CSGSphere *shape = (b3CSGSphere *)m_Shape;

	// Call base class
	CB3ShapeDialog::b3Init();

	// Init vector group
	m_Base.b3Init(&shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Base,&shape->m_Dir,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);

	// Read from registry
	if (m_Creation)
	{
		m_Base.b3Read(b3MakeSection("base"));
		m_Dir1.b3Read(b3MakeSection("dir1"));
	}
}

BOOL CDlgCSG1::OnInitDialog() 
{
	CB3SpanningShapeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Dir1.b3Set(m_DirMode);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCSG1::b3SetDirMode(int dirmode)
{
	m_Dir1.b3Set(m_DirMode);
}

void CDlgCSG1::b3UpdateBase()
{
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

void CDlgCSG1::b3PostProcess() 
{
	// TODO: Add extra validation here
	CB3SpanningShapeDialog::b3PostProcess();
	if (m_Creation)
	{
		m_Base.b3Write(b3MakeSection("base"));
		m_Dir1.b3Write(b3MakeSection("dir1"));
	}
}
