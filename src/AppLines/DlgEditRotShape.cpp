/*
**
**	$Filename:	DlgEditRotShape.cpp $
**	$Release:	Dortmund 2007 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "DlgEditRotShape.h"

/*************************************************************************
**                                                                      **
**                        CDlgEditRotShape implementation               **
**                                                                      **
*************************************************************************/

CDlgEditRotShape::CDlgEditRotShape(CWnd* pParent /*=NULL*/)
	: CB3SpanningShapeDialog(CDlgEditRotShape::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEditRotShape)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgEditRotShape::CDlgEditRotShape(UINT IDD,CWnd* pParent /*=NULL*/)
	: CB3SpanningShapeDialog(IDD, pParent)
{
}

void CDlgEditRotShape::DoDataExchange(CDataExchange* pDX)
{
	CB3SpanningShapeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditRotShape)
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


BEGIN_MESSAGE_MAP(CDlgEditRotShape, CB3SpanningShapeDialog)
	//{{AFX_MSG_MAP(CDlgEditRotShape)
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
// CDlgEditRotShape message handlers

int CDlgEditRotShape::b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create)
{
	CDlgEditRotShape dlg;

	B3_ASSERT(!create);
	return CB3ShapeDialog::b3Edit(&dlg, shading_class_type, item, create);
}

void CDlgEditRotShape::b3Init()
{
	b3SplineRotShape *shape = (b3SplineRotShape *)m_Shape;

	// Call base class (Skipping CDlgShape1)
	CB3SpanningShapeDialog::b3Init();

	// Init vector group
	m_Base.b3Init(&shape->m_Axis.pos,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Axis.pos,&shape->m_Axis.dir,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
}

BOOL CDlgEditRotShape::OnInitDialog() 
{
	CB3SpanningShapeDialog::OnInitDialog();
	
	m_Dir1.b3Set(m_DirMode);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditRotShape::b3SetDirMode(int dirmode)
{
	m_Dir1.b3Set(m_DirMode);
}

void CDlgEditRotShape::b3UpdateBase()
{
	m_Dir1.b3Update(m_DirMode);
}

void CDlgEditRotShape::OnChangedBase() 
{
	m_Base.b3Update();
	b3UpdateBase();
}

void CDlgEditRotShape::OnChangedDir1() 
{
	m_Dir1.b3Update(m_DirMode);
}

void CDlgEditRotShape::OnChangedLen1() 
{
	m_Dir1.b3UpdateLen(m_DirMode);
}

void CDlgEditRotShape::b3PostProcess() 
{
	CB3SpanningShapeDialog::b3PostProcess();
}

