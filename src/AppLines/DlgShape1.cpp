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

#include "AppLinesInclude.h"

#include "DlgShape1.h"

/*************************************************************************
**                                                                      **
**                        CDlgShape1 implementation                     **
**                                                                      **
*************************************************************************/

CDlgShape1::CDlgShape1(CWnd* pParent /*=NULL*/)
	: CB3SpanningShapeDialog(CDlgShape1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShape1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgShape1::CDlgShape1(UINT IDD,CWnd* pParent /*=NULL*/)
	: CB3SpanningShapeDialog(IDD, pParent)
{
}


void CDlgShape1::DoDataExchange(CDataExchange* pDX)
{
	CB3SpanningShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShape1)
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


BEGIN_MESSAGE_MAP(CDlgShape1, CB3SpanningShapeDialog)
	//{{AFX_MSG_MAP(CDlgShape1)
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
// CDlgShape1 message handlers

int CDlgShape1::b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create)
{
	CDlgShape1 dlg;

	return CB3ShapeDialog::b3Edit(&dlg,shading_class_type, item,create);
}

void CDlgShape1::b3Init()
{
	b3Sphere *shape = (b3Sphere *)m_Shape;

	// Call base class
	CB3SpanningShapeDialog::b3Init();

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

BOOL CDlgShape1::OnInitDialog() 
{
	CB3SpanningShapeDialog::OnInitDialog();
	
	m_Dir1.b3Set(m_DirMode);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShape1::b3SetDirMode(int dirmode)
{
	m_Dir1.b3Set(m_DirMode);
}

void CDlgShape1::b3UpdateBase()
{
	m_Dir1.b3Update(m_DirMode);
}

void CDlgShape1::OnChangedBase() 
{
	m_Base.b3Update();
	b3UpdateBase();
}

void CDlgShape1::OnChangedDir1() 
{
	m_Dir1.b3Update(m_DirMode);
}

void CDlgShape1::OnChangedLen1() 
{
	m_Dir1.b3UpdateLen(m_DirMode);
}

void CDlgShape1::b3PostProcess() 
{
	CB3SpanningShapeDialog::b3PostProcess();
	if (m_Creation)
	{
		m_Base.b3Write(b3MakeSection("base"));
		m_Dir1.b3Write(b3MakeSection("dir1"));
	}
}
