/*
**
**	$Filename:	DlgMatThinFilm.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Thin film material
**
**	(C) Copyright 2004  Steffen A. Mork
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
#include "AppObjectDoc.h"
#include "b3ExampleScene.h"
#include "DlgMatThinFilm.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**
**	Revision 1.2  2004/07/27 19:05:59  sm
**	- Some typo cleanups.
**	
**	Revision 1.1  2004/07/27 16:33:49  sm
**	- Added thin film material rendering
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatThinFilm implementation                **
**                                                                      **
*************************************************************************/

CDlgMatThinFilm::CDlgMatThinFilm(b3Item *item,CAppObjectDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(item, CDlgMatThinFilm::IDD, pParent)
{
	m_Material                = (b3MatThinFilm *)item;
	m_PageMaterial.m_Material = m_Material;
	m_PageScaling.m_Scaling   = m_Material;
	m_MatScene                = b3ExampleScene::b3CreateMaterial(&m_MatHead, pDoc->b3GetParentShading());
	m_MatHead->b3Append(m_Material);
	//{{AFX_DATA_INIT(CDlgMatThinFilm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ThicknessCtrl.b3SetRange(0.1,1000.0);
	m_ThicknessCtrl.b3SetDigits(4.2);
	m_ThicknessCtrl.b3SetIncrement(1);
}

CDlgMatThinFilm::~CDlgMatThinFilm()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatThinFilm::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatThinFilm)
	DDX_Control(pDX, IDC_COLOR_INTERFERENCE, m_InterferenceCtrl);
	DDX_Control(pDX, IDC_THICKNESS_SPIN, m_ThicknessCtrl);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	//}}AFX_DATA_MAP
	m_ThicknessCtrl.b3DDX(pDX, m_Material->m_Thickness);
}


BEGIN_MESSAGE_MAP(CDlgMatThinFilm, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatThinFilm)
	ON_BN_CLICKED(IDC_CHANGE_INTERFERENCE, OnColorInterference)
	ON_EN_KILLFOCUS(IDC_THICKNESS, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_THICKNESS_SPIN, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatChess message handlers

void CDlgMatThinFilm::b3Register()
{
	b3Loader::b3AddClassType(THIN_FILM,IDS_MAT_THIN_FILM,IDI_MAT_THIN_FILM,b3Edit,b3Edit);
}

b3_bool CDlgMatThinFilm::b3Edit(b3Item *item,void *ptr)
{
	CDlgMatThinFilm dlg(item, (CAppObjectDoc *)ptr);

	return dlg.DoModal() == IDOK;
}

void CDlgMatThinFilm::b3InitDialog()
{
	m_PageMaterial.b3AddToSheet(&m_PropertySheet);
	m_PageScaling.b3AddToSheet(&m_PropertySheet);
	m_InterferenceCtrl.b3Init(&m_Material->m_Intensity,this);
}

void CDlgMatThinFilm::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgMatThinFilm::OnColorInterference() 
{
	// TODO: Add your control notification handler code here
	if (m_InterferenceCtrl.b3Select())
	{
		b3UpdateUI();
	}
}
