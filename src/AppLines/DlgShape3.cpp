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
**	Revision 1.4  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**
**	Revision 1.3  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**	Revision 1.2  2002/02/24 17:45:31  sm
**	- Added CSG edit dialogs
**	- Corrected shape edit inheritance.
**	
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
	: CDlgShape2(CDlgShape3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShape3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgShape3::DoDataExchange(CDataExchange* pDX)
{
	CDlgShape2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShape3)
	DDX_Control(pDX, IDC_DIR3_X,   m_xDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_Y,   m_yDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_Z,   m_zDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_LEN, m_lenDir3Ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShape3, CDlgShape2)
	//{{AFX_MSG_MAP(CDlgShape3)
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

	return CB3ShapeDialog::b3Edit(&dlg,item,create);
}

const char *CDlgShape3::b3GetSection()
{
	return "shape3";
}

void CDlgShape3::b3Init()
{
	b3Shape3 *shape = (b3Shape3 *)m_Shape;

	m_Base.b3Init(&shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Base,&shape->m_Dir1,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
	m_Dir2.b3Init(&shape->m_Base,&shape->m_Dir2,&m_xDir2Ctrl,&m_yDir2Ctrl,&m_zDir2Ctrl,&m_lenDir2Ctrl);
	m_Dir3.b3Init(&shape->m_Base,&shape->m_Dir3,&m_xDir3Ctrl,&m_yDir3Ctrl,&m_zDir3Ctrl,&m_lenDir3Ctrl);
}

BOOL CDlgShape3::OnInitDialog() 
{
	CDlgShape2::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_Creation)
	{
		m_Dir3.b3Read(b3GetSection() + CString(".dir3"));
	}

	m_Dir3.b3Set(m_DirMode,true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShape3::b3SetDirMode(int dirmode)
{
	CDlgShape2::b3SetDirMode(dirmode);
	m_Dir3.b3Set(m_DirMode);
}

void CDlgShape3::b3UpdateBase()
{
	CDlgShape2::b3UpdateBase();
	m_Dir2.b3Update(m_DirMode);
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
	CDlgShape2::OnOK();
	if (m_Creation)
	{
		m_Dir3.b3Write(b3GetSection() + CString(".dir3"));
	}
}
