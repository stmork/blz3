/*
**
**	$Filename:	DlgCSGTorus.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Edit dialog for CSG torus
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

#include "DlgCSGTorus.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**
**	Revision 1.4  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.3  2002/03/10 20:34:17  sm
**	- Cleaned up and tested CB3ShapeDialgo derivates:
**	  o Ordered meaning of methods
**	  o Made registry entries of stencil creation unique for
**	    each shape.
**	  o Fixed some bugs.
**	
**	Revision 1.2  2002/03/08 16:46:14  sm
**	- Added new CB3IntSpinButtonCtrl. This is much
**	  better than standard integer CSpinButtonCtrl.
**	- Added a test control to test spin button controls
**	  and float control.
**	- Made spin button controls and float edit control
**	  DDXable. The spin button controls need only
**	  a simple edit field without any DDX CEdit reference
**	  or value reference inside a dialog.
**	- Changed dialogs to reflect new controls. This was a
**	  major cleanup which shortens the code in an elegant way.
**	
**	Revision 1.1  2002/02/28 16:58:45  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgShape3 implementation                     **
**                                                                      **
*************************************************************************/

CDlgCSGTorus::CDlgCSGTorus(CWnd* pParent /*=NULL*/)
	: CDlgCSG1(CDlgCSGTorus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCSGTorus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCSGTorus::DoDataExchange(CDataExchange* pDX)
{
	CDlgCSG1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCSGTorus)
	DDX_Control(pDX, IDC_DIR2_X,   m_xDir2Ctrl);
	DDX_Control(pDX, IDC_DIR2_Y,   m_yDir2Ctrl);
	DDX_Control(pDX, IDC_DIR2_Z,   m_zDir2Ctrl);
	DDX_Control(pDX, IDC_DIR3_X,   m_xDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_Y,   m_yDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_Z,   m_zDir3Ctrl);
	DDX_Control(pDX, IDC_DIR3_LEN, m_lenDir3Ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCSGTorus, CDlgCSG1)
	//{{AFX_MSG_MAP(CDlgCSGTorus)
	ON_EN_KILLFOCUS(IDC_DIR1_X, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_Y, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR1_Z, OnChangedDir1)
	ON_EN_KILLFOCUS(IDC_DIR2_X, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR2_Y, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR2_Z, OnChangedDir2)
	ON_EN_KILLFOCUS(IDC_DIR1_LEN, OnChangedLen2)
	ON_EN_KILLFOCUS(IDC_DIR3_X, OnChangedDir3)
	ON_EN_KILLFOCUS(IDC_DIR3_Y, OnChangedDir3)
	ON_EN_KILLFOCUS(IDC_DIR3_Z, OnChangedDir3)
	ON_EN_KILLFOCUS(IDC_DIR3_LEN, OnChangedLen3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShape3 message handlers

int CDlgCSGTorus::b3Edit(b3_u32 shading_class_type, b3Item *item,b3_bool create)
{
	CDlgCSGTorus dlg;

	return CB3ShapeDialog::b3Edit(&dlg, shading_class_type, item, create);
}

void CDlgCSGTorus::b3Init()
{
	b3CSGTorus *shape = (b3CSGTorus *)m_Shape;

	// Copy values to local instance
	m_TorusDir1 = shape->m_Dir1;
	m_TorusDir2 = shape->m_Dir2;
	m_TorusDir3 = shape->m_Dir3;
	b3Vector::b3Scale(&m_TorusDir1,shape->m_aRad);
	b3Vector::b3Scale(&m_TorusDir2,shape->m_aRad);
	b3Vector::b3Scale(&m_TorusDir3,shape->m_bRad);

	// Call base class (Skipping CDlgCSG1)
	CB3ShapeDialog::b3Init();

	// Init vector group
	m_Base.b3Init(&shape->m_Base,&m_xBaseCtrl,&m_yBaseCtrl,&m_zBaseCtrl);
	m_Dir1.b3Init(&shape->m_Base,&m_TorusDir1,&m_xDir1Ctrl,&m_yDir1Ctrl,&m_zDir1Ctrl,&m_lenDir1Ctrl);
	m_Dir2.b3Init(&shape->m_Base,&m_TorusDir2,&m_xDir2Ctrl,&m_yDir2Ctrl,&m_zDir2Ctrl,&m_lenDir1Ctrl);
	m_Dir3.b3Init(&shape->m_Base,&m_TorusDir3,&m_xDir3Ctrl,&m_yDir3Ctrl,&m_zDir3Ctrl,&m_lenDir3Ctrl);

	// Read from registry
	if (m_Creation)
	{
		m_Base.b3Read(b3MakeSection("base"));
		m_Dir1.b3Read(b3MakeSection("dir1"));
		m_Dir2.b3Read(b3MakeSection("dir2"));
		m_Dir3.b3Read(b3MakeSection("dir3"));
	}
}

BOOL CDlgCSGTorus::OnInitDialog() 
{
	CDlgCSG1::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Dir2.b3Set(m_DirMode);
	m_Dir3.b3Set(m_DirMode);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCSGTorus::b3SetDirMode(int dirmode)
{
	CDlgCSG1::b3SetDirMode(dirmode);
	m_Dir2.b3Set(m_DirMode);
	m_Dir3.b3Set(m_DirMode);
}

void CDlgCSGTorus::b3UpdateBase()
{
	CDlgCSG1::b3UpdateBase();
	m_Dir2.b3Update(m_DirMode);
	m_Dir3.b3Update(m_DirMode);
}

void CDlgCSGTorus::OnChangedDir1() 
{
	// TODO: Add your control notification handler code here
	m_Dir1.b3Update(m_DirMode);
	m_Dir2.b3UpdateLen(m_DirMode);
}

void CDlgCSGTorus::OnChangedDir2() 
{
	// TODO: Add your control notification handler code here
	m_Dir2.b3Update(m_DirMode);
	m_Dir1.b3UpdateLen(m_DirMode);
}

void CDlgCSGTorus::OnChangedLen2() 
{
	// TODO: Add your control notification handler code here
	m_Dir1.b3UpdateLen(m_DirMode);
	m_Dir2.b3UpdateLen(m_DirMode);
}

void CDlgCSGTorus::OnChangedDir3() 
{
	// TODO: Add your control notification handler code here
	m_Dir3.b3Update(m_DirMode);
}

void CDlgCSGTorus::OnChangedLen3() 
{
	// TODO: Add your control notification handler code here
	m_Dir3.b3UpdateLen(m_DirMode);
}

void CDlgCSGTorus::b3PostProcess() 
{
	// TODO: Add extra validation here
	b3CSGTorus *shape = (b3CSGTorus *)m_Shape;

	CDlgCSG1::b3PostProcess();
	if (m_Creation)
	{
		m_Dir2.b3Write(b3MakeSection("dir2"));
		m_Dir3.b3Write(b3MakeSection("dir3"));
	}

	// Compute shape values
	shape->m_aRad = b3Vector::b3Length(&m_TorusDir1);
	shape->m_bRad = b3Vector::b3Length(&m_TorusDir3);
	shape->m_Dir1 = m_TorusDir1;
	shape->m_Dir2 = m_TorusDir2;
	shape->m_Dir3 = m_TorusDir3;
	b3Vector::b3Normalize(&shape->m_Dir1);
	b3Vector::b3Normalize(&shape->m_Dir2);
	b3Vector::b3Normalize(&shape->m_Dir3);
}
