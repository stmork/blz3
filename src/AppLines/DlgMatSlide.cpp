/*
**
**	$Filename:	DlgMatSlide.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Slide material
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
#include "b3ExampleScene.h"
#include "DlgMatSlide.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2004/04/24 20:15:51  sm
**	- Further slide material dialog development
**
**	Revision 1.3  2004/04/24 15:40:12  sm
**	- Started slide material dialog implementation
**	- Added simple property sheet/preview dialog template
**	
**	Revision 1.2  2003/07/12 10:20:16  sm
**	- Fixed ticketno. 12 (memory leak in b3ItemRegistry)
**	
**	Revision 1.1  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgMatSlide implementation                   **
**                                                                      **
*************************************************************************/

CDlgMatSlide::CDlgMatSlide(b3Item *item,CWnd* pParent /*=NULL*/)
	: CB3SimplePropertyPreviewDialog(CDlgMatSlide::IDD, pParent)
{
	m_Material = (b3MatSlide *)item;
	m_PageLeft.m_Material  = &m_Material->m_Material[0];
	m_PageRight.m_Material = &m_Material->m_Material[1];
	m_MatScene   = b3ExampleScene::b3CreateMaterial(&m_MatHead);
	m_MatHead->b3Append(m_Material);
	//{{AFX_DATA_INIT(CDlgMatSlide)
	//}}AFX_DATA_INIT
		m_SlideMode = m_Material->m_ModeFlag & 1;
		m_CutOff = (m_Material->m_ModeFlag & 2) != null;
}

CDlgMatSlide::~CDlgMatSlide()
{
	m_MatHead->b3RemoveAll();
	delete m_MatScene;
}

void CDlgMatSlide::DoDataExchange(CDataExchange* pDX)
{
	CB3SimplePropertyPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatSlide)
	DDX_Control(pDX, IDC_SPIN_FROM, m_FromCtrl);
	DDX_Control(pDX, IDC_SPIN_TO, m_ToCtrl);
	DDX_Control(pDX, IDC_PREVIEW_MATERIAL, m_PreviewMaterialCtrl);
	DDX_Check(pDX, IDC_CUTOFF, m_CutOff);
	DDX_Radio(pDX, IDC_SLIDE_HORIZONTAL, m_SlideMode);
	//}}AFX_DATA_MAP
	m_FromCtrl.b3DDX(pDX,m_Material->m_From);
	m_ToCtrl.b3DDX(pDX,m_Material->m_To);
}


BEGIN_MESSAGE_MAP(CDlgMatSlide, CB3SimplePropertyPreviewDialog)
	//{{AFX_MSG_MAP(CDlgMatSlide)
	ON_BN_CLICKED(IDC_CUTOFF, OnSlideMode)
	ON_BN_CLICKED(IDC_SLIDE_HORIZONTAL, OnSlideMode)
	ON_BN_CLICKED(IDC_SLIDE_VERTICAL, OnSlideMode)
	ON_EN_KILLFOCUS(IDC_EDIT_FROM, OnEdit)
	ON_EN_KILLFOCUS(IDC_EDIT_TO, OnEdit)
	ON_NOTIFY(WM_LBUTTONUP,IDC_EDIT_FROM, OnSpin)
	ON_NOTIFY(WM_LBUTTONUP,IDC_EDIT_TO, OnSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatSlide message handlers

void CDlgMatSlide::b3Register()
{
	b3Loader::b3AddClassType(SLIDE,IDS_MAT_SLIDE,IDI_MAT_SLIDE,b3Edit,b3Edit);
}

b3_bool CDlgMatSlide::b3Edit(b3Item *item)
{
	CDlgMatSlide dlg(item);

	dlg.DoModal();
	return true;
}

void CDlgMatSlide::b3InitDialog()
{
	m_PageLeft.b3SetCaption(IDS_FROM);
	m_PageRight.b3SetCaption(IDS_TO);
	m_PropertySheet.AddPage(&m_PageLeft);
	m_PropertySheet.AddPage(&m_PageRight);
}

void CDlgMatSlide::b3UpdateUI()
{
	m_PreviewMaterialCtrl.b3Update(m_MatScene);
}

void CDlgMatSlide::OnSlideMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_Material->m_ModeFlag = m_SlideMode | (m_CutOff ? SLIDE_CUT : 0);
	b3Preview();
}
