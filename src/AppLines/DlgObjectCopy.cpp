/*
**
**	$Filename:	DlgObjectCopy.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Dialog for copying objects
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
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

#include "DlgObjectCopy.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.8  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.7  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**	
**	Revision 1.6  2003/02/22 17:21:32  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.5  2003/02/18 16:52:57  sm
**	- Fixed no name error on new scenes (ticket no. 4).
**	- Introduced new b3Matrix class and renamed methods.
**	
**	Revision 1.4  2002/03/08 16:46:14  sm
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
**	Revision 1.3  2002/01/09 17:47:54  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**	
**	Revision 1.2  2002/01/08 16:21:58  sm
**	- Added center to copy dialog
**	
**	Revision 1.1  2002/01/07 21:19:30  sm
**	- Adding object copy dialog
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgObjectCopy implementation                 **
**                                                                      **
*************************************************************************/

CDlgObjectCopy::CDlgObjectCopy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjectCopy::IDD, pParent)
{
	CWinApp *app = AfxGetApp();

	//{{AFX_DATA_INIT(CDlgObjectCopy)
	m_NumCopies  = app->GetProfileInt(CB3ClientString(),"copy count", 1);
	m_CopyBack   = app->GetProfileInt(CB3ClientString(),"copy back center", 1);
	m_xUseOffset = app->GetProfileInt(CB3ClientString(),"use offset x", 1);
	m_yUseOffset = app->GetProfileInt(CB3ClientString(),"use offset y", 0);
	m_zUseOffset = app->GetProfileInt(CB3ClientString(),"use offset z", 0);
	m_xUseRot    = app->GetProfileInt(CB3ClientString(),"use rot x", 0);
	m_yUseRot    = app->GetProfileInt(CB3ClientString(),"use rot y", 0);
	m_zUseRot    = app->GetProfileInt(CB3ClientString(),"use rot z", 1);
	//}}AFX_DATA_INIT
	m_OrigBBox = null;
	m_Move.x   = atof(app->GetProfileString(CB3ClientString(),"move object x", "100.0"));
	m_Move.y   = atof(app->GetProfileString(CB3ClientString(),"move object y",   "0.0"));
	m_Move.z   = atof(app->GetProfileString(CB3ClientString(),"move object z",   "0.0"));
	m_Rotate.x = atof(app->GetProfileString(CB3ClientString(),"rotate object x", "0.0"));
	m_Rotate.y = atof(app->GetProfileString(CB3ClientString(),"rotate object y", "0.0"));
	m_Rotate.z = atof(app->GetProfileString(CB3ClientString(),"rotate object z","90.0"));

	b3Matrix::b3Unit(&m_Transformation);
}


void CDlgObjectCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjectCopy)
	DDX_Control(pDX, IDC_NUM_COPYIES_SPIN, m_CtrlNumCopies);
	DDX_Control(pDX, IDC_OFFSET_X, m_xCtrlOffset);
	DDX_Control(pDX, IDC_OFFSET_Y, m_yCtrlOffset);
	DDX_Control(pDX, IDC_OFFSET_Z, m_zCtrlOffset);
	DDX_Control(pDX, IDC_ROT_X, m_xCtrlRot);
	DDX_Control(pDX, IDC_ROT_Y, m_yCtrlRot);
	DDX_Control(pDX, IDC_ROT_Z, m_zCtrlRot);
	DDX_Control(pDX, IDC_CENTER_Z, m_xCtrlCenter);
	DDX_Control(pDX, IDC_CENTER_Y, m_yCtrlCenter);
	DDX_Control(pDX, IDC_CENTER_X, m_zCtrlCenter);
	DDX_Text(pDX, IDC_NUM_COPIES, m_NumCopies);
	DDV_MinMaxUInt(pDX, m_NumCopies, 1, 1023);
	DDX_Check(pDX, IDC_CENTER_COPYBACK, m_CopyBack);
	DDX_Check(pDX, IDC_OFFSET_X_USE, m_xUseOffset);
	DDX_Check(pDX, IDC_OFFSET_Y_USE, m_yUseOffset);
	DDX_Check(pDX, IDC_OFFSET_Z_USE, m_zUseOffset);
	DDX_Check(pDX, IDC_ROT_X_USE, m_xUseRot);
	DDX_Check(pDX, IDC_ROT_Y_USE, m_yUseRot);
	DDX_Check(pDX, IDC_ROT_Z_USE, m_zUseRot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjectCopy, CDialog)
	//{{AFX_MSG_MAP(CDlgObjectCopy)
	ON_BN_CLICKED(IDC_CENTER_CLEAR, OnCenterClear)
	ON_BN_CLICKED(IDC_OFFSET_X_USE, OnOffsetXUse)
	ON_BN_CLICKED(IDC_OFFSET_Y_USE, OnOffsetYUse)
	ON_BN_CLICKED(IDC_OFFSET_Z_USE, OnOffsetZUse)
	ON_EN_KILLFOCUS(IDC_OFFSET_X, OnKillfocusOffsetX)
	ON_EN_KILLFOCUS(IDC_OFFSET_Y, OnKillfocusOffsetY)
	ON_EN_KILLFOCUS(IDC_OFFSET_Z, OnKillfocusOffsetZ)
	ON_BN_CLICKED(IDC_ROT_X_USE, OnUseRotation)
	ON_BN_CLICKED(IDC_ROT_Y_USE, OnUseRotation)
	ON_BN_CLICKED(IDC_ROT_Z_USE, OnUseRotation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjectCopy message handlers

BOOL CDlgObjectCopy::OnInitDialog() 
{
	CDialog::OnInitDialog();
	B3_ASSERT(m_Center != null);

	// TODO: Add extra initialization here
	m_xCtrlOffset.b3SetDigits(5,2);
	m_yCtrlOffset.b3SetDigits(5,2);
	m_zCtrlOffset.b3SetDigits(5,2);
	m_xCtrlOffset.b3SetPos(m_xUseOffset ? m_OrigBBox->m_DimSize.x : m_Move.x);
	m_yCtrlOffset.b3SetPos(m_yUseOffset ? m_OrigBBox->m_DimSize.y : m_Move.y);
	m_zCtrlOffset.b3SetPos(m_zUseOffset ? m_OrigBBox->m_DimSize.z : m_Move.z);

	m_xCtrlRot.b3SetMin(b3Scene::epsilon);
	m_yCtrlRot.b3SetMin(b3Scene::epsilon);
	m_zCtrlRot.b3SetMin(b3Scene::epsilon);
	m_xCtrlRot.b3SetMax(180);
	m_yCtrlRot.b3SetMax(180);
	m_zCtrlRot.b3SetMax(180);
	m_xCtrlRot.b3SetDigits(5,2);
	m_yCtrlRot.b3SetDigits(5,2);
	m_zCtrlRot.b3SetDigits(5,2);
	m_xCtrlRot.b3SetPos(m_Rotate.x);
	m_yCtrlRot.b3SetPos(m_Rotate.y);
	m_zCtrlRot.b3SetPos(m_Rotate.z);

	m_xCtrlCenter.b3SetDigits(5,2);
	m_yCtrlCenter.b3SetDigits(5,2);
	m_zCtrlCenter.b3SetDigits(5,2);
	m_xCtrlCenter.b3SetPos(m_Center->x);
	m_yCtrlCenter.b3SetPos(m_Center->y);
	m_zCtrlCenter.b3SetPos(m_Center->z);
	m_CtrlNumCopies.SetRange(1,1024);

	b3UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgObjectCopy::b3UpdateUI()
{
	b3_bool need_center;

	GetDlgItem(IDC_ROT_X)->EnableWindow(m_xUseRot);
	GetDlgItem(IDC_ROT_Y)->EnableWindow(m_yUseRot);
	GetDlgItem(IDC_ROT_Z)->EnableWindow(m_zUseRot);

	need_center = (m_xUseRot || m_yUseRot || m_zUseRot);
	GetDlgItem(IDC_CENTER_X)->EnableWindow(need_center);
	GetDlgItem(IDC_CENTER_Y)->EnableWindow(need_center);
	GetDlgItem(IDC_CENTER_Z)->EnableWindow(need_center);
	GetDlgItem(IDC_CENTER_COPYBACK)->EnableWindow(need_center);
	GetDlgItem(IDC_CENTER_CLEAR)->EnableWindow(need_center);
}

void CDlgObjectCopy::OnUseRotation() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b3UpdateUI();
}

void CDlgObjectCopy::OnCenterClear() 
{
	// TODO: Add your control notification handler code here
	m_xCtrlCenter.b3SetPos(0);
	m_yCtrlCenter.b3SetPos(0);
	m_zCtrlCenter.b3SetPos(0);
}

void CDlgObjectCopy::OnOffsetXUse() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_xUseOffset)
	{
		m_xCtrlOffset.b3SetPos(m_OrigBBox->m_DimSize.x);
	}
}

void CDlgObjectCopy::OnOffsetYUse() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_yUseOffset)
	{
		m_yCtrlOffset.b3SetPos(m_OrigBBox->m_DimSize.y);
	}
}

void CDlgObjectCopy::OnOffsetZUse() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_zUseOffset)
	{
		m_zCtrlOffset.b3SetPos(m_OrigBBox->m_DimSize.z);
	}
}

void CDlgObjectCopy::OnKillfocusOffsetX() 
{
	// TODO: Add your control notification handler code here
	m_xUseOffset = (m_xCtrlOffset.m_Value == m_OrigBBox->m_DimSize.x);
	UpdateData(FALSE);
}

void CDlgObjectCopy::OnKillfocusOffsetY() 
{
	// TODO: Add your control notification handler code here
	m_yUseOffset = (m_yCtrlOffset.m_Value == m_OrigBBox->m_DimSize.y);
	UpdateData(FALSE);
}

void CDlgObjectCopy::OnKillfocusOffsetZ() 
{
	// TODO: Add your control notification handler code here
	m_zUseOffset = (m_zCtrlOffset.m_Value == m_OrigBBox->m_DimSize.z);
	UpdateData(FALSE);
}

void CDlgObjectCopy::b3WriteProfileFloat(const char *keyword,b3_f64 value)
{
	char buffer[64];

	snprintf(buffer,sizeof(buffer),"%f",value);
	AfxGetApp()->WriteProfileString(CB3ClientString(),keyword,buffer);
}

void CDlgObjectCopy::OnOK() 
{
	// TODO: Add extra validation here
	CWinApp *app = AfxGetApp();
	b3_vector move,center,rotate;

	UpdateData();
	app->WriteProfileInt(CB3ClientString(),"copy count", m_NumCopies);
	app->WriteProfileInt(CB3ClientString(),"copy back center", m_CopyBack);
	app->WriteProfileInt(CB3ClientString(),"use offset x", m_xUseOffset);
	app->WriteProfileInt(CB3ClientString(),"use offset y", m_yUseOffset);
	app->WriteProfileInt(CB3ClientString(),"use offset z", m_zUseOffset);
	app->WriteProfileInt(CB3ClientString(),"use rot x", m_xUseRot);
	app->WriteProfileInt(CB3ClientString(),"use rot y", m_yUseRot);
	app->WriteProfileInt(CB3ClientString(),"use rot z", m_zUseRot);

	move.x = m_xCtrlOffset.m_Value;
	move.y = m_yCtrlOffset.m_Value;
	move.z = m_zCtrlOffset.m_Value;
	b3WriteProfileFloat("move object x", move.x);
	b3WriteProfileFloat("move object y", move.y);
	b3WriteProfileFloat("move object z", move.z);
	b3Matrix::b3Move(null,&m_Transformation,&move);

	center.x = m_xCtrlCenter.m_Value;
	center.y = m_yCtrlCenter.m_Value;
	center.z = m_zCtrlCenter.m_Value;
	if (m_CopyBack)
	{
		*m_Center = center;
	}

	rotate.x = m_xCtrlRot.m_Value;
	if (m_xUseRot)
	{
		b3WriteProfileFloat("rotate object x",rotate.x);
		b3Matrix::b3RotateX(
			&m_Transformation,&m_Transformation,
			&center,rotate.x * M_PI / 180);
	}

	rotate.y = m_yCtrlRot.m_Value;
	if (m_yUseRot)
	{
		b3WriteProfileFloat("rotate object y",rotate.y);
		b3Matrix::b3RotateY(
			&m_Transformation,&m_Transformation,
			&center,rotate.y * M_PI / 180);
	}
	
	rotate.z = m_zCtrlRot.m_Value;
	if (m_zUseRot)
	{
		b3WriteProfileFloat("rotate object z",rotate.z);
		b3Matrix::b3RotateZ(
			&m_Transformation,&m_Transformation,
			&center,rotate.z * M_PI / 180);
	}

	CDialog::OnOK();
}
