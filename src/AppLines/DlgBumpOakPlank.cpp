/*
**
**	$Filename:	DlgBumpOakPlank.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Wood bump map
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
#include "DlgBumpOakPlank.h"
#include "blz3/system/b3Plugin.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**
**	Revision 1.1  2004/04/17 17:18:33  sm
**	- Made some include adjustments
**	- Added oakplank bump as dialog
**	- Fixed b3BumpWood and b3BumpOakPlank
**	  bump computation
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgBumpWood implementation                   **
**                                                                      **
*************************************************************************/

CDlgBumpOakPlank::CDlgBumpOakPlank(b3Item *item,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBumpOakPlank::IDD, pParent)
{
	m_Bump                    = (b3BumpOakPlank *)item;
	m_PageBump.m_Bump         = m_Bump;
	m_PageOakPlank.m_OakPlank = m_Bump;
	m_PageWood.m_Wood         = m_Bump;
	//{{AFX_DATA_INIT(CDlgBumpOakPlank)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBumpOakPlank::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBumpOakPlank)
	DDX_Control(pDX, IDC_PREVIEW_BUMP, m_PreviewBumpCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBumpOakPlank, CDialog)
	//{{AFX_MSG_MAP(CDlgBumpOakPlank)
	ON_MESSAGE(WM_USER,OnPreviewBump)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBumpOakPlank message handlers

void CDlgBumpOakPlank::b3Register()
{
	b3Loader::b3AddClassType(BUMP_OAKPLANK,IDS_BUMP_OAKPLANK,IDI_MAT_WOOD,b3Edit,b3Edit);
}

b3_bool CDlgBumpOakPlank::b3Edit(b3Item *item)
{
	CDlgBumpOakPlank dlg(item);

	dlg.DoModal();
	return true;
}

BOOL CDlgBumpOakPlank::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	m_PreviewBumpCtrl.b3Init();
//	m_MatSampler = new b3MaterialSampler(m_PreviewBumpCtrl);
//	m_MatSampler->b3SetMaterial(m_Material);

	m_PropertySheet.AddPage(&m_PageBump);
	m_PropertySheet.AddPage(&m_PageWood);
	m_PropertySheet.AddPage(&m_PageOakPlank);

	m_PropertySheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_PropertySheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_PropertySheet.ModifyStyle( 0, WS_TABSTOP );

	CRect rcSheet;
	GetDlgItem( IDC_PROPERTY )->GetWindowRect( &rcSheet );
	ScreenToClient( &rcSheet );
	m_PropertySheet.SetWindowPos( NULL, rcSheet.left-7, rcSheet.top-7, 0, 0, 
			SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );

	b3UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBumpOakPlank::b3UpdateUI()
{
//	m_PreviewBumpCtrl.b3Update(m_MatSampler);
}

void CDlgBumpOakPlank::OnPreviewBump() 
{
	// TODO: Add your control notification handler code here
	b3UpdateUI();
}
