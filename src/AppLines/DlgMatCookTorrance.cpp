/*
**
**	$Filename:	DlgMatCookTorrance.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Cook&Torrance material shader
**
**	(C) Copyright 2003  Steffen A. Mork
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
#include "DlgMatCookTorrance.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2004/04/03 13:04:00  sm
**	- Filling material dialogs with life.
**
**	Revision 1.2  2004/03/07 13:08:52  sm
**	- Added dialog for Cook/Torrance material
**	
**	Revision 1.1  2004/03/07 12:23:26  sm
**	- Added Cook/Torrance material editors.
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatCookTorrance implementation            **
**                                                                      **
*************************************************************************/

CDlgMatCookTorrance::CDlgMatCookTorrance(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatCookTorrance::IDD, pParent)
{
	m_Material = (b3MatCookTorrance *)item;
	//{{AFX_DATA_INIT(CDlgMatCookTorrance)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_KaCtrl.b3SetRange(0.0,100.0);
	m_KdCtrl.b3SetRange(0.0,100.0);
	m_KsCtrl.b3SetRange(0.0,100.0);
	m_ReflectionCtrl.b3SetRange(0.0,100.0);
	m_RefractionCtrl.b3SetRange(0.0,100.0);
	m_RefrValueCtrl.b3SetRange(-5.0,5.0);
	m_RefrValueCtrl.b3SetDigits(0,2);
	m_RefrValueCtrl.b3SetIncrement(0.01);
	m_HighLightCtrl.b3SetRange(1.0,100000);
	m_HighLightCtrl.b3SetDigits(0,1);
	m_HighLightCtrl.b3SetIncrement(10.0);
}


void CDlgMatCookTorrance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatCookTorrance)
	DDX_Control(pDX, IDC_KA_SPIN, m_KaCtrl);
	DDX_Control(pDX, IDC_KD_SPIN, m_KdCtrl);
	DDX_Control(pDX, IDC_KS_SPIN, m_KsCtrl);
	DDX_Control(pDX, IDC_M_SPIN, m_MCtrl);
	DDX_Control(pDX, IDC_SPEC_EXPONENT_SPIN, m_HighLightCtrl);
	DDX_Control(pDX, IDC_REFRACTANCE_SPIN, m_RefractionCtrl);
	DDX_Control(pDX, IDC_REFLECTANCE_SPIN, m_ReflectionCtrl);
	DDX_Control(pDX, IDC_INDEX_OF_REFRACTION_SPIN, m_RefrValueCtrl);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	DDX_Control(pDX, IDC_COLOR_SPECULAR, m_SpecCtrl);
	DDX_Control(pDX, IDC_COLOR_DIFFUSE, m_DiffCtrl);
	DDX_Control(pDX, IDC_COLOR_AMBIENT, m_AmbCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatCookTorrance, CDialog)
	//{{AFX_MSG_MAP(CDlgMatCookTorrance)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatCookTorrance message handlers

void CDlgMatCookTorrance::b3Register()
{
	b3Loader::b3AddClassType(COOK_TORRANCE,IDS_MAT_COOK_TORRANCE,IDI_MAT_COOK_TORRANCE,b3Edit,b3Edit);
}

b3_bool CDlgMatCookTorrance::b3Edit(b3Item *item)
{
	CDlgMatCookTorrance dlg(item);

	dlg.DoModal();
	return true;
}
